// -------------------------------
// Blynk & Device Config
// -------------------------------
#define BLYNK_TEMPLATE_ID "TMPL3pzcpWWpv"
#define BLYNK_TEMPLATE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "QjGO2h6Nae1kR6W3nTEIkUiM1WbIJxoq"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// -------------------------------
// WiFi Credentials
// -------------------------------
char ssid[] = "YASH 329";
char pass[] = "00000000";

// -------------------------------
// Sensor Pins
// -------------------------------
#define DHTPIN 2         // D4 = GPIO2
#define DHTTYPE DHT11
#define PIR_PIN 14       // D5 = GPIO14

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Used for room empty alert (5 minutes)
unsigned long lastMotionTime = 0;
const unsigned long emptyRoomDelay = 300000; // 5 minutes

// -------------------------------
// Send Sensor Data to Blynk
// -------------------------------
void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    lastMotionTime = millis();
  }

  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor Error");
    return;
  }

  // Send to Blynk
  Blynk.virtualWrite(V0, h);     // Humidity
  Blynk.virtualWrite(V1, t);     // Temperature
  Blynk.virtualWrite(V2, motion); // PIR

  // Serial Monitor Print
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" | Temp: ");
  Serial.print(t);
  Serial.print(" | Motion: ");
  Serial.println(motion);
}

// -------------------------------
// Temperature Alert (> 30°C)
// -------------------------------
void checkHighTemp() {
  float t = dht.readTemperature();
  if (!isnan(t) && t > 30) {
    Blynk.logEvent("high_temp", "Room temperature is above 30°C");
  }
}

// -------------------------------
// Humidity Alert (> 65%)
// -------------------------------
void checkHumidity() {
  float h = dht.readHumidity();
  if (!isnan(h) && h > 65) {
    Blynk.logEvent("high_humidity", "Humidity is above safe limit");
  }
}

// -------------------------------
// Room Empty Alert (no motion 5 mins)
// -------------------------------
void checkRoomEmpty() {
  if (millis() - lastMotionTime > emptyRoomDelay) {
    Blynk.logEvent("room_empty", "Room empty for more than 5 minutes");
  }
}

// -------------------------------
// Setup
// -------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);

  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Timers for periodic checks
  timer.setInterval(2000L, sendSensorData);     // Every 2 sec
  timer.setInterval(10000L, checkHighTemp);     // Every 10 sec
  timer.setInterval(10000L, checkHumidity);     // Every 10 sec
  timer.setInterval(15000L, checkRoomEmpty);    // Every 15 sec
}

// -------------------------------
// Main Loop
// -------------------------------
void loop() {
  Blynk.run();
  timer.run();
}
