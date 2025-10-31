#ifndef MCAL_ADC_H_
#define MCAL_ADC_H_

#include "../common/std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   2.56

/*******************************************************************************
 *                         Type Definitions                                    *
 *******************************************************************************/

/* Enum for ADC reference voltage options */
typedef enum {
    AREF,        // External AREF pin
    AVCC,        // AVCC with external capacitor at AREF pin
    INTERNAL     // Internal 2.56V with external capacitor at AREF pin
} ADC_ReferenceVoltage;

/* Enum for ADC prescaler options */
typedef enum {
    FCPU_2 = 1,
    FCPU_4,
    FCPU_8,
    FCPU_16,
    FCPU_32,
    FCPU_64,
    FCPU_128
} ADC_Prescaler;

/* Configuration structure for ADC */
typedef struct {
    ADC_ReferenceVoltage ref_volt;
    ADC_Prescaler prescaler;
} ADC_ConfigType;

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
extern volatile uint16 g_adcResult;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Initialize ADC with configuration pointer */
void ADC_init(const ADC_ConfigType * Config_Ptr);

/* Read analog value from ADC channel */
uint16 ADC_readChannel(uint8 channel_num);

#endif /* MCAL_ADC_H_ */
