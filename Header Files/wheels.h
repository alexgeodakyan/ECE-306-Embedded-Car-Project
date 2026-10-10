/*
 * wheels.h
 *
 *  Created on: Sep 15, 2025
 *      Author: ageod
 */

#ifndef WHEELS_H_
#define WHEELS_H_

void reverse(void);

void Run_Figure_Eight(void);
void Run_Circle(void);
void Run_Triangle(void);

void wait_case(void);
void start_case(void);
void run_case(void);
void end_case(void);

void Forward_On_Fast(void);
void Forward_On_Slow(void);
void Forward_On_Slower(void);
void Forward_Off(void);
void Reverse_Off(void);
void Reverse_On_Fast(void);
void Reverse_On_Slow(void);
void Turn_CW(void);
void Turn_CCW(void);
void Turn_CW_Off(void);
void Turn_CCW_Off(void);
void Turn_CCW_Slow(void);
void Turn_CW_Slow(void);
void Brake(void);
void Turn_Brake(void);
void Turn_CW_Fast(void);
void Turn_CCW_Fast(void);

void Check_Wheels(void);

#endif /* WHEELS_H_ */
