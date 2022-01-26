
// #include "Arduino.h"
// #include "driver/dac.h"
// #define potiFrequency 34
// #define potiOverdrive 35
// #define up 33
// #define down 32
// int i; int tu; int td; volatile int menu;
// volatile int menu2;
// int d; int vRead; int vWrite; float factor = 1.00;
// int vReadOld;
// int debounce = 200; int f; volatile int success=0;
// bool taster = true;
// unsigned long lastPotiChange = 0;

// /*void IRAM_ATTR interruptUp() {
//   menu = menu + 1;
//   success = 1;
//   }
//   void IRAM_ATTR interruptDown() {
//   menu = menu - 1;
//   success = 1;
//   }*/

// int sineValsNew [] = {
//   127,
//   136, 145, 154, 163, 171, 179, 186, 192, 198, 203, 207, 211, 213, 215,
//   215,
//   215, 213, 211, 207, 203, 198, 192, 186, 179, 171, 163, 154, 145, 136,
//   127,
//   118, 109, 100, 91, 83, 75, 68, 62, 56, 51, 47, 43, 41, 39,
//   39,
//   39, 41, 43, 47, 51, 56, 62, 68, 75, 83, 91, 100, 109, 118,
//   // 60 elements
// };
// int rectangleValsNew [] = {
//   127,
//   215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215,
//   215,
//   215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215,
//   127,
//   39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
//   39,
//   39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
//   // 60 elements
// };
// int triangleValsNew [] = {
//   127,
//   133, 139, 145, 151, 157, 163, 169, 175, 181, 187, 193, 199, 205, 211,
//   215,
//   211, 205, 199, 193, 187, 181, 175, 169, 163, 157, 151, 145, 139, 133,
//   127,
//   121, 115, 109, 103, 97, 91, 85, 79, 73, 67, 61, 55, 49, 43,
//   39,
//   43, 49, 55, 61, 67, 73, 79, 85, 91, 97, 103, 109, 115, 121,
//   // 60 elements
// };
// int sawtoothValsNew [] = {
//   127,
//   130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169,
//   172, 175, 178, 181, 184, 187, 190, 193, 196, 199, 202, 205, 208, 211,
//   215,
//   127,
//   39,
//   43, 46, 49, 52, 55, 58, 61, 64, 67, 70, 73, 76, 79, 82,
//   85, 88, 91, 94, 97, 100, 103, 106, 109, 112, 115, 118, 121, 124,
//   // 60 elements
// };
// int overdriveValsNew [] {
//   127,
//   136, 145, 154, 163, 171, 179, 186, 192, 198, 203, 207, 211, 213, 215,
//   215,
//   215, 213, 211, 207, 203, 198, 192, 186, 179, 171, 163, 154, 145, 136,
//   127,
//   118, 109, 100, 91, 83, 75, 68, 62, 56, 51, 47, 43, 41, 39,
//   39,
//   39, 41, 43, 47, 51, 56, 62, 68, 75, 83, 91, 100, 109, 118,
//   // 60 elements
// };

// void sineTone () {
//   for (i = 0; i <= 59; i = i + 1) {
//     dac_output_voltage(DAC_CHANNEL_1, sineValsNew [i]);
//     delayMicroseconds(d);
//   }
// }
// void rectangleTone () {
//   for (i = 0; i <= 59; i = i + 1) {
//     dac_output_voltage(DAC_CHANNEL_1, rectangleValsNew [i]);
//     delayMicroseconds(d);
//   }
// }
// void triangleTone () {
//   for (i = 0; i <= 59; i = i + 1) {
//     dac_output_voltage(DAC_CHANNEL_1, triangleValsNew [i]);
//     delayMicroseconds(d);
//   }
// }
// void sawtoothTone () {
//   for (i = 0; i <= 59; i = i + 1) {
//     dac_output_voltage(DAC_CHANNEL_1, sawtoothValsNew [i]);
//     delayMicroseconds(d);
//   }
// }
// void overdriveTone () {
//   for (i = 0; i <= 59; i = i + 1) {
//     dac_output_voltage(DAC_CHANNEL_1, overdriveValsNew [i]);
//     delayMicroseconds(d);
//   }
// }

