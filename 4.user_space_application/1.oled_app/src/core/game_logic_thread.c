// Purpose: Compute game state updates, physics, collision detection, score, and rules.

#include <stdio.h>
#include <unistd.h>
#include "game_logic_thread.h"

extern volatile int input_state;    // From input_thread.c
volatile int game_state             = 0;        // Shared game state
struct stBirdInfo bird;
struct stColumnList col_list;
struct stGameInfo game_info;
int column_spacing                  = 30;
uint8_t game_velocity               = 10;

void *game_logic_thread_func(void *arg)
{
    while(1)
    {
        if( input_state )
        {
            game_state++;
        }
        usleep(500000); // Run at 2Hz, adjust as needed
    }
    return NULL;
}

void move_up(struct stBirdInfo *bird)
{
	bird->bird_y++;
}

void move_down(struct stBirdInfo *bird)
{
	bird->bird_y--;
}

int check_bird_collision(struct stBirdInfo *bird, struct stColumnList *col_list)
{
    while(col_list != NULL)
    {
        if(bird->bird_x == col_list->col.column_x)
        {
            if(bird->bird_y > col_list->col.column_bottom_y && bird->bird_y < col_list->col.column_top_y)
            {
                return -1;
            }
        }
        col_list = col_list->next_column;
    }
    return 0;
}

void create_column(struct stColumnList *col_list)
{
    
}

void update_column(struct stColumnList *col_list)
{
    // Move column

    // Check if column out then create new column

}

void increase_point(struct stGameInfo *game_point)
{
    game_point->points++;
}

stBirdInfo init_bird(struct stBirdInfo *bird, uint8_t bird_x, uint8_t bird_y, uint8_t bird_h, uint8_t bird_w, uint8_t bird_accel)
{
    bird->bird_x = 10;
    bird->bird_y = 30;
    bird->bird_height = 10;
    bird->bird_width = 15;
    bird->bird_acceleration = 5;
}

void init_column(struct stColumnList *stColLst)
{
    stColumnInfo column = {.column_x=100, .column_top_y = 20, .column_bottom_y = 20};
    stColLst->col = column;
    stColLst->next_column = NULL;
}

void init_game_info(struct stGameInfo *game_info)
{
    game_info->points = 0;
    game_info->status = GAME_READY;
    game_info->time_play = 0;
}

void init_game(uint8_t game_vel)
{
    // Init bird
    // init_bird(&bird);

    // Init column
    // init_column(&col_list);

    // Init game status
    // init_game_info(&game_info);
}