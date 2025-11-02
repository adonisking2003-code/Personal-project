#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/types.h>

#define I2C_BUS_AVAILABLE           (1)
#define SLAVE_DEVICE_NAME           ("ETX_OLED")
#define SSD130_SLAVE_ADDR           (0x3C)

#define ETX_IOCTL_BASE              'W'
#define ETX_IOCTL_SET_BRIGHTNESS    _IOW(ETX_IOCTL_BASE, 1, int)
#define ETX_IOCTL_SET_ROTATION      _IOW(ETX_IOCTL_BASE, 2, int)
#define ETX_IOCTL_CLEAR_DISPLAY     _IO(ETX_IOCTL_BASE, 3)
#define ETX_IOCTL_SET_DISPLAY_MODE  _IOW(ETX_IOCTL_BASE, 4, int)
#define ETX_IOCTL_SET_PAGE          _IOW(ETX_IOCTL_BASE, 5, int)
#define ETX_IOCTL_SET_COL_LOW       _IOW(ETX_IOCTL_BASE, 6, int)
#define ETX_IOCTL_SET_COL_HIGH      _IOW(ETX_IOCTL_BASE, 7, int)

#define DISPLAY_WIDTH   128
#define DISPLAY_HEIGHT  64
#define BUFFER_SIZE     (DISPLAY_WIDTH * DISPLAY_HEIGHT / 8)
#define PAGE_NUM        8
#define COL_NUM         128

static unsigned char display_buffer[PAGE_NUM][COL_NUM];
static unsigned char display_buffer_1d[PAGE_NUM * COL_NUM];

static int oled_brightness      = 128;
static int oled_rotation        = 0;
static int oled_display_mode    = 0;

static struct i2c_adapter   *etx_i2c_adapter     = NULL;
static struct i2c_client    *etx_i2c_client_oled = NULL;

static dev_t dev = 0;
static struct class *st_dev_class;
static struct cdev file_ops_cdev;

static int I2C_Write(unsigned char *buf, unsigned int len)
{
    /*
    ** Sending Start condition, Slave address withR/W bit
    ** ACK/NACK and Stop condition will be handled internally.
    */
   int ret = i2c_master_send(etx_i2c_client_oled, buf, len);
   return ret;
}
/* This function reads one byte of the data from the I2C client*/
static int I2C_Read(unsigned char *out_buf, unsigned int len)
{
    /*
    ** Sending Start condition, Slave address with R/W but,
    ** ACK/NACK and Stop conditions will be handled internally.
    */
   int ret = i2c_master_recv(etx_i2c_client_oled, out_buf, len);
   return ret;
}

static void SSD1306_Write(bool is_cmd, unsigned char data)
{
    unsigned char buf[2] ={0};
    int ret;

    if( is_cmd == true )
    {
        buf[0] = 0x00;
    }
    else
    {
        buf[0] = 0x40;
    }
    buf[1] = data;

    ret = I2C_Write(buf, 2);
}

/* This function sends the commands that need to used to initialize OLED */
static int SSD1306_DisplayInit(void)
{
    msleep(100);

    /* Commands to initialize the SSD_1306 OLED Display*/
    SSD1306_Write(true, 0xAE); // Entire Display OFF
    SSD1306_Write(true, 0xD5); // Set Display Clock Divide Ratio and Oscillator Frequency
    SSD1306_Write(true, 0x80); // Default Setting for Display Clock Divide Ratio and Oscillator Frequency that is recommended
    SSD1306_Write(true, 0xA8); // Set Multiplex Ratio
    SSD1306_Write(true, 0x3F); // 64 COM lines
    SSD1306_Write(true, 0xD3); // Set display offset
    SSD1306_Write(true, 0x00); // 0 offset
    SSD1306_Write(true, 0x40); // Set first line as the start line of the display
    SSD1306_Write(true, 0x8D); // Charge pump
    SSD1306_Write(true, 0x14); // Enable charge dump during display on
    SSD1306_Write(true, 0x20); // Set memory addressing mode
    SSD1306_Write(true, 0x00); // Horizontal addressing mode
    SSD1306_Write(true, 0xA1); // Set segment remap with column address 127 mapped to segment 0
    SSD1306_Write(true, 0xC8); // Set com output scan direction, scan from com63 to com 0
    SSD1306_Write(true, 0xDA); // Set com pins hardware configuration
    SSD1306_Write(true, 0x12); // Alternative com pin configuration, disable com left/right remap
    SSD1306_Write(true, 0x81); // Set contrast control
    SSD1306_Write(true, 0x80); // Set Contrast to 128
    SSD1306_Write(true, 0xD9); // Set pre-charge period
    SSD1306_Write(true, 0xF1); // Phase 1 period of 15 DCLK, Phase 2 period of 1 DCLK
    SSD1306_Write(true, 0xDB); // Set Vcomh deselect level
    SSD1306_Write(true, 0x20); // Vcomh deselect level ~ 0.77 Vcc
    SSD1306_Write(true, 0xA4); // Entire display ON, resume to RAM content display
    SSD1306_Write(true, 0xA6); // Set Display in Normal Mode, 1 = ON, 0 = OFF
    SSD1306_Write(true, 0x2E); // Deactivate scroll
    SSD1306_Write(true, 0xAF); // Display ON in normal mode

    return 0;
}

