/*
 * APP.c - HMI ECU (NO CHANGES NEEDED - Already Correct)
 *
 *  Created on: Oct 30, 2025
 *      Author: MARWAN
 */

/*******************************************************************************
 *                      includes                                               *
 *******************************************************************************/
#include "../HAL/lcd.h"
#include "../MCAL/uart.h"
#include "../HAL/keypad.h"
#include <util/delay.h>

/*******************************************************************************
 *                      definitions                                            *
 *******************************************************************************/
#define START_OPERATION    1
#define DISPLAY_VALUES     2
#define RETRIEVE_FAULTS    3
#define STOP_MONITORING    4

#define FAULT_DISTANCE_P001    12
#define FAULT_TEMP_P002        13
#define NO_DISTANCE_FAULT      14
#define NO_TEMP_FAULT          15

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
uint8 g_monitoring_active = 0;  // flag to check if system is running

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
void display_menu(void);
void StartOperation(void);
void DisplayValues(void);
void Retrieve_Faults(void);
void stop_monitor(void);

/*******************************************************************************
 *                      Functions Implementation                               *
 *******************************************************************************/

void display_menu(void)
{
    LCD_clearScreen();
    LCD_displayString("1- Start");
    LCD_displayStringRowColumn(1, 0, "2- Display Value");
    LCD_displayStringRowColumn(2, 0, "3- Retrieve Fault");
    LCD_displayStringRowColumn(3, 0, "4- Stop");
}

void StartOperation(void)
{
    // variables for receiving data from Control ECU
    uint8 temp, distance_high, distance_low;
    uint16 distance;
    uint8 window_states;

    g_monitoring_active = 1; // mark system as active

    LCD_clearScreen();
    LCD_displayString("Starting...");
    _delay_ms(500);

    // tell Control ECU to start monitoring
    UART_sendByte(START_OPERATION);
    _delay_ms(50);

    // loop to read 10 samples (1 per second)
    for(uint8 sec = 1; sec <= 10; sec++)
    {
        // receive temperature
        temp = UART_recieveByte();
        _delay_ms(10);

        // receive distance (2 bytes)
        distance_high = UART_recieveByte();
        _delay_ms(10);
        distance_low = UART_recieveByte();
        _delay_ms(10);
        distance = ((uint16)distance_high << 8) | distance_low;

        // receive window states
        window_states = UART_recieveByte();
        _delay_ms(10);

        // show everything on LCD
        LCD_clearScreen();
        LCD_displayString("Sec:");
        LCD_intgerToString(sec);
        LCD_displayString("/10");

        LCD_displayStringRowColumn(1, 0, "T:");
        LCD_intgerToString(temp);
        LCD_displayString("C D:");
        LCD_intgerToString(distance);
        LCD_displayString("cm");

        // check window 1 status
        LCD_displayStringRowColumn(2, 0, "W1:");
        if(window_states & 0x01)
            LCD_displayString("Open ");
        else
            LCD_displayString("Close");

        // check window 2 status
        LCD_displayStringRowColumn(3, 0, "W2:");
        if(window_states & 0x02)
            LCD_displayString("Open ");
        else
            LCD_displayString("Close");

        _delay_ms(50);
    }

    LCD_clearScreen();
    LCD_displayString("Completed!");
    _delay_ms(2000);
}

void DisplayValues(void)
{
    // vars for data
    uint8 temp, distance_high, distance_low;
    uint16 distance;
    uint8 window_states;

    // if system not active, tell user
    if(g_monitoring_active == 0)
    {
        LCD_clearScreen();
        LCD_displayString("Start system");
        LCD_displayStringRowColumn(1, 0, "first!");
        _delay_ms(3000);
        return;
    }

    LCD_clearScreen();
    LCD_displayString("Reading...");
    _delay_ms(300);

    // send display request
    UART_sendByte(DISPLAY_VALUES);
    _delay_ms(50);

    // receive all values
    temp = UART_recieveByte();
    _delay_ms(10);
    distance_high = UART_recieveByte();
    _delay_ms(10);
    distance_low = UART_recieveByte();
    _delay_ms(10);
    distance = ((uint16)distance_high << 8) | distance_low;

    window_states = UART_recieveByte();
    _delay_ms(10);

    // print readings
    LCD_clearScreen();
    LCD_displayString("Temp:");
    LCD_intgerToString(temp);
    LCD_displayCharacter('C');

    LCD_displayStringRowColumn(1, 0, "Dist:");
    LCD_intgerToString(distance);
    LCD_displayString("cm");

    LCD_displayStringRowColumn(2, 0, "W1:");
    if(window_states & 0x01)
        LCD_displayString("Open ");
    else
        LCD_displayString("Closed");

    LCD_displayStringRowColumn(3, 0, "W2:");
    if(window_states & 0x02)
        LCD_displayString("Open ");
    else
        LCD_displayString("Closed");

    _delay_ms(10000);
}

void Retrieve_Faults(void)
{
    uint8 fault_distance = 0;
    uint8 fault_temp = 0;

    LCD_clearScreen();
    LCD_displayString("Reading...");
    _delay_ms(300);

    // ask Control ECU to send fault codes
    UART_sendByte(RETRIEVE_FAULTS);
    _delay_ms(50);

    // receive stored faults
    fault_distance = UART_recieveByte();
    _delay_ms(10);
    fault_temp = UART_recieveByte();
    _delay_ms(10);

    LCD_clearScreen();
    LCD_displayString("Faults Log:");

    // show distance fault status
    LCD_displayStringRowColumn(1, 0, "");
    if(fault_distance == FAULT_DISTANCE_P001)
        LCD_displayString("P001:Crash Risk!");
    else
        LCD_displayString("No Distance Fault");

    // show temperature fault status
    LCD_displayStringRowColumn(2, 0, "");
    if(fault_temp == FAULT_TEMP_P002)
        LCD_displayString("P002:Overheat!");
    else
        LCD_displayString("No Temp Fault");

    LCD_displayStringRowColumn(3, 0, "--End of List--");

    _delay_ms(10000);
}

void stop_monitor(void)
{
    LCD_clearScreen();
    LCD_displayString("Stopping...");
    _delay_ms(300);

    // tell Control ECU to stop
    UART_sendByte(STOP_MONITORING);
    _delay_ms(50);

    g_monitoring_active = 0; // reset state

    LCD_clearScreen();
    LCD_displayString("System is OFF");
    _delay_ms(2000);
}

/*******************************************************************************
 *                   Main Function                                             *
 *******************************************************************************/
int main(void)
{
    uint8 key = 0;

    // setup LCD
    LCD_init();
    _delay_ms(50);

    // setup UART for communication
    UART_ConfigType uartConfig = {UART_8BIT, disabled, ONE_STOP_BIT, 9600};
    UART_init(&uartConfig);
    _delay_ms(50);

    while(1)
    {
        display_menu();  // show menu on LCD

        // wait for user input
        key = KEYPAD_getPressedKey();

        if(key >= 1 && key <= 4)
        {
            _delay_ms(200);  // small debounce

            // choose function based on key
            switch(key)
            {
                case 1:
                    StartOperation();
                    break;

                case 2:
                    DisplayValues();
                    break;

                case 3:
                    Retrieve_Faults();
                    break;

                case 4:
                    stop_monitor();
                    break;
            }
        }

        _delay_ms(100);
    }

    return 0;
}
