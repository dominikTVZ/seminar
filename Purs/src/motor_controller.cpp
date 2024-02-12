#include "motor_controller.h"
#include <Arduino.h>

void startMotor() {
    ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);
  
    // Pokretanje elektromotora s ventilatorom
    for (int i = 0; i < 255; i++) {
        ledcWrite(PWM_CHANNEL, i);
        delay(1000);
    }
}
