################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/AdcDriver.c \
../Src/BasicTimer.c \
../Src/CaptureFreqDriver.c \
../Src/EXTIDriver.c \
../Src/FPUDriver.c \
../Src/GPIOxDriver.c \
../Src/I2CDriver.c \
../Src/MPUAccel.c \
../Src/OLEDDriver.c \
../Src/PIDDriver.c \
../Src/PwmDriver.c \
../Src/RCCHunMHz.c \
../Src/RTCDriver.c \
../Src/SPIDriver.c \
../Src/SysTickDriver.c \
../Src/USARTxDriver.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/AdcDriver.o \
./Src/BasicTimer.o \
./Src/CaptureFreqDriver.o \
./Src/EXTIDriver.o \
./Src/FPUDriver.o \
./Src/GPIOxDriver.o \
./Src/I2CDriver.o \
./Src/MPUAccel.o \
./Src/OLEDDriver.o \
./Src/PIDDriver.o \
./Src/PwmDriver.o \
./Src/RCCHunMHz.o \
./Src/RTCDriver.o \
./Src/SPIDriver.o \
./Src/SysTickDriver.o \
./Src/USARTxDriver.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/AdcDriver.d \
./Src/BasicTimer.d \
./Src/CaptureFreqDriver.d \
./Src/EXTIDriver.d \
./Src/FPUDriver.d \
./Src/GPIOxDriver.d \
./Src/I2CDriver.d \
./Src/MPUAccel.d \
./Src/OLEDDriver.d \
./Src/PIDDriver.d \
./Src/PwmDriver.d \
./Src/RCCHunMHz.d \
./Src/RTCDriver.d \
./Src/SPIDriver.d \
./Src/SysTickDriver.d \
./Src/USARTxDriver.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx -c -I../Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

