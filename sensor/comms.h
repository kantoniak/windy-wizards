#include <RCSwitch.h>

#define ALIVE_INTERVAL_MS 10000

unsigned long last_message_timestamp = 0;

enum MessageType : uint8_t {
  UNKNOWN = 0,
  ALIVE = 1,
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
  last_message_timestamp = millis();

  unsigned long int buffer = 0;
  buffer += (message->device_id << 8);
  buffer += (message->type << 4) & 0b11110000;
  buffer += (message->payload << 0) & 0b00001111;

  digitalWrite(LED_PIN, HIGH);
  transmitter->send(buffer, 16);
  digitalWrite(LED_PIN, LOW);
}

void comms_heartbeat(RCSwitch* transmitter, uint8_t device_id) {
#ifdef _DEBUG
  digitalWrite(DEBUG_PIN, !digitalRead(DEBUG_PIN));
#endif

  if (millis() - last_message_timestamp <= ALIVE_INTERVAL_MS) {
    return;
  }

  Message message;
  message.device_id = device_id;
  message.type = ALIVE;
  message.payload = true;
  send_message(transmitter, &message);
}