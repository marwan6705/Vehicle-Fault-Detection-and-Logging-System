#include "PWM.h"
#include <avr/io.h>
#include "gpio.h"
void PWM_Timer0_Start(uint8 duty_cycle)
{
	TCNT0 = 0;
	GPIO_setupPinDirection(PWM_PORT,PWM_PIN,PIN_OUTPUT);
	TCCR0 |= (1<<WGM00) |(1<<WGM01) |(1<<COM01) |(1<<CS00) |(1<<CS01);
	OCR0 = (uint8)((duty_cycle * 255UL) / 100);
}

