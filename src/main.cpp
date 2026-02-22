#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "drv.hpp"
#include "enc.hpp"
#include "cfg.hpp"
#include "pid.hpp"
#include "i2c.hpp"

void core1_main()
{
    ENC enc0(ENC0_PIN1); // encoder pin 2 must be the next pin on from encoder pin 1
                         //  (e.g. if pin 1 is 28, pin 2 must be 29)
    while (true)
    {
        enc0.update();
        multicore_fifo_push_timeout_us(enc0._state.speed / ENC0_RPM_SCALE, 0);
        sleep_ms(5);
    }
}

int main()
{
    stdio_init_all();

    // sleep_ms(10000);
    DRV motor0(MOTOR0_PIN1, MOTOR0_PIN2, MOTOR0_FREQ, MOTOR0_CLKDIV, MOTOR0_EEP, MOTOR0_EEP_STATE);

    multicore_launch_core1(core1_main);

    PID pid0(PID0_KP, PID0_KI, PID0_KD, PID0_SCALE, PID0_MIN_OUTPUT, PID0_MAX_OUTPUT);

    I2C _i2c0(I2C0_PORT, I2C0_SDA_PIN, I2C0_SCL_PIN, I2C0_SLAVE_ADDR);

    while (true)
    {
        /// I2C input handling
        if (_i2c0.newData){
            uint16_t i2cTarget = _i2c0.read();
            pid0.setRPM(i2cTarget);
            printf("New target RPM: %d\n", i2cTarget);
            _i2c0.newData = false;
        }

        if (pid0.getTargetRPM() == 0) {
            motor0.enable(false); // Disable the driver (coast) if target speed is 0
        } else {
            motor0.enable(true); // Enable the driver otherwise
        }

        /// FIFO
        // Drain the FIFO to get the latest speed value from core1
        while (multicore_fifo_rvalid())
        {
            multicore_fifo_pop_blocking();
        }

        // Imedietly after draining, use the next value
        uint32_t speedNow = multicore_fifo_pop_blocking();

        /// PID calculation and motor control

        uint16_t duty = pid0.calculate(speedNow);

        motor0.setDuty(duty);

        _i2c0.outVal = speedNow;

        sleep_ms(5);
    }
}
