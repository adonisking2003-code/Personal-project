// Purpose: Draw the current game state on the SSD1306 OLED or other display

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "render_thread.h"
#include "ssd1306_data.h"
#include "game_logic_thread.h"

extern eGameState game_state;
extern bool render_flag;
extern struct stBirdInfo bird;
extern struct stColumnInfo col_info;
extern struct stGameInfo game_info;

void *render_thread_func(void *arg)
{
    int fd = (int)arg; // Cast arg to int file descriptor
    while(1)
    {   
        // render buffer
        if(render_flag)
        {
            if(game_state == GAME_STATE_READY)
            {
                start_screen(fd);
            }

            else if(game_state == GAME_STATE_PLAYING)
            {
                oled_clear_display(fd);
                draw_bird(fd, &bird);
                draw_column(fd, &col_info);
                draw_score(fd, game_info.points);
                update_oled_display(fd);
                update_game_play(fd);
            }

            else if(game_state == GAME_STATE_OVER)
            {
                end_screen(fd);
            }

            render_flag = false;
        }
        usleep(33333); // Run at 30Hz (30 frame/s), adjust if needed
    }
    return NULL;
}

void draw_column(int fd, struct stColumnInfo *col_info)
{
    uint8_t column_width  = 20;
    uint8_t column_height = col_info->column_bottom_y - col_info->column_top_y;
    // If it error, create function crop array to exactly resolution
    // stColumnInfo column_bitmap_new = crop_bitmap(column_bitmap, column_width, column_height);
    // draw_bit_map(fd, col_info->column_x - column_width, col_info->column_top_y, &column_bitmap[0], column_width, column_height, true);
    draw_bit_map(fd, 30, 64-16, &column_simple[0], 10, 16, true); // Test draw column
}

void draw_bird(int fd, struct stBirdInfo *bird)
{
    draw_bit_map(fd, bird->bird_x, bird->bird_y, &flappyBird_bitmap[0], 10, 15, true);
}

void draw_score(int fd, int score)
{
    char score_str[10];
    snprintf(score_str, sizeof(score_str), "%d", score);
    oled_print_str(fd, score_str, 0, 0, true);
}

void start_screen(int fd)
{
    oled_clear_display(fd);
    printf("Start Screen\n");
    draw_rectangle(fd, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    oled_print_str(fd, "Flappy Bird", 0, 5, true);
    oled_print_str(fd, "Start!", 39, 25, true);
    update_oled_display(fd);
}

void end_screen(int fd)
{
    oled_clear_display(fd);
    printf("End Screen\n");
    draw_rectangle(fd, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    oled_print_str(fd, "Game Over", 20, 5, true);
    
    char score_str[20];
    snprintf(score_str, sizeof(score_str), "Score: %d", game_info.points);
    oled_print_str(fd, score_str, 30, 25, true);
    
    oled_print_str(fd, "Restart?", 30, 45, true);
    update_oled_display(fd);
}