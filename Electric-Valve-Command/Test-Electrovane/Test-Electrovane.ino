 #define RELAY_PIN 25 // ESP32 pin GPIO16 connected to the electromagnetic lock via the relay

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin GPIO16 as an output.
  pinMode(RELAY_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(RELAY_PIN, HIGH); // lock the door
  delay(4000);
  digitalWrite(RELAY_PIN, LOW);  // unlock the door
  delay(4000);
}