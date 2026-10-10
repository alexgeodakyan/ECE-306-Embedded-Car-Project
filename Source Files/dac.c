/*
 * dac.c
 *
 *  Created on: Oct 25, 2025
 *      Author: ageod
 *
 *      Initializes the DAC (digital-to-analog conversion) board which ensures that a stable voltage is supplied to the wheels.
 */

#include "msp430.h"
#include "ports.h"
#include "macros.h"
#include "globals.h"


void Init_DAC(void) {
    SAC3DAC = DACSREF_0; // Select VCC as DAC reference
    SAC3DAC |= DACLSEL_0; // DAC latch loads when DACDAT written

    SAC3OA = NMUXEN; // SAC Negative input MUX control
    SAC3OA |= PMUXEN; // SAC Positive input MUX control
    SAC3OA |= PSEL_1; // 12-bit reference DAC source selected
    SAC3OA |= NSEL_1; // Select negative pin input
    SAC3OA |= OAPM; // Select low speed and low power mode
    SAC3PGA = MSEL_1; // Set OA as buffer mode
    SAC3OA |= SACEN; // Enable SAC
    SAC3OA |= OAEN; // Enable OA

    DAC_data = DAC_Begin; // Starting Low value for DAC output [2v]
    SAC3DAT = DAC_data; // Initial DAC data

    TB1CTL |= TBIE; // Timer B0 overflow interrupt enable
    P1OUT |= RED_LED; // Set RED_LED On indicates the OVERFLOW timer start
    SAC3DAC |= DACEN; // Enable DAC

    P2OUT |= DAC_ENB;
}

