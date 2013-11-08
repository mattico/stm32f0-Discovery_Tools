/**
  ******************************************************************************
  * @file    MikroElektronika/spi.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   SPI mikroBUS API for STM32F051 Discovery Kit
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
#ifndef __SPI_H
#define __SPI_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define _SPI_4_BIT  0x01
#define _SPI_5_BIT	0x02
#define _SPI_6_BIT	0x03
#define _SPI_7_BIT	0x04
#define _SPI_8_BIT	0x05
#define _SPI_9_BIT	0x06
#define _SPI_10_BIT	0x07
#define _SPI_11_BIT	0x08
#define _SPI_12_BIT	0x09
#define _SPI_13_BIT	0x0A
#define _SPI_14_BIT	0x0B
#define _SPI_15_BIT	0x0C
#define _SPI_16_BIT	0x0D
#define _SPI_MASK_BIT	0x000F

#define _SPI_CLK_IDLE_LOW	0x00
#define _SPI_CLK_IDLE_HIGH	0x10
#define _SPI_MASK_CLK_IDLE	0x0010

#define _SPI_FIRST_CLK_EDGE	0x00
#define _SPI_SECOND_CLK_EDGE	0x20
#define _SPI_MASK_CLK_EDGE	0x0020

#define _SPI_FIRST_BIT_MSB	0x00
#define _SPI_FIRST_BIT_LSB	0x40
#define _SPI_MASK_CLK_FIRST_BIT	0x0040

#define _SPI_2L_FULL_DUPLEX 0x100
#define _SPI_2L_RX          0x200
#define _SPI_1L_RX          0x300
#define _SPI_1L_TX          0x400
#define _SPI_MASK_DIRECTION 0x700

#define _SPI_NSS_SOFT 0x000
#define _SPI_NSS_HARD 0x800
#define _SPI_MASK_NSS 0x0800

#define _SPI_BAUD_RATE_PRESCALER_2 0x1000
#define _SPI_BAUD_RATE_PRESCALER_4 0x2000
#define _SPI_BAUD_RATE_PRESCALER_8 0x3000
#define _SPI_BAUD_RATE_PRESCALER_16 0x4000
#define _SPI_BAUD_RATE_PRESCALER_32 0x5000
#define _SPI_BAUD_RATE_PRESCALER_64 0x6000
#define _SPI_BAUD_RATE_PRESCALER_128 0x7000
#define _SPI_BAUD_RATE_PRESCALER_256 0x8000
#define _SPI_MASK_BAUD_RATE 0xF000


#define _SPI_MASTER_MODE 0x00
#define _SPI_SLAVE_MODE  0x01


#define SCK_PIN  GPIO_Pin_3 
#define MISO_PIN GPIO_Pin_4
#define MOSI_PIN GPIO_Pin_5
#define SPI0_PORT GPIOB

/* Exported functions ------------------------------------------------------- */ 
void SPI0_Init(void);  //Default initialisation
void SPI0_Init_Advanced(unsigned long notUsed, unsigned int master_slave_mode, unsigned int config);
unsigned int SPI0_Read(uint16_t buffer); //Read value
void SPI0_Write(uint16_t data_out); //Write value
void SPI_Set_Active(unsigned int(*read_ptr)(uint16_t), void(*write_ptr)(uint16_t)); //set default read/write call (8/16bit)
unsigned int SPI_Read(uint16_t buffer); //SPI Read SPI0/1 8/16bit depending on SPI_Set_Active call (default is SPI0 8bit)
void SPI_Write(uint16_t data_out); //SPI Write SPI0/1 8/16bit depending on SPI_Set_Active call (default is SPI0 8bit)

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

