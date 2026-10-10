/*
 * bangbang.c
 *
 *  Created on: Dec 2, 2025
 *      Author: ageod
 *
 *      Implements the bang bang control system for black line tracking
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"
#include  "ports.h"
#include "wheels.h"
#include "globals.h"
#include "Display.h"
#include "adc_math.h"
#include "dac.h"



void bangbang(void) {

    if ((ADC_Left_Detect >= black_threshold_left) && (ADC_Right_Detect >= black_threshold_right)) {     // if both are on black
        Turn_CW_Off();
        Turn_CCW_Off();

        Forward_On_Slower();
        return;
    }

    if ((ADC_Left_Detect < black_threshold_left) && (ADC_Right_Detect >= black_threshold_right)) { // if left is off and right is on
        Forward_Off();
        Turn_CW_Off();

        Turn_CCW_Slow();
        return;
    }

    if ((ADC_Right_Detect < black_threshold_right) && (ADC_Left_Detect >= black_threshold_left)) {  // if right is off and left is on
        Forward_Off();
        Turn_CCW_Off();

        Turn_CW_Slow();
        return;
    }

    if ((ADC_Right_Detect < black_threshold_right) && (ADC_Left_Detect < black_threshold_left)) {       // if both are off course, reverse last move
        Turn_CCW_Off();
        Turn_CW_Off();

        Forward_On_Slower();

        return;
    }

}
