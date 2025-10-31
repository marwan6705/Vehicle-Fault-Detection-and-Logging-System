/******************************************************************************
 *                               INCLUDES
 ******************************************************************************/
#include "../common/std_types.h"
#include "../common/common_macros.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "timer.h"
/******************************************************************************
 *                               GLOBAL
 ******************************************************************************/
static volatile void (*g_callBackPtr[3])(void) = {NULL_PTR, NULL_PTR, NULL_PTR};
/******************************************************************************
 *                             INITIALIZATION
 ******************************************************************************/
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
    switch(Config_Ptr->timer_ID)
    {
    case Timer0:
        TCCR0 |= (Config_Ptr->timer_clock & 0x07);
        TCNT0 = (uint8) Config_Ptr->timer_InitialValue;
        if(Config_Ptr->timer_mode == Normal_Mode)
        {
            SET_BIT(TCCR0, FOC0);
            SET_BIT(TIMSK, TOIE0);
            CLEAR_BIT(TCCR0, WGM00);
            CLEAR_BIT(TCCR0, WGM01);
        }
        else if(Config_Ptr->timer_mode == Compare_Mode)
        {
            OCR0 = (uint8) Config_Ptr->timer_compare_MatchValue;
            SET_BIT(TCCR0, FOC0);
            CLEAR_BIT(TCCR0, WGM00);
            SET_BIT(TCCR0, WGM01);
            SET_BIT(TIMSK, OCIE0);
        }
        break;
    case Timer1:
        TCNT1 = Config_Ptr->timer_InitialValue;
        TCCR1B |= (Config_Ptr->timer_clock & 0x07);
        if(Config_Ptr->timer_mode == Normal_Mode)
        {
            SET_BIT(TIMSK, TOIE1);
            SET_BIT(TCCR1A, FOC1A);
            CLEAR_BIT(TCCR1B, WGM12);
        }
        else if(Config_Ptr->timer_mode == Compare_Mode)
        {
            OCR1A = Config_Ptr->timer_compare_MatchValue;
            SET_BIT(TCCR1B, WGM12);
            SET_BIT(TIMSK, OCIE1A);
            SET_BIT(TCCR1A, FOC1A);
        }
        break;
    case Timer2:
        TCNT2 = (uint8) Config_Ptr->timer_InitialValue;
        TCCR2 |= (Config_Ptr->timer_clock & 0x07);
        if(Config_Ptr->timer_mode == Normal_Mode)
        {
            CLEAR_BIT(TCCR2, FOC2);
            SET_BIT(TIMSK, TOIE2);
            CLEAR_BIT(TCCR2, WGM20);
            CLEAR_BIT(TCCR2, WGM21);
        }
        else if(Config_Ptr->timer_mode == Compare_Mode)
        {
            OCR2 = (uint8) Config_Ptr->timer_compare_MatchValue;
            SET_BIT(TCCR2, WGM21);
            CLEAR_BIT(TCCR2, WGM20);
            SET_BIT(TCCR2, FOC2);
            SET_BIT(TIMSK, OCIE2);
        }
        break;
    }
}
/******************************************************************************
 *                        DISABLE THE TIMERS
 ******************************************************************************/
void Timer_deInit(Timer_ID_Type timer_type)
{
    switch(timer_type)
    {
    case Timer0:
        TCCR0 = 0;
        TCNT0 = 0;
        OCR0 = 0;
        CLEAR_BIT(TIMSK, OCIE0);
        CLEAR_BIT(TIMSK, TOIE0);
        break;
    case Timer1:
        TCNT1 = 0;
        TCCR1A = 0;
        TCCR1B = 0;
        OCR1A = 0;
        CLEAR_BIT(TIMSK, OCIE1A);
        CLEAR_BIT(TIMSK, TOIE1);
        break;
    case Timer2:
        TCCR2 = 0;
        TCNT2 = 0;
        OCR2 = 0;
        CLEAR_BIT(TIMSK, OCIE2);
        CLEAR_BIT(TIMSK, TOIE2);
        break;
    }
    if(timer_type == Timer0 || timer_type == Timer1 || timer_type == Timer2)
    {
        g_callBackPtr[timer_type] = NULL_PTR;
    }
}
/******************************************************************************
 *                         CALL BACK FUNCTION
 ******************************************************************************/
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID )
{
    if(a_timer_ID == Timer0 || a_timer_ID == Timer1 || a_timer_ID == Timer2)
    {
        g_callBackPtr[a_timer_ID] = a_ptr;
    }
}
/******************************************************************************
 *                               ISR
 ******************************************************************************/
ISR(TIMER0_OVF_vect)
{
    if(g_callBackPtr[Timer0] != NULL_PTR)
    {
        (*g_callBackPtr[Timer0])();
    }
}

ISR(TIMER0_COMP_vect)
{
    if(g_callBackPtr[Timer0] != NULL_PTR)
    {
        (*g_callBackPtr[Timer0])();
    }
}

ISR(TIMER1_OVF_vect)
{
    if(g_callBackPtr[Timer1] != NULL_PTR)
    {
        (*g_callBackPtr[Timer1])();
    }
}

ISR(TIMER1_COMPA_vect)
{
    if(g_callBackPtr[Timer1] != NULL_PTR)
    {
        (*g_callBackPtr[Timer1])();
    }
}

ISR(TIMER2_OVF_vect)
{
    if(g_callBackPtr[Timer2] != NULL_PTR)
    {
        (*g_callBackPtr[Timer2])();
    }
}

ISR(TIMER2_COMP_vect)
{
    if(g_callBackPtr[Timer2] != NULL_PTR)
    {
        (*g_callBackPtr[Timer2])();
    }
}
