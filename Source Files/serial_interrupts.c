/*
 * serial_interrupts.c
 *
 *  Created on: Oct 29, 2025
 *      Author: ageod
 *
 *      Defines interrupt service routines for serial communication involving UCA0 and UCA1
 */

#include "msp430.h"
#include "macros.h"
#include "globals.h"


// UCA0 is connected to the IOT

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {

    char iot_receive;

    switch(__even_in_range(UCA0IV,0x08)){
    // RX interrupt
    case 2:

        iot_receive = UCA0RXBUF;
        IOT_Ring_Rx[iot_rx_wr++] = iot_receive; // Add to Ring Buffer
        if (iot_rx_wr >= sizeof(IOT_Ring_Rx)) {
            iot_rx_wr = BEGINNING;
        }
       // UCA1IE |= UCTXIE;
        while (!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = iot_receive;       // put received character into transmit for A1 (send back to PC)

        break;

    // TX interrupt
    case 4:
        if (iot_TX_buf[iot_tx] != '\0') {
            UCA0TXBUF = iot_TX_buf[iot_tx++];   // transmit a character out until NULL character is read
        }
        else {      // if current char is NULL, reset ring buffer index and disable interrupts (all data is transmitted)
            UCA0IE &= ~UCTXIE;
        }
        break;
    }
}

// UCA1 is used for a temporary connection between the PC and IOT through the USB port

#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){ //This interrupt is the interrupt relating to serial communication port UCA1
    //------------------------------------------------------------------------------
    // Interrupt name: eUSCI_A1_ISR
    // Description: This interrupt transmits and receives through UCA1
    //------------------------------------------------------------------------------
    char temp;
    switch(__even_in_range(UCA1IV,0x08)){
    case 0:
        break; //Vector 0 - no interrupt

    case 2: // Vector 2 - Rx1IFG

        communication_flag = TRUE;
        temp = UCA1RXBUF;

        if (temp == '/') {
            command_flag = TRUE;
            // take some action
        }

        if (temp == 0x0D && command_flag == TRUE) {     // carriage return, command is finished
            command_flag = FALSE;
        }

        if (!command_flag) {        // send char to IOT if not a command
            UCA0TXBUF = temp;       // put received character into transmit for A0 (send to IOT)
        }


        break;
    case 4: // Vector 4 - TX1IFG


    break;

    default:
    break;
    }
    //------------------------------------------------------------------------------
}
