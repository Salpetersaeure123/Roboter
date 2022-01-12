#include <Arduino.h>

#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Motor.h"
#include <functional>

std::function<void()> functions[] = {
  //sinus
  [](){

  },
  //cosinus
  [](){

  },
  //sägezahn
  [](){

  }
};
#define SINUS 0
#define COSINUS 1
#define SAEGEZAHN 2

Motor a = Motor(18, 19);
Motor b = Motor(16, 17);
Motor c = Motor(26, 27);

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void printSpeeds() {
  Serial.print("Motor a: ");
  Serial.print(a.getSpeed());
  Serial.print(" Richtung: ");
  Serial.println(a.getDirection()?"Links":"Rechts");
  Serial.print("Motor b: ");
  Serial.print(b.getSpeed());
  Serial.print(" Richtung: ");
  Serial.println(b.getDirection()?"Links":"Rechts");
  Serial.print("Motor c: ");
  Serial.print(c.getSpeed());
  Serial.print(" Richtung: ");
  Serial.println(c.getDirection()?"Links":"Rechts");
}

void setup() {
  functions[0] = printSpeeds;
  functions[1] = [](){
    Serial.println("test");
  };

  functions[SINUS]();

  Serial.begin(115200);
  //Serial.println("Color View Test!");
    Wire.setPins(17,16);
  if (tcs.begin(TCS34725_ADDRESS)) {
    //Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    // while (1); // halt!
  }

  // use these three pins to drive an LED
// #if defined(ARDUINO_ARCH_ESP32)
//   ledcAttachPin(redpin, 1);
//   ledcSetup(1, 12000, 8);
//   ledcAttachPin(greenpin, 2);
//   ledcSetup(2, 12000, 8);
//   ledcAttachPin(bluepin, 3);
//   ledcSetup(3, 12000, 8);
// #else
//   pinMode(redpin, OUTPUT);
//   pinMode(greenpin, OUTPUT);
//   pinMode(bluepin, OUTPUT);
// #endif

  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
  }
  printSpeeds();
  a.setSpeed(-50);
  b.setSpeed(20);
  c.setSpeed(500);
  printSpeeds();
  c.setSpeed(0.5);
  b.setSpeed(0.3);
  printSpeeds();
  delay(100000);
}

// The commented out code in loop is example of getRawData with clear value.
// Processing example colorview.pde can work with this kind of data too, but It requires manual conversion to 
// [0-255] RGB value. You can still uncomments parts of colorview.pde and play with clear value.
void loop() {
  float red, green, blue;
  
  tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRGB(&red, &green, &blue);
  
  tcs.setInterrupt(true);  // turn off LED

  Serial.print("R:\t"); Serial.print(int(red)); 
  Serial.print("\tG:\t"); Serial.print(int(green)); 
  Serial.print("\tB:\t"); Serial.print(int(blue));

//  Serial.print("\t");
//  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\n");

//  uint16_t red, green, blue, clear;
//  
//  tcs.setInterrupt(false);  // turn on LED
//
//  delay(60);  // takes 50ms to read
//
//  tcs.getRawData(&red, &green, &blue, &clear);
//  
//  tcs.setInterrupt(true);  // turn off LED
//
//  Serial.print("C:\t"); Serial.print(int(clear)); 
//  Serial.print("R:\t"); Serial.print(int(red)); 
//  Serial.print("\tG:\t"); Serial.print(int(green)); 
//  Serial.print("\tB:\t"); Serial.print(int(blue));
//  Serial.println();


// #if defined(ARDUINO_ARCH_ESP32)
//   ledcWrite(1, gammatable[(int)red]);
//   ledcWrite(2, gammatable[(int)green]);
//   ledcWrite(3, gammatable[(int)blue]);
// #else
//   analogWrite(redpin, gammatable[(int)red]);
//   analogWrite(greenpin, gammatable[(int)green]);
//   analogWrite(bluepin, gammatable[(int)blue]);
// #endif
}
