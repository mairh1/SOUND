################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../External/MultiButton/multi_button.c 

C_DEPS += \
./External/MultiButton/multi_button.d 

OBJS += \
./External/MultiButton/multi_button.o 



# Each subdirectory must supply rules for building sources it contributes
External/MultiButton/%.o: ../External/MultiButton/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/Mai/Desktop/SOUND/V1.0.0/CH32V003_GPIO_TEST/Debug" -I"c:/Users/Mai/Desktop/SOUND/V1.0.0/CH32V003_GPIO_TEST/Core" -I"c:/Users/Mai/Desktop/SOUND/V1.0.0/CH32V003_GPIO_TEST/User" -I"c:/Users/Mai/Desktop/SOUND/V1.0.0/CH32V003_GPIO_TEST/Peripheral/inc" -I"c:/Users/Mai/Desktop/SOUND/V1.0.0/CH32V003_GPIO_TEST/External/MultiButton" -I"c:/Users/Mai/Desktop/SOUND/V1.0.0/CH32V003_GPIO_TEST/External/CX588" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
