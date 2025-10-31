/*
 * PWM.h
 *
 *  Created on: Sep 27, 2025
 *      Author: MARWAN
 */

#ifndef MCAL_PWM_H_
#define MCAL_PWM_H_
#include "../common/std_types.h"
#define PWM_PORT PORTB_ID
#define PWM_PIN PIN3_ID
void PWM_Timer0_Start(uint8 duty_cycle);




#endif /* MCAL_PWM_H_ */
