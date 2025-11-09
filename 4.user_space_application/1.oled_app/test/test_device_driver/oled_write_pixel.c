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

    size_t buffer_size = PAGE_NUM * COL_NUM;
    unsigned char buffer[buffer_size];

    memset(buffer, 0, buffer_size);

    printf("Clear display \n");

    oled_clear_display(fd);

    update_oled_display(fd);
    delay_ms(2000);
    // Test all characters
    for (uint8_t i = 5; i <= 15; i++) {
        for (uint8_t t = 0; t < 2; t++) {
            draw_pixel(5 + t, i, 0xFF);
        }
    }
    update_oled_display(fd);
    delay_ms(5000);
    close(fd);
    return 0;
}
