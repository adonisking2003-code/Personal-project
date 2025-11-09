#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

#define GPIO_CHIP           "/dev/gpiochip0"
#define BUTTON_LINE_OFFSET  20

/* Mutex create */
extern pthread_mutex_t mutex_button;

typedef struct stButtonInfo {
    bool pressed;
} stButtonInfo;

int button_init(int gpio);
void button_deinit(int gpio);

void button_read_state(int gpio);
int button_is_pressed(int gpio);

int button_register_callback(int button_id, void (*press_callback)(void), void (*release_callback)(void));
int button_unregister_callback(int button_id);

void button_poll(void);

#endif // BUTTON_H