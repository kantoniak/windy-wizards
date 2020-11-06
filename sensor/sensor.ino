#include <RCSwitch.h>

// Pins
#define TRANSMIT_PIN 3
#define LED_PIN 1
#define SWITCH_PIN 2

// Radio communications
const uint8_t device_id = 0x8C;
RCSwitch transmitter = RCSwitch();

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  transmitter.enableTransmit(TRANSMIT_PIN);
}

void loop() {
  // Light LED when window closed
  /*
  bool windowClosed = digitalRead(SWITCH_PIN);
  digitalWrite(LED_PIN, windowClosed);
  */

  // Send device ID
  digitalWrite(LED_PIN, HIGH);
  transmitter.send(device_id, 8);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
