/*
 * adc.c
 *
 *  Created on: Oct 16, 2025
 *      Author: ageod
 *
 *      Contains the interrupt service routine for ADC (analog-to-digital converter), which enables reading of analog values. In this case, the values of
 *      interest are the left and right IR detectors and the thumbwheel potentiometer.
 */

#include "macros.h"
#include "globals.h"
#include "msp430.h"

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
    case ADCIV_NONE:
        break;
    case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
        // before its previous conversion result was read.
        break;
    case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
        break;
    case ADCIV_ADCHIIFG: // Window comparator interrupt flags
        break;
    case ADCIV_ADCLOIFG: // Window comparator interrupt flag
        break;
    case ADCIV_ADCINIFG: // Window comparator interrupt flag
        break;
    case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result

        ADCCTL0 &= ~ADCENC; // Disable ENC bit -> disable conversions to allow samples to be gathered

        switch (ADC_Channel++){
        case 0x00: // Channel A2 Interrupt
            ADC_Left_Detect = ADCMEM0; // Move result into Global Values

            ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4 -> 8 bit resolution

            ADCMCTL0 &= ~ADCINCH_2; // Disable Current channel A2
            ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3

            break;
        case 0x01: // Channel A3 Interrupt
            ADC_Right_Detect = ADCMEM0; // Move result into Global Values

            ADC_Right_Detect = ADC_Right_Detect >> 2; // Divide the result by 4 -> 8 bit resolution

            adc_cycle_count++;

            new_value = TRUE;

            if (adc_cycle_count >= ADC_CYCLES_THUMB_WHEEL) {
                ADCMCTL0 &= ~ADCINCH_3; // Disable Current channel A3
                ADCMCTL0 |= ADCINCH_5; // Enable Next channel A5
                adc_cycle_count = 0;
            }
            else {
                ADCMCTL0 &= ~ADCINCH_3; // Disable Current channel A3
                ADCMCTL0 |= ADCINCH_2; // Enable First channel 2
                ADC_Channel=0;
            }

            break;
        case 0x02: // Channel A5 Interrupt
            ADC_THUMB_WHEEL = ADCMEM0; // Move result into Global Values

            ADC_THUMB_WHEEL = ADC_THUMB_WHEEL >> 9;     // Divide the result by 2^9 -> 1 bit resolution -> 2 options

            ADCMCTL0 &= ~ADCINCH_5; // Disable Current channel A5
            ADCMCTL0 |= ADCINCH_2; // Enable First channel 2
            ADC_Channel=0;
            break;
        default:
            break;
        }
        ADCCTL0 |= ADCENC; // Enable Conversions
        break;
    default:
        break;
    }
}
