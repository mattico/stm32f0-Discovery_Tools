/**
  ******************************************************************************
  * @file    Arduino/serial.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Arduino serial communication API for STM32F051 Discovery Kit
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
#ifndef _SERIAL_H_
#define _SERIAL_H_

#ifdef __cplusplus
extern "C"{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Exported types ------------------------------------------------------------*/
/*
 * Serial functions struct to run on usart
 * to be compatible with Arduino API functions call
 * Serial.read, serial.write etc...
 */
typedef struct
{
  uint32_t baudRate;
  void (*begin)(uint32_t);
  uint16_t (*available)(void);
  void (*flush)(void);
  char (*read)(void);
  char (*peek)(void);
  void (*write)(unsigned char);
  void (*print)(char*);
  void (*println)(char*);  
}Serial_TypeDef;
 
/* Serial Arduino object created in serial.c file */
extern Serial_TypeDef Serial;

/* Exported constants --------------------------------------------------------*/
/*Define the dedicated reception circular buffer size. */
#define BUFFER_RCV_SIZE 	256

/*Pins definition */
#define TX_PIN  			GPIO_Pin_2 
#define RX_PIN  			GPIO_Pin_3
#define TX_PIN_SOURCE   	GPIO_PinSource2
#define RX_PIN_SOURCE   	GPIO_PinSource3
#define UART_PORT 			GPIOA

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void begin_fcn(uint32_t);
uint16_t available_fcn(void);
void flush_fcn(void);
char read_fcn(void);
char peek_fcn(void);
void write_fcn(unsigned char);
void print_fcn(char*);
void println_fcn(char*);  

#ifdef __cplusplus
}
#endif

#endif /* _SERIAL_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


