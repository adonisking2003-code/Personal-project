#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "ssd1306_data.h"
#include "utils.h"
#include "ssd1306.h"
#include "alphabet_font10.h"

#define PAGE_NUM 8
#define COL_NUM 128

int main()
{
    int fd = open("/dev/etx_oled_device", O_WRONLY);
    if (fd < 0) {
        perror("Open device failed");
        return 1;
    }

    // // Tổng kích thước buffer
    size_t buffer_size = PAGE_NUM * COL_NUM;
    unsigned char buffer[buffer_size];

    memset(buffer, 0, buffer_size);

    printf("Clear display \n");

    clear_display(fd);

    // draw_bit_map(fd, 0, 0, &VietNam_map[0], 128, 64, true);

    // delay_ms(2000);

    // update_oled_display(fd);
    // delay_ms(2000);

    // clear_display(fd);
    // memset(buffer, 0xFF, buffer_size);
    // ssize_t ret = write(fd, buffer, buffer_size);
    // delay_ms(2000);

    // draw_bit_map(fd, 30, 5, &flappyBirdBitmap[0], 50, 50, true);
    // update_oled_display(fd);

    // clear_display(fd);
    // delay_ms(2000);

    draw_bit_map(fd, 0, 0, &number_1[0], 1, 16, true);
    update_oled_display(fd);
    print_display_buffer();
    close(fd);
    return 0;
}
