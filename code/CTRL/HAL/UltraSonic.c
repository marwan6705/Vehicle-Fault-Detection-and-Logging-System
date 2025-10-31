#include "UltraSonic.h"
#include "../MCAL/icu.h"
#include <util/delay.h>

volatile uint16 time_high = 0;
volatile uint8 edges_count = 0;

void Ultrasonic_init(void)
{
    ICU_ConfigType icu_initial = {F_CPU_8, RAISING};
    ICU_init(&icu_initial);
    ICU_setCallBack(Ultrasonic_edgeProcessing);
    GPIO_setupPinDirection(ULTRA_SONIC_PORT, TRIGGER_PIN, PIN_OUTPUT);
}

void Ultrasonic_Trigger(void)
{
    GPIO_writePin(ULTRA_SONIC_PORT, TRIGGER_PIN, LOGIC_HIGH);
    _delay_us(10);
    GPIO_writePin(ULTRA_SONIC_PORT, TRIGGER_PIN, LOGIC_LOW);
}

uint16 Ultrasonic_readDistance(void)
{
    uint16 distance = 0;

    /* Reset edge counter */
    edges_count = 0;
    time_high = 0;

    /* Trigger ultrasonic */
    Ultrasonic_Trigger();

    /* Wait for measurement to complete */
    _delay_ms(50);

    /* Calculate distance in cm
     * Formula: distance = (time_high * sound_speed) / 2
     * With F_CPU/8 prescaler: time in microseconds = time_high
     * Sound speed = 343 m/s = 0.0343 cm/us
     * distance = (time_high * 0.0343) / 2 = time_high / 58.3
     * Using 58 for integer math
     */
    if(time_high > 0)
    {
        distance = time_high / 58;
    }

    return distance+1;
}

void Ultrasonic_edgeProcessing(void)
{
    edges_count++;

    if(edges_count == 1)
    {
        /* First edge (rising) - start counting */
        ICU_clearTimerValue();
        ICU_setEdgeDetectionType(FALLING);
    }
    else if(edges_count == 2)
    {
        /* Second edge (falling) - get pulse width */
        time_high = ICU_getInputCaptureValue();
        ICU_setEdgeDetectionType(RAISING);
        edges_count = 0;
    }
}
