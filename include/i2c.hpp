#ifndef I2C_HPP
#define I2C_HPP

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/i2c_slave.h"

class I2C {
    private:
        i2c_inst_t* i2c_port;
        uint16_t inVal = 0;
        bool highByte = true;
        static I2C* inst[2];
        static void handler(i2c_inst_t *i, i2c_slave_event_t e);
    public:
        bool newData = false;
        uint16_t outVal = 0;
        I2C(i2c_inst_t* i2c_port, uint sda_pin, uint scl_pin, uint8_t slave_addr);
        uint16_t read();
};

#endif