/*
 * APP.c - ctrl ecu
 *
 *  Created on: Oct 23, 2025
 *      Author: MARWAN
 *
 *
 *
 */
#include "../HAL/lm35_sensor.h"
#include "../HAL/UltraSonic.h"
#include "../HAL/external_eeprom.h"
#include "../HAL/MOTOR.h"
#include "../MCAL/uart.h"
#include "../MCAL/adc.h"
#include "../MCAL/twi.h"
#include "../MCAL/PWM.h"
#include "../MCAL/gpio.h"
#include <util/delay.h>
#include "../HAL/motor2.h"
#include <avr/io.h>

/*******************************************************************************
 *                      definitions                                            *
 *******************************************************************************/
#define START_OPERATION    1
#define DISPLAY_VALUES     2
#define RETRIEVE_FAULTS    3
#define STOP_MONITORING    4

#define MAX_TEMP           90
#define ALLOWABLE_DISTANCE 10

#define WINDOW_PORT        PORTA_ID
#define WIN1_OPEN_PIN      PIN2_ID
#define WIN1_CLOSE_PIN     PIN3_ID
#define WIN2_OPEN_PIN      PIN4_ID
#define WIN2_CLOSE_PIN     PIN5_ID

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
uint8 g_win1_status = 0;  // 0 = closed, 1 = open
uint8 g_win2_status = 0;  // same as window 1
uint8 g_monitoring_active = 0; // flag to track if system is running

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
void Control_Windows(void);
void start_monitor(void);
void display_val(void);
void retrieveFaults(void);
void stopMonitoring(void);

/*******************************************************************************
 *                      Functions Implementation                               *
 *******************************************************************************/

void Control_Windows(void)
{
    // keep track of button press states (avoid multiple triggers)
    static uint8 win1_open_pressed = 0;
    static uint8 win1_close_pressed = 0;
    static uint8 win2_open_pressed = 0;
    static uint8 win2_close_pressed = 0;

    // read button inputs (active LOW)
    uint8 win1_open = GPIO_readPin(WINDOW_PORT, WIN1_OPEN_PIN);
    uint8 win1_close = GPIO_readPin(WINDOW_PORT, WIN1_CLOSE_PIN);
    uint8 win2_open = GPIO_readPin(WINDOW_PORT, WIN2_OPEN_PIN);
    uint8 win2_close = GPIO_readPin(WINDOW_PORT, WIN2_CLOSE_PIN);

    /* ---- Window 1 ---- */
    if(win1_open == LOGIC_LOW && !win1_open_pressed && g_win1_status == 0)
    {
        win1_open_pressed = 1;

        // stop window 2 just in case both were moving
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_LOW);
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_LOW);

        // run motor1 to open window 1
        GPIO_writePin(MOTOR_PORT, IN1, LOGIC_HIGH);
        GPIO_writePin(MOTOR_PORT, IN2, LOGIC_LOW);
        PWM_Timer0_Start(100);
        _delay_ms(500);

        // stop motor after short delay
        GPIO_writePin(MOTOR_PORT, IN1, LOGIC_LOW);
        GPIO_writePin(MOTOR_PORT, IN2, LOGIC_LOW);

        g_win1_status = 1; // mark as open
    }
    else if(win1_close == LOGIC_LOW && !win1_close_pressed && g_win1_status == 1)
    {
        win1_close_pressed = 1;

        // stop other motor first
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_LOW);
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_LOW);

        // reverse motor1 to close window
        GPIO_writePin(MOTOR_PORT, IN1, LOGIC_LOW);
        GPIO_writePin(MOTOR_PORT, IN2, LOGIC_HIGH);
        PWM_Timer0_Start(100);
        _delay_ms(500);

        // stop again
        GPIO_writePin(MOTOR_PORT, IN1, LOGIC_LOW);
        GPIO_writePin(MOTOR_PORT, IN2, LOGIC_LOW);

        g_win1_status = 0; // now closed
    }

    // release button flags when button is up
    if(win1_open == LOGIC_HIGH) win1_open_pressed = 0;
    if(win1_close == LOGIC_HIGH) win1_close_pressed = 0;

    /* ---- Window 2 ---- */
    if(win2_open == LOGIC_LOW && !win2_open_pressed && g_win2_status == 0)
    {
        win2_open_pressed = 1;

        // stop motor1 before using motor2
        GPIO_writePin(MOTOR_PORT, IN1, LOGIC_LOW);
        GPIO_writePin(MOTOR_PORT, IN2, LOGIC_LOW);

        // open window 2
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_HIGH);
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_LOW);
        PWM_Timer0_Start(100);
        _delay_ms(500);

        // stop it
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_LOW);
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_LOW);

        g_win2_status = 1;
    }
    else if(win2_close == LOGIC_LOW && !win2_close_pressed && g_win2_status == 1)
    {
        win2_close_pressed = 1;

        // stop motor1 first
        GPIO_writePin(MOTOR_PORT, IN1, LOGIC_LOW);
        GPIO_writePin(MOTOR_PORT, IN2, LOGIC_LOW);

        // reverse motor2 to close window
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_LOW);
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_HIGH);
        PWM_Timer0_Start(100);
        _delay_ms(500);

        // stop after move
        GPIO_writePin(MOTOR2_PORT, IN21, LOGIC_LOW);
        GPIO_writePin(MOTOR2_PORT, IN22, LOGIC_LOW);

        g_win2_status = 0;
    }

    // release press flags
    if(win2_open == LOGIC_HIGH) win2_open_pressed = 0;
    if(win2_close == LOGIC_HIGH) win2_close_pressed = 0;
}

