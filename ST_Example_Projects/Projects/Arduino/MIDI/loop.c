/**
  ******************************************************************************
  * @file    MIDI/loop.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   main loop function call for Arduino sketch
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
#include "Midi_Shield.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
byte incomingByte;
byte note;
byte velocity;
int pot;

byte byte1;
byte byte2;
byte byte3;

int action=2; //1 =note off ; 2=note on ; 3= wait


/**
  * @brief  loop Arduino processing function
  * @param  None
  * @retval None
  */void loop () {

  while(1) {
  //*************** MIDI OUT ***************//

  pot = analogRead(0);
  note = pot/8;  // convert value to value 0-127
  if(button(BUTTON1) || button(BUTTON2) || button(BUTTON3))
  {  
    Midi_Send(0x90,note,0x45);
    while(button(BUTTON1) || button(BUTTON2) || button(BUTTON3));
  }

  //*************** MIDI LOOPBACK ******************//
  if(Serial.available() > 0)
  {
    byte1 = Serial.read();
    byte2 = Serial.read();
    byte3 = Serial.read();

    Midi_Send(byte1, byte2, byte3);
  }

  //*************** MIDI IN ***************//
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // wait for as status-byte, channel 1, note on or off
    if (incomingByte== 144) // Note on
    { 
      action = OFF;
    }
    else if (incomingByte== 128) // Note off
    { 
      action = ON;
    }
    else if (note==0 && action != WAIT) // note on, wait for note value
    { 
      note=incomingByte;
    }
    else if (note!=0 && action != WAIT)  // velocity
    { 
      velocity=incomingByte;
      if(action == ON){ 
        Midi_Send(0x90,note,velocity); 
      }
      if(action == OFF){ 
        Midi_Send(0x80,note,velocity); 
      }
      note=0;
      velocity=0;
      action=WAIT;
    }
    else{
    }
   }
  } //End of while(1)
}



/**
  * @brief  Midi_Send  send cmd
  * @param  cmd and data to send
  * @retval None
  */
  void Midi_Send(byte cmd, byte data1, byte data2) {
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
}

/**
  * @brief  blink 
  * @param  None
  * @retval None
  */
void blink(void){
  digitalWrite(STAT1, HIGH);
  delay(100);
  digitalWrite(STAT1, LOW);
  delay(100);
}

/**
  * @brief  button 
  * @param  button number
  * @retval button pressed?
  */
char button(char button_num)
{
  return (!(digitalRead(button_num)));
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
