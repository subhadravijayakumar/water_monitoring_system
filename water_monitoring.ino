// Smart Water Monitoring System
// Arduino Uno + HC-SR04: estimates water level (not water quality).

const byte trigPin = 9;
const byte echoPin = 10;
const byte redLedPin = 2;
const byte yellowLedPin = 3;
const byte greenLedPin = 4;
const byte buzzerPin = 5;

// Replace these example dimensions with measurements from your tank.
const float tankHeightCm = 30.0;          // Internal tank height, bottom to rim.
const float sensorAboveTankCm = 0.0;      // Sensor face above the tank rim; use 0 if level with it.

const float lowThresholdPercent = 20.0;   // LOW at or below this percentage.
const float highThresholdPercent = 80.0;  // HIGH at or above this percentage.

const unsigned long echoTimeoutUs = 30000UL;
const float minimumDistanceCm = 2.0;
const float maximumDistanceCm = 400.0;
const unsigned long measurementIntervalMs = 1000UL;

enum WaterStatus {
  STATUS_LOW,
  STATUS_NORMAL,
  STATUS_HIGH
};

float measureDistanceCm() {
  // Send a short trigger pulse to start one ultrasonic measurement.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // pulseIn returns 0 if no echo arrives before the timeout.
  const unsigned long echoDurationUs = pulseIn(echoPin, HIGH, echoTimeoutUs);
  if (echoDurationUs == 0) {
    return -1.0;
  }

  // Sound travels to the surface and back; dividing by 58 gives centimetres.
  const float distanceCm = echoDurationUs / 58.0;
  if (distanceCm < minimumDistanceCm || distanceCm > maximumDistanceCm) {
    return -1.0;
  }

  return distanceCm;
}

void setIndicators(bool redOn, bool yellowOn, bool greenOn, bool buzzerOn) {
  digitalWrite(redLedPin, redOn ? HIGH : LOW);
  digitalWrite(yellowLedPin, yellowOn ? HIGH : LOW);
  digitalWrite(greenLedPin, greenOn ? HIGH : LOW);
  digitalWrite(buzzerPin, buzzerOn ? HIGH : LOW);
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  setIndicators(false, false, false, false);
  Serial.begin(9600);
  Serial.println(F("Smart Water Monitoring System"));
  Serial.println(F("This prototype measures water level, not water quality."));
}

void loop() {
  const float distanceCm = measureDistanceCm();

  if (distanceCm < 0.0) {
    // Unknown measurements must not be reported as a valid level or alert.
    setIndicators(false, false, false, false);
    Serial.println(F("Invalid reading: check sensor wiring, alignment, and range."));
    delay(measurementIntervalMs);
    return;
  }

  // The sensor-to-rim offset lets the sensor sit above the tank without
  // counting that external gap as part of the water level.
  const float distanceFromRimCm = distanceCm - sensorAboveTankCm;
  float waterLevelCm = tankHeightCm - distanceFromRimCm;

  // Keep the reported level within the tank's physical 0-to-height range.
  if (waterLevelCm < 0.0) {
    waterLevelCm = 0.0;
  } else if (waterLevelCm > tankHeightCm) {
    waterLevelCm = tankHeightCm;
  }

  const float waterLevelPercent = (waterLevelCm / tankHeightCm) * 100.0;
  WaterStatus status;

  if (waterLevelPercent <= lowThresholdPercent) {
    status = STATUS_LOW;
    setIndicators(true, false, false, true);
  } else if (waterLevelPercent >= highThresholdPercent) {
    status = STATUS_HIGH;
    setIndicators(false, true, false, false);
  } else {
    status = STATUS_NORMAL;
    setIndicators(false, false, true, false);
  }

  Serial.print(F("Distance to water: "));
  Serial.print(distanceCm, 1);
  Serial.print(F(" cm | Water level: "));
  Serial.print(waterLevelCm, 1);
  Serial.print(F(" cm ("));
  Serial.print(waterLevelPercent, 1);
  Serial.print(F("%) | Status: "));

  if (status == STATUS_LOW) {
    Serial.println(F("LOW"));
  } else if (status == STATUS_HIGH) {
    Serial.println(F("HIGH"));
  } else {
    Serial.println(F("NORMAL"));
  }

  delay(measurementIntervalMs);
}
