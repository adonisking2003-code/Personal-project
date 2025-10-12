#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "ssd1306.h"
#include <string.h>

// int i2c_fd = -1;
bool is_collision;	// Cờ báo va chạm
unsigned char display_buffer[PAGE_NUM*COL_NUM];

void print_display_buffer()
{
    // for (int i = 0; i < PAGE_NUM; i++) {
    //     for (int j = 0; j < COL_NUM; j++) {
    //         // In giá trị theo định dạng hex để dễ nhìn
    //         printf("0x%02X ", display_buffer[i*j]);
    //     }
    //     printf("\n\n\n"); // Xuống dòng sau mỗi page (hàng)
    // }
    printf("0x%02X \n", display_buffer[0]);
    printf("0x%02X \n", display_buffer[1]);
    printf("0x%02X \n", display_buffer[128]);
    printf("0x%02X \n", display_buffer[129]);
}

void set_brightness(int fd, int level)
{
    if(ioctl(fd, ETX_IOCTL_SET_BRIGHTNESS, &level) == -1)
    {
        perror("ioctl set brightness");
    }
}

void set_rotation(int fd, int rotation)
{
    if(ioctl(fd, ETX_IOCTL_SET_ROTATION, &rotation) == -1)
    {
        perror("ioctl set rotation");
    }
}

void clear_display(int fd)
{
    memset(&display_buffer, 0, sizeof(display_buffer));
    if(ioctl(fd, ETX_IOCTL_CLEAR_DISPLAY) == -1)
    {
        perror("ioctl clear display");
    }
}

void set_display_mode(int fd, int mode)
{
    if(ioctl(fd, ETX_IOCTL_SET_DISPLAY_MODE) == -1)
    {
        perror("ioctl set display mode");
    }
}

uint8_t read_display_buffer(int fd, uint8_t *buffer, size_t size)
{
    uint8_t ret = read(fd, buffer, size);
    if(ret == -1)
    {
        perror("read");
    }
    return ret;
}

uint8_t write_display_buffer(int fd, uint8_t *buffer, size_t size)
{
    uint8_t ret = write(fd, buffer, size);
    if(ret == -1)
    {
        perror("write");
    }
    return ret;
}

int ssd1306_init(void)
{
    int i2c_fd = open(OLED_DEVICE_FILE, O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open i2c device");
        return -1;
    }

    set_brightness(i2c_fd, 200);

    clear_display(i2c_fd);

    uint8_t buffer[8][128];
    for (int i = 0; i < 4; i++) {
        for(int j=0; j<128; j++)
        {
            buffer[i][j] = 0xFF;
        }
    }

    // if(write_display_buffer(fd, (uint8_t *)buffer, 8*128) == -1)
    if(write_display_buffer(i2c_fd, &buffer[0][0], 8*128) == -1)
    {
        ssd1306_close(i2c_fd);
        return 1;
    }

    uint8_t read_buf[8*128];
    if(read_display_buffer(i2c_fd, read_buf, sizeof(read_buf)) > 0)
    {
        printf("Read display buffer successfully!\n");
    }

    clear_display(i2c_fd);

    return i2c_fd;
}

void ssd1306_close(int fd)
{
    if(fd > 0)
    {
        close(fd);
        fd = -1;
    }
}

//Need to check

void draw_pixel(uint8_t x, uint8_t y, uint8_t bit_mask) 
{
    uint8_t page = y/8;
    display_buffer[page*128 + x] |= bit_mask; 	// Ghi pixel vào buffer
}
// Ví dụ: Vẽ cột cao 5 pixels từ y = 0 đến y = 4 tại x = 10

// for (uint8_t y = 0; y < 5; y++) {
//     draw_pixel(10, y);
// }

// Xóa pixels:

void delete_pixel(int fd, uint8_t x, uint8_t y)
{
    uint8_t page = y / 8; 					// Xác định page chứa pixel 
    uint8_t bit_mask = 1 << (y % 8); 		// Tạo mask để bật đúng bit trong byte
    display_buffer[page * x + x] &= ~bit_mask;
}
// display_buffer[page][x] &= ~bit_mask;

void draw_line(int fd, uint8_t x, uint8_t y, uint8_t x_end, uint8_t y_end, uint8_t thin)
{
	uint8_t swap;
	if(x_end < x) 
	{
		swap = x_end;
		x_end = x;
		x = swap;
	}
	if(y_end < y)
	{
		swap = y_end;
		y_end = y;
		y = swap;
	}
    if (x == x_end) {
        // Vẽ đường dọc
        for (uint8_t i = y; i <= y_end; i++) {
            for (uint8_t t = 0; t < thin; t++) {
                draw_pixel(x + t, i, 0xFF);
            }
        }
    } else if (y == y_end) {
        // Vẽ đường ngang
        for (uint8_t i = x; i <= x_end; i++) {
            for (uint8_t t = 0; t < thin; t++) {
                draw_pixel(i, y + t, 0xFF);
            }
        }
    } else {
        // Vẽ đường chéo (dùng thuật toán Bresenham đơn giản)
        int dx = x_end - x;
        int dy = y_end - y;
        int abs_dx = dx > 0 ? dx : -dx;
        int abs_dy = dy > 0 ? dy : -dy;
        int sx = dx > 0 ? 1 : -1;
        int sy = dy > 0 ? 1 : -1;
        int err = (abs_dx > abs_dy ? abs_dx : -abs_dy) / 2;
        int e2;

        while (1) {
            for (uint8_t t = 0; t < thin; t++) {
                draw_pixel(x, y + t, 0xFF); // bạn có thể thay đổi theo hướng dày mong muốn
            }

            if (x == x_end && y == y_end) break;

            e2 = err;
            if (e2 > -abs_dx) {
                err -= abs_dy;
                x += sx;
            }
            if (e2 < abs_dy) {
                err += abs_dx;
                y += sy;
            }
        }
    }
}


void draw_rectangle(int fd, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill)
{
	
}

void draw_circle(int fd, uint8_t x, uint8_t y, uint8_t radian, bool fill)
{

}

void draw_bit_map(int fd, uint8_t x, uint8_t y, unsigned char* bitmap, uint8_t width, uint8_t height, bool color)
{
    // uint8_t byteWidth = (width + 7) / 8; // Number of byte each row bitmap
    uint8_t bit_mask = 0x01;
    uint8_t bit_on = 0x00;
    for(uint8_t j=y; j<y+height; j++)
    {
        for(uint8_t i = x; i<x+width; i++)
        {
            uint16_t byteIndex = (j/8) * width + i - x;
            bit_on =  bit_mask << (j%8);
            if(bitmap[byteIndex] & bit_on)
            {
                draw_pixel(i, j, bit_on);
            }
        }
    }
}

uint8_t update_oled_display(int fd) 
{
    uint8_t ret;
    ret = write_display_buffer(fd, &display_buffer[0], PAGE_NUM*COL_NUM);
    return ret;
}