/**
  ******************************************************************************
  * @file    MIDI/loop.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   includes for loop.c
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
/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef __LOOP_H
#define __LOOP_H

/* Includes ------------------------------------------------------------------*/
#include "utilities.h"
#include "gpio.h"
#include "serial.h"

/* Exported constants --------------------------------------------------------*/


/* Exported macros -----------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/

/* function prototypes -------------------------------------------------------*/
void loop(void);
void Midi_Send(byte, byte, byte);
void blink(void);
char button(char);

#endif /* __LOOP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/