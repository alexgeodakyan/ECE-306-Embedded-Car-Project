/*
 * timers.c
 *
 *  Created on: Oct 5, 2025
 *      Author: ageod
 *
 *      Initializes timers B0, B1, B2, and B3. Timer B3 is used for wheel and backlight PWM
 */

#include "globals.h"
#include "msp430.h"
#include "macros.h"
#include "timers.h"

void Init_Timers(void) {
    Init_Timer_B0();
    Init_Timer_B1();
    Init_Timer_B2();
    Init_Timer_B3();
}

//------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK; // SMCLK source
    TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS; // Continuous up
    TB0CTL |= ID__8; // Divide clock by 8

    TB0EX0 = TBIDEX__8; // Divide clock by an additional 8

    TB0CCR0 = BRAKING_INTERVAL; // CCR0                        // ATTENTION
    TB0CCTL0 |= CCIE; // CCR0 enable interrupt

    TB0CCR1 = SW1_DEBOUNCE_INTERVAL; // CCR1

    TB0CCR2 = SW2_DEBOUNCE_INTERVAL; // CCR2

    TB0CTL &= ~TBIE; // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}
//------------------------------------------------------------------------------

void Init_Timer_B1(void) {
    TB1CTL = TBSSEL__SMCLK;     // SMCLK source
    TB1CTL |= TBCLR;            // Resets TB1R, clock divider, count direction
    TB1CTL |= MC__CONTINOUS;    // Continuous up
    TB1CTL |= ID__8; // Divide clock by 8

    TB1EX0 = TBIDEX__8; // Divide clock by an additional 8

    TB1CCR0 = DISPLAY_INTERVAL;     // CCR0
    TB1CCTL0 |= CCIE;       // CCR0 enable interrupt

    TB1CCR1 = SAMPLING_INTERVAL;        // CCR1
    TB1CCTL1 |= CCIE;                   // CCR1 enable interrupt

    TB1CTL &= ~TBIE; // Disable Overflow Interrupt
    TB1CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}

void Init_Timer_B2(void) {
    TB2CTL = TBSSEL__ACLK;     // ACLK source
    TB2CTL |= TBCLR;            // Resets TB2R, clock divider, count direction
    TB2CTL |= MC__CONTINUOUS;   // Continuous up
    TB2CTL |= ID__2;        // Divide clock by 2

    TB2CCR0 = INTIAL_DELAY_INTERVAL;    // startup delay

    TB2CCR1 = SECOND;

    TB2CTL &= ~TBIE; // Disable Overflow Interrupt
    TB2CTL &= ~TBIFG; // Clear Overflow Interrupt flag

}

void Init_Timer_B3(void) {
    //------------------------------------------------------------------------------
    // SMCLK source, up count mode, PWM Right Side
    // TB3.1 P6.1 L_FORWARD
    // TB3.2 P6.2 R_FORWARD
    // TB3.3 P6.3 L_REVERSE
    // TB3.4 P6.4 R_REVERSE
    // TB3.5 P6.5 LCD_BACKLITE
    //------------------------------------------------------------------------------

    TB3CTL = TBSSEL__SMCLK; // SMCLK
    TB3CTL |= MC__UP; // Up Mode
    TB3CTL |= TBCLR; // Clear TAR

    PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]

    TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Left Forward PWM duty cycle

    TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Right Forward PWM duty cycle

    TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM duty cycle

    TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Right Reverse PWM duty cycle

    TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
    LCD_BACKLITE_DIMMING = BACKLITE_OFF; // P6.5 LCD_BACKLITE On Dimming percent
    //------------------------------------------------------------------------------
}
