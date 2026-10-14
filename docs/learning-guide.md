# Beginner Learning Guide

## Embedded-system ideas

- **Embedded system:** A small computer built into a larger device to do a focused job. This project combines a sensor, a program, and outputs to monitor a tank.
- **Microcontroller:** The Arduino Uno's ATmega328P reads electrical signals, runs the sketch, and switches output pins. It repeats this work while powered.
- **Water level versus water quality:** This project estimates how deep the water is. It does not tell whether water is clean or safe to drink. Temperature, pH, and turbidity need their own sensors and calibration.

## How the HC-SR04 works

The sensor emits a short burst of ultrasonic sound. The sound reflects from the water surface and returns to the sensor.

- **TRIG** is an input to the sensor. The Arduino holds it HIGH briefly to request a measurement.
- **ECHO** is an output from the sensor. It stays HIGH for the sound's round-trip travel time.
- `pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US)` measures how many microseconds the ECHO pin stays HIGH. The timeout prevents the program from waiting forever. A returned duration of zero means no pulse arrived in time.
- Sound travels to the surface and back, so the sketch divides the travel distance by two. It uses the approximate speed of sound in air, `0.0343 cm` per microsecond.

In the code:

```text
distance = echo time * speed of sound / 2
```

This is an estimate. Temperature, turbulence, obstacles, mounting angle, and the sensor's limits affect accuracy.

## How the level is calculated and classified

The sensor reports the air gap above the water. The sketch calculates:

```text
water depth = sensor-to-bottom distance - air gap
percentage = water depth / usable tank depth * 100
```

`SENSOR_TO_BOTTOM_CM` includes any gap above the rim, while `TANK_WATER_DEPTH_CM` is only the useful depth inside the tank. If the sensor is level with the tank's top reference, those dimensions are normally equal.

An `if` / `else if` / `else` decision checks the percentage:

- At or below `LOW_LEVEL_THRESHOLD_PERCENT`, the status is LOW.
- Otherwise, at or above `HIGH_LEVEL_THRESHOLD_PERCENT`, the status is HIGH.
- Any value between the two thresholds is NORMAL.

The red LED and buzzer indicate LOW, yellow indicates NORMAL, and green indicates HIGH. For an invalid measurement, the sketch turns all indicators off and prints an error rather than using a stale value.

## Sketch walkthrough, statement by statement

The following follows the source file from top to bottom. Repeated statements that set up each pin or print each part of a line are described individually.

### Pin and configuration declarations

- The opening comments identify the project, target board, and sensor; they are notes for readers and do not change program behavior.
- `const byte TRIG_PIN = 9;` assigns digital pin 9 to the sensor trigger.
- `const byte ECHO_PIN = 10;` assigns digital pin 10 to the sensor echo.
- `RED_LED_PIN`, `YELLOW_LED_PIN`, and `GREEN_LED_PIN` assign pins 2, 3, and 4 to the three status LEDs.
- `BUZZER_PIN = 5` assigns pin 5 to an active buzzer.
- `TANK_WATER_DEPTH_CM` stores the real usable depth inside your tank. Change it from the example value.
- `SENSOR_TO_BOTTOM_CM` stores the vertical sensor-face-to-bottom distance. Include the above-rim mounting gap if present.
- `LOW_LEVEL_THRESHOLD_PERCENT` sets the percentage at or below which the water is considered low.
- `HIGH_LEVEL_THRESHOLD_PERCENT` sets the percentage at or above which the water is considered high.
- `ECHO_TIMEOUT_US` limits how long to wait for the sensor's echo.
- `MEASUREMENT_INTERVAL_MS` sets the pause between measurements to 1000 milliseconds (one second).
- `SOUND_SPEED_CM_PER_US` is the approximate speed used to convert time into distance.
- `MIN_VALID_DISTANCE_CM` and `MAX_VALID_DISTANCE_CM` describe the accepted HC-SR04 distance range used by this sketch.
- `BOTTOM_TOLERANCE_CM` allows a small error near the tank bottom before an echo is considered too far away.

