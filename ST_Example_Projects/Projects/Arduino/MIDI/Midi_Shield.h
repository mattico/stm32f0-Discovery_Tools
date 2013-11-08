/**
  ******************************************************************************
  * @file    MIDI/Midi_shield.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   includes for MIDI shield board
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
#ifndef __MIDI_SHIELD_H
#define __MIDI_SHIELD_H

/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
// defines for MIDI Shield components only
#define KNOB1  0
#define KNOB2  1

#define BUTTON1  2
#define BUTTON2  3
#define BUTTON3  4

#define STAT1  7
#define STAT2  6

#define OFF 1
#define ON 2
#define WAIT 3

/* Exported macros -----------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/

/* function prototypes -------------------------------------------------------*/
void Midi_Send(byte, byte, byte);
char button(char);

#endif /* __MIDI_SHIELD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


