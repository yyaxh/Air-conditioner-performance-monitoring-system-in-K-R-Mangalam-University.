#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "YASH";
const char* password = "Dinero1122";
unsigned long channelID = 3109067;
const char* apiKey = "RPQXEV15KKX1ZM2M";

WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  ThingSpeak.begin(client);
}

void loop() {
  float temp = random(22, 30);     // Fake temperature
  float humidity = random(40, 70); // Fake humidity
  float power = random(1, 10);     // Fake power usage
  int occupancy = random(0, 2);    // Fake occupancy (1 or 0)

  Serial.println("Uploading simulated data...");
  Serial.println("Temp: " + String(temp));
  Serial.println("Humidity: " + String(humidity));
  Serial.println("Power: " + String(power));
  Serial.println("Occupancy: " + String(occupancy));

  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, power);
  ThingSpeak.setField(4, occupancy);

  int result = ThingSpeak.writeFields(channelID, apiKey);
  if (result == 200) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.println("Error: " + String(result));
  }

  delay(20000); // 20 seconds
}