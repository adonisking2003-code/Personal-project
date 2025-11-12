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
    oled_clear_display(fd);
    delay_ms(1000);
    // oled_clear_display(fd);
    draw_bit_map(fd, 0, 0, number_0, 10, 16, true);

    update_oled_display(fd);
    // delay_ms(2000);
    // printf("Clear display \n");
    // oled_clear_display(fd);
    // // update_oled_display(fd);
    // delay_ms(5000);
    close(fd);
    return 0;
}
