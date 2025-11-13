
#ifndef PWM_CONTROL_H
#define PWM_CONTROL_H

#include <stdint.h>
#include "utils.h"

int pwm_init(const char *device_path);
int pwm_deinit(void);
int pwm_set_duty(int duty_cycle);
int pwm_set_period(int period);

#endif // PWM_CONTROL_H