// void off() {
//   dac_output_voltage(DAC_CHANNEL_1, 127);
// }
// void getDelay() {
//   int f_ = analogRead(potiFrequency);
//   // f_ = 20;
//   if(abs(f_ - f) > 5) {
//     lastPotiChange = millis();
//     f = f_;
//     // Serial.println("delay change");
//   } else
//     return;
//   int d_ = f / 15;
//   if (d_ < 1) {
//     d_ = 1;
//   }
//   d = d_;
// }
// void getOverdrive() {
//   if(menu != 5)
//     return;
//   int vRead_ = analogRead(potiOverdrive);
//   vRead_ += 1;
//   if(abs(vRead_ - vRead) > 5) {
//     lastPotiChange = millis();
//     vRead = vRead_;
//     // Serial.println("overdrive change");
//   }
// }
// void getMenu() {
//   if(taster) {
//   /*if (success = 1) {
//     off();
//     delay(debounce);
//     success = 0;
//     }*/
//     int menu_ = menu;
//   tu=digitalRead(up);
//   td=digitalRead(down);
//   if(tu==0) {
//     menu_=menu_+1;
//     off();
//     vTaskDelay(debounce);
//   }
//   if(td==0) {
//     menu_=menu_-1;
//     off();
//     vTaskDelay(debounce);
//   }
//   if (menu_ > 5) {
//     menu_ = 5;
//   }
//   if (menu_ < 1) {
//     menu_ = 1;
//   }
//   if(menu_ != menu) {
//     menu = menu_;
//   }
//   }
//   if(Serial.available()) {
//     while(Serial.available()) {
//     Serial.read();
//     }
//     Serial.println("mode change");
//     taster = !taster;
//     Serial.println(taster);
//   }
// }

// void calculateOverdriveVals() {
//   if(vReadOld == vRead)
//     return;
//   vReadOld = vRead;
//   factor = vRead / 512.00;
//   factor = factor + 1.00;
//   for (i = 0; i <= 59; i = i + 1) {
//     vWrite = sineValsNew[i];
//     vWrite = vWrite - 127;
//     vWrite = vWrite * factor;
//     vWrite = vWrite + 127;
//     if (vWrite > 215) {
//       vWrite = 215;
//     }
//     if (vWrite < 39) {
//       vWrite = 39;
//     }
//     overdriveValsNew[i] = vWrite;
//   }
// }
// void output () {
//   if (menu == 1) {
//     sineTone();
//   }
//   if (menu == 2) {
//     triangleTone();
//   }
//   if (menu == 3) {
//     sawtoothTone();
//   }
//   if (menu == 4) {
//     rectangleTone();
//   }
//   if (menu == 5) {
//     calculateOverdriveVals();
//     overdriveTone();
//   }
// }

// void loop2(void* parm) {
//   for(;;) {
//     getMenu();
//     vTaskDelay(100);
//     if(taster && (millis()-lastPotiChange)>500) {
//       getDelay();
//       getOverdrive();
//     }
//   }
// }
// void setup() {
//   Serial.begin(115200);
//   dac_output_enable(DAC_CHANNEL_2);
//   dac_output_voltage(DAC_CHANNEL_2, 127);
//   dac_output_enable(DAC_CHANNEL_1);
//   off();
//   pinMode(up, INPUT_PULLUP);
//   //attachInterrupt(up, interruptUp, FALLING);
//   pinMode(down, INPUT_PULLUP);
//   //attachInterrupt(down, interruptDown, FALLING);
//   menu = 1;
//   getDelay();
//   xTaskCreatePinnedToCore(loop2, "loop2", 4*1024, NULL, 0, NULL, 0);

// }
// void loop() {
//   output();
// }



#include <Adafruit_VL53L0X.h>
#include <EEPROM.h>
#include "Util.h"
#include "Motor.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"

//pins
#define LIDAR_SDA 18
#define LIDAR_SCL 19
#define IR_ANALOG 34

//general
#define FORCE_SERIAL true
#define TIMECODE_FORMAT TIMECODE_FORMAT_SEPERATE
#define MAP_PRECISION 1000
#define PAPER_SIZE 40

//debug
#define DEBUG_GENERAL true
#define DEBUG_SETUP true
#define DEBUG_TIMECODE true
#define DEBUG_VALUES true
#define DEBUG_ERROR true
#define DEBUG_LIDAR true
#define DEBUG_IR true

