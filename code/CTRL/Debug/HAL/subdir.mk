################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/MOTOR.c \
../HAL/UltraSonic.c \
../HAL/external_eeprom.c \
../HAL/lm35_sensor.c \
../HAL/motor2.c 

OBJS += \
./HAL/MOTOR.o \
./HAL/UltraSonic.o \
./HAL/external_eeprom.o \
./HAL/lm35_sensor.o \
./HAL/motor2.o 

C_DEPS += \
./HAL/MOTOR.d \
./HAL/UltraSonic.d \
./HAL/external_eeprom.d \
./HAL/lm35_sensor.d \
./HAL/motor2.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/%.o: ../HAL/%.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


