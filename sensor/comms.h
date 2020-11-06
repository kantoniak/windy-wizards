#include <RCSwitch.h>

enum MessageType : uint8_t {
  UNKNOWN = 0,
  PING = 1,
  WINDOW_OPEN = 2
};

/*
  Communication protocol:
  - 8 bits for device ID
  - 4 bits for message type
  - 4 bits for payload
*/
struct Message {
  uint8_t device_id;
  MessageType type;
  uint8_t payload;
};

void send_message(RCSwitch* transmitter, Message* message) {
  unsigned long int buffer = 0;
  buffer += (message->device_id << 8);
  buffer += (message->type << 4) & 0b11110000;
  buffer += (message->payload << 0) & 0b00001111;

  digitalWrite(LED_PIN, HIGH);
  transmitter->send(buffer, 16);
  digitalWrite(LED_PIN, LOW);
  delay(200);
}