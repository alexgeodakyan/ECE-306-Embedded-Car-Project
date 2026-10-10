/*
 * macros.h
 *
 *  Created on: Sep 8, 2025
 *      Author: ageod
 *
 *      Contains all constants used throughout the program, as well as alternate names to simplify understanding
 */

#ifndef MACROS_H_
#define MACROS_H_


#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define FALSE               (0x00)
#define P4PUD               (P4OUT) // allows using P4PUD to refer to pull up/down resistor
#define P2PUD               (P2OUT)
#define DISPLAY_SIZE        (10)


// STATES ======================================================================


#define CALIBRATION (0)
#define INTERCEPTING (1)
#define BRAKING (2)
#define STOP_BRAKING (3)
#define FOUND (4)
#define TURNING (5)
#define STOPPING (6)
#define STOPPED (7)
#define TRAVELING (8)
#define TURNING_TO_CENTER (9)
#define DRIVING_TO_CENTER (10)
#define FINAL_STOP (11)
#define STARTUP1 (12)
#define STARTUP2 (13)
#define STARTUP3 (14)
#define CHILLING (15)
#define PUSHING_IP (16)
#define PUSHING_SSID (17)
#define FORWARD_SLOW (18)
#define TURNING_LEFT (19)
#define TURNING_RIGHT (20)
#define REVERSE_SLOW (21)
#define FORWARD_OFF (22)
#define TURNING_LEFT_OFF (23)
#define TURNING_RIGHT_OFF (24)
#define REVERSE_OFF (25)
#define DELAYING (26)
#define BL_START (27)
#define END (28)

//  ===============================================================


#define BRAKING_INTERVAL (6250) // 8,000,000 / 8 / 8 / (1 / 50msec) -> 50 ms period
#define SW1_DEBOUNCE_INTERVAL (62500) // 8,000,000 / 8 / 8 / (1 / 500msec) -> 500 ms period
#define SW2_DEBOUNCE_INTERVAL (62500) // 8,000,000 / 8 / 8 / (1 / 500msec) -> 500 ms period

#define DISPLAY_INTERVAL (25000)    // 8,000,000 / 8 / 8 / (1 / 200msec) -> 200 ms period
#define SAMPLING_INTERVAL (1563)    // 8,000,000 / 8 / 8 / (1 / 12.5msec) -> 12.5ms period

#define INTIAL_DELAY_INTERVAL (16384)    // 32,768 / 2 = 1 s
#define SECOND (16384) // 1 second
#define HALF_SECOND (8172)  // 500 ms
#define MOVEMENT_UNIT_INTERVAL (163)   // 32,768 / 2 / 100 = 10 ms

#define TIME_COUNT_FREQUENCY (200)
#define DISPLAY_CYCLES (40)     // number of timer B0 cycles before one display update is performed (200 ms)
#define DEBOUNCE_CYCLES (2)    // number of CCR1/2 cycles used for debounce. 2 -> 1000 ms debounce

#define PWM_PERIOD (TB3CCR0)
#define LEFT_FORWARD_SPEED (TB3CCR1)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define LEFT_REVERSE_SPEED (TB3CCR3)
#define RIGHT_REVERSE_SPEED (TB3CCR4)
#define LCD_BACKLITE_DIMMING (TB3CCR5)

#define WHEEL_PERIOD (50005)
#define WHEEL_OFF (0)
#define BACKLITE_OFF (0)
#define LEFT_SPEED (26500)            // change eventually. right wheel is slightly faster -> 25500 for left?
#define RIGHT_SPEED (25000)
#define SLOWER_LEFT (15900)
#define SLOWER_RIGHT (15000)
#define FAST (50000)
#define PERCENT_100 (50000)
#define PERCENT_80 (45000)
#define PERCENT_50 (25000)

#define BLACK_LINE_VAL_LEFT (210)  // 5 below the maximum value for ADC_Left_Detect when a black line is found
#define BLACK_LINE_VAL_RIGHT (200) // 5 below the maximum value for ADC_Right_Detect when a black line is found

#define BAUD_SLOW (0)
#define BAUD_FAST (1)

#define DIFFERENCE_THRESHOLD (30)
#define TURN_THRESHOLD_LEFT (195)
#define TURN_THRESHOLD_RIGHT (185)

#define DAC_Begin (2500) // 2v
#define DAC_Limit (1800)
#define DAC_Adjust (1815)   // 2.9982v

#define ADC_CYCLES_THUMB_WHEEL (20)

#define ADC_TOLERANCE_BLACK (8)
#define ADC_TOLERANCE_WHITE (12)
#define TURNING_ADJUSTMENT (25)

#define BEGINNING (0)
#define IP_START (14)       // index of start of IP address from IOT message
#define SSID_START (8)      // index of start of SSID from IOT message
#define MESSAGE_START (10)
#define TIME_STRING_LENGTH (3)
#define SAFETY_DELAY (6)    // 600 ms of safety delay between movement commands
#define QUEUE_SIZE (5)      // size of queue for IOT commands

#define PING_INTERVAL (50)                  // interval at which the IOT pings google
#define BL_INITIAL_FORWARD_DURATION (100)   // 1 s of forward travel time from pad 7
#define BL_EXIT_FORWARD_DURATION (400)      // 4 s
#define LEFT_LOOP (0)        // if the duration part of a command evaluates to this, the car will take the left loop
#define RIGHT_LOOP (1)       // same as above, but for right
#define BL_INITIAL_TURN_DURATION (65)      //  (~90 degree turn)
#define DISPLAY_DELAY (100)        // 10 s of delay so the TAs can see text       //////////////////////////////////////////////////////////////
#define CIRCLE_DELAY (100)           // 10 s of delay before the text is changed

#define BANG_BANG_ADJUSTMENT (5000)

#endif /* MACROS_H_ */
