#ifndef CONFIG_HPP
#define CONFIG_HPP

// motor0 configuration
#define MOTOR0_PIN1 2
#define MOTOR0_PIN2 3
#define MOTOR0_FREQ 1 * 1000 // 1 kHz
#define MOTOR0_CLKDIV 10.0f // Clock divider for PWM, higher values allow for lower frequencies but reduce resolution
#define MOTOR0_EEP 5 // Sleep pin, set to -1 if not used
#define MOTOR0_EEP_STATE true // State of pin to enable driver, true for HIGH

#endif