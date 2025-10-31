# Vehicle Fault Detection and Logging System (VFDLS)

A dual-ECU embedded system using ATmega32 microcontrollers to detect vehicle faults in real time, log errors to EEPROM, and display sensor data and DTCs via LCD. It monitors temperature, distance, and window states with UART-based communication.

## Features
- Real-time fault detection (temperature, distance)
- Diagnostic Trouble Code (DTC) logging to EEPROM
- LCD-based user interface with live sensor data
- UART communication between HMI and Control ECUs
- Window control via DC motors

## System Architecture
- HMI ECU: Keypad + LCD interface
- Control ECU: Sensors, actuators, EEPROM logging
- Communication: UART (between ECUs), I2C (EEPROM)

## Hardware Used
- 2 × ATmega32 (8 MHz)
- LM35 Temperature Sensor
- Ultrasonic Sensor
- 4x4 Keypad
- 4x16 LCD
- 2 × DC Motors
- 24C16 EEPROM

## Drivers
- GPIO, UART, LCD, Keypad
- ADC, PWM, Timer, I2C (TWI)
- DC Motor, EEPROM, ICU, Ultrasonic

## How to Run
1. Flash both ECUs with their respective hex files.
2. Connect hardware as per schematic.
3. Power on system.
4. Use keypad to navigate menu:
   - 1: Start Monitoring
   - 2: Display Values
   - 3: Retrieve Faults
   - 4: Stop Monitoring

## Simulation
Proteus simulation included for full system behavior.
