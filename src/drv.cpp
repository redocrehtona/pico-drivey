#include "drv.hpp"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <hardware/clocks.h>

// Constructor
DRV::DRV(uint8_t pin_a, uint8_t pin_b, uint32_t freq, float clkdiv, int eep_pin, bool eep_state)
{
    // Configure sleep pin if used
    if (eep_pin >= 0)
    {
        gpio_init(eep_pin);
        gpio_set_dir(eep_pin, GPIO_OUT);
        gpio_put(eep_pin, eep_state);
    }

    // Set pins as PWM outputs
    gpio_set_function(pin_a, GPIO_FUNC_PWM);
    gpio_set_function(pin_b, GPIO_FUNC_PWM);

    // Get the PWM slice number for each pin
    slice_a = pwm_gpio_to_slice_num(pin_a);
    slice_b = pwm_gpio_to_slice_num(pin_b);

    // Caclulate PWM channel, A is even pins, B is odd pins
    channel_a = pin_a % 2; // 0 for even pins, 1 for odd pins
    channel_b = pin_b % 2; // 0 for even pins, 1 for odd pins

    // Set clock devider
    pwm_set_clkdiv(slice_a, clkdiv);
    pwm_set_clkdiv(slice_b, clkdiv);

    // Get the clock speed of the system (in Hz)
    uint32_t clock_freq = clock_get_hz(clk_sys);

    // Calculate the PWM wrap value for the desired frequency
    wrap = clock_freq / (freq * clkdiv) - 1; // -1 because counter begins at 0

    // Configure the PWM slices with the given frequency
    pwm_set_wrap(slice_a, wrap);
    pwm_set_wrap(slice_b, wrap);

    // Begin motor in coasting mode (0% duty cycle)
    pwm_set_chan_level(slice_a, channel_a, 0);
    pwm_set_chan_level(slice_b, channel_b, 0);

    // Enable the PWM slices
    pwm_set_enabled(slice_a, true);
    pwm_set_enabled(slice_b, true);
}

// Method to get the raw 16-bit value of the duty cycle
uint16_t DRV::getDuty()
{
    return 0x7FFF;
}

// Method to get the duty cycle as a percentage
uint8_t DRV::getDutyPercent()
{
    uint16_t duty = getDuty();
    uint8_t percent = duty * 100 / 0xFFFF; // = 65535 = max value of uint16_t
    return percent;
}

// Method to set the duty cycle, returns true if successful
bool DRV::setDuty(uint16_t duty, bool direction)
{
    duty = duty * wrap / 0xFFFF; // Scale duty to the wrap value
    if (direction)
    {
        pwm_set_chan_level(slice_a, channel_a, duty);
        pwm_set_chan_level(slice_b, channel_b, 0);
    }
    else
    {
        pwm_set_chan_level(slice_a, channel_a, 0);
        pwm_set_chan_level(slice_b, channel_b, duty);
    }
    return true;
}