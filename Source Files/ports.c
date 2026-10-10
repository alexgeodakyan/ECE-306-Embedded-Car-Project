/*
 * ports.c
 *
 *  Created on: Sep 11, 2025
 *      Author: ageod
 *
 *      Defines functions to initialize each port. The MSP430FR2355 has 6 ports, each with a varying number of pins.
 *      The "select" registers are used to set the specific pin function.
 */


#include "ports.h"
#include "msp430.h"
#include "macros.h"

// initializes all ports. Init_Ports() consists of 6 sub-functions that initialize each port.

void Init_Ports(void) {
    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();
}

void Init_Port1(void) {

    P1OUT  = 0x00;
    P1DIR  = 0x00;
    P1SEL0 = 0x00;
    P1SEL1 = 0x00;

    // initialize all pins to safe state: GPIO, pins are inputs, outputs are 0 by default

    P1SEL0 &= ~RED_LED;
    P1SEL1 &= ~RED_LED;
    P1OUT &= ~RED_LED;
    P1DIR |= RED_LED;       // pin 1 is a GPIO output set to low

    P1SELC |= V_DETECT_L;

    P1SELC |= V_DETECT_R;

    P1SELC |= A1_SEEED;

    P1SELC |= A4_SEEED;

    P1SELC|= V_THUMB;  // pins 2–5 are ADC pins

    P1SEL0 |= UCA0TXD;
    P1SEL1 &= ~UCA0TXD;

    P1SEL0 |= UCA0RXD;
    P1SEL1 &= ~UCA0RXD;      // pins 6 and 7 are UART pins


}

void Init_Port2(void) {

    P2OUT = 0x00;
    P2DIR = 0x00;
    P2SEL0 = 0x00;
    P2SEL1 = 0x00;

    // initialize all pins

     P2SEL0 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
     P2SEL1 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
     P2OUT &= ~SLOW_CLK; // Initial Value = Low / Off
     P2DIR |= SLOW_CLK; // Direction = output

     P2SEL0 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
     P2SEL1 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
     P2OUT &= ~CHECK_BAT; // Initial Value = Low / Off
     P2DIR |= CHECK_BAT; // Direction = output

     P2SEL0 &= ~IR_LED; // P2_2 GPIO operation
     P2SEL1 &= ~IR_LED; // P2_2 GPIO operation
     P2OUT &= ~IR_LED; // Initial Value = OFF           ////////////////////////////////////////// ATTENTION
     P2DIR |= IR_LED; // Direction = output

     P2SEL0 &= ~SW2; // SW2 Operation
     P2SEL1 &= ~SW2; // SW2 Operation
     P2PUD |= SW2; // Configure pullup resistor
     P2DIR &= ~SW2; // Direction = input
     P2REN |= SW2; // Enable pullup resistor
     P2IES |= SW2; // SW2 Hi/Lo edge interrupt
     P2IFG &= ~SW2; // IFG SW2 cleared
     P2IE |= SW2; // SW2 interrupt Enabled

     P2SEL0 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
     P2SEL1 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
     P2OUT &= ~IOT_RUN_CPU; // Initial Value = Low / Off
     P2DIR |= IOT_RUN_CPU; // Direction = output

     P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
     P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
     P2OUT |= DAC_ENB; // Initial Value = High
     P2DIR |= DAC_ENB; // Direction = output

     P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
     P2SEL1 |= LFXOUT; // LFXOUT Clock operation

     P2SEL0 &= ~LFXIN; // LFXIN Clock operation
     P2SEL1 |= LFXIN; // LFXIN Clock operation

}


