/*
 * Display.c
 *
 *  Created on: Sep 11, 2025
 *      Author: ageod
 *
 *      Contains code relating to the display. The most important function is Display_Process(), which updates the display every 200 ms.
 */

#include <math.h>
#include "Display.h"
#include "globals.h"
#include "macros.h"
#include "ports.h"
#include <strings.h>
#include "msp430.h"
#include "adc_math.h"


void Display_Process(void){rry

    if(update_display){


        display_changed = TRUE;



        if (state == CALIBRATION) {

            switch (ADC_THUMB_WHEEL) {
            case 0:     // white

                if (!white_set) {      // if white value has not been set from calibration

                    strcpy(display_line[1], "L =       ");
                    strcpy(display_line[2], "R =       ");
                    Display_ADC_Values();
                }

                else {
                    HEXtoBCD(white_threshold_left);
                    adc_line(2, 6);
                    HEXtoBCD(white_threshold_right);
                    adc_line(3, 6);
                }

                break;
            case 1:     // black

                if (!black_set) {      // if black value has not been set from calibration

                    strcpy(display_line[1], "L =       ");
                    strcpy(display_line[2], "R =       ");
                    Display_ADC_Values();
                }

                else {
                    HEXtoBCD(black_threshold_left);
                    adc_line(2, 6);
                    HEXtoBCD(black_threshold_right);
                    adc_line(3, 6);
                }

                break;
            default:
                break;

            }



        }

        else if ((state != END) && (first_command)) {
            Display_Time(4, 6);
        }

        update_display = 0;
        if(display_changed){
          display_changed = 0;
          Display_Update(0,0,0,0);
        }
    }

}

void IR_Status_Check(void) {
    if (P2OUT & IR_LED) {
        // Pin is HIGH
        strcpy(display_line[0], "IR LED ON ");
        display_changed = TRUE;
    }
    else {
        // Pin is LOW
        strcpy(display_line[0], "IR LED OFF");
        display_changed = TRUE;
    }
}

void Display_ADC_Values(void) {
    HEXtoBCD(ADC_Left_Detect);
    adc_line(2, 6);
    HEXtoBCD(ADC_Right_Detect);
    adc_line(3, 6);
//    HEXtoBCD(ADC_THUMB_WHEEL);
//    adc_line(4, 3);

    display_changed = TRUE;
}

void DisplayIPAddress(unsigned int start_line) {    // displays the ip address at start_line and start_line + 1
    unsigned int i;

    start_index1 = ((DISPLAY_SIZE - ip_length1) / 2);

    for (i = 0; i < ip_length1; i++) {
        display_line[start_line][start_index1 + i] = ip_line1[i];
    }

    start_index2 = ((DISPLAY_SIZE - ip_length2) / 2);
    start_line++;       // increment line

    for (i = 0; i < ip_length2; i++) {
        display_line[start_line][start_index2 + i] = ip_line2[i];
    }
}

