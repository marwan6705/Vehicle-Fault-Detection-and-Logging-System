#include "adc.h"
#include "avr/io.h"
#include "../common/common_macros.h"

volatile uint16 g_adcResult = 0;

void ADC_init(const ADC_ConfigType * Config_Ptr)
{
    /* Set reference voltage */
    switch(Config_Ptr->ref_volt) {
        case AREF:
            ADMUX &= ~(1<<REFS1 | 1<<REFS0);
            break;
        case AVCC:
            ADMUX = (ADMUX & ~(1<<REFS1)) | (1<<REFS0);
            break;
        case INTERNAL:
            ADMUX |= (1<<REFS1 | 1<<REFS0);
            break;
    }

    /* Right adjust result */
    ADMUX &= ~(1<<ADLAR);

    /* Enable ADC, disable auto trigger, clear interrupt flag */
    ADCSRA = (1<<ADEN);

    /* Set prescaler */
    ADCSRA |= Config_Ptr->prescaler;
}

uint16 ADC_readChannel(uint8 channel_num)
{
    channel_num &= 0x07;
    ADMUX = (ADMUX & 0xE0) | channel_num;

    SET_BIT(ADCSRA, ADSC);
    while(BIT_IS_CLEAR(ADCSRA, ADIF));
    SET_BIT(ADCSRA, ADIF);

    return ADC;
}
