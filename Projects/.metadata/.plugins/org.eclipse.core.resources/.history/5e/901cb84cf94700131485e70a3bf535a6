################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c 

OBJS += \
./src/main.o 

C_DEPS += \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DUSE_STDPERIPH_DRIVER -DUSE_STM32F0_DISCOVERY -DSTM32F0XX -I"C:\stm32f0-Discovery_Tools\Projects\ST_Library_Template\Libraries\CMSIS\Include" -I"C:\stm32f0-Discovery_Tools\Projects\ST_Library_Template\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\stm32f0-Discovery_Tools\Projects\ST_Library_Template\src" -I"C:\stm32f0-Discovery_Tools\Projects\ST_Library_Template\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