/* This function fills the complete OLED with this data byte */
static void SSD1306_Fill(unsigned char data)
{
    unsigned int total  = 128*8; // 8 pages x 128 segments
    unsigned int i      = 0;

    // Fill the Display
    for(i = 0; i < total; i++)
    {
        SSD1306_Write(false, data);
    }
}

static void oled_clear_display(void)
{
    memset(display_buffer, 0x00, PAGE_NUM * COL_NUM * sizeof(uint8_t));
    SSD1306_Fill(0x00);
}

static void oled_set_brightness(int level)
{
    if(level < 0) level = 0;
    else if(level > 255) level = 255;

    oled_brightness = level;
    SSD1306_Write(true, 0x81); // Constast control command
    SSD1306_Write(true, oled_brightness);
}

static void oled_set_rotation(int rotation)
{
    oled_rotation = rotation;

    switch(rotation)
    {
        case 0:
            SSD1306_Write(true, 0xA1); // segment remap column address 127 to 0
            SSD1306_Write(true, 0xC8); // COM output scan direction remapped
            break;
        case 1:
            SSD1306_Write(true, 0xA0);
            SSD1306_Write(true, 0xC0);
            break;
        default:
            break;
    }
}

static void oled_set_display_mode(int mode)
{
    oled_display_mode = mode;
    switch(mode)
    {
        case 0:
            SSD1306_Write(true, 0xA6);
            break;
        case 1:
            SSD1306_Write(true, 0xA7);
            break;
        default:
            break;
    }
}

static long etx_oled_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int val = 0;

    switch(cmd)
    {
        case ETX_IOCTL_SET_BRIGHTNESS:
            if(copy_from_user(&val, (int __user *)arg, sizeof(val)))
                return -EFAULT;
            oled_set_brightness(val);
            break;
        case ETX_IOCTL_SET_ROTATION:
            if(copy_from_user(&val, (int __user *)arg, sizeof(val)))
                return -EFAULT;
            oled_set_rotation(val);
            break;
        case ETX_IOCTL_CLEAR_DISPLAY:
            oled_clear_display();
            break;
        case ETX_IOCTL_SET_DISPLAY_MODE:
            if(copy_from_user(&val, (int __user *)arg, sizeof(val)))
                return -EFAULT;
            oled_set_display_mode(val);
            break;
        case ETX_IOCTL_SET_PAGE:
            if (copy_from_user(&val, (uint8_t __user *)arg, sizeof(val)))
                return -EFAULT;
            SSD1306_Write(true, 0xB0 + val);
            break;
        case ETX_IOCTL_SET_COL_LOW:
            if (copy_from_user(&val, (uint8_t __user *)arg, sizeof(val)))
                return -EFAULT;
            SSD1306_Write(true, val);  // ở đây giá trị trỏ vào 0x00
        case ETX_IOCTL_SET_COL_HIGH:
            if (copy_from_user(&val, (uint8_t __user *)arg, sizeof(val)))
                return -EFAULT;
            SSD1306_Write(true, val);  // ở đây giá trị trỏ vào 0x10
        default:
            return -EINVAL;
    }
    return 0;
}

static ssize_t etx_oled_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    // ssise_t ret;
    // size_t bytes_to_read;

    // if(*ppos >= BUFFER_SIZE)
    //     return 0; //EOF
    
    // bytes_to_read = min(count, (size_t)(BUFFER_SIZE - *ppos));

    // if(copy_to_user(buf, display_buffer + *ppos, bytes_to_read))
    //     return -EFAULT;

    // *ppos += bytes_to_read;
    // ret = bytes_to_read;

    printk(KERN_INFO  "Read function!\n");

    return 0;
}

void print_display_buffer(uint8_t *buffer, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++) {
        printk(KERN_INFO "Buf[%zu] = 0x%02X\n", i, buffer[i]);
    }
}

