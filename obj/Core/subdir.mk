################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/core_riscv.c 

C_DEPS += \
./Core/core_riscv.d 

OBJS += \
./Core/core_riscv.o 



# Each subdirectory must supply rules for building sources it contributes
Core/%.o: ../Core/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/Mai/Desktop/SOUND/Debug" -I"c:/Users/Mai/Desktop/SOUND/Core" -I"c:/Users/Mai/Desktop/SOUND/User" -I"c:/Users/Mai/Desktop/SOUND/Peripheral/inc" -I"c:/Users/Mai/Desktop/SOUND/External/MultiButton" -I"c:/Users/Mai/Desktop/SOUND/External/CX588" -I"c:/Users/Mai/Desktop/SOUND/External/Button" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
