/*
 * motor2.c
 *
 *  Created on: Oct 30, 2025
 *      Author: Marwan
 */

#include "motor2.h"
#include <avr/io.h>
#include "../MCAL/gpio.h"
#include "../MCAL/PWM.h"

void DcMotor_Init2(void)
{
    GPIO_setupPinDirection(MOTOR2_PORT, IN21, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR2_PORT, IN22, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR2_PORT, ENABLE21, PIN_OUTPUT);

    GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_LOW);
    GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_LOW);

    PWM_Timer0_Start(0);
}

void DcMotor_Rotate2(DcMotor_State2 state, uint8 speed)
{
    if (speed > 100) speed = 100;
    OCR0 = (uint8)((speed * 255) / 100);

    switch (state)
    {
    case STOP2:
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_LOW);
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_LOW);
        break;

    case CW2:
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_HIGH);
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_LOW);
        break;

    case A_CW2:
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_HIGH);
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_LOW);
        break;

    default:
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_LOW);
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_LOW);
        break;
    }
}
