#include "temperature_sensor.h"

float measureTemperature() {
  int sensorValue = analogRead(32); // Čitanje analognog očitanja s pina 32
  float voltage = sensorValue * (5.0 / 4095.0); // Pretvaranje ADC vrijednosti u napon (u V)
  return voltage * 100.0; // Pretvaranje napona u temperaturu (u °C)
}

