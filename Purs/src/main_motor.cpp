#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const int PWM_CHANNEL = 13;
const int PWM_PIN = 12;
const int RESOLUTION = 8;
const int FREQUENCY = 50;

void startMotor();
bool motorRunning = false;
void stopMotor();

// WiFi
const char* SSID = "konekcijski point";
const char* PASSWORD = "senna1960";



// HTTP
const char *serverName = "http://192.168.0.13/temperatura";
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
            // Pokretanje motora ako nije već pokrenut
            if (!motorRunning) {
                startMotor();
                motorRunning = true;
            }
        } else {
            // Zaustavljanje motora ako je temperatura manja od 13 i motor je pokrenut
            if (motorRunning) {
                stopMotor();
                motorRunning = false;
            }
        }
    }

    // Završavanje HTTP sesije
    http.end();

    // Odmor
    delay(5000);
}

void stopMotor() {
    // Postavljanje PWM signala na nulu kako bi se motor zaustavio
    ledcWrite(PWM_CHANNEL, 0);
}

void startMotor() {
    ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);
    
    // Pokretanje elektromotora s ventilatorom
    for (int i = 0; i < 120; i+=2) {
        ledcWrite(PWM_CHANNEL, i);

        delay(100);
    }
}

