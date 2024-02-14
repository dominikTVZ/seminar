#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const int PWM_CHANNEL = 13;
const int PWM_PIN = 12;
const int RESOLUTION = 8;
const int FREQUENCY = 50;
void startMotor();

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
    // Slanje GET zahtjeva za dohvaćanje temperature
    http.begin(serverName);
    Serial.print("u loop-u sam");
    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK) {
        // Primanje odgovora s temperaturom
        String response = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, response);
        int temperatura = doc["temperatura"];
        Serial.print(temperatura);

        // Provjera temperature i izvršavanje odgovarajuće akcije
        if (temperatura > 13) {
            // Pokretanje motora
           
            startMotor();
        }
    }

    // Završavanje HTTP sesije
    http.end();

    // Odmor
    delay(5000);
}


void startMotor() {
    ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);
    
    // Pokretanje elektromotora s ventilatorom
    for (int i = 0; i < 255; i++) {
        ledcWrite(PWM_CHANNEL, i);
        delay(1000);
    }
}


