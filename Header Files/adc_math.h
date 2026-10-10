/*
 * adc_math.h
 *
 *  Created on: Oct 20, 2025
 *      Author: ageod
 */

#ifndef ADC_MATH_H_
#define ADC_MATH_H_


void HEXtoBCD(int hex_value);
void adc_line(char line, char location);
int adc_difference(int ADC_L, int ADC_R);
void DecToBCD_Time(unsigned int time);
void Display_Time(char line, char location);
unsigned int String_To_MS(unsigned int start_index, unsigned int line);
void Display_Command_Duration(unsigned int command_duration);

#endif /* ADC_MATH_H_ */
