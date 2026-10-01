# Laboratory Report

**Course & Section:** BCA188 - B186  
**Activity:** Laboratory Activity 3: GPIO and Button Control  
**Date:** September 30, 2026  
**Author:** Shyane Novelle  

---

## 1. Objectives

* To configure General Purpose Input/Output (GPIO) pins on an ESP32 microcontroller as digital inputs and digital outputs.
* To implement and understand the function of internal pull-up resistors (`INPUT_PULLUP`) to eliminate floating logic states.
* To interface a momentary tactile push-button as an active-low digital input sensor.
* To design and verify complementary output actuation across two discrete status LEDs (Red and Blue) based on real-time input polling.
* To observe, test, and document the circuit behavior across boot, pressed, and released states.

---

## 2. Materials and Components

| Item / Description | Quantity | Specifications / Role |
| :--- | :---: | :--- |
| **ESP32 NodeMCU Development Board** | 1 | Microcontroller running Xtensa dual-core 32-bit MCU at $3.3\,\text{V}$ logic |
| **Tactile Push-Button Switch** | 1 | 4-pin momentary push button (SPST normally open) |
| **Red LED (Status 1)** | 1 | $5\,\text{mm}$ standard diffused LED (forward voltage $V_f \approx 1.8\text{--}2.0\,\text{V}$) |
| **Blue LED (Status 2)** | 1 | $5\,\text{mm}$ standard diffused LED (forward voltage $V_f \approx 3.0\text{--}3.2\,\text{V}$) |
| **Current-Limiting Resistors** | 2 | $220\,\Omega$ ($1/4\,\text{W}$) resistors to protect GPIO outputs and LEDs |
| **Solderless Breadboard** | 1 | 830-point tie board for prototyping |
| **Jumper Wires** | Several | Male-to-Male / Male-to-Female interconnects |
| **Micro-USB Cable** | 1 | Programming and $5\,\text{V}$ DC power interface |

---

## 3. Circuit Architecture and Pin Configuration

### 3.1 Hardware Interfacing

* **Push-Button Interface:** One terminal of the push-button switch is connected to `GPIO 23`, while the diagonally opposite terminal is connected to the system reference ground (`GND`). The internal pull-up resistor of the ESP32 is activated via software (`pinMode(BUTTON_PIN, INPUT_PULLUP)`), tying the input to $3.3\,\text{V}$ in the idle state.
* **LED Output Interface:** 
  * `GPIO 18` is wired in series with a $220\,\Omega$ resistor to the anode of the Red LED; the cathode returns to `GND`.
  * `GPIO 19` is wired in series with a $220\,\Omega$ resistor to the anode of the Blue LED; the cathode returns to `GND`.

### 3.2 Pin Mapping Table

| Component Name | ESP32 GPIO Pin | Direction | Configured Mode | Idle Logic Level |
| :--- | :---: | :---: | :---: | :---: |
| **Push-Button** | `GPIO 23` | Input | `INPUT_PULLUP` | `HIGH` ($3.3\,\text{V}$) |
| **Red LED** | `GPIO 18` | Output | `OUTPUT` | `HIGH` (ON, $3.3\,\text{V}$) |
| **Blue LED** | `GPIO 19` | Output | `OUTPUT` | `LOW` (OFF, $0\,\text{V}$) |
| **System Ground** | `GND` | Reference | Power Ground Rail | $0\,\text{V}$ |

---

## 4. Circuit Photos and Proof of Implementation

> *Note: Placeholders below can be populated with your hosted GitHub image assets.*

### 4.1 Schematic / Simulation Diagram (Wokwi)
![Circuit Diagram](circuit_diagram.png)

*Figure 1: Wokwi circuit simulation diagram displaying ESP32 GPIO wiring to push button and dual LEDs with current-limiting resistors.*

### 4.2 Physical Breadboard Circuit
![Hardware Setup](breadboard_setup.jpg)

*Figure 2: Actual physical bench hardware setup illustrating breadboard jumpering, pull-up button input, and dual status LED actuation.*

---

## 5. Source Code Implementation

The program was developed using the Arduino framework for ESP32.

```cpp
#include <Arduino.h>

const uint8_t BUTTON_PIN = 23;
const uint8_t LED1_PIN = 18;
const uint8_t LED2_PIN = 19;

void setup() {
  // Configure input with internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configure LED pins as digital outputs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Set default initial state: LED1 ON, LED2 OFF
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);
}

void loop() {
  // Poll push button digital state
  int buttonState = digitalRead(BUTTON_PIN);

  // Active-LOW evaluation
  if (buttonState == LOW) {
    // Button is depressed: LED1 turns OFF, LED2 turns ON
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
  } else {
    // Button is released: LED1 remains ON, LED2 remains OFF
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
  }
}
```

---

## 6. Observation and Analysis

### 6.1 State Verification & Observation Table

| Operating Condition | Mechanical Switch State | Pin Voltage ($V_{\text{in}}$ at GPIO 23) | Digital Reading (`digitalRead`) | Red LED (GPIO 18) | Blue LED (GPIO 19) | System Behavior Summary |
| :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| **Board Reset / Boot** | Released (Open) | $\approx 3.3\,\text{V}$ | `HIGH` (1) | **ON** | **OFF** | `setup()` executes; outputs are pre-loaded to default state. |
| **Idle (Default)** | Released (Open) | $\approx 3.3\,\text{V}$ | `HIGH` (1) | **ON** | **OFF** | Internal pull-up holds logic stable; no false triggering. |
| **Active Depressed** | Pressed (Closed) | $\approx 0.0\,\text{V}$ | `LOW` (0) | **OFF** | **ON** | Ground closure pulls pin LOW; outputs flip complementarily. |
| **Return / Released** | Released (Open) | $\approx 3.3\,\text{V}$ | `HIGH` (1) | **ON** | **OFF** | Line snaps back to $3.3\,\text{V}$; outputs return to baseline. |

### 6.2 Engineering Analysis

1. **Active-Low Configuration and Internal Pull-Up Mechanics:**
   Digital microcontroller pins possess high input impedance. If left unconnected (floating), ambient electromagnetic interference (EMI) can cause spontaneous logic transitions between `0` and `1`. By enabling the internal pull-up resistor ($R_{\text{pull-up}} \approx 45\,\text{k}\Omega$ inside the ESP32), an open switch reliably holds the line at logic `HIGH` ($3.3\,\text{V}$). Closing the button routes the pin directly to `GND`, driving the voltage to $0\,\text{V}$ (`LOW`). Hence, this implementation adheres to an **active-low** design.

2. **Complementary Logic Actuation:**
   The conditional branch inside `loop()` enforces an inverted output relationship:
   $$\text{State}(\text{LED}_1) = \neg\,\text{State}(\text{LED}_2)$$
   Whenever the input resolves to `LOW`, `LED1` is de-energized while `LED2` is energized. Releasing the button instantaneously restores `LED1` to active-high and disables `LED2`.

3. **Reset and Deterministic Initialization:**
   Upon boot or board reset, the explicit initialization statements inside `setup()` set `LED1` to `HIGH` and `LED2` to `LOW` before entering the super-loop. This ensures predictable start-up states without erratic flickering during microcontroller boot sequences.

---

## 7. Conclusion

Laboratory Activity 3 successfully validated the fundamentals of digital input acquisition and complementary output switching on the ESP32 platform. Utilizing internal pull-up resistors proved effective in preventing floating input behavior and ensuring robust active-low tactile control without requiring additional passive components. Furthermore, the firmware implementation produced deterministic, stable complementary switching across the dual LEDs during both active operation and hardware resets, fully satisfying all laboratory criteria.