################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v00x_it.c \
../User/main.c \
../User/system_ch32v00x.c 

C_DEPS += \
./User/ch32v00x_it.d \
./User/main.d \
./User/system_ch32v00x.d 

OBJS += \
./User/ch32v00x_it.o \
./User/main.o \
./User/system_ch32v00x.o 



# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/Debug" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/Core" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/User" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/Peripheral/inc" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/External/MultiButton" -I"c:/Users/Mai/Desktop/SOUND/SOUND_V101T/External/CX588" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