### Status names and helper functions

- `enum WaterLevelStatus` gives the three possible named statuses: `STATUS_LOW`, `STATUS_NORMAL`, and `STATUS_HIGH`. Names are easier to understand than unexplained numbers.
- `setIndicators(...)` receives four true/false values. Each `digitalWrite(...)` turns its output pin HIGH when its matching argument is true, and LOW when false. This function is used to update LEDs and buzzer together.
- `configurationIsValid()` returns true only if the tank dimensions are positive and consistent, the thresholds are within 0–100, and LOW is less than HIGH. A bad configuration is not used for measurement.
- `measureDistanceCm()` first drives TRIG LOW, waits 2 microseconds, drives it HIGH for 10 microseconds, then drives it LOW. This requests a measurement.
- Inside `measureDistanceCm()`, `pulseIn(...)` waits for a HIGH pulse on ECHO and returns its duration in microseconds. If it returns zero, the helper returns `-1.0` to mark a missing echo.
- If an echo arrives, `(echoDurationUs * SOUND_SPEED_CM_PER_US) / 2.0` converts round-trip time to one-way distance in centimetres.
- `distanceIsValid(distanceCm)` returns true only when the distance is within the sensor's accepted range and not much farther than the configured tank bottom.
- `classifyLevel(percentage)` returns LOW when the percentage is at or under the low threshold, HIGH when it is at or over the high threshold, and NORMAL otherwise.
- `printStatus(status)` prints the matching readable word to Serial. The `F("...")` macro keeps fixed text in program memory, which is useful on small microcontrollers.

### `setup()`

- `void setup()` runs once after the Arduino starts or resets.
- Each `pinMode(..., OUTPUT)` sets TRIG and the LEDs/buzzer as outputs. `pinMode(ECHO_PIN, INPUT)` makes ECHO an input.
- `setIndicators(false, false, false, false)` starts with every indicator off.
- `Serial.begin(9600)` starts USB serial communication at 9600 baud.
- `Serial.println(...)` prints the project name, followed by a newline.
- `if (!configurationIsValid())` checks for incorrect dimensions or thresholds. The `!` means “not.”
- If the configuration is invalid, the sketch prints an explanation and enters `while (true)`. That loop keeps indicators off and pauses instead of making misleading measurements.

### `loop()`

- `void loop()` runs repeatedly after `setup()` finishes.
- `measureDistanceCm()` asks the sensor for a reading and stores the result in `distanceCm`.
- `if (!distanceIsValid(distanceCm))` handles a missing or out-of-range reading. It switches all indicators off, prints `INVALID`, waits one measurement interval, and `return` skips the rest of this loop pass.
- `SENSOR_TO_BOTTOM_CM - distanceCm` estimates depth from the tank bottom to the water surface.
- `constrain(...)` limits the computed depth to between zero and the tank's usable inside depth. This avoids displaying a negative level or more water than the tank depth.
- The next statement divides the depth by the usable tank depth and multiplies by 100 to get a percentage.
- `classifyLevel(waterLevelPercent)` chooses LOW, NORMAL, or HIGH.
- The `if` / `else if` / `else` block turns on only the matching LED. It also turns on the buzzer for LOW only.
- The series of `Serial.print(...)` statements builds one useful output line: measured distance, estimated centimetres, percentage, and status. `Serial.println()` finishes the line.
- `delay(MEASUREMENT_INTERVAL_MS)` waits one second before the next reading.

## Simple learning experiments

1. Change a threshold and observe when the status changes (without changing the hardware).
2. Compare the displayed distance with a ruler at several water depths.
3. Temporarily change the measurement interval, then restore it to a sensible value.
4. Watch how the indicators respond when the water is below, between, and above the thresholds.
