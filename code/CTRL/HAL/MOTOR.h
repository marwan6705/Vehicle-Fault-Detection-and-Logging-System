/* MOTOR.h */

#ifndef HAL_MOTOR_H_
#define HAL_MOTOR_H_

#include "../common/std_types.h"

// Motor control pin definitions
#define MOTOR_PORT PORTB_ID
#define IN1 PIN1_ID
#define IN2 PIN2_ID
#define ENABLE1 PIN3_ID  // Shared PWM pin

// Motor state enum
typedef enum
{
    STOP,
    CW,
    A_CW
} DcMotor_State;

// Function prototypes
void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* HAL_MOTOR_H_ */
