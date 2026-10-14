# Testing and Calibration

## Set dimensions and thresholds

The sketch has four values to configure near its top:

```cpp
const float TANK_WATER_DEPTH_CM = 100.0;
const float SENSOR_TO_BOTTOM_CM = 100.0;
const float LOW_LEVEL_THRESHOLD_PERCENT = 20.0;
const float HIGH_LEVEL_THRESHOLD_PERCENT = 85.0;
```

1. Measure the tank's usable inside water depth, from the bottom reference point to the maximum intended water surface. Enter it in `TANK_WATER_DEPTH_CM`.
2. Measure vertically from the front face of the mounted ultrasonic sensor to the tank bottom/reference point. Enter it in `SENSOR_TO_BOTTOM_CM`.
3. If the sensor is at the tank's top reference, the first two measurements are typically equal. If it is mounted above the rim, include that extra vertical gap in `SENSOR_TO_BOTTOM_CM`; do not add the gap to `TANK_WATER_DEPTH_CM`.
4. Set the LOW and HIGH thresholds as percentages from 0 to 100, with LOW strictly less than HIGH.
5. Save, compile for Arduino Uno, upload, and open Serial Monitor at 9600 baud.

For example, if the usable inside depth is 80 cm and the sensor face is 12 cm above the rim, use `TANK_WATER_DEPTH_CM = 80.0` and `SENSOR_TO_BOTTOM_CM = 92.0`. These are example dimensions only; measure your own tank.

The estimated level is:

```text
water level (cm) = SENSOR_TO_BOTTOM_CM - measured distance (cm)
percentage = (water level / TANK_WATER_DEPTH_CM) * 100
```

The sketch clamps a computed level to the usable depth. For a tank with sloped or irregular sides, the percentage is a depth percentage, not necessarily a volume percentage.

## Test checklist

1. **Visual wiring check:** With power disconnected, verify polarity, resistor placement, shared ground, and every pin against [circuit-connections.md](circuit-connections.md).
2. **Compile:** Select Arduino Uno and verify the sketch compiles. Correct the tank configuration if the program reports a configuration error.
3. **Empty tank:** Point the sensor at the bottom reference. The distance should be close to `SENSOR_TO_BOTTOM_CM`; the reported level should be close to zero.
4. **Known depth:** Add water to a measured depth. The displayed distance should shorten by approximately the added depth, and the computed water level should increase.
5. **Thresholds:** Use measured fill levels to check LOW at or below the low threshold, NORMAL between thresholds, and HIGH at or above the high threshold.
6. **Indicators:** Confirm red + buzzer for LOW, yellow for NORMAL, green for HIGH, and all indicators off when the echo is invalid.
7. **Invalid echo:** Temporarily point the sensor away from a reflecting surface (without exposing electronics to water). Confirm it reports `INVALID` and does not retain a stale alert.
8. **Repeatability:** Take multiple readings at the same level. If readings vary, improve mounting, avoid ripples and obstructions, and re-check the measured reference distances.

These steps describe a test procedure; the repository does not claim that the physical circuit has been assembled or tested.

## Measurement and range notes

The sketch waits up to 30 milliseconds for an echo and rejects a missing echo, distances below 2 cm, distances above 400 cm, and distances substantially beyond the configured sensor-to-bottom distance. A small 5 cm bottom tolerance accommodates measurement variation; readings in that tolerance are clamped to zero water level.

If the water surface can come within about 2 cm of the sensor, the HC-SR04 may not measure it reliably. Mount the sensor at a suitable distance or choose an appropriate sensor. Re-check calibration whenever the sensor mount or tank changes.
