// Smart Water Monitoring System
// Board: Arduino Uno
// Sensor: HC-SR04 ultrasonic distance sensor

const byte TRIG_PIN = 9;
const byte ECHO_PIN = 10;
const byte RED_LED_PIN = 2;
const byte YELLOW_LED_PIN = 3;
const byte GREEN_LED_PIN = 4;
const byte BUZZER_PIN = 5;

// Set these values to match your tank. All distances are in centimetres.
const float TANK_WATER_DEPTH_CM = 100.0;       // Usable inside depth of the tank
const float SENSOR_TO_BOTTOM_CM = 100.0;       // Sensor face to tank bottom/reference
const float LOW_LEVEL_THRESHOLD_PERCENT = 20.0;
const float HIGH_LEVEL_THRESHOLD_PERCENT = 85.0;

const unsigned long ECHO_TIMEOUT_US = 30000UL;
const unsigned long MEASUREMENT_INTERVAL_MS = 1000UL;
const float SOUND_SPEED_CM_PER_US = 0.0343;
const float MIN_VALID_DISTANCE_CM = 2.0;
const float MAX_VALID_DISTANCE_CM = 400.0;
const float BOTTOM_TOLERANCE_CM = 5.0;

enum WaterLevelStatus {
  STATUS_LOW,
  STATUS_NORMAL,
  STATUS_HIGH
};

void setIndicators(bool redOn, bool yellowOn, bool greenOn, bool buzzerOn) {
  digitalWrite(RED_LED_PIN, redOn ? HIGH : LOW);
  digitalWrite(YELLOW_LED_PIN, yellowOn ? HIGH : LOW);
  digitalWrite(GREEN_LED_PIN, greenOn ? HIGH : LOW);
  digitalWrite(BUZZER_PIN, buzzerOn ? HIGH : LOW);
}

bool configurationIsValid() {
  return TANK_WATER_DEPTH_CM > 0.0 &&
         SENSOR_TO_BOTTOM_CM >= TANK_WATER_DEPTH_CM &&
         LOW_LEVEL_THRESHOLD_PERCENT >= 0.0 &&
         HIGH_LEVEL_THRESHOLD_PERCENT <= 100.0 &&
         LOW_LEVEL_THRESHOLD_PERCENT < HIGH_LEVEL_THRESHOLD_PERCENT;
}

float measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long echoDurationUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (echoDurationUs == 0UL) {
    return -1.0;  // No echo arrived before the timeout.
  }

  return (echoDurationUs * SOUND_SPEED_CM_PER_US) / 2.0;
}

bool distanceIsValid(float distanceCm) {
  return distanceCm >= MIN_VALID_DISTANCE_CM &&
         distanceCm <= MAX_VALID_DISTANCE_CM &&
         distanceCm <= SENSOR_TO_BOTTOM_CM + BOTTOM_TOLERANCE_CM;
}

WaterLevelStatus classifyLevel(float percentage) {
  if (percentage <= LOW_LEVEL_THRESHOLD_PERCENT) {
    return STATUS_LOW;
  } else if (percentage >= HIGH_LEVEL_THRESHOLD_PERCENT) {
    return STATUS_HIGH;
  } else {
    return STATUS_NORMAL;
  }
}

void printStatus(WaterLevelStatus status) {
  if (status == STATUS_LOW) {
    Serial.print(F("LOW"));
  } else if (status == STATUS_HIGH) {
    Serial.print(F("HIGH"));
  } else {
    Serial.print(F("NORMAL"));
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  setIndicators(false, false, false, false);

  Serial.begin(9600);
  Serial.println(F("Smart Water Monitoring System"));

  if (!configurationIsValid()) {
    Serial.println(F("Configuration error: check tank dimensions and thresholds."));
    while (true) {
      setIndicators(false, false, false, false);
      delay(1000);
    }
  }
}

void loop() {
  const float distanceCm = measureDistanceCm();

  if (!distanceIsValid(distanceCm)) {
    setIndicators(false, false, false, false);
    Serial.println(F("Distance: INVALID - check sensor alignment and wiring."));
    delay(MEASUREMENT_INTERVAL_MS);
    return;
  }

  float waterLevelCm = SENSOR_TO_BOTTOM_CM - distanceCm;
  waterLevelCm = constrain(waterLevelCm, 0.0, TANK_WATER_DEPTH_CM);
  const float waterLevelPercent =
      (waterLevelCm / TANK_WATER_DEPTH_CM) * 100.0;
  const WaterLevelStatus status = classifyLevel(waterLevelPercent);

  if (status == STATUS_LOW) {
    setIndicators(true, false, false, true);
  } else if (status == STATUS_HIGH) {
    setIndicators(false, false, true, false);
  } else {
    setIndicators(false, true, false, false);
  }

  Serial.print(F("Distance: "));
  Serial.print(distanceCm, 1);
  Serial.print(F(" cm | Water level: "));
  Serial.print(waterLevelCm, 1);
  Serial.print(F(" cm ("));
  Serial.print(waterLevelPercent, 1);
  Serial.print(F("%) | Status: "));
  printStatus(status);
  Serial.println();

  delay(MEASUREMENT_INTERVAL_MS);
}
