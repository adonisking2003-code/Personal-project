// Purpose: Draw the current game state on the SSD1306 OLED or other display

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "render_thread.h"
#include "ssd1306_data.h"

extern volatile int game_state;     // From game_logic_thread.c
extern struct stBirdInfo bird;
extern struct stColumnList col_list;
extern struct stGameInfo game_info;

static int game_state = GAME_STATE_START;

void *render_thread_func(void *arg)
{
    while(1)
    {
        // render buffer
        usleep(500000); // Run at 2Hz, adjust if needed
    }
    return NULL;
}

void draw_column(int fd, struct stColumnList *col_list)
{

}

void draw_bird(int fd, struct stBirdInfo *bird)
{
    draw_bit_map(fd, bird->bird_x, bird->bird_y, &flappyBirdBitmap[0], 50, 50, true);
}

void start_screen(int fd)
{
    oled_clear_display(fd);
    printf("Start Screen\n");
    draw_rectangle(fd, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    oled_print_str(fd, "Flappy Bird", 0, 5, true);
    oled_print_str(fd, "Start!", 39, 25, true);
    update_oled_display(fd);
}

void end_screen(int fd)
{
    oled_clear_display(fd);
    printf("End Screen\n");
    draw_rectangle(fd, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    oled_print_str(fd, "Game Over", 20, 5, true);
    
    char score_str[20];
    snprintf(score_str, sizeof(score_str), "Score: %d", game_info.score);
    oled_print_str(fd, score_str, 30, 25, true);
    
    oled_print_str(fd, "Restart?", 30, 45, true);
    update_oled_display(fd);
}