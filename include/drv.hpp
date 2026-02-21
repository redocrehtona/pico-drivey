# ifndef DRV_HPP
# define DRV_HPP
#include <cstdint>  // For inetegers of specific sizes
#include "hardware/pwm.h" // For PWM control

class DRV {
    public:
        DRV();
        uint16_t getDuty();
        uint8_t getDutyPercent();
        bool setDuty(uint16_t duty);
};

# endif