#ifndef __CONFIG_H__
#define __CONFIG_H__

/* 
 * Configuration parameter
 */

// === PINS ===
#define L1_PIN 11
#define L2_PIN 12
#define L3_PIN 13
#define RESET_BUTTON_PIN 7
#define PIR_PIN 2
#define SONAR_TRIG_PIN 9
#define SONAR_ECHO_PIN 8
#define TEMP_PIN A0
#define SERVO_PIN 6
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2


// === BEHAVIOUR PARAMETRS ===

// --- Sonar (Distance in cm) ---
#define D1 50.0  // drone exited
#define D2 10.0  // drone landed

// --- Durate (time in ms) ---
#define T1 2000  // time over D1 to confirm exit
#define T2 2000  // time under D2 to confirm landing
#define T3 5000  // time over Temp1 to confirm pre-allarm
#define T4 3000  // time over Temp2 to confirm allarm

// --- Temperature (degrees in Celsius) ---
#define TEMP1 35.0 // pre-allarm threshold
#define TEMP2 45.0 // allarm threshold

// --- MSG
#define DRONE_INSIDE_MSG "DRONE INSIDE"
#define TAKE_OFF_MSG "TAKE OFF"
#define DRONE_OUT_MSG "DRONE OUT"
#define LANDING_MSG "LANDING"
#define ALARM_MSG "ALARM"
#define NORMAL_MSG "NORMAL"
#define PRE_ALARM_MSG "PRE ALARM"


// === PARAMETRI DEI TASK ===
#define SONAR_SAMPLING_PERIOD 100 // sonar sampling time (ms)
#define TEMP_SAMPLING_PERIOD 2000 // temperature sampling time (ms)
#define L2_BLINK_PERIOD 500       // L2 blinking time (ms)
#define SCHEDULER_PERIOD 50      // Scheduler base period (ms)

#endif