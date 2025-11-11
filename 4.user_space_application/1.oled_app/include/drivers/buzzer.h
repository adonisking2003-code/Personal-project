#ifndef BUZZER_H
#define BUZZER_H

#include "utils.h"

#define BUZZER_GPIO 12 // PWM0

e_app_return_t buzzer_init(uint8_t buzzer_gpio);

e_app_return_t buzzer_setup();

e_app_return_t buzzer_control();

e_app_return_t buzzer_deinit();

#endif // BUZZER_H