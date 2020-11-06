#include <VirtualWire.h>

// Pins
#define TRANSMIT_PIN 0
#define LED_PIN 1
#define SWITCH_PIN 2

// Radio communications
#define BITS_PER_SECOND 2000
uint8_t message_buffer[4];

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  vw_set_tx_pin(TRANSMIT_PIN);
  vw_setup(BITS_PER_SECOND);
  
  // Device ID?
  message_buffer[0] = 0b0001;
  message_buffer[1] = 0b0011;
  message_buffer[2] = 0b0111;
  message_buffer[3] = 0b1111;
}

void loop() {
  // Light LED when window closed
  /*
  bool windowClosed = digitalRead(SWITCH_PIN);
  digitalWrite(LED_PIN, windowClosed);
  */

  // Send device ID
  digitalWrite(LED_PIN, HIGH);
  vw_send(message_buffer, sizeof(message_buffer));
  vw_wait_tx();
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
