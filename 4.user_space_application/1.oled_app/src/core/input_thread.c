// Purpose: Handle user input asynchronously from hardware buttons, keyboard, or other controls.

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <input_thread.h>
#include <button.h>
#include <utils.h>

void *thread_read_button(void* arg)
{
    int*val = (int*)arg;
    while(1)
    {
        button_read_state(*val);
        delay_ms(100);
    }
}

void *input_thread_func(void *arg)
{
    while(1)
    {
        // Poll or wait for input (e.g., read buttons)
        sleep(2);
    }
    return NULL;
}

int initial_input_thread(int button_gpio)
{
    pthread_t pid1;

    if ( button_init(button_gpio) < 0)
    {
        printf("[ERROR]: Cannot initialize button GPIO %d !\n", button_gpio);
        return -1;
    }

    if(pthread_create(&pid1, NULL, thread_read_button, &button_gpio) != 0)
    {
        perror("[ERROR]: Cannot create thread1!\n");
        return -1;
    }

    pthread_detach(pid1); // Detach the thread to avoid memory leaks
    return 0;
}