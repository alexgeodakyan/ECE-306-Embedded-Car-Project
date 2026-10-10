/*
 * Display.h
 *
 *  Created on: Sep 11, 2025
 *      Author: ageod
 *
 *  CCS 12.8.1
 *
 *
 */

#ifndef HW_03_FILES_DISPLAY_H_
#define HW_03_FILES_DISPLAY_H_


void Display_Process(void);
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4);
void IR_Status_Check(void);
void Display_ADC_Values(void);
void DisplayIPAddress(unsigned int start_line);


#endif /* HW_03_FILES_DISPLAY_H_ */
