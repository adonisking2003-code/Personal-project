// Purpose: Handle user input asynchronously from hardware buttons, keyboard, or other controls.

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <input_thread.h>
#include <button.h>
#include "utils.h"

static uint8_t button_gpio;

void *thread_read_button(void* arg)
{
    PRINTF_INFO("Input thread started!\n");
    int*val = (int*)arg;
    while(1)
    {
        button_read_state(*val);
        delay_ms(50);
    }
}

void *input_thread_func(void *arg)
{
    initial_input_thread(BUTTON_LINE_OFFSET);
}

int initial_input_thread(int button_gpio_input)
{
    button_gpio = button_gpio_input;
    pthread_t pid1;

    if(pthread_create(&pid1, NULL, thread_read_button, &button_gpio) != 0)
    {
        perror("[ERROR]: Cannot create thread1!\n");
        return -1;
    }

    pthread_join(pid1, NULL);
    return 0;
}