const int valvePin = 25;    // Pin connected to the electrovalve
const int buttonPin = 26;   // Pin connected to the button
bool valveState = LOW;      // Initial state of the electrovalve

void setup() {
  pinMode(valvePin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Assuming the button is connected to ground when pressed
}

void loop() {
  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) { // Button is pressed
    // Toggle the state of the electrovalve
    valveState = !valveState;
    digitalWrite(valvePin, valveState);
    
    // Wait until the button is released to prevent multiple toggles
    while (digitalRead(buttonPin) == LOW) {
      delay(10);
    }
  }
}
