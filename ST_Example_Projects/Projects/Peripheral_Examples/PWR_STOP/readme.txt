/**
  @page PWR_STOP PWR_STOP
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    PWR_STOP/readme.txt  
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Description of the PWR STOP example.
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

This example shows how to enter the system to STOP mode and wake-up using EXTI
Line interrupts. The EXTI Line sources are PA.0 and RTC Alarm.

The EXTI line0 is configured to generate interrupt on falling edge.
The EXTI line17(RTC Alarm) is configured to generate interrupt on rising edge.

The system enters and exits STOP mode as following:
After system start-up, the RTC is configured to generate an Alarm event then the 
system enters STOP mode. To wake-up from STOP mode you have to apply a falling
edge on EXTI line0, otherwise the  RTC Alarm will wake-up the system within 5 
seconds. After exit from STOP, the system clock is reconfigured to its previous 
state (as HSE and PLL are disabled in STOP mode).
Then, after a delay the system will enter again in STOP mode and exit in the way
described above. This behavior is repeated in an infinite loop.

Three leds LD3 and LD4 are used to monitor the system state as following:
 - LD3 Toggling: system in RUN mode
 - LD3 off: system in STOP mode
 - LD4 is on if EXTI Line0 is used to exit from STOP
 - LD4 is off if EXTI line17(RTC Alarm) is used to exit from STOP 
 @note To measure the current consumption in STOP mode, please refer to 
      @subpage PWR_CurrentConsumption example.


@par Directory contents 

  - PWR_STOP/stm32f0xx_conf.h    Library Configuration file
  - PWR_STOP/stm32f0xx_it.c      Interrupt handlers
  - PWR_STOP/stm32f0xx_it.h      Interrupt handlers header file
  - PWR_STOP/main.c              Main program
  - PWR_STOP/system_stm32f0xx.c  STM32F0xx system source file
  
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


@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the PWR_STOP.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the PWR_STOP.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "PWR_STOP"   
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
    - Browse to the TrueSTUDIO workspace directory and select the project "PWR_STOP" 
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
