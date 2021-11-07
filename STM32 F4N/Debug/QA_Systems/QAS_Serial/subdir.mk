################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../QA_Systems/QAS_Serial/QAS_Serial_Dev_Base.cpp \
../QA_Systems/QAS_Serial/QAS_Serial_Dev_UART.cpp 

OBJS += \
./QA_Systems/QAS_Serial/QAS_Serial_Dev_Base.o \
./QA_Systems/QAS_Serial/QAS_Serial_Dev_UART.o 

CPP_DEPS += \
./QA_Systems/QAS_Serial/QAS_Serial_Dev_Base.d \
./QA_Systems/QAS_Serial/QAS_Serial_Dev_UART.d 


# Each subdirectory must supply rules for building sources it contributes
QA_Systems/QAS_Serial/%.o: ../QA_Systems/QAS_Serial/%.cpp QA_Systems/QAS_Serial/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core -I../QA_Drivers -I../QA_Drivers/QAD_PeripheralManagers -I../QA_Tools -I../QA_Systems -I../QA_Systems/QAS_Serial -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

