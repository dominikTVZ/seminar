#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define PWM_CHANNEL 13
#define PWM_PIN 12
#define RESOLUTION 8
#define FREQUENCY 50

float temperature;
StaticJsonDocument<200> doc;

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
  int sensorValue = analogRead(32); // Čitanje analognog očitanja s pina 32
  float voltage = sensorValue * (5.0 / 4095.0); // Pretvaranje ADC vrijednosti u napon (u V)
  temperature = voltage * 100.0; // Pretvaranje napona u temperaturu (u °C)

  // Provjera temperature
  if (temperature > 14.0) {
    // Postavljanje PWM kanala
    ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);

    // Pokretanje elektromotora s ventilatorom
    for (int i = 0; i < 255; i++) {
      ledcWrite(PWM_CHANNEL, i);
      delay(1000);
    }
  }

  // Postavljanje temperature u JSON dokument
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





/*


  String temperatura = "22";

  // Postavljanje temperature u JSON dokument
  doc["temperatura"] = temperatura;

  // Serializacija JSON dokumenta
  String json;
  serializeJson(doc, json);
  http.collectHeaders(headerKeys, numberOfHeaders);
  int httpResponseCode = http.GET();
  Serial.println(httl.header("Server")); 
  const char *headerKeys[] = {"Server"};
  const size_t numberOfHeaders = 1;

  String temperatura = "22";
  String json = "{\"temperatura\": " + temperatura +"}";
  http.addHeader("Host", serverName);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json);
  










#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

float Vout;
float Temp;
StaticJsonDocument<200> doc;

// WiFi
const char* SSID = "konekcijski point";
const char* PASSWORD = "senna1960";

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
  // Inicijalizacija HTTP klijenta
  http.begin(serverName);
  
  Vout = analogRead(0);
  Temp = (Vout * 500) / 1023;

  // Slanje POST zahtjeva s JSON podacima
  doc["temperature"] = Temp;
  char buffer[200];
  serializeJson(doc, buffer);
  int httpResponseCode = http.POST(buffer);

  // Ispisivanje statusnog koda HTTP odgovora
  Serial.print("Status code: ");
  Serial.println(httpResponseCode);

  // Ispisivanje odgovora servera (opcionalno)
  Serial.println(http.getString());

  // Završavanje HTTP sesije
  http.end();

  // Odmor
  delay(15000);
}


    */