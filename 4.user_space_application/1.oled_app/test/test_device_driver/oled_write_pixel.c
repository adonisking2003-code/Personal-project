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
    int x = 0, y = 0;
    for(int i = 0; i < sizeof(test_data)/sizeof(test_data[0]); i++) {
        if(test_data[i] == user_char_space || test_data[i] == user_char_colon ||
           test_data[i] == user_char_dot || test_data[i] == user_char_minus ||
           test_data[i] == user_char_exclamation || test_data[i] == user_char_comma) 
        {
            draw_bit_map(fd, x, y, &test_data[i][0], 2, 16, true);
        }
        else {
            draw_bit_map(fd, x, y, &test_data[i][0], 10, 16, true);
        }
        x += 10;
        if (x >= 128) {
            x = 0;
            y += 16;
        }
        if (y >= 64) {
            break;
        }
    }
    update_oled_display(fd);
    delay_ms(5000);
    close(fd);
    return 0;
}