void Init_Port3(void) {

    P3OUT = 0x00;
    P3DIR = 0x00;
    P3SEL0 = 0x00;
    P3SEL1 = 0x00;

    // initialize all pins. GPIO input by default. initializes pins 0, 1, 3, 4, 5, 6, and 7

    P3SEL0 &= ~TEST_PROBE;
    P3SEL1 &= ~TEST_PROBE;
    P3OUT &= ~TEST_PROBE;
    P3DIR &= ~TEST_PROBE;       // 3.0 is a GPIO input set to low

    P3SEL0 &= ~DAC_CTRL_2;
    P3SEL1 &= ~DAC_CTRL_2;
    P3OUT  &= ~DAC_CTRL_2;
    P3DIR  &= ~DAC_CTRL_2;

    P3SEL0 &= ~OA2N;
    P3SEL1 &= ~OA2N;     // set to gpio function
    P3OUT &= ~OA2N;
    P3DIR &= ~OA2N;

    P3SEL0 &= ~OA2P;
    P3SEL1 &= ~OA2P;     // set to gpio function
    P3OUT &= ~OA2P;
    P3DIR &= ~OA2P;

    P3SEL0 &= ~SMCLK_OUT;
    P3SEL1 &= ~SMCLK_OUT;
    P3OUT  &= ~SMCLK_OUT;
    P3DIR  &= ~SMCLK_OUT;       // 3.4 is a GPIO input set to low

    P3SELC |= DAC_CTRL_3;
//    P3SEL1 &= ~DAC_CTRL_3;
//    P3OUT  |= DAC_CTRL_3;
//    P3DIR  |= DAC_CTRL_3;          // DAC_CTRL_3 = DAC_CNTL

    P3SEL0 &= ~IOT_LINK_CPU;
    P3SEL1 &= ~IOT_LINK_CPU;
    P3OUT  &= ~IOT_LINK_CPU;
    P3DIR  &= ~IOT_LINK_CPU;        // 3.6 is a user-controlled LED set to LOW by default

    P3SEL0 &= ~IOT_EN_CPU;
    P3SEL1 &= ~IOT_EN_CPU;
    P3OUT  &= ~IOT_EN_CPU;
    P3DIR  |= IOT_EN_CPU;      // 3.7 is a GPIO output set to low


}


void Init_Port4(void) {

    P4OUT = 0x00; // P4 set Low
    P4DIR = 0x00; // Set P4 direction to input

    P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P4OUT &= ~RESET_LCD; // Initial Value = Low / Off
    P4DIR |= RESET_LCD; // Direction = output

    P4SEL0 &= ~SW1; // SW1 GPIO operation
    P4SEL1 &= ~SW1; // SW1 GPIO operation
    P4PUD |= SW1; // Configure pullup resistor
    P4DIR &= ~SW1; // Direction = input
    P4REN |= SW1; // Enable pullup resistor
    P4IES |= SW1; // SW1 Hi/Lo edge interrupt
    P4IFG &= ~SW1; // IFG SW1 cleared
    P4IE |= SW1; // SW1 interrupt Enabled

    P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

    P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation

    P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
    P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation

    P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
    P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

    P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
    P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

    P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
    P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

    P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
    P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation

}


void Init_Port5(void) {

    P5OUT = 0x00;
    P5DIR = 0x00;
    P5SEL0 = 0x00;
    P5SEL1 = 0x00;

    // initialize all pins


    P5SEL0 |= V_BAT;
    P5SEL1 |= V_BAT;

    P5SEL0 |= V_5;
    P5SEL1 |= V_5;

    P5SEL0 |= V_DAC;
    P5SEL1 |= V_DAC;

    P5SEL0 |= V3_3;
    P5SEL1 |= V3_3;     // 5.0–5.3 are set to ADC for analog readings

    P5SEL0 &= ~IOT_BOOT_CPU;
    P5SEL1 &= ~IOT_BOOT_CPU;
    P5OUT  |= IOT_BOOT_CPU;
    P5DIR  |= IOT_BOOT_CPU;    // 5.4 is a GPIO output set to high, control to enable program downloaded

}

void Init_Port6(void) {

    P6SEL0 |= L_FORWARD;
    P6SEL1 &= ~L_FORWARD;       // configure for TB3 PWM
    P6DIR  |= L_FORWARD;        // L_FORWARD is an output

    P6SEL0 |= R_FORWARD;
    P6SEL1 &= ~R_FORWARD;       // configure for TB3 PWM
    P6DIR  |= R_FORWARD;        // R_FORWARD is an output

    P6SEL0 |= L_REVERSE;
    P6SEL1 &= ~L_REVERSE;       // configure for TB3 PWM
    P6DIR  |= L_REVERSE;        // L_REVERSE is an output

    P6SEL0 |= R_REVERSE;
    P6SEL1 &= ~R_REVERSE;       // configure for TB3 PWM
    P6DIR  |= R_REVERSE;        // R_REVERSE is an output

    P6SEL0 |= LCD_BACKLITE;
    P6SEL1 &= ~LCD_BACKLITE;    // configure for TB3 PWM
    P6OUT &= ~LCD_BACKLITE;
    P6DIR |= LCD_BACKLITE;      // 6.4 is a GPIO output set to low

    P6SEL0 &= ~P6_5;
    P6SEL1 &= ~P6_5;
    P6OUT  &= ~P6_5;
    P6DIR  &= ~P6_5;        // 6.5 is a GPIO input set to low

    P6SEL0 &= ~GRN_LED;
    P6SEL1 &= ~GRN_LED;
    P6OUT  &= ~GRN_LED;
    P6DIR |= GRN_LED;       // 6.6 is a GPIO output set to low

}
