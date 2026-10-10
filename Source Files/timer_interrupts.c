/*
 * timer_interrupts.c
 *
 *  Created on: Oct 7, 2025
 *      Author: ageod
 *
 *
 *      Defines behavior for all timer interrupts where applicable (TB0, TB1, TB2)
 */


#include "globals.h"
#include "msp430.h"
#include "macros.h"
#include "timers.h"
#include "ports.h"
#include "wheels.h"




#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//----------------------------------------------------------------------------


    // TB0CCR0 is for implementing braking

    state = STOP_BRAKING;
    once = TRUE;

    TB0CCTL0 &= ~CCIE;     // disable interrupt when done with braking

//----------------------------------------------------------------------------
}
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
 switch(__even_in_range(TB0IV,14)){


 // TB0CCR1 and TB0CCR2 are used for SW1 and SW2 debounce

 case 0: break; // No interrupt
 case 2:

     debounce_count1++;

     if (debounce_count1 == DEBOUNCE_CYCLES) {        // count 2 cycles of the timer -> 1000 ms total debounce tume
         debounce_count1 = 0;
         TB0CCTL1 &= ~CCIE;      // disable timer interrupt since debouncing is done
         P4IE |= SW1;        // reenable switch interrupt
     }


 break;
 case 4:

     debounce_count2++;

     if (debounce_count2 == DEBOUNCE_CYCLES) {        // count 16 cycles of the timer -> 800 ms total debounce tume
         debounce_count2 = 0;
         TB0CCTL2 &= ~CCIE;      // disable timer interrupt since debouncing is done
         P2IE |= SW2;        // reenable switch interrupt
     }

 break;
 case 14: // overflow


 break;
 default: break;
 }
//----------------------------------------------------------------------------
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB1 0 Interrupt handler
//----------------------------------------------------------------------------

    // TB1CCR0 is used as a global timer for display updates and human-scale timing

     TB1CCR0 += DISPLAY_INTERVAL; // Add Offset to TBCCR0
     update_display = TRUE;
     time_count += 2;     // adds .2 s (decimal added later) to time_count (tracks elapsed time)
     time_loop += 2;


//----------------------------------------------------------------------------
}
#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB1 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
 switch(__even_in_range(TB1IV,14)){
 case 0: break; // No interrupt
 case 2:

     // TB1CCR1 is used for sampling the ADC at the set sampling interval

     TB1CCR1 += SAMPLING_INTERVAL;
     ADCCTL0 |= ADCSC; // Start next sample
     //P1OUT ^= RED_LED;


 break;
 case 4:



 break;
 case 14: // overflow

     // TB1 overflow configures the DAC on startup

     DAC_data = DAC_data - 200;
     SAC3DAT = DAC_data; // Initial DAC data

     if(DAC_data <= DAC_Limit){
         DAC_data = DAC_Adjust;
         SAC3DAT = DAC_data; // Initial DAC data
         TB1CTL &= ~TBIE; // disable TimerB0 overflow interrupt
         P1OUT &= ~RED_LED; // Set RED_LED Off DAC voltage set
     }


 break;
 default: break;
 }
//----------------------------------------------------------------------------
}


#pragma vector = TIMER2_B0_VECTOR
__interrupt void Timer2_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB2 0 Interrupt handler
//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
}
#pragma vector=TIMER2_B1_VECTOR
__interrupt void TIMER2_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB2 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
switch(__even_in_range(TB2IV,14)){
case 0: break; // No interrupt
case 2:

    // TB2 CCR1 enables braking after a delay

    state = BRAKING;   // turn braking
    once = TRUE;
    TB2CCTL1  &= ~CCIE;    // disable interrupt once state is set


break;
case 4:

    // TB2 CCR2 is used for implementing timing for movement commands sent through the IOT

    if (delay_counter == delay_counter_movement) {
        once = TRUE;

        switch (state) {
        case FORWARD_SLOW:
            state = FORWARD_OFF;
            break;

        case TURNING_LEFT:
            state = TURNING_LEFT_OFF;
            break;

        case TURNING_RIGHT:
            state = TURNING_RIGHT_OFF;
            break;

        case REVERSE_SLOW:
            state = REVERSE_OFF;
            break;

        default:
            break;
        }

        TB2CCTL2 &= ~CCIE;      // disable interrupts after movement is done
        delay_counter = 0;
        delay_counter_movement = 0;
    }

    else {
        delay_counter++;
    }



    break;
case 14: // overflow


    break;
default: break;
}
//----------------------------------------------------------------------------
}

