# GPIO Interface: Push-Button State Detection and Dual LED Actuation

A laboratory implementation which shows the fundamental digital interfacing of General Purpose Input/Output (GPIO) on the ESP32 microcontroller using the Arduino framework, this project illustrating the handling of active-low inputs by means of an internal pull-up resistor and the complementary activation of the outputs on two status LEDs.


---

## Table of Contents

- [Overview](#overview)
- [Circuit Diagram and Hardware Setup](#circuit-diagram-and-hardware-setup)
- [Pin Mapping](#pin-mapping)
- [Logic and Working Principle](#logic-and-working-principle)
- [Source Code](#source-code)
- [Observation Table](#observation-table)
- [Success Verification](#success-verification)

---

## Overview

The primary objective of this experiment is to interface an external digital input (momentary push-button) and two digital outputs (LEDs) with an ESP32 development board.

Key technical concepts demonstrated:
* Configuring GPIO pins for digital input with internal pull-up resistors (`INPUT_PULLUP`).
* Understanding active-low input logic and preventing floating state conditions.
* Real-time polling of input states using `digitalRead()`.
* Complementary switching logic (alternating LED activation states based on user interaction).

---

## Circuit Diagram and Hardware Setup
<img width="260" height="263" alt="Circuit Diagram" src="https://github.com/user-attachments/assets/700f1ef0-a277-463e-aca5-07c2979df6ba" />

<img width="1536" height="2048" alt="Hardware Setup" src="https://github.com/user-attachments/assets/672d99e2-397e-4351-b6b3-5419dfff93a3" />



The system consists of an ESP32 board, a tactile momentary push-button connected between `GPIO23` and `GND`, and two LEDs (Red and Blue) connected through current-limiting resistors to `GPIO18` and `GPIO19`.

### Schematic 


```
                 +-------------------+
                 |    ESP32 Board    |
                 |                   |
                 |            GPIO23 +-----[ Push-Button ]-----+
                 |                   |                         |
                 |            GPIO18 +---[ 220Ω ]--->| (LED 1) +-- GND
                 |                   |       (Red)             |
                 |            GPIO19 +---[ 220Ω ]--->| (LED 2) +-- GND
                 |                   |       (Blue)            |
                 |               GND +-------------------------+
                 +-------------------+
```

---

## Pin Mapping

| Component | ESP32 Pin | Mode | Configuration / Role |
| :--- | :--- | :--- | :--- |
| **Tactile Push-Button** | `GPIO 23` | `INPUT_PULLUP` | Digital input; pulled to $3.3\,\text{V}$ internally when open, grounded when pressed |
| **Status LED 1 (Red)** | `GPIO 18` | `OUTPUT` | Active HIGH; default state is ON |
| **Status LED 2 (Blue)** | `GPIO 19` | `OUTPUT` | Active HIGH; default state is OFF (complement to LED 1) |
| **Common Ground** | `GND` | Ground | Common reference rail for button and LED cathodes |

---

## Logic and Working Principle

### 1. Meaning of HIGH and LOW for the Button

The button input is configured using the ESP32 internal pull-up resistor:

$$\text{Pin Configuration} = \texttt{pinMode(BUTTON\_PIN, INPUT\_PULLUP)}$$

* **`HIGH` ($3.3\,\text{V}$ / Released):** When the push-button is open (released), the internal pull-up resistor ties the GPIO pin to the positive logic supply rail ($3.3\,\text{V}$). This prevents an indeterminate or "floating" state and ensures stable, noise-free readings without requiring an external resistor.
* **`LOW` ($0\,\text{V}$ / Pressed):** When the button is closed (pressed), it connects the GPIO directly to ground (`GND`), pulling the voltage level down to $0\,\text{V}$. Thus, the input operates on an **active-low** logic scheme.

### 2. Complementary LED Control

The micro-controller continuously polls the input state inside `loop()`:
* **Default / Released State (`buttonState == HIGH`):**
  * `LED1_PIN` (Red) is set to `HIGH` ($\text{ON}$)
  * `LED2_PIN` (Blue) is set to `LOW` ($\text{OFF}$)
* **Actuated / Pressed State (`buttonState == LOW`):**
  * `LED1_PIN` (Red) is set to `LOW` ($\text{OFF}$)
  * `LED2_PIN` (Blue) is set to `HIGH` ($\text{ON}$)

---

## Source Code

```cpp
#include <Arduino.h>

const uint8_t BUTTON_PIN = 23;
const uint8_t LED1_PIN = 18;
const uint8_t LED2_PIN = 19;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Set initial complementary state
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    // Button is actively pressed (Active-LOW)
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
  } else {
    // Button is released
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
  }
}
```

---

## Observation Table

| Test Condition | Button Mechanical State | Voltage Level at GPIO 23 | Logic State (`digitalRead`) | LED 1 (Red, GPIO 18) | LED 2 (Blue, GPIO 19) | Behavioral Summary |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Initial Boot / Reset** | Released (Open) | $\approx 3.3\,\text{V}$ | `HIGH` (1) | **ON** | **OFF** | Boot defaults verified via `setup()` |
| **Idle / Unpressed** | Released (Open) | $\approx 3.3\,\text{V}$ | `HIGH` (1) | **ON** | **OFF** | Pulled up internally; no floating noise |
| **User Interaction** | Pressed (Closed) | $\approx 0.0\,\text{V}$ | `LOW` (0) | **OFF** | **ON** | Complementary inversion active |
| **Release Interaction** | Released (Open) | $\approx 3.3\,\text{V}$ | `HIGH` (1) | **ON** | **OFF** | Returns immediately to idle complementary state |

---

## Success Verification

- [x] **Complementary Operation:** The two LEDs consistently exhibit strictly inverted states ($\text{LED}_1 = \neg \text{LED}_2$) at all times.
- [x] **Floating State Mitigation:** When the push-button is released, the `INPUT_PULLUP` resistor maintains a constant logic `HIGH`, preventing spurious switching caused by electrostatic or electromagnetic interference.
- [x] **Boot State Stability:** Upon hardware reset or power cycling, the microcontroller boots into the defined default output state (`LED 1 = ON`, `LED 2 = OFF`).
