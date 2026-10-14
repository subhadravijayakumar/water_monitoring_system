# Testing and Calibration

## 1. Measure the installation

Do not assume a standard tank size. Measure the actual installation:

1. Measure the tank's **internal vertical height** from the inside bottom to the rim. Enter this value in centimetres as `tankHeightCm`.
2. Measure the vertical distance from the tank rim up to the HC-SR04's acoustic face. Enter this as `sensorAboveTankCm`. If the sensor face is level with the rim, use `0.0`.
3. Mount the sensor securely so it points down toward the water, not at a wall, pipe, or brace. Keep the complete electronics assembly dry.
4. Confirm the expected sensor-to-water distance at a known fill level fits the HC-SR04's approximate 2–400 cm range.

With the sensor above the rim, the empty-tank sensor-to-water distance is approximately `tankHeightCm + sensorAboveTankCm`. At a full-to-rim level it is approximately `sensorAboveTankCm`. The sketch subtracts the mounting offset before calculating the water level:

```text
Distance from rim to water = measured distance − sensorAboveTankCm
Water level = tankHeightCm − distance from rim to water
Percentage = (water level / tankHeightCm) × 100
```

The sketch limits the displayed level to 0 through the configured tank height. The sensor should not be mounted where the full-tank echo is inside its minimum range; raise the sensor if necessary.

## 2. Set thresholds

At the top of `water_monitoring.ino`, edit:

```cpp
const float lowThresholdPercent = 20.0;
const float highThresholdPercent = 80.0;
```

LOW is selected when the percentage is at or below the low threshold. HIGH is selected when it is at or above the high threshold. Values between them are NORMAL. Choose values appropriate to the tank and use a low threshold less than the high threshold.

## 3. Upload and observe

1. Check the pin connections against [circuit-connections.md](./circuit-connections.md).
2. Connect the Uno to the computer by USB.
3. In Arduino IDE, select **Arduino Uno** and its port. Open `water_monitoring.ino`, click **Verify**, then click **Upload**.
4. Open Serial Monitor at **9600 baud**.
5. Compare the printed distance with a ruler measurement from the transducer face to a flat water surface.
6. Record results with the tank empty and at multiple measured fill heights. Adjust the physical mounting and dimensions first if results show a consistent offset.
7. Check the LED and buzzer response on either side of both thresholds.

## 4. Invalid-reading test

Temporarily point the sensor away from a reflecting surface or otherwise prevent a valid echo without getting the electronics wet. The Serial Monitor should show an invalid-reading message. The code turns all LEDs and the buzzer off for that measurement instead of presenting a stale or invented level.

## Calibration and interpretation limits

- Measure vertically; a slanted measurement path is not the tank's water depth.
- Keep the water surface reasonably still and free of obstacles in the sensor's beam.
- If the sensor is below the rim or uses a different reference, adapt the offset and formula to that geometry before relying on its results.
- The program reports a height percentage. For a tank with a changing cross-section, height percentage is not volume percentage; use a measured height-to-volume table if volume is required.
- Foam, ripples, condensation, obstacles, and echoes outside the sensor's range can reduce accuracy.
- Calibration is specific to the physical tank, sensor placement, and sensor module. Recheck after moving any of them.

No physical prototype testing is claimed by this repository; perform and record your own tests.
