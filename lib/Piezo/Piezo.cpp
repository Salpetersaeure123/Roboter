#include "Piezo.h"

#define OFF 0
#define ON 255
#define HALF 127
#define D 1.5

void Piezo::setup() {
  ledcSetup(0, 100000, 8);
  ledcAttachPin(13, 0);
}

void Piezo::signal() {
  //this generates one period of the loudest possible sound
  ledcWrite(0, ON);
  delayMicroseconds(106);
  ledcWrite(0, OFF);
  delayMicroseconds(106);
}

void Piezo::hupe() {
  // this generates one period of two tones simoultaneously
  ledcWrite(0, ON);
  delayMicroseconds(930*D);
  ledcWrite(0, HALF);
  delayMicroseconds(230*D);
  ledcWrite(0, OFF);
  delayMicroseconds(680*D);
  ledcWrite(0, HALF);
  delayMicroseconds(450*D);
  ledcWrite(0, ON);
  delayMicroseconds(440*D);
  ledcWrite(0, HALF);
  delayMicroseconds(680*D);
  ledcWrite(0, OFF);
  delayMicroseconds(230*D);
  ledcWrite(0, HALF);
  delayMicroseconds(1810*D);
  ledcWrite(0, ON);
  delayMicroseconds(230*D);
  ledcWrite(0, HALF);
  delayMicroseconds(680*D);
  ledcWrite(0, OFF);
  delayMicroseconds(440*D);
  ledcWrite(0, HALF);
  delayMicroseconds(450*D);
  ledcWrite(0, ON);
  delayMicroseconds(680*D);
  ledcWrite(0, HALF);
  delayMicroseconds(230*D);
  ledcWrite(0, OFF);
  delayMicroseconds(930*D);
}