#ifndef RENDER_THREAD_H
#define RENDER_THREAD_H

#include "game_logic_thread.h"

void *render_thread_func(void *arg);
void draw_column(int fd, struct stColumnList *col_list);
void draw_bird(int fd, struct stBirdInfo *bird);

void start_screen();
void end_screen();

#endif // RENDER_THREAD_H