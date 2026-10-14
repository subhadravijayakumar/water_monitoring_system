# Circuit Connections

## Pin-by-pin wiring

Disconnect USB power while building or changing the circuit. Use a separate current-limiting resistor for each LED.

| Component pin | Arduino Uno connection | Notes |
| --- | --- | --- |
| HC-SR04 VCC | 5V | Sensor supply |
| HC-SR04 GND | GND | Common ground |
| HC-SR04 TRIG | D9 | Arduino trigger output |
| HC-SR04 ECHO | D10 | Echo pulse input; safe for the 5 V Uno |
| Red LED anode | D2 through 220–330 ohms | Long LED leg is normally the anode |
| Red LED cathode | GND | Short leg / flat side |
| Yellow LED anode | D3 through 220–330 ohms | Long leg is normally the anode |
| Yellow LED cathode | GND | Short leg / flat side |
| Green LED anode | D4 through 220–330 ohms | Long leg is normally the anode |
| Green LED cathode | GND | Short leg / flat side |
| Active buzzer + | D5 | Check current rating before direct connection |
| Active buzzer - | GND | Common ground |

The three LED resistors are not optional: they limit current and protect the LEDs and Arduino pins. An active buzzer should make a sound when powered with its rated DC voltage. If the buzzer draws more current than an Arduino pin can safely supply, use a transistor driver and suitable supply rather than driving it directly. Do not connect a higher-voltage buzzer to the Uno pin.

## Sensor placement

Mount the HC-SR04 above the tank, facing straight down at the water. Keep both transducers clear of obstructions, and avoid positioning it so the sound beam hits a nearby wall or brace. Secure the mount against movement. Protect the electronics and wiring from splashes and condensation.

The HC-SR04 commonly operates at 5 V. Its ECHO pin can output 5 V. A 3.3 V-only input such as an ESP32 GPIO must not receive that signal directly: add a suitable level shifter or resistor divider when using a 3.3 V board.

## LED meaning in this project

| Status | Red LED | Yellow LED | Green LED | Buzzer |
| --- | --- | --- | --- | --- |
| LOW | On | Off | Off | On |
| NORMAL | Off | On | Off | Off |
| HIGH | Off | Off | On | Off |
| Invalid reading | Off | Off | Off | Off |
