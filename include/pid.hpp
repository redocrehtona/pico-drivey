#ifndef PID_HPP
#define PID_HPP

#include "pico/stdlib.h"

class PID {
public:
    PID(int32_t kp, int32_t ki, int32_t kd, int32_t scale, int32_t minOutput, int32_t maxOutput);

    void setRPM(int32_t target);
    int32_t calculate(int32_t currentRPM);
    void reset();
    int32_t getTargetRPM();

private:
    int32_t kp, ki, kd, scale;
    int32_t targetRPM;
    int32_t minOutput, maxOutput;

    int32_t integral;
    int32_t prevError;
    
    // Hardware timer state
    absolute_time_t last_time;
    bool first_run;
};

#endif