#include <WiFi.h>

const char* ssid = "PolyAdminn";
const char* password = "test12345678";
const char* server = "api.thingspeak.com";
const char* apiKey = "9VX6E81YCHZV5WTS";

const int sensorPin = 2;    // Sensor pin
const int valvePin = 25;    // Electrovalve pin
const int buttonPin = 26;   // Button pin

volatile long pulse = 0; // Pulse count
long lastPulseCount = 0; // Last pulse count sent to ThingSpeak
bool valveState = LOW; // Initial valve state

void setup() {
    // Configure pins
    pinMode(sensorPin, INPUT);
    pinMode(valvePin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);

    // Initialize serial communication
    Serial.begin(115200);
    delay(100);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to Wi-Fi");

    // Attach interrupt for pulse count
    attachInterrupt(digitalPinToInterrupt(sensorPin), increasePulse, RISING);
}

void loop() {
    // Handle button press for valve control
    int buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
        // Toggle valve state
        valveState = !valveState;
        digitalWrite(valvePin, valveState);

        // Wait for button release
        while (digitalRead(buttonPin) == LOW) {
            delay(10);
        }
    }

    // Check if connected to Wi-Fi
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;

        // Read the current pulse count
        long currentPulseCount = pulse;

        // Check if pulse count has changed since last update
        if (currentPulseCount != lastPulseCount) {
            // Prepare URL for ThingSpeak update
            String url = "/update?api_key=" + String(apiKey) + "&field2=" + String(currentPulseCount);

            // Connect to ThingSpeak server
            if (client.connect(server, 80)) {
                // Send HTTP GET request
                client.print("GET " + url + " HTTP/1.1\r\n");
                // Convert the server C-style string to a String object and concatenate
                client.print(String("Host: ") + server + "\r\n");
                client.print("Connection: close\r\n\r\n");

                // Read server response
                while (client.available()) {
                    String line = client.readStringUntil('\r');
                    // You may choose to handle the server response if necessary
                }
                client.stop();

                // Update last pulse count
                lastPulseCount = currentPulseCount;
            } else {
                Serial.println("Failed to connect to ThingSpeak server");
            }
        }
    } else {
        Serial.println("Wi-Fi connection lost");
    }

    // Add delay to control frequency of data sending
    delay(2000); //delay of 2s
}

void increasePulse() {
    // Increment pulse count when sensor is triggered
    pulse++;
}
