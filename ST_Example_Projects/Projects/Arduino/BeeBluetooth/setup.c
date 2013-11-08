/**
  ******************************************************************************
  * @file    BeeBluetooth/setup.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   configuration function call for Arduino sketch
  *  Translation of "BluetoothBee Demo Code" for STM32F051 Discovery Kit
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
/* Includes ------------------------------------------------------------------*/       
#include "setup.h"
#include "stm32f0xx_conf.h"

/*
BluetoothBee Demo Code - Flowcontrol Based Implementation
2010,2011 Copyright (c) Seeed Technology Inc.  All right reserved.
 
Author: Visweswara R
 
This demo code is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
 
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
For more details about the product please check http://www.seeedstudio.com/depot/
 
*/
 
 
/* Upload this sketch to Seeeduino and press reset*/
 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEBUG_ENABLED  1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
 
 
/**
  * @brief  config Arduino processing function
  * @param  None
  * @retval None
  */
void setup(void) 
{ 
    setupBlueToothConnection();
} 
 

/**
  * @brief  setupBlueToothConnection configuration of serial com and BT initialization
  * @param  None
  * @retval None
  */
void setupBlueToothConnection(void)
{
    Serial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
    delay(1000);
    sendBlueToothCommand("\r\n+STWMOD=0\r\n");
    sendBlueToothCommand("\r\n+STNA=SeeeduinoBluetooth\r\n");
    sendBlueToothCommand("\r\n+STAUTO=0\r\n");
    sendBlueToothCommand("\r\n+STOAUT=1\r\n");
    sendBlueToothCommand("\r\n +STPIN=0000\r\n");
    delay(4000); // This delay is required.
    sendBlueToothCommand("\r\n+INQ=1\r\n");
    delay(2000); // This delay is required.
}
 

/**
  * @brief  CheckOK Checks if the response "OK" is received
  * @param  None
  * @retval None
  */
void CheckOK(void)
{
  char a,b;
  while(1)
  {
    if(Serial.available())
    {
    a = Serial.read();
 
    if('O' == a)
    {
      // Wait for next character K. available() is required in some cases, as K is not immediately available.      
      while(!Serial.available());
      b = Serial.read();
      if('K' == b)
      {
        break;
      }
    }
   }
  }
  delay(10); 
  while( (a = Serial.read()) != (char)-1)
  {
    //Wait until all other response chars are received
  }
}
 
/**
  * @brief  sendBlueToothCommand Send a eting of char for bluetooth taregt
  * @param  None
  * @retval None
  */
void sendBlueToothCommand(char* command)
{
    Serial.print(command);
//    delay(1500); // This delay is required.   
    CheckOK();   
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
