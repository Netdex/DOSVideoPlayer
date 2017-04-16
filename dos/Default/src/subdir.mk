################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/decoder.c \
../src/keyboard.c \
../src/main.c \
../src/util.c \
../src/vga.c 

OBJS += \
./src/decoder.o \
./src/keyboard.o \
./src/main.o \
./src/util.o \
./src/vga.o 

C_DEPS += \
./src/decoder.d \
./src/keyboard.d \
./src/main.d \
./src/util.d \
./src/vga.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"D:\Security\VMSHARED\Development\tools\DJGPP\include" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


