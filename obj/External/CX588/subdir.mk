################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../External/CX588/CX588.c 

C_DEPS += \
./External/CX588/CX588.d 

OBJS += \
./External/CX588/CX588.o 



# Each subdirectory must supply rules for building sources it contributes
External/CX588/%.o: ../External/CX588/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"e:/program/CH32/CH32L103/SOUND/Debug" -I"e:/program/CH32/CH32L103/SOUND/Core" -I"e:/program/CH32/CH32L103/SOUND/User" -I"e:/program/CH32/CH32L103/SOUND/Peripheral/inc" -I"e:/program/CH32/CH32L103/SOUND/External/Button" -I"e:/program/CH32/CH32L103/SOUND/External/CX588" -I"e:/program/CH32/CH32L103/SOUND/External/MultiButton" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
