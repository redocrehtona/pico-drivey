#include "drv.hpp"

// Constructor
DRV::DRV() {
    // Constructor implementation
}

// Method to get the raw 16-bit value of the duty cycle
uint16_t DRV::getDuty() {
    return 0x7FFF; 
}

// Method to get the duty cycle as a percentage
uint8_t DRV::getDutyPercent() {
    uint16_t duty = getDuty();
    uint8_t percent = (duty * 100) / 0xFFFF; // = 65535 = max value of uint16_t
    return percent;
}

// Method to set the duty cycle, returns true if successful
bool DRV::setDuty(uint16_t duty) {
    
    return true;
}