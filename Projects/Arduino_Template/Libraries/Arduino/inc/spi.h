/**
  ******************************************************************************
  * @file    Arduino/spi.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Arduino spi communication API for STM32F051 Discovery Kit
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
#ifndef _ARDUINO_SPI_H_
#define _ARDUINO_SPI_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "gpio.h"

/* Exported types ------------------------------------------------------------*/
/*
 * SPI struct definition
 * to be compatible with Arduino API functions call
 */
typedef struct
{
  uint16_t SpiOneLine; //0=Default SPI config is FullDuplex (2Lines)
  uint16_t Spi16Bit; //0=Default SPI config is 8 bit write 1 for 16 bit config
  void (*begin)();
  unsigned int (*read)(uint16_t);
  void (*write)(uint16_t);
}SPI_ArduinoTypeDef;
 

/* Exported constants --------------------------------------------------------*/
/*Define the dedicated reception circular buffer size. */
#define BUFFER_RCV_SIZE 256

/*Pins definition */
#define CS   10
#define MISO 12
#define MOSI 11
#define SCK  13

/*Possible solution for SpiOneLine settings*/
#define _SPI_2L_FULL_DUPLEX 0
#define _SPI_2L_RX          1
#define _SPI_1L_RX          2
#define _SPI_1L_TX          3


/* SPI Arduino object created in .c file */
extern SPI_ArduinoTypeDef SPI;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void begin_spi_fcn(void);
unsigned int read_spi_fcn(uint16_t);
void write_spi_fcn(uint16_t);

#ifdef __cplusplus
}
#endif

#endif /* _ARDUINO_SPI_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

