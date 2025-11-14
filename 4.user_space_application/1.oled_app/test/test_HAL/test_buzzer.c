#include "buzzer.h"
#include "utils.h"

int main()
{
    int ret = buzzer_init(12);
    if(ret != APP_RET_OK)
    {
        return ret;
    }
    buzzer_run(BUZZER_MODE_OFF);
    buzzer_deinit(12);
    return 0;

    // e_app_return_t buzzer_control(int duty_cycle, int period)

}