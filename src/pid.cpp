#include "pid.hpp"

PID::PID(int32_t kp, int32_t ki, int32_t kd, int32_t scale, int32_t minOutput, int32_t maxOutput)
    : kp(kp), ki(ki), kd(kd), scale(scale), minOutput(minOutput), maxOutput(maxOutput),
      targetRPM(0), integral(0), prevError(0), first_run(true) {}

void PID::setRPM(int32_t target) {
    targetRPM = target;
}

int32_t PID::calculate(int32_t currentRPM) {
    absolute_time_t now = get_absolute_time();
    
    if (first_run) {
        last_time = now;
        prevError = targetRPM - currentRPM;
        first_run = false;
        return 0;
    }

    // Get delta time from hardware timer in microseconds
    int64_t dt_us = absolute_time_diff_us(last_time, now);
    if (dt_us <= 0) return 0; // Prevent calculation if called too fast

    int32_t error = targetRPM - currentRPM;

    // 1. Proportional
    int32_t pOut = kp * error;

    // 2. Integral: (Error * dt). 
    // We use dt_us/1000 to keep numbers manageable (milliseconds)
    int32_t dt_ms = (int32_t)(dt_us / 1000);
    integral += (error * dt_ms);
    
    // Anti-windup (Clamping the integral sum)
    int32_t iOut = ki * integral;

    // 3. Derivative: (Change in Error / dt)
    // Multiplied by 1000 to maintain precision with the integer dt_ms
    int32_t dOut = 0;
    if (dt_ms > 0) {
        dOut = (kd * (error - prevError) * 1000) / dt_ms;
    }

    // Combined output scaled back down
    int32_t output = (pOut + iOut + dOut) / scale;

    // Clamp to PWM limits
    if (output > maxOutput) output = maxOutput;
    if (output < minOutput) output = minOutput;

    // Save state for next hardware timer comparison
    prevError = error;
    last_time = now;

    return output;
}

void PID::reset() {
    integral = 0;
    prevError = 0;
    first_run = true;
}

int32_t PID::getTargetRPM() {
    return targetRPM;
}