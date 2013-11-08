/**
  ******************************************************************************
  * @file    TEST/loop.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Arduino functions test for STM32F051 Discovery Kit
  *          Get ADC value from pin A0 to A6.
  *          Set different frequencies to PWM pins 3,5,6,9,10 and 11
  *          Configure Rx/Tx pins 0 and 1 and use Serial "object" to print info
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
uint16_t G_ADCValue=0;
float G_ReadVal =0;
char G_SendBuff[64];

/**
  * @brief  loop Arduino processing function
  * @param  None
  * @retval None
  */
void loop()
{
uint16_t i = 0;
char L_readChar;
   //pinMode(motor_cmd,OUTPUT);
pwmFrequency(pwm_pin_3,1000); //set pwm frequencies
pwmFrequency(pwm_pin_5,2000); //it is not a standart arduino function
pwmFrequency(pwm_pin_6,3000);
pwmFrequency(pwm_pin_9,4000);
pwmFrequency(pwm_pin_10,5000);
pwmFrequency(pwm_pin_11,6000);
Serial.begin(115200); //Open com on uart 0-1 pins

    while(1){
     analogWrite(pwm_pin_3,i*10);
     analogWrite(pwm_pin_5,i*10);
     analogWrite(pwm_pin_6,i*10);
     analogWrite(pwm_pin_9,i*10);
     analogWrite(pwm_pin_10,i*10);
     analogWrite(pwm_pin_11,i*10);
     delay(2000); //Wait for 1 sec 
     G_ADCValue = analogRead(A2); 
     G_ReadVal = (float)(POWER_SUPPLY * G_ADCValue) / 4095;
     sprintf(G_SendBuff,"%f Volt",G_ReadVal);
     Serial.print("ADC read: ");
     Serial.println(G_SendBuff);     
     i++;
     if(i>10) i=0;
     while(Serial.available()) {
       L_readChar = Serial.read();
       Serial.write(L_readChar);
     }
     Serial.println("");     
     
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

