/**
  @page IWDG_Reset IWDG Reset example
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    IWDG_Reset/readme.txt  
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Description of the IWDG Reset example.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *   
  ******************************************************************************
   @endverbatim

@par Example Description 

This example shows how to update at regular period the IWDG reload counter and 
how to simulate a software fault generating an MCU IWDG reset on expiry of a 
programmed time period.

The IWDG timeout is set to 250 ms (the timeout may varies due to LSI frequency 
dispersion).

First, the TIM14 timer is configured to measure the LSI frequency as the LSI is 
internally connected to TIM14 CH1, in order to adjust the IWDG clock.

The LSI measurement using the TIM14 is described below:
 - Configure the TIM14 to remap internally the TIM14 Channel 1 Input Capture to 
   the LSI clock output.
 - Enable the TIM14 Input Capture interrupt: after one cycle of LSI clock, the
   period value is stored in a variable and compared to the HCLK clock to get
   its real value. 

@note The LSI is internally connected to TIM14 IC1 on STM32F0xx Devices.
      When using other devices, you should comment the "#define LSI_TIM_MEASURE"
      in main.c file and in this case the LSI frequency is set by default to 40KHz.
   
Then, the IWDG reload counter is configured to obtain 240ms according to the 
measured LSI frequency.

The IWDG reload counter is refreshed each 240ms in the main program infinite loop 
to prevent a IWDG reset.
LED4 is also toggled each 240ms indicating that the program is running.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the rising edge of the signal.

The EXTI Line is used to simulate a software failure: once the EXTI Line event 
occurs, by pressing the User-button, the corresponding interrupt is served.
In the ISR, a write to invalid address generates a Hardfault exception containing
an infinite loop and preventing to return to main program (the IWDG reload counter 
is not refreshed).
As a result, when the IWDG counter reaches 00h, the IWDG reset occurs.
If the IWDG reset is generated, after the system resumes from reset, LED3 turns on.

If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no IWDG reset.

In this example the system clock is set to 48 MHz.

@note The IWDG Counter can be only written when the RVU flag is reset. 
      In this example, as the SysTick period is too higher than the IWDG Counter
      Update timing (5 Cycles 40KHz RC), the Reload Value Update "RVU" flag is 
      not checked before reloading the counter.


@par Directory contents 

  - IWDG_Reset/stm32f10x_conf.h     Library Configuration file
  - IWDG_Reset/stm32f10x_it.c       Interrupt handlers
  - IWDG_Reset/stm32f10x_it.h       Header for stm32f0xx_it.c
  - IWDG_Reset/main.c               Main program
  - IWDG_Reset/system_stm32f10x.c   STM32F0xx system source file
  
@note The "system_stm32f0xx.c" file contains the system clock configuration for
      STM32F0xx devices, and is customized for use with STM32F0-Discovery Kit. 
      The STM32F0xx is configured to run at 48 MHz, following the three  
      configuration below:
        + PLL_SOURCE_HSI
           - HSI (~8 MHz) used to clock the PLL, and the PLL is used as system 
             clock source.  
        + PLL_SOURCE_HSE          
           - HSE (8 MHz) used to clock the PLL, and the PLL is used as system
             clock source.
           - The HSE crystal is not provided with the Discovery Kit, some 
             hardware modification are needed in manner to connect this crystal.
             For more details, refer to section "4.7 OSC clock" in "STM32F0 discovery kit User manual (UM1525)"
        + PLL_SOURCE_HSE_BYPASS   
           - HSE bypassed with an external clock (fixed at 8 MHz, coming from 
             ST-Link circuit) used to clock the PLL, and the PLL is used as 
             system clock source.
           - Some  hardware modification are needed in manner to bypass the HSE 
             with clock coming from the ST-Link circuit.
             For more details, refer to section "4.7 OSC clock" in "STM32F0 discovery kit User manual (UM1525)"
      User can select one of the three configuration in system_stm32f0xx.c file
      (default configuration is PLL_SOURCE_HSI).
         

@par Hardware and Software environment

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM32F0-Discovery (MB1034) 
    RevB and can be easily tailored to any other supported device and development board.
  
  - STM32F0-Discovery Set-up  
    - Use LD3 and LD4 leds connected respectively to PC.08 and PC.09 pins
    - Use the USER button connected to PA.00 pin (EXTI Line0).  


@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the IWDG_Reset.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the IWDG_Reset.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "IWDG_Reset"   
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

 + TrueSTUDIO for ARM
    - Open the TrueSTUDIO for ARM toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project "IWDG_Reset" 
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