static ssize_t etx_oled_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    size_t total_bytes = PAGE_NUM * COL_NUM;
    size_t bytes_to_write = (count > total_bytes) ? total_bytes : count;
    uint16_t count_print = 0;
    printk(KERN_INFO "Number of bytes to write: %d \n", bytes_to_write);

    // if(copy_from_user(&display_buffer[0][0], buf, bytes_to_write))
    if(copy_from_user(&display_buffer_1d, buf, bytes_to_write))
        return -EFAULT;
    
    // print_display_buffer(display_buffer_1d, 1024);
    pr_info("display_buffer[0] = %d\n", display_buffer_1d[0]);
    pr_info("display_buffer[1] = %d\n", display_buffer_1d[1]);
    pr_info("display_buffer[128] = %d\n", display_buffer_1d[128]);
    pr_info("display_buffer[129] = %d\n", display_buffer_1d[129]);

    for(int page = 0; page < PAGE_NUM; page++)
    {   
        SSD1306_Write(true, 0xB0 + page);              // Set page start address
        SSD1306_Write(true, 0x00);              // Set lower column start address
        SSD1306_Write(true, 0x10);              // Set higher column start address
        // send data column in page
        for(int col = 0; col < COL_NUM; col++)
        {
            SSD1306_Write(false, display_buffer_1d[page * COL_NUM + col]);
            // msleep(500);
        }
    }

    return bytes_to_write;
}

static const struct file_operations etx_oled_fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = etx_oled_ioctl,
    .read           = etx_oled_read,
    .write          = etx_oled_write,
};

/* This function getting called when the slave has been found */
static int etx_oled_probe(struct i2c_client *client)
{
    SSD1306_DisplayInit();

    // Fill the OLED with this data
    SSD1306_Fill(0xFF); // 0x00 is white, 0xFF is full

    pr_info("OLED Probed!!!\n");

    return 0;
}

/* This function getting called when the slave has been remove */
static void etx_oled_remove(struct i2c_client *client)
{
    // Fill the OLED with this data
    SSD1306_Fill(0x00);

    pr_info("OLED Removed!!!\n");
}

/* Structure that has slave device id*/
static const struct i2c_device_id etx_oled_id[] = {
    { SLAVE_DEVICE_NAME, 0},
    {}
};
MODULE_DEVICE_TABLE(i2c, etx_oled_id);

/* I2C driver structure that has to be added to Linux */
static struct i2c_driver etx_oled_driver = {
    .driver = {
        .name   = SLAVE_DEVICE_NAME,
        .owner  = THIS_MODULE,
    },
    .probe      = etx_oled_probe,
    .remove     = etx_oled_remove,
    .id_table   = etx_oled_id,
};

/* I2C Board Info Structure */
static struct i2c_board_info oled_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, SSD130_SLAVE_ADDR)
};

/* Module Init function */
static int __init etx_driver_init(void)
{
    int ret = -1;
    etx_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

    if( etx_i2c_adapter != NULL )
    {
        etx_i2c_client_oled = i2c_new_client_device(etx_i2c_adapter, &oled_i2c_board_info);

        if( etx_i2c_client_oled != NULL )
        {
            i2c_add_driver(&etx_oled_driver);
            ret = 0;
        }

        i2c_put_adapter(etx_i2c_adapter);
    }

    if(alloc_chrdev_region(&dev, 0, 1, "oled_device_chrdev") < 0)
    {
        printk(KERN_INFO "Dynamic Allocate failed! \n");
        return -1;
    }
    printk("Dynamic allocate successfully with Major = %d, Minor = %d \n", MAJOR(dev), MINOR(dev));

    cdev_init(&file_ops_cdev, &etx_oled_fops);
    // Adding character device to the system
    if((cdev_add(&file_ops_cdev, dev, 1)) < 0)
    {
        pr_err("Cannot add the device to the system! \n");
        goto err_class;
    }
    
    // Newer kernels (6.4 and later): class_create now takes only 1 argument - the class name string.
    st_dev_class = class_create("etx_oled_class");
    if(IS_ERR(st_dev_class))
    {
        pr_err("Cannot create struct class for this device! \n");
        goto err_class;
    }
    printk(KERN_INFO "Create struct class successfully for this device");

    if(IS_ERR(device_create(st_dev_class, NULL, dev, NULL, "etx_oled_device")))
    {
        pr_err("Cannot create device file! \n");
        goto err_device;
    }
    printk(KERN_INFO "Create device file successfuly! \n");

    pr_info("Driver Added!!!\n");
    return ret;

err_device:
    class_destroy(st_dev_class);

err_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

/* Module Exit function */
static void __exit etx_driver_exit(void)
{
    i2c_unregister_device(etx_i2c_client_oled);
    i2c_del_driver(&etx_oled_driver);
    device_destroy(st_dev_class, dev);
    class_destroy(st_dev_class);
    cdev_del(&file_ops_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Driver Removed!!!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX");
MODULE_DESCRIPTION("Simple I2C driver explanation (SSD_130 OLED)");
MODULE_VERSION("1.0");
