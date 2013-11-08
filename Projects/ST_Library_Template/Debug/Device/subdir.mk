################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Device/startup_stm32f0xx.S 

OBJS += \
./Device/startup_stm32f0xx.o 

S_UPPER_DEPS += \
./Device/startup_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -x assembler-with-cpp -DUSE_STDPERIPH_DRIVER -DUSE_STM32F0_DISCOVERY -DSTM32F0XX -I"C:\stm32f0-Discovery_Tools\Projects\ST_Library_Template\Libraries\CMSIS\Include" -I"C:\stm32f0-Discovery_Tools\Projects\ST_Library_Template\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\stm32f0-Discovery_Tools\Projects\ST_Library_Template\src" -I"C:\stm32f0-Discovery_Tools\Projects\ST_Library_Template\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


