#include "Speaker.h"

#define OFF 0
#define ON 255
#define HALF 127


void Speaker::setup() {
  // setup pwm
  ledcAttachPin(SPEAKER_PIN, 0);
  ledcSetup(0, 100000, 8);
}

void Speaker::signal() {
  //this generates one period of the loudest possible sound
  ledcWrite(0, ON);
  delayMicroseconds(155);
  ledcWrite(0, OFF);
  delayMicroseconds(155);
}

void Speaker::hupe(double d) {
  // this generates one period of two tones simoultaneously (a third)
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

void Speaker::startHupe() {
  ledcWriteTone(0, 335);
}

void Speaker::startSignal() {
  ledcWriteTone(0, 500);
}

void Speaker::startSignal2() {
  ledcWriteTone(0, 3200);
}

void Speaker::stopHupe() {
  ledcWriteTone(0, 0);
}