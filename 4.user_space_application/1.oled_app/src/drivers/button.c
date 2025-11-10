#include <button.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "utils.h"

/* mutex declare */
pthread_mutex_t mutex_button = PTHREAD_MUTEX_INITIALIZER;

static uint8_t btn_state_1 = 0;
static uint8_t btn_state_2 = 0;
static uint8_t btn_state_3 = 0;
static int btn_fd;
stButtonInfo *btn_20;

int button_init(int gpio)
{
    if(gpio == 20)
    {
        btn_fd = open("/dev/etx_device", O_RDONLY);
        if (btn_fd < 0) {
            perror("[ERROR]: Open device failed");
            return -1;
        }
        btn_20 = (stButtonInfo*) malloc(sizeof(stButtonInfo));
        if (btn_20 == NULL) {
            perror("[ERROR]: Failed to allocate memory for btn_20");
            close(btn_fd);
            return -1;
        }
        printf("[INFO]: Button %d init success!\n", gpio);
    }
    return 0;
}

void button_deinit(int gpio)
{
    if(gpio == 20 && btn_fd > 0)
    {
        free(btn_20);
        btn_20 = NULL;
    }
    close(btn_fd);
}

void button_read_state(int gpio)
{
    // process button debounce logic
    pthread_mutex_lock(&mutex_button);
    if(btn_fd<0)
    {
        PRINTF_DEBUG("read btn_%d failed - not initial button!\n", gpio);
    }
    int read_btn = read(btn_fd, &btn_state_3, 1); // read 1 byte from device
    if(read_btn < 0)
    {
        printf("[DEBUG]: read btn_%d failed, return: %d!\n", gpio, read_btn);
    }
    else if(btn_state_3 == 1)
    {
        if(btn_state_1 == btn_state_2 && btn_state_2 == btn_state_3)
        {      
            if (btn_20 != NULL) {
                btn_20->pressed = true;
            }
        }
    }
    btn_state_1 = btn_state_2;
    btn_state_2 = btn_state_3;
    pthread_mutex_unlock(&mutex_button);
}

int button_is_pressed(int gpio)
{
    pthread_mutex_lock(&mutex_button);
    if(!btn_fd) return -1;
    if(btn_20->pressed == true)
    {
        btn_20->pressed = false;
        pthread_mutex_unlock(&mutex_button);
        return 1;
    }
    pthread_mutex_unlock(&mutex_button);
    return 0;
}

/**
 * @brief Register callback functions for a button's press and release events
 *
 * This function allows the caller to specify two callback functions:
 * one to be called when the button with button_id is pressed,
 * and another to be called when it is released.
 *
 * @param button_id The identifier of the button
 * @param press_callback Pointer to the function called on button press (can be NULL)
 * @param release_callback Pointer to the function called on button release (can be NULL)
 *
 * @return 0 on success, negative error code on failure
 *
 * @note Should store callbacks internally and invoke them from input polling or interrupt.
 */
int button_register_callback(int button_id, void (*press_callback)(void), void (*release_callback)(void))
{
    // TODO: Implement storage and invocation of callbacks for button_id
    return 0;
}

/**
 * @brief Register callback functions for a button's press and release events
 *
 * This function allows the caller to specify two callback functions:
 * one to be called when the button with button_id is pressed,
 * and another to be called when it is released.
 *
 * @param button_id The identifier of the button
 * @param press_callback Pointer to the function called on button press (can be NULL)
 * @param release_callback Pointer to the function called on button release (can be NULL)
 *
 * @return 0 on success, negative error code on failure
 *
 * @note Should store callbacks internally and invoke them from input polling or interrupt.
 */
int button_unregister_callback(int button_id)
{
    // TODO: Implement removal of stored callbacks for button_id
    return 0;
}

/**
 * @brief Poll buttons to update their states
 *
 * This function should be called regularly (e.g., in main loop or timer)
 * to check button states, detect state changes, and trigger callbacks.
 *
 * Handles debouncing and calls registered press/release callbacks as needed.
 */
void button_poll(void)
{
    // TODO: Implement button polling, debouncing, and callback invocation
}
