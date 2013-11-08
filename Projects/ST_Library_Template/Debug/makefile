################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include Libraries/STM32F0xx_StdPeriph_Driver/src/subdir.mk
-include Libraries/CMSIS/Device/ST/STM32F0xx/Source/Templates/subdir.mk
-include Device/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ST_Library_Template.hex \

SECONDARY_LIST += \
ST_Library_Template.lst \

SECONDARY_SIZE += \
ST_Library_Template.siz \


# All Target
all: ST_Library_Template.elf secondary-outputs

# Tool invocations
ST_Library_Template.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -T stm32f0.ld -Xlinker --gc-sections -L"C:\stm32f0-Discovery_Tools\Projects\ST_Library_Template\Device\ldscripts" -Wl,-Map,"ST_Library_Template.map" -o "ST_Library_Template.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ST_Library_Template.hex: ST_Library_Template.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy "ST_Library_Template.elf" -O ihex  "ST_Library_Template.hex"
	@echo 'Finished building: $@'
	@echo ' '

ST_Library_Template.lst: ST_Library_Template.elf
	@echo 'Invoking: Cross ARM GNU Create Listing'
	arm-none-eabi-objdump "ST_Library_Template.elf" --source --all-headers --demangle --line-numbers --wide > "ST_Library_Template.lst"
	@echo 'Finished building: $@'
	@echo ' '

ST_Library_Template.siz: ST_Library_Template.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size  --format=berkeley "ST_Library_Template.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(S_UPPER_DEPS) ST_Library_Template.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
