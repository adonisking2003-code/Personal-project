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

void oledPrint(int fd, const char* text, int x, int y)
{
    // set currsor at (x, y)

    // update each char to buffer

}