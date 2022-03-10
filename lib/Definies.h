#ifndef DEFINES_H
#define DEFINES_H

//Pins
#define MOTOR_LV        5
#define MOTOR_LH        18
#define MOTOR_LS        19
#define MOTOR_RV        17
#define MOTOR_RH        16
#define MOTOR_RS        4

#define SPEAKER_PIN     15

#define SONIC_TRIG      32
#define SONIC_ECHO      33

#define BACK_LIGHT_R    13
#define BACK_LIGHT_L    14
#define FRONT_LIGHT     27

#define LASER           12

#define LDR             39

//I2C
#define I2C_1           Wire
#define SDA_1           -1 // default: 20
#define SCL_1           -1 // default: 21
#define I2C_2           Wire1
#define SDA_2           25
#define SCL_2           26

//Debug
#define DEBUG               true
#define DEBUG_COLOR         DEBUG&&Serial&& true
#define DEBUG_LIDAR         DEBUG&&Serial&& false
#define DEBUG_ULTRASONIC    DEBUG&&Serial&& true


#endif