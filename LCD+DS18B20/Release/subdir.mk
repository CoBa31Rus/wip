################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../button.c \
../eeprom.c \
../lcd.c \
../main.c \
../onewire.c \
../pid.c 

OBJS += \
./button.o \
./eeprom.o \
./lcd.o \
./main.o \
./onewire.o \
./pid.o 

C_DEPS += \
./button.d \
./eeprom.d \
./lcd.d \
./main.d \
./onewire.d \
./pid.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g3 -gdwarf-2 -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


