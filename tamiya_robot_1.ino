#include <ESP8266WiFi.h>


/***
 * DRV8835
 */
#define PIN_AIN1 14 // Right Motor Blue
#define PIN_AIN2 12 // Right Motor Yellow
#define PIN_BIN1 13 // Left Motor Blue
#define PIN_BIN2 15 // Left Motor Yellow

#define PWM_FREQ 1000 // PWM Frequency: 1000Hz(1kHz)
#define PWM_RANGE 100 // PWM range: 100

void setup() {
  delay(10);
  Serial.begin(115200);

  // Init GPIO mode
  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);
  pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_BIN2, OUTPUT);

  // Initialize PWM
  analogWriteFreq(PWM_FREQ);
  analogWriteRange(PWM_RANGE);
}

void loop() {
  delay(3000);
  forward();
  delay(3000);
  stop();
  delay(3000);
  reverse();
}

void stop() {
  Serial.println("stop");
  analogWrite(PIN_AIN1, 0);
  analogWrite(PIN_AIN2, 0);
  analogWrite(PIN_BIN1, 0);
  analogWrite(PIN_BIN2, 0);
}

void forward() {
  Serial.println("forward");
  analogWrite(PIN_AIN1, 0);
  analogWrite(PIN_AIN2, 100);
  analogWrite(PIN_BIN1, 0);
  analogWrite(PIN_BIN2, 100);
}

void reverse() {
  Serial.println("reverse");
  analogWrite(PIN_AIN1, 100);
  analogWrite(PIN_AIN2, 0);
  analogWrite(PIN_BIN1, 100);
  analogWrite(PIN_BIN2, 0);
}
