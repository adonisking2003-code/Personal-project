// Purpose: Compute game state updates, physics, collision detection, score, and rules.

#include <stdio.h>
#include <unistd.h>
#include "game_logic_thread.h"
#include "button.h"

eGameState game_state = GAME_STATE_READY;
bool render_flag = false;

struct  stBirdInfo bird;
struct  stColumnInfo col_info;
struct  stGameInfo game_info;
int     column_spacing              = 30;
uint8_t game_speed                  = 5;    

void *game_logic_thread_func(void *arg)
{
    while(1)
    {
        if(game_state == GAME_STATE_READY)
        {
            // Wait for user input to start the game
            if(button_is_pressed(BUTTON_LINE_OFFSET))
            {
                game_state = GAME_STATE_PLAYING;
                render_flag = true;
            }
        }
        else if(game_state == GAME_STATE_PLAYING)
        {
            // Update bird position
            if(button_is_pressed(BUTTON_LINE_OFFSET))
            {
                move_up(&bird);
            }
            else
            {
                move_down(&bird);
            }

            // Update columns
            update_column(&col_info);

            // Check for collisions
            if(check_bird_collision(&bird, &col_info) < 0)
            {
                game_state = GAME_STATE_OVER;
                render_flag = true;
            }

            // Update score
            if(game_state == GAME_STATE_PLAYING)
            {
                increase_point(&game_info);
            }
        }
        else if(game_state == GAME_STATE_OVER)
        {
            // Handle game over state
            if(button_is_pressed(BUTTON_LINE_OFFSET))
            {
                init_game(game_speed);
                game_state = GAME_STATE_READY;
                render_flag = true;
            }
        }
    }
    return NULL;
}

void move_up(struct stBirdInfo *bird)
{
	// bird->bird_y+=bird_acceleration;
}

void move_down(struct stBirdInfo *bird)
{
	// bird->bird_y-=bird_acceleration;
}

int check_bird_collision(struct stBirdInfo *bird, struct stColumnInfo *col_info)
{
    // while(col_list != NULL)
    // {
    //     if(bird->bird_x == col_list->col.column_x)
    //     {
    //         if(bird->bird_y > col_list->col.column_bottom_y && bird->bird_y < col_list->col.column_top_y)
    //         {
    //             return -1;
    //         }
    //     }
    //     col_list = col_list->next_column;
    // }
    return 0;
}

static stColumnInfo init_column(uint8_t column_x, uint8_t column_top_y, uint8_t column_bottom_y)
{
    stColumnInfo column = {.column_x=column_x, .column_top_y = column_top_y, .column_bottom_y = column_bottom_y};
    return column;
}

void create_column()
{
    // Create a new column and add it to the column list
    struct stColumnInfo new_column = init_column(100, 20, 20);
    col_info = new_column;
}

void update_column(struct stColumnInfo *col_info)
{
    // Move column
    // col_list->col.column_x -= game_speed;
    col_info->column_x -= game_speed;
    // Check if column out then create new column

}

void increase_point(struct stGameInfo *game_point)
{
    game_point->points++;
}

stBirdInfo init_bird(uint8_t bird_x, uint8_t bird_y, uint8_t bird_h, uint8_t bird_w, uint8_t bird_accel)
{
    stBirdInfo bird;
    bird.bird_x = bird_x;
    bird.bird_y = bird_y;
    bird.bird_height = bird_h;
    bird.bird_width = bird_w;
    bird.bird_acceleration = bird_accel;
    return bird;
}

void init_game_info(struct stGameInfo *game_info)
{
    game_info->points = 0;
    game_info->status = GAME_STATE_READY;
    game_info->time_play = 0;
}

void init_game(uint8_t game_speed)
{
    // Init bird
    bird = init_bird(10, 30, 10, 15, 5);

    // Init column
    col_info = init_column(100, 20, 20);

    // Init game status
    init_game_info(&game_info);
}

void update_game_play(int fd)
{
    // Update game play status if needed
    if(game_state == GAME_STATE_PLAYING)
    {
        // e.g., increase time played
        game_info.time_play++;
    }
}