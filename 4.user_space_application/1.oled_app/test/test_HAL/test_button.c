#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "/home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include/drivers/ssd1306_data.h"
#include "/home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include/utils/utils.h"
#include "/home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include/drivers/ssd1306.h"
#include "/home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include/drivers/button.h"

void *thread_read_button(void* arg)
{
    int*val = (int*)arg;
    while(1)
    {
        button_read_state(*val);
        delay_ms(100);
    }
}

void *thread_check_button(void *arg)
{
    int *val = (int*)arg;
    while(1)
    {
        if(button_is_pressed(*val) == 1)
        {
            printf("[INFO]: button is pressed! \n");
        } else {
            printf("[INFO]: button is not pressed! \n");
        }
        delay_ms(1000);
    }
}

int main()
{
    pthread_t pid1, pid2;

    int btn_gpio = 20;
    if ( button_init(btn_gpio) < 0)
    {
        printf("[ERROR]: Cannot initialize button GPIO %d !\n", btn_gpio);
        return -1;
    }

    if(pthread_create(&pid1, NULL, thread_read_button, &btn_gpio) != 0)
    {
        perror("[ERROR]: Cannot create thread1!\n");
        return 1;
    }

    if(pthread_create(&pid2, NULL, thread_check_button, &btn_gpio) != 0)
    {
        perror("[ERROR]: Cannot create thread2!\n");
        return 1;
    }

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);

    button_deinit(btn_gpio);

    return 0;
}