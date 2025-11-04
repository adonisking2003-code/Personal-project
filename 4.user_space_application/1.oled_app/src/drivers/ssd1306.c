#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "ssd1306.h"
#include <string.h>
#include "ssd1306_data.h"
#include "alphabet_font10.h"

// int i2c_fd = -1;
bool is_collision;
unsigned char display_buffer[PAGE_NUM*COL_NUM];
unsigned char *test_data[] = {
    number_0, number_1, number_2, number_3, number_4,
    number_5, number_6, number_7, number_8, number_9,
    user_char_A, user_char_B, user_char_C, user_char_D, user_char_E,
    user_char_F, user_char_G, user_char_H, user_char_I, user_char_J,
    user_char_K, user_char_L, user_char_M, user_char_N, user_char_O,
    user_char_P, user_char_Q, user_char_R, user_char_S, user_char_T,
    user_char_U, user_char_V, user_char_W, user_char_X, user_char_Y,
    user_char_Z, user_char_space, user_char_colon, user_char_dot,
    user_char_minus, user_char_exclamation, user_char_comma
};

void print_display_buffer()
{
    for (int i = 0; i < PAGE_NUM; i++) {
        for (int j = 0; j < COL_NUM; j++) {
            if(display_buffer[i * COL_NUM + j] != 0x00)
            {
                if(display_buffer[i * COL_NUM + j] <= 0x0F)
                {
                    printf("index %d: 0x0%X \n", i*COL_NUM + j, display_buffer[i * COL_NUM + j]);
                }
                else
                {
                    printf("index %d: 0x%X \n", i*COL_NUM + j, display_buffer[i * COL_NUM + j]);
                }
            }
        }
    }
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

void oled_clear_display(int fd)
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

    oled_clear_display(i2c_fd);

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

    oled_clear_display(i2c_fd);

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

uint8_t get_bit_pixel(uint8_t bitmap_byte, uint8_t num_byte_want)
{
    return bitmap_byte >> ( 8 - num_byte_want%8);
}

void draw_bit_map(int fd, uint8_t x, uint8_t y, unsigned char* bitmap, uint8_t width, uint8_t height, bool color)
{
    // Check boundary
    if(x + width > SCREEN_WIDTH || y + height > SCREEN_HEIGHT || 
        x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
    {
        return;
    }

    uint8_t bit_mask = 0x01;
    uint8_t bit_on = 0x00;
    uint8_t bounded = (y%8 == 0) ? y : y-y%8;
    for(uint8_t j=y; j<y+height; j++)
    {
        for(uint8_t i = x; i<x+width; i++)
        {
            // uint16_t byteIndex = ( (j - y) /8) * width + i - x;
            uint16_t byteIndex = ( (j - bounded) /8) * width + i - x;
            bit_on =  bit_mask << ((j)%8);
            // printf("i = %d, j = %d, byteIndex: %d, bit_on = 0x%0x \n", i, j, byteIndex, bit_on);
            // if(bitmap[byteIndex]<<(y%8) & bit_on)
            if(j < y+height - (y+height)%8) 
            {
                // if((bitmap[byteIndex]<<(y%8) | get_bit_pixel(bitmap[byteIndex - width], y)) & bit_on)
                if(byteIndex - width > 0)
                {
                    // printf("byteIndex - width = %d \n", byteIndex - width);
                    if((bitmap[byteIndex]<<(y%8) | get_bit_pixel(bitmap[byteIndex - width], y) ) & bit_on)
                    {
                        draw_pixel(i, j, bit_on);
                    }
                }
                else
                {
                    if((bitmap[byteIndex]<<(y%8)) & bit_on)
                    {
                        draw_pixel(i, j, bit_on);
                    }
                }
                
            }
            else 
            {
                if(get_bit_pixel(bitmap[byteIndex - width], y) & bit_on)
                {
                    draw_pixel(i, j, bit_on);
                    // printf("Draw pixel at i = %d, j = %d \n", i, j);
                }
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

unsigned char* get_bit_map(char c, bool *is_special_char, uint8_t* char_width, uint8_t* char_height)
{
    unsigned char* bitmap;

    switch(c)
    {
        case '0':
            bitmap = number_0;
            break;
        case '1': 
            bitmap = number_1;
            break;
        case '2': 
            bitmap = number_2;
            break;
        case '3': 
            bitmap = number_3;
            break;
        case '4':            
            bitmap = number_4;
            break;
        case '5': 
            bitmap = number_5;
            break;
        case '6': 
            bitmap = number_6;
            break;
        case '7': 
            bitmap = number_7;
            break;
        case '8': 
            bitmap = number_8;
            break;
        case '9': 
            bitmap = number_9;
            break;
        case 'A': 
            bitmap = user_char_A;
            break;
        case 'B': 
            bitmap = user_char_B;
            break;
        case 'C': 
            bitmap = user_char_C;
            break;
        case 'D': 
            bitmap = user_char_D;
            break;
        case 'E': 
            bitmap = user_char_E;
            break;
        case 'F': 
            bitmap = user_char_F;
            break;
        case 'G': 
            bitmap = user_char_G;
            break;
        case 'H': 
            bitmap = user_char_H;
            break;
        case 'I': 
            bitmap = user_char_I;
            break;
        case 'J': 
            bitmap = user_char_J;
            break;
        case 'K': 
            bitmap = user_char_K;
            break;
        case 'L': 
            bitmap = user_char_L;
            break;
        case 'M': 
            bitmap = user_char_M;
            break;
        case 'N': 
            bitmap = user_char_N;
            break;
        case 'O': 
            bitmap = user_char_O;
            break;
        case 'P': 
            bitmap = user_char_P;
            break;
        case 'Q': 
            bitmap = user_char_Q;
            break;
        case 'R': 
            bitmap = user_char_R;
            break;
        case 'S': 
            bitmap = user_char_S;
            break;
        case 'T': 
            bitmap = user_char_T;
            break;
        case 'U': 
            bitmap = user_char_U;
            break;
        case 'V': 
            bitmap = user_char_V;
            break;
        case 'W': 
            bitmap = user_char_W;
            break;
        case 'X': 
            bitmap = user_char_X;
            break;
        case 'Y': 
            bitmap = user_char_Y;
            break;
        case 'Z': 
            bitmap = user_char_Z;
            break;
        case ' ': 
            bitmap = user_char_space;
            *is_special_char = true;
            break;
        case ':': 
            bitmap = user_char_colon;
            *is_special_char = true;
            break;
        case '.': 
            bitmap = user_char_dot;
            *is_special_char = true;
            break;
        case '-': 
            bitmap = user_char_minus;
            *is_special_char = true;
            break;
        case '!': 
            bitmap = user_char_exclamation;
            *is_special_char = true;
            break;
        case ',': 
            bitmap = user_char_comma;
            *is_special_char = true;
            break;
        default:
            bitmap = user_char_space;
            *is_special_char = true;
            break;
    }
    if(*is_special_char)
    {
        *char_width = 2;
    }
    else
    {
        *char_width = 10;
    }
    *char_height = 16;
    return bitmap;
}

void oled_print_str(int fd, const char *str, uint8_t cursor_x, uint8_t cursor_y, bool color)
{
    uint8_t char_width = 0;
    uint8_t char_height = 0;
    bool is_special_char = false;

    while (*str) {
        char c = *str++;

        unsigned char *bitmap = get_bit_map(c, &is_special_char, &char_width, &char_height);
        if(bitmap == NULL) {
            continue; // Bỏ qua ký tự không hợp lệ
        }
        if(cursor_x + char_width > SCREEN_WIDTH) {
            cursor_x = 0;
            cursor_y += char_height;
        }
        draw_bit_map(fd, cursor_x, cursor_y, bitmap, char_width, char_height, color);
        if(is_special_char)
        {
            cursor_x += 2;
        }
        else
        {
            cursor_x += 10;
        }
        if (cursor_x >= SCREEN_WIDTH) {
            cursor_x = 0;
            cursor_y += char_height;
        }
        if (cursor_y >= SCREEN_HEIGHT) {
            break; // Ngắt nếu vượt quá chiều cao màn hình
        }
    }
}  