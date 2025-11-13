#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

int pwm_init(void);
void pwm_exit(void);
int pwm_set_duty(int duty_percent);

#endif // _PWM_DRIVER_H_