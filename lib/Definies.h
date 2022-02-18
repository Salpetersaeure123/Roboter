#ifndef DEFINES_H
#define DEFINES_H

//Pins
#define MOTOR_LV        18
#define MOTOR_LH        19
#define MOTOR_RV        17
#define MOTOR_RH        16

#define I2C_1           Wire
#define SDA_1           -1
#define SCL_1           -1
#define I2C_2           Wire1
#define SDA_2           25
#define SCL_2           26

#define SONIC_TRIG      32
#define SONIC_ECHO      33


//Settings
#define FORCE_INIT_COLOR    false
#define FORCE_INIT_LIDAR    false

//Debug
#define DEBUG               true
#define DEBUG_COLOR         DEBUG&&Serial&& true
#define DEBUG_LIDAR         DEBUG&&Serial&& false


#endif