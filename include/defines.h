#include <Arduino.h>
#include <FanController.h>
#include <LiquidCrystal.h>

#define SENSOR_THRESHOLD1 400
#define SENSOR_THRESHOLD2 400

const int FAN_OFF_TEMP = 0;
const int FAN_MAX_TEMP = 15;
const int UPDATE_INTERVAL = 5000;

// Poti & Switch:
const int SWITCH_PIN = 12;
const int POTI_PIN = 2;

// TEMP SENS:
const int TEMP0_PIN = 0;
const int TEMP1_PIN = 1;
// Fan Left:
#define Fan_Left_SENS_PIN 2
#define Fan_Left_PWM_PIN 10
// Fan Right:
#define Fan_Right_SENS_PIN 3
#define Fan_Right_PWM_PIN 11