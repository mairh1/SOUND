################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

C_DEPS += \
./Debug/debug.d 

OBJS += \
./Debug/debug.o 



# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/Debug" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/Core" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/User" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/Peripheral/inc" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/External/MultiButton" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/External/CX588" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
