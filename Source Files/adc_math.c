/*
 * adc_math.c
 *
 *  Created on: Oct 20, 2025
 *      Author: ageod
 *
 *      Contains functions used for evaluating and displaying ADC values. Also manipulates other string in order to properly work with them and place them in the display.
 */

#include "globals.h"
#include "adc_math.h"
#include "macros.h"


//-----------------------------------------------------------------
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//-----------------------------------------------------------------
void HEXtoBCD(int hex_value){
    int value = 0;
    unsigned int i;

    for (i = 0; i < 4; i++) {
        adc_char[i] = '0';
    }
    while (hex_value > 999){
        hex_value = hex_value - 1000;
        value = value + 1;
        adc_char[0] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 99){
        hex_value = hex_value - 100;
        value = value + 1;
        adc_char[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9){
        hex_value = hex_value - 10;
        value = value + 1;
        adc_char[2] = 0x30 + value;
    }
    adc_char[3] = 0x30 + hex_value;
}
//-----------------------------------------------------------------
//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//
//-------------------------------------------------------------
void adc_line(char line, char location){
//-------------------------------------------------------------
    int i;
    unsigned int real_line;
    real_line = line - 1;

    for(i=0; i < 4; i++) {
        display_line[real_line][i+location] = adc_char[i];
    }
}
//-------------------------------------------------------------


int adc_difference(int ADC_L, int ADC_R) {
    if (ADC_R > ADC_L) {
        return ADC_R - ADC_L;
    }

    else return ADC_L - ADC_R;

}



void DecToBCD_Time(unsigned int time) {
    int value = 0;
    unsigned int i;

    for (i = 0; i < 5; i++) {
        time_char[i] = '0';
    }

    while (time > 999) {
        time -= 1000;
        value += 1;
    }
    time_char[0] = value + 0x30;
    value = 0;

    while (time > 99) {
        time -= 100;
        value += 1;
    }
    time_char[1] = value + 0x30;
    value = 0;

    while (time > 9) {
        time -= 10;
        value += 1;
    }
    time_char[2] = value + 0x30;
    value = 0;

    while (time > 0.9) {
        time -= 1;
        value += 1;
    }
    time_char[3] = '.';     // radix
    time_char[4] = value + 0x30;
    value = 0;
}

void Display_Time(char line, char location) {
    int i;
    unsigned int real_line;
    real_line = line - 1;

    DecToBCD_Time(time_count);

    for(i=0; i < 3; i++) {
        display_line[real_line][i+location] = time_char[i];
    }
    display_changed = TRUE;

}


unsigned int String_To_MS(unsigned int start_index, unsigned int line) {
    unsigned int value = 0;
    unsigned int digit;
    unsigned int k;

    for (k = 0; k < TIME_STRING_LENGTH; k++) {
        digit = IOT_Data[line][start_index + k] - '0';   // convert char to int
        value = value * 10 + digit;                      // shift and add
    }

    return value;
}

void Display_Command_Duration(unsigned int command_duration) {
    unsigned int temp_dur = command_duration;
    unsigned int value = 0;

    while (temp_dur > 99) {
        temp_dur -= 100;
        value++;
    }
    display_line[1][4] = value + 0x30;
    value = 0;

    while (temp_dur > 9) {
        temp_dur -= 10;
        value++;
    }
    display_line[1][6] = value + 0x30;

    display_line[1][7] = temp_dur + 0x30;

}
