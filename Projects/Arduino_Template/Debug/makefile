################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include Libraries/Utilities/subdir.mk
-include Libraries/STM32F0xx_StdPeriph_Driver/src/subdir.mk
-include Libraries/CMSIS/Device/ST/STM32F0xx/Source/Templates/subdir.mk
-include Libraries/Arduino/src/subdir.mk
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
Arduino_Template.hex \

SECONDARY_LIST += \
Arduino_Template.lst \

SECONDARY_SIZE += \
Arduino_Template.siz \


# All Target
all: Arduino_Template.elf secondary-outputs

# Tool invocations
Arduino_Template.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -T stm32f0.ld -L"C:\stm32f0-Discovery_Tools\Projects\Arduino_Template\Device\ldscripts" -Wl,-Map,"Arduino_Template.map" -o "Arduino_Template.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Arduino_Template.hex: Arduino_Template.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy "Arduino_Template.elf" -O ihex  "Arduino_Template.hex"
	@echo 'Finished building: $@'
	@echo ' '

Arduino_Template.lst: Arduino_Template.elf
	@echo 'Invoking: Cross ARM GNU Create Listing'
	arm-none-eabi-objdump "Arduino_Template.elf" --source --all-headers --demangle --line-numbers --wide > "Arduino_Template.lst"
	@echo 'Finished building: $@'
	@echo ' '

Arduino_Template.siz: Arduino_Template.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size  --format=berkeley "Arduino_Template.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(S_UPPER_DEPS) Arduino_Template.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
