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

void oled_run_test()
{
    printf("[Start]: Test oled! \n");
    int fd = ssd1306_init();
    if( fd != 0 )
    {
        printf("SSD1306 initialize failed\n ");
        return;
    }

    uint8_t buffer[8][128];
    for (int i = 0; i < 4; i++) {
        for(int j=0; j<128; j++)
        {
            buffer[i][j] = 0xFF;
        }
    }
    write_display_buffer(fd, &buffer[0][0], 8*128);
    delay_ms(5000);
    clear_display(fd);
    draw_pixel(64, 32, 0xFF);
    update_oled_display(fd);
    delay_ms(5000);
    ssd1306_close(fd);
    printf("[End]: Test oled! \n");
}

void seven_segment_run_test(void)
{
    printf("[Start]: Test seven segment! \n");
    seven_segment_init();
    for(int i=0; i<10; i++)
    {
        seven_segment_display_digit(i);
        delay_ms(1000);
    }
    seven_segment_clear();
    seven_segment_close();
    printf("[End]: Test seven segment! \n");
}

void button_test(void)
{
    printf("[Start]: Test button! \n");
    int count = 0;
    if ( button_init(BUTTON_GPIO) == 0 )
    {
        printf("[INFO]: BUTTON GPIO %d initialize success!\n", BUTTON_GPIO);
    }
    else {
        printf("[ERROR]: Cannot initialize BUTTON GPIO: %d !\n", BUTTON_GPIO);
    }

    while(1)
    {
        if(button_is_pressed(BUTTON_GPIO) == 1)
        {
            printf("[Check]: Button is pressed, Count = %d ! \n", count);
            count++;
            if(count>50) break;
        }
        button_read_state(BUTTON_GPIO);
        delay_ms(10);
    }
    button_deinit(BUTTON_GPIO);
    printf("[End]: Test button! \n");
}

void *input_thread_func(void*);
void *game_logic_thread_func(void*);
void *render_thread_func(void*);

void app_run(void)
{
    pthread_t input_thread, logic_thread, render_thread;

    pthread_create(&input_thread, NULL, input_thread_func, NULL);
    pthread_create(&logic_thread, NULL, game_logic_thread_func, NULL);
    pthread_create(&render_thread, NULL, render_thread_func, NULL);

    pthread_join(input_thread, NULL);
    pthread_join(logic_thread, NULL);
    pthread_join(render_thread, NULL);
}

void app_run_test(void)
{
    oled_run_test();
}