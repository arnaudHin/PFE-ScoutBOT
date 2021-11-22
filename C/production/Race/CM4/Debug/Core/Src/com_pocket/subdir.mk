################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/com_pocket/dispatcher_robot.c \
../Core/Src/com_pocket/intercore_protocol.c \
../Core/Src/com_pocket/postman_robot.c 

OBJS += \
./Core/Src/com_pocket/dispatcher_robot.o \
./Core/Src/com_pocket/intercore_protocol.o \
./Core/Src/com_pocket/postman_robot.o 

C_DEPS += \
./Core/Src/com_pocket/dispatcher_robot.d \
./Core/Src/com_pocket/intercore_protocol.d \
./Core/Src/com_pocket/postman_robot.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/com_pocket/%.o: ../Core/Src/com_pocket/%.c Core/Src/com_pocket/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DMETAL_MAX_DEVICE_REGIONS=2 -DVIRTIO_SLAVE_ONLY -DUSE_HAL_DRIVER -DSTM32MP157Cxx -c -I../OPENAMP -I../Core/Inc -I../../Middlewares/Third_Party/OpenAMP/open-amp/lib/include -I../../Middlewares/Third_Party/OpenAMP/libmetal/lib/include -I../../Drivers/STM32MP1xx_HAL_Driver/Inc -I../../Drivers/STM32MP1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32MP1xx/Include -I../../Middlewares/Third_Party/OpenAMP/virtual_driver -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

