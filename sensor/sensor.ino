#define RADIO_PIN 0
#define LED_PIN 1
#define SWITCH_PIN 2

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
}

void loop() {
  // Light LED when window closed
  bool windowOpen = digitalRead(SWITCH_PIN);
  digitalWrite(LED_PIN, !windowOpen);
}