//activation settings
#define LIDAR true
#define IR false

//LIDAR
#define LIDAR_FORCE_INIT true
#define LIDAR_SAMPLES 1000
#define LIDAR_AVERAGE_PERCENTAGE 77
#define LIDAR_VALUE_CORRECTION -30
#define LIDAR_CALIBRATION_SAMPLES 1

//IR
#define IR_SAMPLES 1000
#define IR_CYCLES 50
#define IR_CALIBRATION_SAMPLES 10

//IR mapping
#define A0 +9.42450609268053
#define A1 +1.76515427480637
#define A2 -5.03610159944910
#define A3 +6.88283703313403
#define A4 -5.02366498969399
#define A5 +2.08076080866994
#define A6 -4.87776360389973
#define A7 +5.99683334169558
#define A8 -2.97424332815192
#define E0 +0
#define E1 -1
#define E2 -4
#define E3 -7
#define E4 -10
#define E5 -13
#define E6 -17
#define E7 -21
#define E8 -25
#define GRADE 8
#define PRINT_PARTS false

//EEPROM
#define EEPROM_SIZE (FILL_LEVELS*SENSORS*VALUE_SIZE)
#define FILL_LEVELS 3
#define SENSORS 2
#define VALUE_SIZE sizeof(double)
#define FULL 0
#define LOW_ FULL+SENSORS*VALUE_SIZE
#define EMPTY LOW_+SENSORS*VALUE_SIZE


//TODO disconnect recognition

//macross
#define DEBUG (debug&&Serial&&DEBUG_GENERAL)
#define DEBUG_CONST (Serial&&DEBUG_GENERAL)
#define A(n) (n==0?A0:n==1?A1:n==2?A2:n==3?A3:n==4?A4:n==5?A5:n==6?A6:n==7?A7:n==8?A8:0)
#define E(n) (n==0?E0:n==1?E1:n==2?E2:n==3?E3:n==4?E4:n==5?E5:n==6?E6:n==7?E7:n==8?E8:0)
#define PART(n, x) (A(n)*pow(10, E(n))*pow(x, n))

//instances
// Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

Motor motor1(26, 27);
Motor motor2(33, 25);

