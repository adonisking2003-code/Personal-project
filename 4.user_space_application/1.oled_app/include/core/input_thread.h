#ifndef INPUT_THREAD_H
#define INPUT_THREAD_H

void *input_thread_func(void *arg);

/**
 * @brief Initialize and start the input reading thread for the specified button GPIO.
 *
 * This function creates and launches a thread that continuously reads the button state
 * from the specified GPIO pin and processes button input events.
 *
 * @param button_gpio_input The GPIO number for the button input.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int initial_input_thread(int button_gpio_input);

#endif // INPUT_THREAD_H