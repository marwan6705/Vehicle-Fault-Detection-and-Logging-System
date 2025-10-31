/*
 * motor2.h
 *
 *  Created on: Oct 30, 2025
 *      Author: workstation
 */

#ifndef HAL_MOTOR2_H_
#define HAL_MOTOR2_H_

#include "../common/std_types.h"
#define MOTOR2_PORT PORTB_ID
#define IN21 PIN4_ID
#define IN22 PIN5_ID
#define ENABLE21 PIN3_ID
typedef enum
{
	STOP2,CW2,A_CW2
}DcMotor_State2;
void DcMotor_Init2(void);
void DcMotor_Rotate2(DcMotor_State2 state, uint8 speed);



#endif /* HAL_MOTOR2_H_ */