void setup() {
  Serial.begin(115200);

  motor1.setSpeed(1.);
  motor2.setSpeed(1.);

  Wire.setPins(18, 19);
  if (tcs.begin(TCS34725_ADDRESS, &Wire)) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  Wire1.setPins(17, 5);
  if (tcs2.begin(TCS34725_ADDRESS, &Wire1)) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop() {
uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
  Serial.print("Color 1: ");
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs2.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs2.calculateLux(r, g, b);
  Serial.print("Color 2: ");
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}
/*
double map(double value, bool ir, bool percent) {
  double full = EEPROM.readDouble(FULL+(ir?VALUE_SIZE:0))*MAP_PRECISION;
  double low = EEPROM.readDouble(LOW_+(ir?VALUE_SIZE:0))*MAP_PRECISION;
  double empty = EEPROM.readDouble(EMPTY+(ir?VALUE_SIZE:0))*MAP_PRECISION;
  double min = 0*MAP_PRECISION;
  double max = PAPER_SIZE*MAP_PRECISION;
  value *= MAP_PRECISION;
  /*if(value > empty)
    return -1;*/
 /* double mapped = (map(value, low, full, min, max))/(double)MAP_PRECISION;
  if(percent)
    mapped = mapped*100/(double)PAPER_SIZE;
  if(percent)
    if(mapped > 100)
      mapped = 100;
  return mapped;
}

void lidarInit(bool debug = true) {
  if(DEBUG) {
    Serial.println();
    Serial.println(F("Lidar init started"));
    Serial.println(F("Adafruit VL53L0X start init"));
  }
  if(!Wire.begin(LIDAR_SDA, LIDAR_SCL)) {
    if(DEBUG) {
      Serial.println(F("SPI Connection failed"));
      Serial.print(F("retry: ")); Serial.println(F(LIDAR_FORCE_INIT?"true":"false"));
    }
    while(LIDAR_FORCE_INIT&&!Wire.begin(LIDAR_SDA, LIDAR_SCL));
  }
  if (!lox.begin()) {
    if(DEBUG) {
      Serial.println(F("Failed to boot VL53L0X"));
      Serial.print(F("retry: ")); Serial.println(F(LIDAR_FORCE_INIT?"true":"false"));
    }
    while(LIDAR_FORCE_INIT&&!lox.begin());
  }
  if(DEBUG) {
    Serial.print(F("Adafruit VL53L0X init ")); Serial.println(F(lox.Status!=-5?"successfull":"failed"));
    Serial.println(F("Lidar init finished"));
    Serial.println();
  }
}

double getLidarDistance(bool debug = false, bool mapValue = true) {
  if(DEBUG)
    Serial.print("Lidar: \t");
  double value = centralMean([](){VL53L0X_RangingMeasurementData_t measure;lox.rangingTest(&measure, false);if(measure.RangeStatus==4)return-1;else return(int)measure.RangeMilliMeter;}, LIDAR_SAMPLES, LIDAR_AVERAGE_PERCENTAGE, -1, 5, DEBUG?"lidar out of range":"");
  value+=LIDAR_VALUE_CORRECTION;
  if(DEBUG) {
    Serial.print("distance: ");
    Serial.print(value);
    Serial.print("mm\t");
  }
  if(!mapValue)
    return value;
  double percent = map(value, false, true);
  value = map(value, false, false);
  if(DEBUG) {
    Serial.print(F("mapped distance: "));
    Serial.print(value);
    Serial.print(F("mm\t"));
    Serial.print(F("fill level: "));
    Serial.print(percent);
    Serial.print(F("%\t"));
  }
  return value;
}

void irInit(bool debug = true) {
  if(DEBUG)
    Serial.println(F("IR init started"));
  pinMode(IR_ANALOG, INPUT);
  if(DEBUG)
    Serial.println(F("IR init finished"));
}

short getIrDistanceRaw() {
  return median([](){return modal([](){return analogRead(IR_ANALOG);}, IR_SAMPLES);}, IR_CYCLES);
}

short getIrDistance(bool debug = false, bool mapValue = true) {
  if(DEBUG)
    Serial.print(F("IR: \t"));
  int raw = getIrDistanceRaw();
  if(DEBUG) {
    Serial.print(F("raw: "));
    Serial.print(raw);
    Serial.print(F("\t"));
  }
  double value_ = 0;
  for(int i = 0; i <= GRADE; i++) {
    double part = PART(i, raw);
    if(DEBUG && PRINT_PARTS) {
      Serial.print(F("part x^"));
      Serial.print(i);
      Serial.print(F(": "));
      Serial.print(part);
      Serial.print(F("\t"));
    }
    value_+=part;
  }
  if(DEBUG) {
    Serial.print(F("distance: "));
    Serial.print(value_);
    Serial.print(F("mm\t"));
  }
  if(!mapValue)
    return value_;
  double percent = map(value_, true, true);
  value_ = map(value_, true, false);
  if(DEBUG) {
    Serial.print(F("mapped distance: "));
    Serial.print(value_);
    Serial.print(F("mm\t"));
    Serial.print(F("fill level: "));
    Serial.print(percent);
    Serial.print(F("%\t"));
  }
  return value_;
}

void printLines() {
  for(int i = 0; i < 10; i++)
    Serial.println();
}

byte waitEnter() {
  while(Serial.available())
    Serial.read();
    Serial.println(F("Press [enter] to continue"));
    Serial.println(F("Press [s] to skip calibration step"));
    Serial.println(F("Press [e] to end calibration"));
  while(!Serial.available()&&Serial);
    printLines();
    String response = Serial.readString();
  if(response == "s")
    return 1;
  if(response == "e")
    return 2;
  return 0;
}

void printCalibrationValue(String fillLevel, String sensor) {
  Serial.print(fillLevel+F(": "));
  Serial.print(EEPROM.readDouble((fillLevel=="full"?FULL:fillLevel=="low"?LOW_:EMPTY)+(sensor=="Ir"?VALUE_SIZE:0))); Serial.print("mm\t");
}

void printCalibrationValues(String sensor) {
  Serial.print(sensor+F(":\t"));
  printCalibrationValue("full", sensor);
  printCalibrationValue("low", sensor);
  printCalibrationValue("empty", sensor);
  Serial.println();
}

void printCalibrationValues() {
  Serial.println(F("current calibration values:"));
  printCalibrationValues("Lidar");
  printCalibrationValues("Ir");
}

bool printCalibrateState(String sensor, float complete) {
  Serial.print(sensor+F(" calibration: "));
  Serial.println(complete<0?"waiting":complete==0?"started":complete>=1?"finished":String((int)round((complete*100)))+"% complete");
  if(complete >= 1)
    return true;
  return false;
}

void printCalibrateText(String fillLevel, String instruction, float complete) {
  printLines();
  Serial.println(fillLevel+F(" calibration started"));
  Serial.println(F("this may take around 7 minutes to complete"));
  Serial.println(F("Please do not disturb the sensors while calibrating"));
  printCalibrateState("Lidar", complete);
  if(!printCalibrateState("Ir   ", complete-1))
    return;
  printLines();
  Serial.println(fillLevel+F(" calibration ended"));
 }

bool calibrateCycle(String fillLevel, String instruction) {
  printCalibrationValues();
  Serial.println();
  Serial.print(F("Put ")); Serial.println(instruction+F(" infront of the sensors"));
  byte response = waitEnter();
  if(response == 1) {
    printLines();
    Serial.println(fillLevel+F(" calibration skiped"));
    return false;
  }
  if(response == 2) {
    printLines();
    return true;
  }
  int v = 0;
  int* i = &v;
  double lidarValue;
  double irValue;
  if(!LIDAR)
    v = 10;
  if(LIDAR) lidarValue = /*getLidarDistance(false, false);*//*centralAverage([i, fillLevel, instruction](){printCalibrateText(fillLevel, instruction, *i/10.); (*i)++;; return getLidarDistance(false, false);}, LIDAR_CALIBRATION_SAMPLES, 80);
  if(IR) irValue = /*getIrDistance(false, false);*//*modal([i, fillLevel, instruction](){printCalibrateText(fillLevel, instruction, *i/10.); (*i)++; return getIrDistance(false, false);}, IR_CALIBRATION_SAMPLES);
  /*printCalibrateText(fillLevel, fillLevel, *i/10.);
  int address = fillLevel=="full"?FULL:fillLevel=="low"?LOW_:EMPTY;
  if(LIDAR) EEPROM.writeDouble(address, lidarValue);
  if(IR) EEPROM.writeDouble(address+VALUE_SIZE, irValue);
    EEPROM.commit();
  return false;
}

void calibrate() {
  if(!Serial)
    return;
  printLines();
  Serial.println(F("Calibration Menu"));
  printCalibrationValues();
  Serial.println(F("enter [c] to calibrate"));
  Serial.println(F("Press [enter] to continue"));
  while(!Serial.available());
  printLines();
  if(Serial.readString() != "c")
    return;
  if(calibrateCycle("full", "a full roll of toilett paper")) return;
  if(calibrateCycle("low", "a roll of toilett paper with one layer of paper left")) return;
  if(calibrateCycle("empty", "an empty roll of toilett paper")) return;
  EEPROM.commit();
  printLines();
  Serial.println(F("Calibration finished"));
  printCalibrationValues();
  waitEnter();
}

void setup() {
  Serial.begin(115200);
  // wait until serial port opens for native USB devices
  while (!Serial && FORCE_SERIAL)
    delay(1);
  if(DEBUG_SETUP&&DEBUG_CONST) {
    Serial.println();
    Serial.println(F("Setup started"));
  }
  if(LIDAR) lidarInit(DEBUG_SETUP&&DEBUG_LIDAR);
  if(IR) irInit(DEBUG_SETUP&&DEBUG_IR);
  Serial.print(F("EEPROM init\tEEPROM size: ")); Serial.println(EEPROM_SIZE);
  EEPROM.begin(EEPROM_SIZE);
  if(DEBUG_SETUP&&DEBUG_CONST) {
    Serial.println(F("Setup finished"));
    Serial.println();
  }
  calibrate();
}

void loop() {
  if(DEBUG_TIMECODE)
    printTimeCode(TIMECODE_FORMAT);
  if(LIDAR) getLidarDistance(DEBUG_VALUES&&DEBUG_LIDAR);
  if(IR) getIrDistance(DEBUG_VALUES&&DEBUG_IR);
  if(DEBUG_CONST)
    Serial.println();
}
*/