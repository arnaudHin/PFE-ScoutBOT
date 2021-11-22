################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lib/MOTORS.c \
../Core/Src/lib/TIMERS.c \
../Core/Src/lib/stm32mp1xx_error_handler.c \
../Core/Src/lib/stm32mp1xx_gpio.c \
../Core/Src/lib/stm32mp1xx_ipcc.c \
../Core/Src/lib/stm32mp1xx_systemclock.c \
../Core/Src/lib/stm32mp1xx_uart.c \
../Core/Src/lib/stm32mp1xx_vuart.c 

OBJS += \
./Core/Src/lib/MOTORS.o \
./Core/Src/lib/TIMERS.o \
./Core/Src/lib/stm32mp1xx_error_handler.o \
./Core/Src/lib/stm32mp1xx_gpio.o \
./Core/Src/lib/stm32mp1xx_ipcc.o \
./Core/Src/lib/stm32mp1xx_systemclock.o \
./Core/Src/lib/stm32mp1xx_uart.o \
./Core/Src/lib/stm32mp1xx_vuart.o 

C_DEPS += \
./Core/Src/lib/MOTORS.d \
./Core/Src/lib/TIMERS.d \
./Core/Src/lib/stm32mp1xx_error_handler.d \
./Core/Src/lib/stm32mp1xx_gpio.d \
./Core/Src/lib/stm32mp1xx_ipcc.d \
./Core/Src/lib/stm32mp1xx_systemclock.d \
./Core/Src/lib/stm32mp1xx_uart.d \
./Core/Src/lib/stm32mp1xx_vuart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lib/%.o: ../Core/Src/lib/%.c Core/Src/lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DMETAL_MAX_DEVICE_REGIONS=2 -DVIRTIO_SLAVE_ONLY -DUSE_HAL_DRIVER -DSTM32MP157Cxx -c -I../OPENAMP -I../Core/Inc -I../../Middlewares/Third_Party/OpenAMP/open-amp/lib/include -I../../Middlewares/Third_Party/OpenAMP/libmetal/lib/include -I../../Drivers/STM32MP1xx_HAL_Driver/Inc -I../../Drivers/STM32MP1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32MP1xx/Include -I../../Middlewares/Third_Party/OpenAMP/virtual_driver -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

