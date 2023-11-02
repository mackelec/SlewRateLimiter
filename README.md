# SlewRateLimiter Library

The `SlewRateLimiter` is an all-integer Arduino compatible library that enables smooth signal transition control by limiting the rate of signal changes — the slew rate. By avoiding the use of floating-point operations, the library ensures fast execution suitable for real-time systems.

## Features

- **Fixed Slew Rate Limiting**: A constant maximum allowed change between consecutive output values.
- **Adaptive Slew Rate Limiting**: Dynamically adjusts the rate of change according to input deviations, enhancing responsiveness while preventing instability.
- **Hysteresis**: Implements a dead zone to suppress output fluctuations in response to minor input changes, thus reducing noise.
- **EMA Smoothing**: Utilizes an integer-based Exponential Moving Average algorithm to smooth out the reference signal.

## SmoothingExponent Enum

The `SmoothingExponent` enum provides predefined settings for the smoothing operation, controlling the weight of the most recent input values on the moving average. Higher values result in smoother outputs but with a slower response to changes:

- `SMOOTHING_VALUE_1`: Fastest response, least smoothing.
- `SMOOTHING_VALUE_2`: ...
- `SMOOTHING_VALUE_4`: ...
- (Continue for each enum value)
- `SMOOTHING_VALUE_512`: Slowest response, most smoothing.

Each enum value effectively represents a power of 2. For instance, `SMOOTHING_VALUE_16` corresponds to using \( 2^{4} \) in the EMA calculation.

## Methods

Here are the main methods provided by the `SlewRateLimiter`:

- `processValue(int currentValue)`: Applies rate limiting to an input value and returns the processed output. Note that the input value here refers to the new value to be processed, not the EMA directly.
- `setRateLimit(int limit)`: Configures the maximum change permitted per update in fixed mode.
- `setHysteresisBand(int band)`: Establishes the range within which the output remains unchanged to filter out noise.
- `setSmoothingExponent(SmoothingExponent exponent)`: Sets the EMA smoothing factor.
- `setAdaptiveSlope(int slope)`: Determines the rate at which the slew rate increases with larger input deviations.
- `reset()`: Clears the internal state, including the EMA and last output.

## Usage Examples

### Example 1: Basic Rate Limiting

```
#include "SlewRateLimiter.h"

// Initialize a limiter with default smoothing, a rate limit of 5 units, and a hysteresis band of 2 units
SlewRateLimiter myLimiter(SMOOTHING_VALUE_4, 5, 2);

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
SlewRateLimiter myLimiter(SMOOTHING_VALUE_16, 10, 3, 50);

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
