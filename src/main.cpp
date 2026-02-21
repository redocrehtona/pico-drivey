#include <stdio.h>
#include "pico/stdlib.h"
#include "drv.hpp"

int main()
{
    DRV motor0;
    
    stdio_init_all();

    while (true) {
        printf("%d\n", motor0.getSpeed());
        sleep_ms(1000);
    }
}
