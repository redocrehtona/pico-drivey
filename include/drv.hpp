#ifndef DRV_HPP
#define DRV_HPP
#include <cstdint>  // For inetegers of specific sizes

class DRV {
    private:
        uint8_t _slice_a;
        uint8_t _slice_b;
        uint8_t _channel_a;
        uint8_t _channel_b;
        uint32_t _wrap;
        int _eep_pin;
    public:
        DRV(uint8_t pin_a, 
            uint8_t pin_b, 
            uint32_t freq, 
            float clkdiv, 
            int eep_pin = -1, 
            bool eep_state = false);
        uint16_t getDuty();
        uint8_t getDutyPercent();
        bool setDuty(uint16_t duty, bool direction = 1); // direction: 1 for forward, 0 for reverse
        void enable(bool state);
};

#endif