void start_monitor(void)
{
    uint8 temp = 0;
    uint16 distance = 0;
    uint8 fault_distance_logged = 0;
    uint8 fault_temp_logged = 0;

    g_monitoring_active = 1; // mark system as active

    for(uint8 i = 0; i < 10; i++) // loop for 10 readings
    {
        temp = LM35_getTemperature();
        distance = Ultrasonic_readDistance();

        Control_Windows(); // handle manual button actions

        // send temp and distance to HMI ECU
        UART_sendByte(temp);
        _delay_ms(10);
        UART_sendByte((uint8)(distance >> 8));
        _delay_ms(10);
        UART_sendByte((uint8)distance);
        _delay_ms(10);

        // combine both window states into one byte
        uint8 window_states = (g_win2_status << 1) | g_win1_status;
        UART_sendByte(window_states);
        _delay_ms(10);

        // --- Fault check section ---
        if(distance < ALLOWABLE_DISTANCE && !fault_distance_logged)
        {
            EEPROM_writeByte(0x0000, 12);  // log distance fault (P001)
            _delay_ms(10);
            fault_distance_logged = 1;
        }
        else if(distance >= ALLOWABLE_DISTANCE)
        {
            EEPROM_writeByte(0x0000, 14);  // no fault
            _delay_ms(10);
            fault_distance_logged = 0;
        }

        if(temp > MAX_TEMP && !fault_temp_logged)
        {
            EEPROM_writeByte(0x0001, 13);  // log temp fault (P002)
            _delay_ms(10);
            fault_temp_logged = 1;
        }
        else if(temp <= MAX_TEMP)
        {
            EEPROM_writeByte(0x0001, 15);  // no fault
            _delay_ms(10);
            fault_temp_logged = 0;
        }

        _delay_ms(900);  // wait around 1 second between cycles
    }
}

void display_val(void)
{
    uint8 temp = LM35_getTemperature();
    uint16 distance = Ultrasonic_readDistance();

    Control_Windows(); // still allow manual control

    // send latest readings
    UART_sendByte(temp);
    _delay_ms(10);
    UART_sendByte((uint8)(distance >> 8));
    _delay_ms(10);
    UART_sendByte((uint8)distance);
    _delay_ms(10);

    // pack window status and send
    uint8 window_states = (g_win2_status << 1) | g_win1_status;
    UART_sendByte(window_states);
    _delay_ms(10);
}

void retrieveFaults(void)
{
    uint8 fault_distance = 0;
    uint8 fault_temp = 0;

    // read saved fault codes
    EEPROM_readByte(0x0000, &fault_distance);
    _delay_ms(10);
    EEPROM_readByte(0x0001, &fault_temp);
    _delay_ms(10);

    // send them to HMI ECU
    UART_sendByte(fault_distance);
    _delay_ms(10);
    UART_sendByte(fault_temp);
    _delay_ms(10);
}

void stopMonitoring(void)
{
    g_monitoring_active = 0;
    // stop both motors
    DcMotor_Rotate(STOP, 0);
    DcMotor_Rotate2(STOP2, 0);
    g_win1_status = 0;
    g_win2_status = 0;
}

/*******************************************************************************
 *                   Main Function                                             *
 *******************************************************************************/
int main(void)
{
    uint8 command = 0;

    // init UART
    UART_ConfigType uartConfig = {UART_8BIT, disabled, ONE_STOP_BIT, 9600};
    UART_init(&uartConfig);
    _delay_ms(50);

    // init ADC for LM35
    ADC_ConfigType adc_config = {INTERNAL, FCPU_128};
    ADC_init(&adc_config);
    _delay_ms(50);

    // init TWI for EEPROM
    TWI_ConfigType twiConfig = {0x01, 400000};
    TWI_init(&twiConfig);
    _delay_ms(50);

    // setup all sensors and motors
    Ultrasonic_init();
    DcMotor_Init();
    DcMotor_Init2();
    PWM_Timer0_Start(100);

    // configure buttons as inputs with pull-ups
    GPIO_setupPinDirection(WINDOW_PORT, WIN1_OPEN_PIN, PIN_INPUT);
    GPIO_writePin(WINDOW_PORT, WIN1_OPEN_PIN, LOGIC_HIGH);

    GPIO_setupPinDirection(WINDOW_PORT, WIN1_CLOSE_PIN, PIN_INPUT);
    GPIO_writePin(WINDOW_PORT, WIN1_CLOSE_PIN, LOGIC_HIGH);

    GPIO_setupPinDirection(WINDOW_PORT, WIN2_OPEN_PIN, PIN_INPUT);
    GPIO_writePin(WINDOW_PORT, WIN2_OPEN_PIN, LOGIC_HIGH);

    GPIO_setupPinDirection(WINDOW_PORT, WIN2_CLOSE_PIN, PIN_INPUT);
    GPIO_writePin(WINDOW_PORT, WIN2_CLOSE_PIN, LOGIC_HIGH);

    // enable global interrupts for ultrasonic timing
    SREG |= (1 << 7);

    while(1)
    {
        // get command from HMI ECU
        command = UART_recieveByte();

        switch(command)
        {
            case START_OPERATION:
                start_monitor();
                break;

            case DISPLAY_VALUES:
                display_val();
                break;

            case RETRIEVE_FAULTS:
                retrieveFaults();
                break;

            case STOP_MONITORING:
                stopMonitoring();
                break;
        }

        _delay_ms(10);
    }

    return 0;
}
