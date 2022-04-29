################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../QA_Drivers/QAD_ADC.cpp \
../QA_Drivers/QAD_EXTI.cpp \
../QA_Drivers/QAD_Encoder.cpp \
../QA_Drivers/QAD_Flash.cpp \
../QA_Drivers/QAD_GPIO.cpp \
../QA_Drivers/QAD_I2C.cpp \
../QA_Drivers/QAD_PWM.cpp \
../QA_Drivers/QAD_RGB.cpp \
../QA_Drivers/QAD_SPI.cpp \
../QA_Drivers/QAD_Servo.cpp \
../QA_Drivers/QAD_Timer.cpp \
../QA_Drivers/QAD_UART.cpp 

OBJS += \
./QA_Drivers/QAD_ADC.o \
./QA_Drivers/QAD_EXTI.o \
./QA_Drivers/QAD_Encoder.o \
./QA_Drivers/QAD_Flash.o \
./QA_Drivers/QAD_GPIO.o \
./QA_Drivers/QAD_I2C.o \
./QA_Drivers/QAD_PWM.o \
./QA_Drivers/QAD_RGB.o \
./QA_Drivers/QAD_SPI.o \
./QA_Drivers/QAD_Servo.o \
./QA_Drivers/QAD_Timer.o \
./QA_Drivers/QAD_UART.o 

CPP_DEPS += \
./QA_Drivers/QAD_ADC.d \
./QA_Drivers/QAD_EXTI.d \
./QA_Drivers/QAD_Encoder.d \
./QA_Drivers/QAD_Flash.d \
./QA_Drivers/QAD_GPIO.d \
./QA_Drivers/QAD_I2C.d \
./QA_Drivers/QAD_PWM.d \
./QA_Drivers/QAD_RGB.d \
./QA_Drivers/QAD_SPI.d \
./QA_Drivers/QAD_Servo.d \
./QA_Drivers/QAD_Timer.d \
./QA_Drivers/QAD_UART.d 


# Each subdirectory must supply rules for building sources it contributes
QA_Drivers/%.o QA_Drivers/%.su: ../QA_Drivers/%.cpp QA_Drivers/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core -I../QA_Drivers -I../QA_Drivers/QAD_PeripheralManagers -I../QA_Tools -I../QA_Systems -I../QA_Systems/QAS_Serial -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-QA_Drivers

clean-QA_Drivers:
	-$(RM) ./QA_Drivers/QAD_ADC.d ./QA_Drivers/QAD_ADC.o ./QA_Drivers/QAD_ADC.su ./QA_Drivers/QAD_EXTI.d ./QA_Drivers/QAD_EXTI.o ./QA_Drivers/QAD_EXTI.su ./QA_Drivers/QAD_Encoder.d ./QA_Drivers/QAD_Encoder.o ./QA_Drivers/QAD_Encoder.su ./QA_Drivers/QAD_Flash.d ./QA_Drivers/QAD_Flash.o ./QA_Drivers/QAD_Flash.su ./QA_Drivers/QAD_GPIO.d ./QA_Drivers/QAD_GPIO.o ./QA_Drivers/QAD_GPIO.su ./QA_Drivers/QAD_I2C.d ./QA_Drivers/QAD_I2C.o ./QA_Drivers/QAD_I2C.su ./QA_Drivers/QAD_PWM.d ./QA_Drivers/QAD_PWM.o ./QA_Drivers/QAD_PWM.su ./QA_Drivers/QAD_RGB.d ./QA_Drivers/QAD_RGB.o ./QA_Drivers/QAD_RGB.su ./QA_Drivers/QAD_SPI.d ./QA_Drivers/QAD_SPI.o ./QA_Drivers/QAD_SPI.su ./QA_Drivers/QAD_Servo.d ./QA_Drivers/QAD_Servo.o ./QA_Drivers/QAD_Servo.su ./QA_Drivers/QAD_Timer.d ./QA_Drivers/QAD_Timer.o ./QA_Drivers/QAD_Timer.su ./QA_Drivers/QAD_UART.d ./QA_Drivers/QAD_UART.o ./QA_Drivers/QAD_UART.su

.PHONY: clean-QA_Drivers

