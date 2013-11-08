/**
  ******************************************************************************
  * @file    NFC_SPI/setup.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   NFC example for seeedstudio.com board translated for STM32F051 Discovery Kit
  *  NFC shield V0.9b
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
#include <stdio.h>
#include "setup.h"
#include "stm32f0xx_conf.h"
#include <PN532.h>
 
 
/**
  * @brief  config Arduino processing function
  * @param  None
  * @retval None
  */
void setup(void) {
  char buff[8];
  uint32_t versiondata;

  Serial.begin(115200); //open Serial com pin0(Rx) pin1(Tx)
 
  PN532.begin();
  
  versiondata = PN532.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  sprintf(buff,"%x",(versiondata>>24) & 0xFF);
  Serial.print("Found chip PN5"); Serial.println(buff); 
  sprintf(buff,"%d",(versiondata>>16) & 0xFF);
  Serial.print("Firmware ver. "); Serial.print(buff); 
  sprintf(buff,"%d",(versiondata>>8) & 0xFF);
  Serial.write('.'); Serial.println(buff);
  sprintf(buff,"%x",versiondata & 0xFF);
  Serial.print("Supports "); Serial.println(buff);
 
  // configure board to read RFID tags and cards
  PN532.SAMConfig();
}
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
