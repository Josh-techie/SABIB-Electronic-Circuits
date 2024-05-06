#include <WiFi.h>

const char* ssid = "PolyAdminn";
const char* password = "test12345678";
const char* server = "api.thingspeak.com";
const char* apiKey = "9VX6E81YCHZV5WTS";
const int sensorPin = 2; // Define your sensor pin here
const int electroVan = 3;
volatile long pulse;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(electroVan, INPUT);
  
  Serial.begin(115200);
  delay(100);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");

  delay(1000);
  attachInterrupt(digitalPinToInterrupt(sensorPin), increase, RISING);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    // Read the pulse count and reset it
    long pulseCount = pulse;
    pulse = 0;

    //Serial.print("Pulse: ");
    Serial.println(pulseCount);

    // Prepare the URL for the ThingSpeak update
    String url = "/update?api_key=";
    url += apiKey;
    url += "&field1=";
    url += String(pulseCount); // Send the pulse count

    if (client.connect(server, 80)) {
      // Send the HTTP request
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + server + "\r\n" +
                   "Connection: close\r\n\r\n");
      //Serial.println("Data sent to ThingSpeak");

      // Wait for the response
      while (client.available()) {
        String line = client.readStringUntil('\r');
        //Serial.print(line);
      }
      client.stop();
    } else {
      Serial.println("Failed to connect to ThingSpeak server");
    }
  } else {
    Serial.println("WiFi connection lost");
  }

  delay(12000); // Send data every 12 seconds
}

void increase() {
  pulse++;
}
