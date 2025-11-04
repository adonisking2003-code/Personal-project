#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "/home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include/drivers/ssd1306_data.h"
#include "/home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include/utils/utils.h"
#include "/home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include/drivers/ssd1306.h"
#include "/home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include/drivers/button.h"
#include "/home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include/core/game_logic_thread.h"
#include "render_thread.h"

void test_column(int fd);
void test_bird(int fd);

int main()
{
    int fd = open("/dev/etx_oled_device", O_WRONLY);
    if (fd < 0) {
        perror("Open device failed");
        return 1;
    }
    test_column(fd);
    // test_bird(fd);

    close(fd);
    return 0;
}

void test_column(int fd)
{
    stColumnInfo column = {.column_x=120, .column_top_y = 49, .column_bottom_y = 64};
    printf("Initial Column Position: X=%d, Top Y=%d, Bottom Y=%d\n", column.column_x, column.column_top_y, column.column_bottom_y);
        oled_clear_display(fd);
        // draw_column(fd, &column);
        draw_bit_map(fd, 0, 0, &column_bitmap[0], 34, 16, true);
        update_oled_display(fd);
        sleep(1);
    // Simulate moving the column
    // for(int i=0; i<5; i++)
    // {
    //     column.column_x -= 5; // Move left by 5 units
    //     printf("Updated Column Position: X=%d, Top Y=%d, Bottom Y=%d\n", column.column_x, column.column_top_y, column.column_bottom_y);
    //     update_column(&column);
    //     oled_clear_display(fd);
    //     draw_column(fd, &column);
    //     update_oled_display(fd);
    //     sleep(1); // Pause for a second
    // }
}

void test_bird(int fd)
{
    stBirdInfo bird = {.bird_x=10, .bird_y=30, .bird_height=10, .bird_width=15, .bird_acceleration=5};
    printf("Initial Bird Position: X=%d, Y=%d\n", bird.bird_x, bird.bird_y);

    // Simulate bird movement
    for(int i=0; i<5; i++)
    {
        move_up(&bird);
        printf("Bird Position after moving up: X=%d, Y=%d\n", bird.bird_x, bird.bird_y);
        oled_clear_display(fd);
        draw_bird(fd, &bird);
        update_oled_display(fd);
        usleep(500000); // Pause for half a second

        move_down(&bird);
        printf("Bird Position after moving down: X=%d, Y=%d\n", bird.bird_x, bird.bird_y);
        oled_clear_display(fd);
        draw_bird(fd, &bird);
        update_oled_display(fd);
        usleep(500000); // Pause for half a second
    }
}