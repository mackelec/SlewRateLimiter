/**
 * @file SlewRateLimiter.cpp
 * @brief Implements the SlewRateLimiter class, providing both fixed and adaptive slew rate control.
 *
 * This implementation of the SlewRateLimiter class allows for precise control over the rate at which a 
 * signal can change, also known as its slew rate. By smoothing input signals and limiting their rate of 
 * change, the library helps prevent abrupt signal changes that could lead to undesirable effects in physical 
 * systems, such as mechanical stress, overshooting in control systems, or audible clicks in audio systems.
 *
 * The adaptive mode allows the slew rate to increase with larger signal deviations, making the system more 
 * responsive during rapid changes while still preventing excessively fast transitions. The library uses an 
 * Exponential Moving Average (EMA) to smooth the input signal and provides a hysteresis mechanism to avoid 
 * unnecessary adjustments for small fluctuations, which is particularly useful for noisy signals.
 *
 * Methods:
 * - updateEMA: Internal method to update the EMA with a new signal value.
 * - processValue: Applies rate limiting to an input value based on the current configuration.
 * - setRateLimit: Configures the maximum rate of change allowed in fixed mode.
 * - setHysteresisBand: Defines the range within which the output will not change, to prevent noise.
 * - setSmoothingExponent: Adjusts the weight of new input values in the EMA calculation.
 * - setAdaptiveSlope: Determines how much the slew rate increases with larger input deviations.
 * - reset: Reinitializes the internal state, clearing the EMA and last output value.
 *
 * The implementation is optimized for microcontrollers, using efficient algorithms and avoiding floating-point 
 * arithmetic to ensure it can run on low-resource hardware platforms like the Arduino.
 *
 * @author  Andrew McKinnon
 * @date    2023-11-3
 */


#include "SlewRateLimiter.h"

SlewRateLimiter::SlewRateLimiter(
    SRL_SmoothingExponent exponent, 
    int rate, 
    int hystBand, 
    int slope
)
  : lastValue(0),
    emaValue(0),
    isFirstCall(true),
    currentExponent(exponent),
    rateLimit(rate),
    hysteresisBand(hystBand),
    adaptiveSlopeInternal(0)
{
  setAdaptiveSlope(slope);
}

int SlewRateLimiter::updateEMA(int newValue, int currentEMA, SRL_SmoothingExponent smoothingExponent) 
{
    // Efficient EMA calculation using bit-shifting for powers of 2
    return ((newValue << smoothingExponent) + (currentEMA << 10) - (currentEMA << smoothingExponent)) >> 10;
}

int SlewRateLimiter::processValue(int currentValue)
{
  if (isFirstCall)
  {
    lastValue = currentValue;
    emaValue = currentValue;
    isFirstCall = false;
    return currentValue;
  }

  emaValue = updateEMA(currentValue, emaValue, currentExponent);

  int delta = currentValue - lastValue;
  int allowedChange = rateLimit;

  // Implement adaptive slope if applicable
  if (adaptiveSlopeInternal != 0)
  {
    allowedChange += (abs(delta) * adaptiveSlopeInternal)>>7;
  }

  // Rate limiting
  if (delta > allowedChange)
  {
    lastValue += allowedChange;
  }
  else if (delta < -allowedChange)
  {
    lastValue -= allowedChange;
  }
  else
  {
    lastValue = currentValue;
  }

  // Apply hysteresis
  if (abs(currentValue - lastValue) <= hysteresisBand)
  {
    lastValue = currentValue;
  }

  return lastValue;
}

void SlewRateLimiter::setRateLimit(int limit) 
{
    rateLimit = limit;
}

void SlewRateLimiter::setHysteresisBand(int band) 
{
    hysteresisBand = band;
}

void SlewRateLimiter::setSmoothingExponent(SRL_SmoothingExponent exponent) 
{
    currentExponent = exponent;
}

void SlewRateLimiter::setAdaptiveSlope(int slope) 
{
    // Convert the slope from a percentage to a scale of 128 for efficient calculation
    adaptiveSlopeInternal = (slope * 128 + 50) / 100; // The "+ 50" is for rounding to the nearest integer
}

void SlewRateLimiter::reset() 
{
    isFirstCall = true;
    lastValue = 0;
    emaValue = 0;
}
