#ifndef CFG_HPP
#define CFG_HPP

// motor0 configuration
#define MOTOR0_PIN1 2
#define MOTOR0_PIN2 3
#define MOTOR0_FREQ 1 * 1000 // 1 kHz
#define MOTOR0_CLKDIV 10.0f // Clock divider for PWM, higher values allow for lower frequencies but reduce resolution
#define MOTOR0_EEP 5 // Sleep pin, set to -1 if not used
#define MOTOR0_EEP_STATE false // State of sleep pin at startup (false = LOW, usually sleep mode)

// enc0 configuration
#define ENC0_PIN1 28 // A phase pin, B phase must be connected to the next pin (29 in this case)
#define ENC0_RPM_SCALE 300 // pulses per revolution (30) * gear ratio (10)

// pid0 configuration
#define PID0_KP 50
#define PID0_KI 10
#define PID0_KD 2
#define PID0_SCALE 100
#define PID0_MIN_OUTPUT 0
#define PID0_MAX_OUTPUT 0xFFFF

// i2c0 configuration
#define I2C0_PORT i2c0
#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1
#define I2C0_SLAVE_ADDR 0x42

#endif