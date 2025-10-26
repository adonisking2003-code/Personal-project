// This file code game flappy bird in the oled
#include <stdio.h>
#include <pthread.h>
#include "app.h"
#include "ssd1306.h"
#include "input_thread.h"
#include "game_logic_thread.h"
#include "render_thread.h"
#include "seven_segment.h"
#include "button.h"
#include "utils.h"

void *input_thread_func(void*);
void *game_logic_thread_func(void*);
void *render_thread_func(void*);

void app_run(void)
{
    pthread_t input_thread, logic_thread, render_thread;
    int fd = ssd1306_init();
    if( fd < 0 )
    {
        printf("SSD1306 initialize failed\n ");
        return;
    }

    pthread_create(&input_thread, NULL, input_thread_func, (void*)fd);
    pthread_create(&logic_thread, NULL, game_logic_thread_func, (void*)fd);
    pthread_create(&render_thread, NULL, render_thread_func, (void*)fd);

    pthread_join(input_thread, NULL);
    pthread_join(logic_thread, NULL);
    pthread_join(render_thread, NULL);
}
