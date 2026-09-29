#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// --- WIFI CREDENTIALS ---
const char* ssid = "Anjali";
const char* password = "01061998";

// --- AWS SERVER DETAILS ---
// Replace with your AWS Public IP if it changes tomorrow!
const char* serverName = "http://13.204.77.140:8000/sensor-data";

// --- SENSOR SETUP ---
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  
  // Start Sensor
  sensors.begin();

  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // 1. Read the Sensor
    sensors.requestTemperatures(); 
    float tempC = sensors.getTempCByIndex(0);
    
    // Check if sensor is broken/missing resistor
    if (tempC == -127.00) {
      Serial.println("Hardware Error: Check wiring and resistor!");
      delay(5000);
      return; // Skip sending data to AWS
    }

    // 2. Prepare the JSON payload
    // Note: DS18B20 doesn't have humidity, so we hardcode it to 0
    StaticJsonDocument<200> doc;
    doc["temperature"] = tempC;
    doc["humidity"] = 0.0;
    
    String jsonPayload;
    serializeJson(doc, jsonPayload);

    // 3. Send to AWS
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
      Serial.print("Success! Sent to AWS. Response Code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending to AWS: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  
  // Wait 10 seconds before sending the next reading
  delay(10000);
}