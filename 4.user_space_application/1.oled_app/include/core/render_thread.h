#ifndef RENDER_THREAD_H
#define RENDER_THREAD_H

#include "game_logic_thread.h"

typedef enum {
    GAME_STATE_START,
    GAME_STATE_PLAYING,
    GAME_STATE_GAMEOVER
} GameState;

void *render_thread_func(void *arg);
void draw_column(int fd, struct stColumnList *col_list);
void draw_bird(int fd, struct stBirdInfo *bird);

void start_screen(int fd);
void end_screen(int fd);

#endif // RENDER_THREAD_H