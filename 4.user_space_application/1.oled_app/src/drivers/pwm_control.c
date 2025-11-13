#include "pwm_control.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static int pwm_fd = -1;

int pwm_init(const char *device_path) {
    pwm_fd = open(device_path, O_RDWR);
    if (pwm_fd < 0) {
        perror("Failed to open PWM device");
        return APP_RET_ERR_IO_DEV_OPEN;
    }
    return APP_RET_OK;
}

int pwm_deinit(void) {
    if (pwm_fd >= 0) {
        if (close(pwm_fd) < 0) {
            perror("Failed to close PWM device");
            return APP_RET_ERR_IO_DEV_CLOSE;
        }
        pwm_fd = -1;
    }
    return APP_RET_OK;
}

int pwm_set_duty(int duty_cycle) {
    if (duty_cycle < 0 || duty_cycle > 100) {
        return APP_RET_ERR_INVALID_PARAM;
    }
    char buf[16];
    snprintf(buf, sizeof(buf), "DUTY:%d\n", duty_cycle);
    if (write(pwm_fd, buf, strlen(buf)) < 0) {
        perror("Failed to write duty cycle");
        return APP_RET_ERR_PWM_DUTY;
    }
    return APP_RET_OK;
}

int pwm_set_period(int period) {
    if (period <= 0) {
        return APP_RET_ERR_INVALID_PARAM;
    }
    char buf[32];
    snprintf(buf, sizeof(buf), "PERIOD:%d\n", period);
    if (write(pwm_fd, buf, strlen(buf)) < 0) {
        perror("Failed to write period");
        return APP_RET_ERR_PWM_PERIOD;
    }
    return APP_RET_OK;
}