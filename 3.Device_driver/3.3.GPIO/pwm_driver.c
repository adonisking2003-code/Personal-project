#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include "pwm_driver.h"

#define PWM_GPIO 12

#define IO_OFFSET 512

dev_t pwm_dev = 0;
static struct class *pwm_dev_class;
static struct cdev pwm_cdev;

static int __init pwm_driver_init(void);
static void __exit pwm_driver_exit(void);

/* Driver function */
static int pwm_open(struct inode *inode, struct file *file);
static int pwm_release(struct inode *inode, struct file *file);
static ssize_t pwm_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t pwm_write(struct file *file, const char *buf, size_t len, loff_t *off);
/****************** */

/* File operation structure */
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = pwm_read,
    .write      = pwm_write,
    .open       = pwm_open,
    .release    = pwm_release,
};

static int pwm_open(struct inode *inode, struct file *file)
{
    pr_info("Device File Opened...!\n");
    return 0;
}   

static int pwm_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...!\n");
    return 0;
}

static ssize_t pwm_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    char out_buf[16];
    int ret;

    if(*off > 0)
        return 0;

    int duty_percent = (pwm_state.duty_cycle * 100) / pwm_state.period;

    ret = snprintf(out_buf, sizeof(out_buf), "%d\n", duty_percent);

    if(copy_to_user(buf, out_buf, ret))
    {
        return -EFAULT;
    }
    *off += ret;
    return ret;
}

static ssize_t pwm_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    char in_buf[16];
    int duty_percent;

    if(len > sizeof(in_buf) - 1)
    {
        return -EINVAL;
    }

    if(copy_from_user(in_buf, buf, len))
    {
        return -EFAULT;
    }
    in_buf[len] = '\0';
    if(kstrtoint(in_buf, 10, &duty_percent))
    {
        return -EINVAL;
    }
    if(duty_percent<0 || duty_percent > 100)
    {
        return -EINVAL;
    }
    pwm_state.duty_cycle = (pwm_state.period * duty_percent) / 100;

    pwm_apply_state(pwm_dev, &pwm_state);

    pr_info("[INFO]: PWM duty set to %d%%\n", duty_percent);

    return len;
}

static int request_and_setup_pwm()
{
    pwm_dev = pwm_get(pwm_dev, "pwm"); // "pwm" check this name property from device tree
    if(IS_ERR(pwm_dev))
    {
        pr_err("[ERROR]: Failed to request PWM\n");
        return -1;
    }

    pwm_init_state(pwm_dev, &pwm_state);
    pwm_state.period = 20000000; //20ms (50Hz)
    pwm_state.duty_cycle = 0; // 0%
    pwm_state.enabled = true;

    pwm_apply_state(pwm_dev, &pwm_state);

    pr_info("Request and setup PWM done!\n");
    return 0;

}

static int __init pwm_driver_init(void)
{
    if( (alloc_chrdev_region(&pwm_dev, 0, 1, "pwm_Dev") < 0) )
    {
        pr_err("[ERROR]: Cannot allocate major number\n");
        goto r_unreg;
    }
    pr_info("[INFO]: Major = %d, Minor = %d \n", MAJOR(pwm_dev), MINOR(pwm_dev));

    cdev_init(&pwm_cdev, &fops);

    if( (cdev_add(&pwm_cdev, pwm_dev, 1)) < 0 )
    {
        pr_err("[ERROR]: Cannot add the device to the system\n");
        goto r_del;
    }

    if( IS_ERR(pwm_dev_class = class_create("pwm_class")) )
    {
        pr_err("[ERROR]: Cannot create the struct class\n");
        goto r_class;
    }

    if( IS_ERR(device_create(pwm_dev_class, NULL, pwm_dev, NULL, "pwm_device")) )
    {
        pr_err("[ERROR]: Cannot create the device\n");
        goto r_device;
    }

    if(request_and_setup_pwm() < 0)
    {
        pr_err("[ERROR]: Cannot request and setup PWM!\n");
    }

    pr_info("[INFO]: Device driver pwm insert ... pwm_device ... Done!\n");
    return 0;

r_device:
    device_destroy(pwm_dev_class, pwm_dev);
r_class:
    class_destroy(pwm_dev_class);
r_del:
    cdev_del(&pwm_cdev);
r_unreg:
    unregister_chrdev_region(pwm_dev, 1);

    return -1;
}

static void __exit etx_driver_exit(void)
{
    pwm_disable(pwm_dev);
    pwm_free(pwm_dev);

    device_destroy(pwm_dev_class, pwm_dev);
    class_destroy(pwm_dev_class); 
    cdev_del(&pwm_cdev);
    unregister_chrdev_region(pwm_dev, 1);
    pr_info("[INFO]: Device driver pwm remove...Done!\n");
}

module_init(pwm_driver_init);
module_exit(pwm_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AdonisKing");
MODULE_DESCRIPTION("A simple device driver - PWM");
MODULE_VERSION("1.0");

