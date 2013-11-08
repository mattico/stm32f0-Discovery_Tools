/**
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Description of the STM32F0 Discovery kit projects examples 
  *          for Arduino, Mikroelektronika and ST MEMS DIL24 boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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
  */

Package Description
===================
This package contains projects dedicated to STM32F0 Discovery kit.
Projects examples are available for both IAR and KEIL toolchains from versions :
-IAR EWARM V6.30.1
-KEIL MDK-ARM V4.21.2.0

Projects examples are available for:
- Mikroelektronika boards :  
	ADC_Click, BLUETOOTH_Click, BUZZ_Click, DIGIPOT_Click, RTC_Click and IDC10 EasyLED boards.
	IDC10 LCD board "COG 2x16 LCD" is used as display board in some examples.
	Visit http://www.mikroe.com/
- Arduino shield boards :
	Bluetooth BEE shield board + BEES Shield (visit http://www.seeedstudio.com/wiki/Bees_Shield and http://www.seeedstudio.com/wiki/Bluetooth_Bee )
	NFC shield board (visit http://www.seeedstudio.com/wiki/NFC_Shield )
	Sparkfun MIDI shield board (visit http://www.sparkfun.com/products/9595 ) 
- ST MEMS DIL24 boards :
	STEVAL-MKI108V2 board (visit http://www.st.com/internet/evalboard/product/252687.jsp )
	
Note: Mikroelektronika boards, Arduino shield boards and ST MEMS DIL24 boards are NOT SUPPLIED with the STM32F0 Discovery kit.
	
Look at Board_Connection.xls file for the connection between standard connectors (mikroBUS, IDC10, Arduino and ST MEMS DIL24) and STM32F0 Discovery kit connectors.
 1) Mikroelektronika mikroBUS is described in mikrobus_specification.pdf  
 2) Prototyping board connection for Arduino is compatible with "Arduino Uno" described at http://arduino.cc/en/Main/arduinoBoardUno
 3) ST MEMS DIL24 boards standard connector is described in page 2 of STEVAL-MKI108V2.pdf

Package Overview
================
This STM32F0 Discovery demonstration software consists of the following components:
   1. Libraries
	  1.1 STM32F0xx CMSIS and Standard Peripherals drivers.
	  1.2 Arduino shield board dedicated API
	  1.2 Mikroelektronika IDC10 and mikroBUS dedicated API
	  1.3 ST MEMS DIL24 dedicated API
   2. Projects
	  2.1 Arduino projects
	  2.2 Mikroelektronika projects
	  2.3 ST MEMS DIL24 projects
   3. Documentation
	  3.1 Readme.txt : this file
	  3.2 Board_Connection.xls
	  3.3 mikrobus_specification.pdf
	  3.4 STEVAL-MKI108V2.pdf
	  
Libraries:
----------
- Arduino library has been developped in order to be compatible with existing Arduino software.
main.c file is common to all Arduino projects. 
main() function first calls Discovery kit configuration followed by Arduino setup() and loop() functions.
Each project has its setup() and loop() functions written in setup.c and loop.c files.
Arduino API has not been fully developped but main GPIO and communication functions are available.

- Mikroelektronika libray has been developped to be compatible with "mikroC PRO" toolchain functions call.
It allows customers to re-use Click Boards already developped examples with minimum of modifications.

- ST MEMS DIL24 includes LSM303DLHC MEMS communication driver.

Projects:
---------
1)Arduino 
-TEST project is dedicated to STM32F0 Discovery kit functional test. No shield board is needed. 
-BeeBluetooth is a porting of already existing project for Bees Shield V2.12 (http://www.seeedstudio.com/wiki/Bluetooth_Bee)
-MIDI is a porting of project already developped for MIDI shield (http://www.sparkfun.com/datasheets/DevTools/Arduino/Midi_Shield.zip)
-NFC_SPI is a porting of project already developped for NFC shield V0.9b (http://www.seeedstudio.com/wiki/NFC_Shield)

2)Mikroelektronika projects are based on Click Boards existing examples.
-ADC_Click project based on ADC click board uses SPI bidirectional communication lines.
-BLUETOOTH_Click project based on BLUETOOTH click board uses RX/TX UART communication line.
-BUZZ_Click project based on BUZZ click uses PWM output line.
-DIGIPOT_Click project based on DIGIPOT click uses SPI communication lines.
-RTC_Click project based on RTC click uses I2C and hardware interrupt line.
-EasyLED project based on EasyLED board drives IDC10 port as standard GPIO output. 
Note: IDC10 port is also used for "COG 2x16 LCD" display board.

3)MEMS_DIL24_Eval-Board project is dedicated to STEVAL-MKI108V2 board.
-MKI108V2 project provides example for Magneto and Temperature MEMS.
STM-STUDIO project setting is available for both EWARM and MDK-ARM projects in order to easily display Magneto and Temperature MEMS values on PC without any code modification.
Visit http://www.st.com/stm-studio for STM-STUDIO installation.

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

 