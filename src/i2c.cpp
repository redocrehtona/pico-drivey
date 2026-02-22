#include "i2c.hpp"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/i2c_slave.h"

I2C::I2C(i2c_inst_t* i2c_port, uint sda_pin, uint scl_pin, uint8_t slave_addr) : i2c_port(i2c_port) {
    inst[i2c_hw_index(i2c_port)] = this;
    // Initialize I2C
    i2c_init(i2c_port, 100 * 1000); // 100 kHz
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    i2c_slave_init(i2c_port, slave_addr, &handler);
}

void I2C::handler(i2c_inst_t *i, i2c_slave_event_t e) {
    auto* s = inst[i2c_hw_index(i)];
    if (!s) return;

    static bool is_first_byte = true; // Tracks the Python "Offset" byte

    switch (e) {
        case I2C_SLAVE_RECEIVE: {
            uint8_t b = i2c_read_byte_raw(i);
            
            if (is_first_byte) {
                // This is the 0x00 Python sends. Ignore it.
                is_first_byte = false;
            } else {
                if (s->highByte) {
                    s->inVal = b << 8;
                } else {
                    s->inVal |= b;
                    s->newData = true; // Only set true when a FULL 16-bit word is ready
                }
                s->highByte = !s->highByte;
            }
            break;
        }

        case I2C_SLAVE_REQUEST:
            i2c_write_byte_raw(i, s->highByte ? (s->outVal >> 8) : (s->outVal & 0xFF));
            s->highByte = !s->highByte;
            break;

        case I2C_SLAVE_FINISH:
            s->highByte = true;    // Reset byte toggle
            is_first_byte = true;  // Reset offset tracker for next transaction
            break;

        default:
            break;
    }
}

uint16_t I2C::read() {
    newData = false;
    return inVal;
}

I2C* I2C::inst[2] = {nullptr, nullptr};