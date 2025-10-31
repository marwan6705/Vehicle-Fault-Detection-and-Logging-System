################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/PWM.c \
../MCAL/adc.c \
../MCAL/gpio.c \
../MCAL/icu.c \
../MCAL/twi.c \
../MCAL/uart.c 

OBJS += \
./MCAL/PWM.o \
./MCAL/adc.o \
./MCAL/gpio.o \
./MCAL/icu.o \
./MCAL/twi.o \
./MCAL/uart.o 

C_DEPS += \
./MCAL/PWM.d \
./MCAL/adc.d \
./MCAL/gpio.d \
./MCAL/icu.d \
./MCAL/twi.d \
./MCAL/uart.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/%.o: ../MCAL/%.c MCAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


