#include "buzzer.h"

int fd_buzzer;

static pthread_t buzzer_thread;
static e_buzzer_mode current_mode = BUZZER_MODE_OFF;


void *buzzer_task(void *arg) {
    while (1) {
        switch (current_mode) {
            case BUZZER_MODE_OFF:
                buzzer_control(0, DEFAULT_PERIOD);
                break;
            case BUZZER_MODE_ON:
                buzzer_control(50, DEFAULT_PERIOD);
                usleep(200000); // 200ms
                current_mode = BUZZER_MODE_OFF;
                break;
            case BUZZER_MODE_BEEP:
                buzzer_control(50, DEFAULT_PERIOD);
                usleep(100000);
                buzzer_control(0, DEFAULT_PERIOD);
                usleep(100000);
                break;
            case BUZZER_MODE_PLAY_MELODY:
                // Loop qua mảng melody
                int melody[] = {1000, 1500, 2000}; // Hz
                for (i = 0; i < size; i++) {
                    buzzer_control(50, melody[i]);
                    usleep(300000); // 300ms mỗi nốt
                }
                break;
        }
    }
}

e_app_return_t buzzer_init(uint8_t buzzer_gpio)
{
    int ret = pwm_init("/dev/pwm_device");
    return ret;
}

e_app_return_t buzzer_run(e_buzzer_mode mode)
{
    switch(mode)
    {
        case BUZZER_MODE_OFF:
            buzzer_control(0, 0);
        break;
        case BUZZER_MODE_ON:
            buzzer_control(50, 0);
            delay_ms(200);
            buzzer_control(0, 0);
        break;
        case BUZZER_BEEP:
            while(true)
            {
                buzzer_control(50, 0);
                delay_ms(100);
                buzzer_control(0, 0);
                delay_ms(100);
            }
        break;
        case BUZZER_PLAY_MELODY:
            int melody[] = {1000, 1500, 2000}; // Hz
            for (i = 0; i < size; i++) {
                buzzer_control(50, melody[i]);
                usleep(300000); // 300ms mỗi nốt
            }
        break;
        default:
            return APP_RET_ERR_IO_DEV_CLOSE;
        break;
    }
    return APP_RET_OK;
}

e_app_return_t buzzer_control(int duty_cycle, int period)
{
    if(duty >= 0)
    {
        pwm_set_duty(duty);
    }
    if(period > 0)
    {
        pwm_set_period(period);
    }
    return APP_RET_OK;
}

e_app_return_t buzzer_deinit(uint8_t buzzer_gpio)
{
    int ret = pwm_deinit();
    return ret;
}
