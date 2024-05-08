#include <WiFi.h>

const char* ssid = "PolyAdminn";
const char* password = "test12345678";
const char* server = "api.thingspeak.com";
const char* apiKey = "9VX6E81YCHZV5WTS";
const int sensorPin = 2;    // Define your sensor pin here
const int valvePin = 25;    // Pin connected to the electrovalve
const int buttonPin = 26;   // Pin connected to the button
volatile long pulse;
bool valveState = LOW;
bool wifiConnected = false;
const float maxFlowRate = 2.0; // Maximum flow rate in liters per minute
const float pulsesPerLiter = 300; // Number of pulses per liter from the flow sensor

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Assuming the button is connected to ground when pressed
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(sensorPin), increase, RISING);
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  // If not connected to Wi-Fi, attempt connection
  if (!wifiConnected) {
    connectToWiFi();
  }

  // Read the state of the button
  Serial.println(buttonState);

  if (buttonState == LOW) { // Button is pressed
    // Toggle the state of the electrovalve
    valveState = !valveState;
    digitalWrite(valvePin, valveState);

    // Wait until the button is released to prevent multiple toggles
    while (digitalRead(buttonPin) == LOW) {
      delay(10);
    }
  }

  // If connected to Wi-Fi, send data to ThingSpeak
  if (wifiConnected) {


    WiFiClient client;

    // Read the pulse count and reset it
    long pulseCount = pulse;
    pulse = 0;

    // Convert pulse count to flow rate
    float flowRate = pulseCount / pulsesPerLiter; // Calculate flow rate in liters

//    // Scale flow rate to account for maximum flow rate
//    flowRate = min(flowRate, maxFlowRate); // Ensure flow rate does not exceed maximum

    // Print flow rate to Serial monitor for visualization
    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.println(" L/min");

    // Prepare the URL for the ThingSpeak update
    String url = "/update?api_key=";
    url += apiKey;
    url += "&field1=";
    url += String(flowRate); // Send the scaled flow rate

    if (client.connect(server, 80)) {
      // Send the HTTP request
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + server + "\r\n" +
                   "Connection: close\r\n\r\n");

      // Wait for the response
      while (client.available()) {
        String line = client.readStringUntil('\r');
      }
      client.stop();
    }
  }
}

void increase() {
  pulse++; // Increment pulse when sensor event occurs
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  int attemptCount = 0;
  while (WiFi.status() != WL_CONNECTED && attemptCount < 10) {
    delay(500);
    attemptCount++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
  }
}
