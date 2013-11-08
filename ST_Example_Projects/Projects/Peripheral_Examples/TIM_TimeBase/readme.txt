/**
  @page Time_Base TIM Time Base Description
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    TIM_Time_Base/readme.txt  
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   TIM Time Base Description.
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

This example shows how to configure the TIM peripheral in Output Compare Timing 
mode with the corresponding Interrupt requests for each channel in order to generate
2 different time bases.

The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter 
clock at 6 MHz so the Prescaler is computed as following:
   - Prescaler = (TIM3CLK / TIM3 counter clock) - 1

SystemCoreClock is set to 48MHz.

The TIM3 CC3 register is equal to 13654, 
CC3 update rate = TIM3 counter clock / CCR3_Val = 439.4Hz
so the TIM3 Channel 3 generates an interrupt each 2.27ms

The TIM3 CC4 register is equal to 6826, 
CC4 update rate = TIM3 counter clock / CCR4_Val =  878.9 Hz
so the TIM3 Channel 4 generates an interrupt each 1.13ms.

When the counter value reaches the Output compare registers values, the Output 
Compare interrupts are generated and, in the handler routine, 2 leds (LED3 and
 LED4) are toggled with the following frequencies: 

- LED3: 219.7Hz (CC3) 
- LED4: 439.4Hz (CC4)


@par Directory contents 

  - TIM_Time_Base/stm32f0xx_conf.h    Library Configuration file
  - TIM_Time_Base/stm32f0xx_it.c      Interrupt handlers
  - TIM_Time_Base/stm32f0xx_it.h      Interrupt handlers header file
  - TIM_Time_Base/main.c              Main program
  - TIM_Time_Base/system_stm32f0xx.c  STM32F0xx system source file
  
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
    - None.


@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the TIM_Time_Base.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the TIM_Time_Base.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "TIM_Time_Base"   
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
    - Browse to the TrueSTUDIO workspace directory and select the project "TIM_Time_Base" 
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
