# Circuit Connections

## Arduino Uno wiring

| Component pin / lead | Connect to | Notes |
|---|---|---|
| HC-SR04 VCC | Arduino 5 V | The HC-SR04 commonly operates at 5 V. |
| HC-SR04 GND | Arduino GND | Share ground with all circuit components. |
| HC-SR04 TRIG | Digital pin 9 | Arduino sends the trigger pulse. |
| HC-SR04 ECHO | Digital pin 10 | Arduino times the returned echo pulse. |
| Red LED anode (long lead) | Digital pin 2 through a 220–330 Ω resistor | Put the resistor in series with the LED. |
| Red LED cathode (short lead / flat edge) | GND | LOW status. |
| Yellow LED anode (long lead) | Digital pin 3 through a 220–330 Ω resistor | Put the resistor in series with the LED. |
| Yellow LED cathode (short lead / flat edge) | GND | HIGH status. |
| Green LED anode (long lead) | Digital pin 4 through a 220–330 Ω resistor | Put the resistor in series with the LED. |
| Green LED cathode (short lead / flat edge) | GND | NORMAL status. |
| Active buzzer positive (+) | Digital pin 5 | Use a low-current buzzer compatible with an Arduino output. |
| Active buzzer negative (−) | GND | Sounds only for LOW status. |

Connect each LED as pin → resistor → LED anode, then LED cathode → GND. The resistor may also be placed between LED cathode and GND, as long as it is in series. Do not connect an LED directly between an Arduino pin and ground.

This wiring matches the pin constants in `water_monitoring.ino`: TRIG D9, ECHO D10, red D2, yellow D3, green D4, and buzzer D5. The Uno's 5 V, GND, and signal pins are not interchangeable; check module labels before applying power.

The code assumes an **active** buzzer: applying HIGH to its positive lead makes it sound. A passive piezo element needs a tone-generating signal and will not necessarily sound with this simple on/off code. Do not connect a buzzer that exceeds the Arduino pin's current rating directly; use an appropriate transistor driver for a higher-current load.

## Safety and 3.3 V boards

Keep the Uno, breadboard, and wiring away from water. Secure the sensor so it cannot fall into the tank, point it at the water surface, and keep the USB-powered electronics dry.

The HC-SR04 commonly uses 5 V and its ECHO output can be 5 V. Do not connect ECHO directly to a 3.3 V-only GPIO on an ESP32 or similar board. Use an appropriately designed voltage divider or logic-level shifter. Confirm the selected sensor module's specifications before wiring.
