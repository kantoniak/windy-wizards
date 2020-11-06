#define TRANSMIT_PIN 3
#define LED_PIN 1
#define DEBUG_PIN 0
#define SWITCH_PIN 2
#define DEVICE_ID 0x8C

#include <RCSwitch.h>
#include "comms.h"

RCSwitch transmitter = RCSwitch();

struct WindowState {
  bool initialized = false;            ///< Force sending update on startup
  bool open = false;                   ///< Whether window was open on last change.
  volatile bool change_flag = false;   ///< Set by interrupt on switch change.
} window_state;

void on_switch() {
  window_state.change_flag = true;
  digitalWrite(DEBUG_PIN, HIGH);
}

void handle_window_changed(WindowState* window) {
  window->change_flag = false;
  digitalWrite(DEBUG_PIN, LOW);
  window->open = digitalRead(SWITCH_PIN);
  send_window_open(window);
}

void send_window_open(WindowState* window) {
  Message message;
  message.device_id = DEVICE_ID;
  message.type = WINDOW_OPEN;
  message.payload = window->open;
  send_message(&transmitter, &message);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(DEBUG_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  digitalWrite(DEBUG_PIN, HIGH);

  attachInterrupt(/* P2 */ 0, on_switch, CHANGE);

  transmitter.enableTransmit(TRANSMIT_PIN);
}

void loop() {
  if (!window_state.initialized) {
    window_state.open = digitalRead(SWITCH_PIN);
    send_window_open(&window_state);
    window_state.initialized = true;
  }

  if (window_state.change_flag) {
    handle_window_changed(&window_state);
  }
}
