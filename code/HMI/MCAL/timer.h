/*
 * timer.h
 *
 *  Created on: Oct 23, 2025
 *      Author: mar
 */

#ifndef HMI_ECU_MCAL_TIMER_H_
#define HMI_ECU_MCAL_TIMER_H_
#include "../common/std_types.h"
//first select the timer
typedef enum
{
	Timer0,Timer1,Timer2
}Timer_ID_Type;
//select the clock u want
typedef enum
{
	No_Clock,F_Cpu,F_Cpu_8,F_Cpu_64,F_Cpu_256,F_Cpu_1024
}Timer_ClockType;
//select the mode u need
typedef enum
{
	Normal_Mode,Compare_Mode
}Timer_ModeType;
typedef struct
{
uint16 timer_InitialValue;
uint16 timer_compare_MatchValue; /*it will be used in compare mode
only*/
Timer_ID_Type timer_ID;
Timer_ClockType timer_clock;
Timer_ModeType timer_mode;
}Timer_ConfigType;



void Timer_init(const Timer_ConfigType * Config_Ptr);
/*
▪ Description: Function to initialize the Timer driver
▪ Inputs: pointer to the configuration structure with type
Timer_ConfigType.
▪ Return: None
*/
void Timer_deInit(Timer_ID_Type timer_type);
/*
▪ Description: Function to disable the Timer via Timer_ID.
▪ Inputs: Timer_ID
▪ Return: None
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );
/*
▪ Description: Function to set the Call Back function address to the
required Timer.
▪ Inputs: pointer to Call Back function and Timer Id you want to set
The Callback to it.
▪ Return: None
 */
#endif /* HMI_ECU_MCAL_TIMER_H_ */
