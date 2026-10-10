/*
 * wheels.c
 *
 *  Created on: Sep 15, 2025
 *      Author: ageod
 *
 *      Defines functions to control the wheels.
 */

#include "ports.h"
#include "msp430.h"
#include "wheels.h"
#include "macros.h"
#include "globals.h"


void Forward_On_Fast(void) {
    LEFT_REVERSE_SPEED = FAST;
    RIGHT_FORWARD_SPEED = FAST;
}

void Forward_On_Slow(void) {
    LEFT_FORWARD_SPEED = LEFT_SPEED;
    RIGHT_FORWARD_SPEED = RIGHT_SPEED;
}

void Forward_On_Slower(void) {
    LEFT_FORWARD_SPEED = SLOWER_LEFT;
    RIGHT_FORWARD_SPEED = SLOWER_RIGHT;
}

void Forward_Off(void) {
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
}

void Reverse_Off(void) {
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}

void Reverse_On_Fast(void) {
    LEFT_REVERSE_SPEED = FAST;
    RIGHT_REVERSE_SPEED = FAST;
}

void Reverse_On_Slow(void) {
    LEFT_REVERSE_SPEED = LEFT_SPEED;
    RIGHT_REVERSE_SPEED = RIGHT_SPEED;
}

void Turn_CCW_Slow(void) {
    LEFT_REVERSE_SPEED = LEFT_SPEED;
    RIGHT_FORWARD_SPEED = RIGHT_SPEED;
}

void Turn_CW_Off(void) {
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}

void Turn_CW_Slow(void) {
    RIGHT_REVERSE_SPEED = RIGHT_SPEED;
    LEFT_FORWARD_SPEED = LEFT_SPEED;
}

void Turn_CCW_Off(void) {
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
}

void Turn_CCW_Fast(void) {
    LEFT_REVERSE_SPEED = FAST;
    RIGHT_FORWARD_SPEED = FAST;
}

void Turn_CW_Fast(void) {
    LEFT_FORWARD_SPEED = FAST;
    RIGHT_REVERSE_SPEED = FAST;
}


void Brake(void) {

    TB0CCTL0 &= ~CCIFG;                    // Clear any pending flag
    TB0CCR0 = TB0R + BRAKING_INTERVAL;
    TB0CCTL0 |= CCIE;       // enable interrupt

    switch (previous_state) {
    case INTERCEPTING:

        Reverse_On_Fast();
        break;

    case TURNING:

        if (right_flag) {
            Turn_CCW_Fast();
        }
        else if (left_flag) {
            Turn_CW_Fast();
        }
        break;


    default:
        break;

    }

}

void Turn_Brake(void) {
    TB0CCTL0 &= ~CCIFG;                    // Clear any pending flag
    TB0CCR0 = TB0R + BRAKING_INTERVAL;
    TB0CCTL0 |= CCIE;       // enable interrupt

    if (right_flag) {

    }

    else if (left_flag) {

    }
}

// checks if there are any forward and reverse fets on at the same time
void Check_Wheels(void) {
    if ((P6OUT & R_FORWARD) && (P6OUT & R_REVERSE)) {        // check if R_FORWARD and R_REVERSE are simultaneously active
        P6OUT &= ~R_FORWARD;
        P6OUT &= ~R_REVERSE;        // turn both directions off

        P1OUT |= RED_LED;           // turn on LED to indicate error
    }

    if ((P6OUT & L_FORWARD) && (P6OUT & L_REVERSE)) {   // check if L_FORWARD and L_REVERSE are both active
        P6OUT &= ~L_FORWARD;     // turn off forward FET
        P6OUT &= ~L_REVERSE;     // turn off reverse FET

        P1OUT |= RED_LED;        // indicate fault with LED
    }
}
