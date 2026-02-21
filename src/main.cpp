#include <stdio.h>
#include "pico/stdlib.h"
#include "drv.hpp"
#include "config.hpp"

int main()
{

    stdio_init_all();


    //sleep_ms(10000);
    DRV motor0(MOTOR0_PIN1, MOTOR0_PIN2, MOTOR0_FREQ, MOTOR0_CLKDIV, MOTOR0_EEP, MOTOR0_EEP_STATE);


    while (true) {
        printf("%d\n", motor0.getDuty());
        printf("%d\n", motor0.getDutyPercent());
        printf("%d\n", motor0.setDuty((uint16_t)(0.5 * 0xFFFF)));
        sleep_ms(1000);
    }
}
