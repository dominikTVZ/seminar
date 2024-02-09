#include <Arduino.h>
#define PWM_CHANNEL 0
#define PWM_PIN 16
#define RESOLUTION 8
#define FREQUENCY 50
// put function declarations here:


void setup()
{
ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
ledcAttachPin(PWM_PIN, PWM_CHANNEL);
}
void loop()
{
  for(int i=0; i<255; i++){
    ledcWrite(PWM_CHANNEL, i);
    delay(10);
  }
}