#include "MOTOR.h"
#include <avr/io.h>

#include "../MCAL/gpio.h"
#include "../MCAL/PWM.h"

void DcMotor_Init(void)
{
    GPIO_setupPinDirection(MOTOR_PORT, IN1, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR_PORT, IN2, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR_PORT, ENABLE1, PIN_OUTPUT);

    GPIO_writePin(MOTOR_PORT, IN1, LOGIC_LOW);
    GPIO_writePin(MOTOR_PORT, IN2, LOGIC_LOW);

    PWM_Timer0_Start(0);  // Start PWM with 0% duty cycle
}

void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
    if (speed > 100) speed = 100;
    OCR0 = (uint8)((speed * 255) / 100);  // Set PWM duty cycle

    switch (state)
    {
        case STOP:
            GPIO_writePin(MOTOR_PORT, IN1, LOGIC_LOW);
            GPIO_writePin(MOTOR_PORT, IN2, LOGIC_LOW);
            break;

        case CW:
            GPIO_writePin(MOTOR_PORT, IN1, LOGIC_HIGH);
            GPIO_writePin(MOTOR_PORT, IN2, LOGIC_LOW);
            break;

        case A_CW:
            GPIO_writePin(MOTOR_PORT, IN1, LOGIC_LOW);
            GPIO_writePin(MOTOR_PORT, IN2, LOGIC_HIGH);
            break;

        default:
            GPIO_writePin(MOTOR_PORT, IN1, LOGIC_LOW);
            GPIO_writePin(MOTOR_PORT, IN2, LOGIC_LOW);
            break;
    }
}
