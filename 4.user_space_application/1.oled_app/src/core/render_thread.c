// Purpose: Draw the current game state on the SSD1306 OLED or other display

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "render_thread.h"
#include "ssd1306_data.h"
#include "game_logic_thread.h"
#include "utils.h"

// extern eGameState game_state;
// extern bool render_flag;
extern struct stBirdInfo bird;
extern struct stColumnInfo col_top_info;
extern struct stColumnInfo col_bottom_info;
extern struct stGameInfo game_info;

void *render_thread_func(void *arg)
{
    PRINTF_INFO("Render thread started\n");
    int *fd_ptr = (int*)arg;
    int fd = *fd_ptr;
    bool render_flag_local;
    uint8_t game_state_local = -1;
    while(1)
    {
        get_render_flag_and_state(&render_flag_local, &game_state_local);
        if(render_flag_local)
        {
            if(game_state_local == GAME_STATE_READY)
            {

                start_screen(fd);
            }
            else if(game_state_local == GAME_STATE_PLAYING)
            {
                pthread_mutex_lock(&mutex_game_logic);
                oled_clear_display(fd);
                draw_bird(fd, &bird);
                draw_column(fd, &col_top_info);
                draw_column(fd, &col_bottom_info);
                draw_score(fd, game_info.points);
                update_oled_display(fd);
                update_game_play(fd);
                pthread_mutex_unlock(&mutex_game_logic);
            }
            else if(game_state_local == GAME_STATE_OVER)
            {
                end_screen(fd);
            }
            set_game_state_and_render_flag(game_state_local, false);
        }
        delay_ms(100); // Run at 30Hz (30 frame/s), adjust if needed
    }
    return NULL;
}

void draw_column(int fd, struct stColumnInfo *col_info)
{
    uint8_t column_width  = 20;
    uint8_t column_height = col_info->column_bottom_y - col_info->column_top_y;
    // draw_bit_map(fd, col_info->column_x, col_info->column_top_y, &column_simple[0], column_width, column_height, true);
    draw_rectangle(fd, col_info->column_x, col_info->column_top_y, column_width, column_height, 5, false);
}

void draw_bird(int fd, struct stBirdInfo *bird)
{
    // draw_bit_map(fd, bird->bird_x, bird->bird_y, &flappyBird_bitmap[0], 18, 22, true);
    draw_rectangle(fd, bird->bird_x, bird->bird_y, bird->bird_width, bird->bird_height, 5, false);
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
    draw_rectangle(fd, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 5, false);
    oled_print_str(fd, "FLAPPY BIRD", 10, 10, true);
    oled_print_str(fd, "Start!", 39, 35, true);
    update_oled_display(fd);
}

void end_screen(int fd)
{
    oled_clear_display(fd);
    printf("End Screen\n");
    draw_rectangle(fd, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 5, false);
    oled_print_str(fd, "Game Over", 20, 5, true);
    
    char score_str[20];
    snprintf(score_str, sizeof(score_str), "Score: %d", game_info.points);
    oled_print_str(fd, score_str, 30, 25, true);
    
    oled_print_str(fd, "Restart?", 30, 45, true);
    update_oled_display(fd);
}