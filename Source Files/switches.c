/*
 * switches.c
 *
 *  Created on: Sep 25, 2025
 *      Author: ageod
 *
 *      Defines behavior for all switches on the car (SW1 and SW2)
 */

#include "msp430.h"
#include "ports.h"
#include "macros.h"
#include "globals.h"
#include  <string.h>


#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
    // Switch 1
    if (P4IFG & SW1) {
        P4IFG &= ~SW1; // IFG SW1 cleared
        P4IE &= ~SW1;    // disable SW1 interrupts temporarily

        //---------------------switch action

        if (state != CALIBRATION) {
            state = CALIBRATION;
            once = TRUE;
        }

        else {

            if (white_set && black_set) {
                state = CHILLING;
                once = TRUE;
            }

            switch (ADC_THUMB_WHEEL) {
            case 0:     // white

                white_threshold_left = ADC_Left_Detect + ADC_TOLERANCE_WHITE;
                white_threshold_right = ADC_Right_Detect + ADC_TOLERANCE_WHITE;
                white_set = TRUE;

                break;
            case 1:     // black
                black_threshold_left = ADC_Left_Detect - ADC_TOLERANCE_BLACK;     // subtracts away a small tolerance so the threshold isn't exactly black
                black_threshold_right = ADC_Right_Detect - ADC_TOLERANCE_BLACK;
                black_detector_difference = black_threshold_left - black_threshold_right;
                black_set = TRUE;

                break;
            default:
                break;
            }

        }


        //------------

        TB0CCTL1 |= CCIE; // CCR1 enable interrupt
        TB0CCR1 = TB0R + SW1_DEBOUNCE_INTERVAL;

        // interrupt is enabled and will go to the timer interrupt after the interval is elapsed

    }
}
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    // Switch 2
    if (P2IFG & SW2) {
        P2IFG &= ~SW2; // IFG SW2 cleared
        P2IE &= ~SW2;    // disable SW2 interrupts temporarily

        //---------------------switch action


        if (state == CALIBRATION) {
            switch (ADC_THUMB_WHEEL) {
            case 0:
                white_threshold_left = 0;
                white_threshold_right = 0;
                white_set = FALSE;

                break;

            case 1:
                black_threshold_left = 0;
                black_threshold_right = 0;
                black_set = FALSE;

                break;

            default:
                break;
            }

        }


        //------------

        TB0CCTL2 |= CCIE; // CCR2 enable interrupt
        TB0CCR2 = TB0R + SW2_DEBOUNCE_INTERVAL;

        // interrupt is enabled and will go to the timer interrupt after the interval is elapsed

    }
}
