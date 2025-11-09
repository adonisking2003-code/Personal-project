#ifndef SSD1306_H
#define SSD1306_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define SSD1306_I2C_ADDRESS 0x3C
#define ETX_IOCTL_BASE      'W'
#define ETX_IOCTL_SET_BRIGHTNESS    _IOW(ETX_IOCTL_BASE, 1, int)
#define ETX_IOCTL_SET_ROTATION      _IOW(ETX_IOCTL_BASE, 2, int)
#define ETX_IOCTL_CLEAR_DISPLAY     _IO(ETX_IOCTL_BASE, 3)
#define ETX_IOCTL_SET_DISPLAY_MODE  _IOW(ETX_IOCTL_BASE, 4, int)
#define ETX_IOCTL_SET_PAGE          _IOW(ETX_IOCTL_BASE, 5, int)
#define ETX_IOCTL_SET_COL_LOW       _IOW(ETX_IOCTL_BASE, 6, int)
#define ETX_IOCTL_SET_COL_HIGH      _IOW(ETX_IOCTL_BASE, 7, int)
#define PAGE_NUM        8
#define COL_NUM         128

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64

#define OLED_DEVICE_FILE "/dev/etx_oled_device"

/* Struct definition */
typedef struct {
    unsigned char* bitmap;
    uint8_t width;
    uint8_t height;
} stBitmap;

void set_brightness(int fd, int level);
void set_rotation(int fd, int rotation);
void oled_clear_display(int fd);
void set_display_mode(int fd, int mode);
uint8_t read_display_buffer(int fd, uint8_t *buffer, size_t size);
uint8_t write_display_buffer(int fd, uint8_t *buffer, size_t size);

unsigned char* scale_bitmap_array(const unsigned char* old_buffer, uint8_t old_width, uint8_t old_height, uint8_t new_width, uint8_t new_height);
void draw_pixel(uint8_t x, uint8_t y, uint8_t bit_mask);
void delete_pixel(int fd, uint8_t x, uint8_t y);
void draw_line(int fd, uint8_t x, uint8_t y, uint8_t x_end, uint8_t y_end, uint8_t thin);
void draw_rectangle(int fd, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t thick, bool fill);
void draw_circle(int fd, uint8_t x, uint8_t y, uint8_t radian, bool fill);

void setCursor(int fd, uint8_t x, uint8_t y);
void setTextSize(int fd, uint8_t text_size);

/* color = 1 is white, = 0 is black 
    width = width of bitmap
    height = height of bitmap
*/
void draw_bit_map(int fd, uint8_t x, uint8_t y, unsigned char* bitmap, uint8_t width, uint8_t height, bool color);
uint8_t update_oled_display(int fd);
void print_display_buffer();
void oled_print_str(int fd, const char *str, uint8_t cursor_x, uint8_t cursor_y, bool color);

int ssd1306_init(void);
void ssd1306_close(int fd);

#endif  // SSD1306_H