/**
  ******************************************************************************
  * @file    NFC_SPI/loop.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   main loop function call
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
#include "loop.h"
#include "PN532.h"

 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/


/**
  * @brief  loop Arduino processing function
  * @param  None
  * @retval None
  */
  void loop(void) {
  uint32_t id;
  char buff[5];
  
  while(1) {
    // look for MiFare type cards
    id = PN532.readPassiveTargetID(PN532_MIFARE_ISO14443A);
    sprintf(buff,"%d",id);
 
    if (id != 0) {
      Serial.print("Read card #"); Serial.println(buff);
    }
    delay(2000);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
