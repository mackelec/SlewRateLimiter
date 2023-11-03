# SlewRateLimiter Library

The `SlewRateLimiter` is an all-integer Arduino compatible library that enables smooth signal transition control by limiting the rate of signal changes — the slew rate. By avoiding the use of floating-point operations, the library ensures fast execution suitable for real-time systems.

## Features

- **Fixed Slew Rate Limiting**: A constant maximum allowed change between consecutive output values.
- **Adaptive Slew Rate Limiting**: Dynamically adjusts the rate of change according to input deviations, enhancing responsiveness while preventing instability.
- **Hysteresis**: Implements a dead zone to suppress output fluctuations in response to minor input changes, thus reducing noise.
- **EMA Smoothing**: Utilizes an integer-based Exponential Moving Average algorithm to smooth out the reference signal.

## SRL_SmoothingExponent Enum

The `SRL_SmoothingExponent` enum offers predefined settings for the EMA smoothing operation, which determines the weight of recent input values on the moving average. The actual smoothing factor used in calculations is \( 2^{value} \), where `value` is the enum constant. The larger the exponent, the smoother the output but with a slower response to changes:

- `SRL_SMOOTHING_1`: Equivalent to a smoothing factor of \( 2^0 \), providing the fastest response with minimal smoothing.
- `SRL_SMOOTHING_2`: Corresponds to \( 2^1 \), offering a balance between responsiveness and smoothing.
- `SRL_SMOOTHING_4`: Uses \( 2^2 \), increasing the smoothness with a slightly delayed response.
- `SRL_SMOOTHING_8`: Represents \( 2^3 \), further smoothing the signal.
- `SRL_SMOOTHING_16`: Equivalent to \( 2^4 \), designed for more pronounced smoothing.
- `SRL_SMOOTHING_32`: Matches \( 2^5 \), for a significant reduction in signal noise.
- `SRL_SMOOTHING_64`: Reflects \( 2^6 \), heavily smoothing signal fluctuations.
- `SRL_SMOOTHING_128`: Corresponds to \( 2^7 \), for very smooth signal transitions.
- `SRL_SMOOTHING_256`: Equivalent to \( 2^8 \), creating a very delayed but smooth response.
- `SRL_SMOOTHING_512`: At \( 2^9 \), this provides the slowest response with the highest level of smoothing.

## Methods

- `processValue(int currentValue)`: Applies rate limiting to an input value and returns the processed output. Note that the input value here refers to the new value to be processed, not the EMA directly.
- `setRateLimit(int limit)`: Configures the maximum change permitted per update in fixed mode.
- `setHysteresisBand(int band)`: Establishes the range within which the output remains unchanged to filter out noise.
- `setSmoothingExponent(SRL_SmoothingExponent exponent)`: Adjusts the EMA smoothing factor to control the signal's smoothness and responsiveness.
- `setAdaptiveSlope(int slope)`: Determines the rate at which the slew rate increases with larger input deviations.
- `reset()`: Clears the internal state, including the EMA and last output.

## Usage Examples

### Example 1: Basic Rate Limiting

```
#include <SlewRateLimiter.h>

// Initialize a limiter with default smoothing, a rate limit of 5 units, and a hysteresis band of 2 units
SlewRateLimiter myLimiter(SlewRateLimiter::SRL_SMOOTHING_4, 5, 2);

void setup() {
  // Setup code here
}

void loop() {
  int sensorValue = analogRead(A0); // Read some sensor value
  int limitedValue = myLimiter.processValue(sensorValue); // Limit the rate of change
  // Use limitedValue as needed
}
```

### Example 2: Adaptive Slew Rate

```
#include "SlewRateLimiter.h"

// Initialize with a higher smoothing level, rate limit, hysteresis band, and an adaptive slope
// Adaptive slope has a base of 128.  so a value of 128 will give a slopw of 1
SlewRateLimiter myLimiter(SlewRateLimiter::SRL_SMOOTHING_16, 10, 3, 50);

void setup() {
  // Any required setup code here
}

void loop() {
  int sensorValue = analogRead(A0); // Read the sensor value
  int limitedValue = myLimiter.processValue(sensorValue); // Apply adaptive slew rate limiting
  // Utilize limitedValue for further processing
}
```

## Performance

As the `SlewRateLimiter` uses integer math for all calculations, it's highly efficient and suitable for resource-constrained environments like microcontrollers. This makes the library ideal for high-performance or time-critical applications where every millisecond counts.

## Contributions

Contributions to improve the library, whether through new features, bug fixes, or performance enhancements, are always welcome. Please feel free to fork the repository, make your changes, and submit a pull request.

## Support

If you encounter any issues or have questions regarding the library, please file an issue on the GitHub project page. Community support and feedback are invaluable and help improve the tools we rely on.

Happy coding, and enjoy the precision control provided by `SlewRateLimiter`!
