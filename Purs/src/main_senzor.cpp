#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "temperature_sensor.h"



// WiFi
const char* SSID = "SSID";
const char* PASSWORD = "PASSWORD";

// HTTP
const char *serverName = "http://192.168.0.12/temperatura";
HTTPClient http;

void setup() {
  Serial.begin(9600);

  // Spajanje na WiFi
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Povezivanje na WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(2000);
  }
  Serial.println(WiFi.localIP());
}

void loop() {
  // Mjerenje temperature
  float temperature = measureTemperature();

 
  // Postavljanje temperature u JSON dokument
  StaticJsonDocument<200> doc;
  doc["temperatura"] = temperature;

  // Serializacija JSON dokumenta u string
  String json;
  serializeJson(doc, json);

  // Slanje POST zahtjeva s JSON podacima
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json);
  
  // Ispisivanje statusnog koda HTTP odgovora
  Serial.print("Status code: ");
  Serial.println(httpResponseCode);

  // Završavanje HTTP sesije
  http.end();

  // Odmor
  delay(5000);
}