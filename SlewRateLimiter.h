/**
 * @file SlewRateLimiter.h
 * @brief Provides fixed and adaptive slew rate limiting using an Exponential Moving Average (EMA) calculation.
 *
 * The SlewRateLimiter class is designed to control the rate of change of a signal, which is 
 * particularly useful for preventing abrupt changes that may lead to overshoot, instability, or 
 * undesirable system behavior. It features both fixed and adaptive modes for slew rate limiting, 
 * with the adaptive mode allowing for greater changes in the signal when the difference between 
 * the current input and the EMA is large. This class also includes a hysteresis feature to prevent 
 * minor fluctuations within a specified band, enhancing the stability and smoothness of the output signal.
 *
 * Usage:
 * - Create an instance of SlewRateLimiter, optionally specifying the smoothing exponent, fixed rate limit, 
 *   hysteresis band, and adaptive slope.
 * - Use the processValue method to input a new value and receive the rate-limited output.
 * - Adjust the rate limiting, hysteresis, and smoothing parameters at runtime if needed.
 *
 * @note The adaptive slope is internally scaled to a base of 128 to utilize efficient bit-shifting operations 
 *       during calculations, reducing processing overhead.
 *
 * @author  Andrew McKinnon
 * @date    2023-11-3
 */

#ifndef SlewRateLimiter_h
#define SlewRateLimiter_h

#include "Arduino.h"

enum SmoothingExponent 
{
    SMOOTHING_VALUE_1 = 0,
    // ... (rest of the smoothing values)
    SMOOTHING_VALUE_512 = 9
};

class SlewRateLimiter 
{
public:
    SlewRateLimiter(
        SmoothingExponent exponent = SMOOTHING_VALUE_4, 
        int rate = 5, 
        int hystBand = 2,
        int slope = 0
    );

    int processValue(int currentValue);
    void setRateLimit(int limit);
    void setHysteresisBand(int band);
    void setSmoothingExponent(SmoothingExponent exponent);
    void setAdaptiveSlope(int slope);
    void reset();

private:
    int updateEMA(int newValue, int currentEMA, SmoothingExponent smoothingExponent);
    int lastValue;
    int emaValue;
    bool isFirstCall;
    SmoothingExponent currentExponent;
    int rateLimit;
    int hysteresisBand;
    int adaptiveSlopeInternal;
};

#endif /* SlewRateLimiter_h */

