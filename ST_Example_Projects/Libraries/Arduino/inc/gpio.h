/**
  ******************************************************************************
  * @file    Arduino/gpio.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Arduino GPIO API for STM32F051 Discovery Kit
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
#ifndef _GPIO_PIN_CONF_H_
#define _GPIO_PIN_CONF_H_

#ifdef __cplusplus
extern "C"{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"



/* Exported types ------------------------------------------------------------*/
typedef unsigned char byte ;
typedef unsigned char BYTE ;
typedef unsigned char boolean ;
typedef enum {FALSE = 0, TRUE = !FALSE} bool;  
  
/*
 * GPIO register maps and devices
 * This structure defines Arduino Pin's functionality 
 * and mapping on STM32
 */
typedef struct
{
  uint16_t PinNum;
  GPIO_TypeDef* GPIOx; 
  uint8_t PinMode;
  uint16_t pwmPeriod;
}GPIO_PinTypeDef;
  

/* Exported constants --------------------------------------------------------*/
//GPIO Pin value
#define LOW 0
#define HIGH 1


//Pin configuration definition Input is odd, output is even. Struc PinMode field 
// A simple way to know INPUT or OUTPUT request will be to test bit0
#define NOT_CONFIGURED 0 //Pin is not configured
#define INPUT 0x01 //Pin is used as input
#define OUTPUT 0x02 //Pin is used as output
#define INPUT_AF 0x3 //Pin is configured as Input Alternate Function (GPIO_Mode_AF)
#define OUTPUT_AF 0x4 //Pin is configured as Output Alternate Function (GPIO_Mode_AF)
#define INPUT_AN 0x5 //Pin is configured as Input analog (GPIO_Mode_AN)
#define OUTPUT_AN 0x6 //Pin is configured as Output analog (GPIO_Mode_AN)
#define INPUT_NOPULL 0x7 //Pin is configured as Input NO pull_up/pull_down (GPIO_PuPd_NOPULL)
#define OUTPUT_OD 0x8 //Pin is configured as Output Open_drain (GPIO_OType_OD)
#define INPUT_PU 0x9 //Pin is configured as Input Pull_up (GPIO_PuPd_UP)
#define OUTPUT_PP 0x0A //Pin is configured as Output Push_Pull (GPIO_OType_PP)
#define INPUT_PD 0x0B //Pin is configured as Input Pull_down (GPIO_PuPd_DOWN)


//Analog Pin number definition
#define A0 14 
#define A1 15 
#define A2 16 
#define A3 17 
#define A4 18 
#define A5 19

#define IS_ARDUINO_PIN(PIN) (((PIN) == 0) || \
                             ((PIN) == 1) || \
                             ((PIN) == 2) || \
                             ((PIN) == 3) || \
                             ((PIN) == 4) || \
                             ((PIN) == 5) || \
                             ((PIN) == 6) || \
                             ((PIN) == 7) || \
                             ((PIN) == 8) || \
                             ((PIN) == 9) || \
                             ((PIN) == 10) || \
                             ((PIN) == 11) || \
                             ((PIN) == 12) || \
                             ((PIN) == 13) || \
                             ((PIN) == 14) || \
                             ((PIN) == 15) || \
                             ((PIN) == 16) || \
                             ((PIN) == 17) || \
                             ((PIN) == 18) || \
                             ((PIN) == 19) || \
                             ((PIN) == 20) || \
                             ((PIN) == 21))

#define IS_ARDUINO_MODE(MODE) (((MODE) == INPUT) || \
                               ((MODE) == OUTPUT))


#define IS_PWM_PIN(pin)  (((PIN) == 3) || \
                             ((PIN) == 5) || \
                             ((PIN) == 6) || \
                             ((PIN) == 9) || \
                             ((PIN) == 10)|| \
                             ((PIN) == 11))

#define IS_ADC_PIN(pin)  (((PIN) == A0) || \
                             ((PIN) == A1) || \
                             ((PIN) == A2) || \
                             ((PIN) == A3) || \
                             ((PIN) == A4)|| \
                             ((PIN) == A5))

/* Exported functions ------------------------------------------------------- */ 
void pinMode(uint16_t, uint16_t); 
void pinSetMode(uint16_t, uint16_t);
uint16_t digitalRead(uint16_t);
void digitalWrite(uint16_t, uint16_t);
uint16_t analogRead(uint16_t);
void analogWrite(uint16_t, uint16_t);
void pwmFrequency(uint16_t, uint16_t);
GPIO_PinTypeDef getPinDef(uint16_t);

#ifdef __cplusplus
}
#endif

#endif /* _GPIO_PIN_CONF_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


