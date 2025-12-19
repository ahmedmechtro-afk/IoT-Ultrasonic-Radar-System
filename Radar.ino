#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#include <ESP_Mail_Client.h>   // For Email Alerts

// ---------------- WiFi Settings ----------------
const char* ssid = "Imtiaz 2.4";
const char* password = "imtiaz786";

// ---------------- ThingSpeak Settings ----------------
String apiKey = "6NGGFUF5DZ8V2GHB";
String server = "http://api.thingspeak.com/update";

// ---------------- Email Settings ----------------
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "your_email@gmail.com"      // Sender email
#define AUTHOR_PASSWORD "YOUR_APP_PASSWORD"      // App password
#define RECIPIENT_EMAIL "receiver_email@gmail.com"

SMTPSession smtp;

// ---------------- Pins ----------------
#define trigPin 21
#define echoPin 20
#define servoPin 9
#define buzzerPin 10

Servo radarServo;

// ---------------- Variables ----------------
long duration;
float distance;
int angle = 0;
int step = 1;                 // Smooth servo motion
bool sweepForward = true;

unsigned long lastServoMove = 0;
unsigned long servoInterval = 50;   // Servo update interval (ms)

unsigned long lastThingSpeak = 0;
unsigned long tsInterval = 2000;     // ThingSpeak update interval (ms)

float alertDistance = 20.0;          // Alert threshold (cm)
bool emailSent = false;              // Avoid repeated emails

// ---------------- Email Function ----------------
void sendEmailAlert(float dist) {
  SMTP_Message message;
  message.sender.name = "ESP32 Radar System";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "⚠ Object Detected Alert";
  message.addRecipient("User", RECIPIENT_EMAIL);

  String textMsg = "Alert! Object detected at distance: " + String(dist) + " cm.";
  message.text.content = textMsg.c_str();

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  smtp.connect(&session);
  if (MailClient.sendMail(&smtp, &message)) {
    Serial.println("Email alert sent successfully");
  } else {
    Serial.println("Failed to send email");
  }
}

void setup() {
  Serial.begin(115200);

  radarServo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // -------- WiFi Connection --------
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

  // -------- Servo Sweep --------
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

    // -------- Ultrasonic Distance Measurement --------
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH, 30000);
    distance = duration * 0.034 / 2;

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°  Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // -------- Buzzer + Email Alert --------
    if (distance > 0 && distance <= alertDistance) {
      digitalWrite(buzzerPin, HIGH);

      if (!emailSent) {
        sendEmailAlert(distance);
        emailSent = true;
      }
    } else {
      digitalWrite(buzzerPin, LOW);
      emailSent = false;
    }
  }

  // -------- Send Data to ThingSpeak --------
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
}
