/**
 * @file SlewRateLimiter.h
 * @brief An Arduino library for limiting the rate of change of a signal (slew rate control).
 *
 * The SlewRateLimiter class provides a mechanism for imposing a maximum rate of change on a signal. 
 * It implements an Exponential Moving Average (EMA) for signal smoothing and applies rate limiting to 
 * the changes in the output signal to prevent overshoot, ensure stability, and promote a smooth response.
 * The limiter can operate in a fixed mode, where the rate of change is constant, or an adaptive mode,
 * which allows the rate of change to increase with larger signal deviations, providing a responsive yet
 * stable control system.
 *
 * Major features:
 * - Fixed slew rate limiting: A constant maximum change is allowed between successive output values.
 * - Adaptive slew rate limiting: The rate of change is allowed to increase with larger input deviations.
 * - Hysteresis: Prevents changes to the output when the input changes are within a certain range, reducing noise.
 * - EMA Smoothing: Smooths out the input signal fluctuations using an Exponential Moving Average.
 *
 * Major methods:
 * - processValue: Processes an input value and returns the limited output.
 * - setRateLimit: Sets the fixed rate limit.
 * - setHysteresisBand: Sets the width of the hysteresis band.
 * - setSmoothingExponent: Sets the exponent used for EMA calculation.
 * - setAdaptiveSlope: Sets the slope for adaptive rate limiting.
 * - reset: Resets the EMA and last output value.
 *
 * Major variables:
 * - lastValue: The last output value after rate limiting and hysteresis.
 * - emaValue: The current value of the Exponential Moving Average.
 * - currentExponent: The exponent used for the EMA calculation.
 * - rateLimit: The maximum allowed change per output update in fixed mode.
 * - hysteresisBand: The range within which output changes are suppressed to reduce noise.
 * - adaptiveSlopeInternal: The factor by which the rate limit increases with larger input deviations.
 *
 * @note This library is designed to be efficient enough for use in real-time systems, such as those based on Arduino.
 *
 * @author  Andrew McKinnon
 * @date    2023-11-3
 */

#ifndef SlewRateLimiter_h
#define SlewRateLimiter_h

#include "Arduino.h"

class SlewRateLimiter 
{
public:
    enum SRL_SmoothingExponent {
        SRL_SMOOTHING_1 = 0,
        SRL_SMOOTHING_2 = 1,
        SRL_SMOOTHING_4 = 2,
        SRL_SMOOTHING_8 = 3,
        SRL_SMOOTHING_16 = 4,
        SRL_SMOOTHING_32 = 5,
        SRL_SMOOTHING_64 = 6,
        SRL_SMOOTHING_128 = 7,
        SRL_SMOOTHING_256 = 8,
        SRL_SMOOTHING_512 = 9
    };

    SlewRateLimiter(
        SRL_SmoothingExponent exponent = SRL_SMOOTHING_4, 
        int rate = 5, 
        int hystBand = 2,
        int slope = 0
    );

    int processValue(int currentValue);
    void setRateLimit(int limit);
    void setHysteresisBand(int band);
    void setSmoothingExponent(SRL_SmoothingExponent exponent);
    void setAdaptiveSlope(int slope);
    void reset();

private:
    int updateEMA(int newValue, int currentEMA, SRL_SmoothingExponent smoothingExponent);
    int lastValue;
    int emaValue;
    bool isFirstCall;
    SRL_SmoothingExponent currentExponent;
    int rateLimit;
    int hysteresisBand;
    int adaptiveSlopeInternal;
};

#endif /* SlewRateLimiter_h */
