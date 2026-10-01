#include <Arduino.h>

const uint8_t BUTTON_PIN = 23;
const uint8_t LED1_PIN = 18;
const uint8_t LED2_PIN = 19; 

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == LOW) {
    
    digitalWrite(LED1_PIN, LOW);   
    digitalWrite(LED2_PIN, HIGH);  
  } else {

    digitalWrite(LED1_PIN, HIGH);  
    digitalWrite(LED2_PIN, LOW);   
  }
}