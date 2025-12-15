#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

// --- WiFi Settings ---
const char* ssid = "Imtiaz 2.4";
const char* password = "imtiaz786";

// --- ThingSpeak Settings ---
String apiKey = "6NGGFUF5DZ8V2GHB";
String server = "http://api.thingspeak.com/update";

// --- Pins ---
#define trigPin 21
#define echoPin 20
#define servoPin 9

Servo radarServo;

// --- Variables ---
long duration;
float distance;
int angle = 0;
int step = 1; // smaller step for smoother motion
bool sweepForward = true;

unsigned long lastServoMove = 0;
unsigned long servoInterval = 50; // servo update interval in ms

unsigned long lastThingSpeak = 0;
unsigned long tsInterval = 2000; // send data every 2 seconds

void setup() {
  Serial.begin(115200);
  
  radarServo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  unsigned long currentMillis = millis();

  // --- Servo Sweep ---
  if (currentMillis - lastServoMove >= servoInterval) {
    lastServoMove = currentMillis;
    radarServo.write(angle);

    if (sweepForward) {
      angle += step;
      if (angle >= 180) sweepForward = false;
    } else {
      angle -= step;
      if (angle <= 0) sweepForward = true;
    }

    // Measure distance
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms to avoid blocking
    distance = duration * 0.034 / 2;

    Serial.print("Angle: "); Serial.print(angle);
    Serial.print("° - Distance: "); Serial.print(distance); Serial.println(" cm");
  }

  // --- Send data to ThingSpeak every 2 seconds ---
  if (currentMillis - lastThingSpeak >= tsInterval) {
    lastThingSpeak = currentMillis;
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = server + "?api_key=" + apiKey + "&field1=" + String(distance) + "&field2=" + String(angle);
      http.begin(url);
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.println("Data sent to ThingSpeak");
      } else {
        Serial.println("Error sending data");
      }
      http.end();
    }
  }
} // <-- closing brace for loop
