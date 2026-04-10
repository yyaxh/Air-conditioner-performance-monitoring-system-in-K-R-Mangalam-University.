#define BLYNK_TEMPLATE_ID "TMPL3pzcpWWpv"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "QjGO2h6Nae1kR6W3nTEIkUiM1WbIJxoq"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <BlynkSimpleEsp8266.h>

// WiFi
const char* ssid = "Yash";
const char* password = "yourmomsucks";

// ThingSpeak
unsigned long channelID = 3296003;
const char* writeAPIKey = "ZQLD60VC5M01TP2X";

WiFiClient client;

// Sensor values (smooth simulation)
float temperature = 25;
float humidity = 60;
int airQuality = 300;
int co2 = 500;
int light = 400;
int motion = 0;

// Energy system
float power = 0;
float energy = 0;
float cost = 0;

// Alert
String alert = "Normal";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected");

  ThingSpeak.begin(client);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {

  Blynk.run();

  // 🔁 Smooth sensor variation
  temperature += random(-1, 2);
  humidity += random(-2, 3);
  airQuality += random(-10, 10);
  co2 += random(-20, 20);
  light += random(-30, 30);

  temperature = constrain(temperature, 22, 30);
  humidity = constrain(humidity, 40, 70);
  airQuality = constrain(airQuality, 250, 400);
  co2 = constrain(co2, 400, 800);
  light = constrain(light, 200, 800);

  motion = random(0, 2);

  // ⚡ Energy Logic
  if (temperature > 26 && motion == 1) {
    power = 1500; // AC ON
  } 
  else if (motion == 1) {
    power = 200;  // Fan ON
  } 
  else {
    power = 0;    // OFF
  }

  // Energy (kWh)
  energy += (power * 20.0) / (1000 * 3600);

  // Cost (₹)
  cost = energy * 8;

  // 🚨 Alerts
  alert = "Normal";

  if (temperature > 28) {
    alert = "High Temp!";
  }
  else if (airQuality > 350) {
    alert = "Poor AQ!";
  }
  else if (co2 > 700) {
    alert = "High CO2!";
  }

  // 📱 Blynk Output
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, airQuality);
  Blynk.virtualWrite(V3, co2);
  Blynk.virtualWrite(V4, light);
  Blynk.virtualWrite(V5, motion);
  Blynk.virtualWrite(V6, alert);
  Blynk.virtualWrite(V7, energy);
  Blynk.virtualWrite(V8, cost);

  // ☁️ ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, airQuality);
  ThingSpeak.setField(4, co2);
  ThingSpeak.setField(5, light);
  ThingSpeak.setField(6, motion);
  ThingSpeak.setField(7, energy);
  ThingSpeak.setField(8, cost);

  ThingSpeak.writeFields(channelID, writeAPIKey);

  // Serial Output (for demo)
  Serial.println("------ DATA ------");
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("AQI: "); Serial.println(airQuality);
  Serial.print("CO2: "); Serial.println(co2);
  Serial.print("Light: "); Serial.println(light);
  Serial.print("Motion: "); Serial.println(motion);
  Serial.print("Energy: "); Serial.println(energy);
  Serial.print("Cost: "); Serial.println(cost);
  Serial.print("Alert: "); Serial.println(alert);

  delay(10000);
}