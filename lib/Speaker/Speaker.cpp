#include "Speaker.h"

#define OFF 0
#define ON 255
#define HALF 127


void Speaker::setup() {
  ledcSetup(0, 100000, 8);
  ledcAttachPin(SPEAKER_PIN, 0);
  pinMode(SPEAKER_PIN, OUTPUT);
}

void Speaker::signal() {
  //this generates one period of the loudest possible sound
  ledcWrite(0, ON);
  delayMicroseconds(155);
  ledcWrite(0, OFF);
  delayMicroseconds(155);
}

void Speaker::hupe(double d) {
  // this generates one period of two tones simoultaneously
  ledcWrite(0, ON);
  delayMicroseconds(930*d);
  ledcWrite(0, HALF);
  delayMicroseconds(230*d);
  ledcWrite(0, OFF);
  delayMicroseconds(680*d);
  ledcWrite(0, HALF);
  delayMicroseconds(450*d);
  ledcWrite(0, ON);
  delayMicroseconds(440*d);
  ledcWrite(0, HALF);
  delayMicroseconds(680*d);
  ledcWrite(0, OFF);
  delayMicroseconds(230*d);
  ledcWrite(0, HALF);
  delayMicroseconds(1810*d);
  ledcWrite(0, ON);
  delayMicroseconds(230*d);
  ledcWrite(0, HALF);
  delayMicroseconds(680*d);
  ledcWrite(0, OFF);
  delayMicroseconds(440*d);
  ledcWrite(0, HALF);
  delayMicroseconds(450*d);
  ledcWrite(0, ON);
  delayMicroseconds(680*d);
  ledcWrite(0, HALF);
  delayMicroseconds(230*d);
  ledcWrite(0, OFF);
  delayMicroseconds(930*d);
}