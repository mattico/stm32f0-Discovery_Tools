################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/Utilities/stm32f0_discovery.c \
../Libraries/Utilities/stm32f0xx_it.c \
../Libraries/Utilities/utilities.c 

OBJS += \
./Libraries/Utilities/stm32f0_discovery.o \
./Libraries/Utilities/stm32f0xx_it.o \
./Libraries/Utilities/utilities.o 

C_DEPS += \
./Libraries/Utilities/stm32f0_discovery.d \
./Libraries/Utilities/stm32f0xx_it.d \
./Libraries/Utilities/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/Utilities/%.o: ../Libraries/Utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DUSE_STDPERIPH_DRIVER -DUSE_STM32F0_DISCOVERY -DSTM32F0XX -I"C:\stm32f0-Discovery_Tools\Projects\Arduino_Template\Libraries\Arduino\inc" -I"C:\stm32f0-Discovery_Tools\Projects\Arduino_Template\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\stm32f0-Discovery_Tools\Projects\Arduino_Template\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\stm32f0-Discovery_Tools\Projects\Arduino_Template\Libraries\CMSIS\Include" -I"C:\stm32f0-Discovery_Tools\Projects\Arduino_Template\src" -I"C:\stm32f0-Discovery_Tools\Projects\Arduino_Template\Libraries\Utilities" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


