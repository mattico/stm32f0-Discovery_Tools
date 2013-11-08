/**
  ******************************************************************************
  * @file    MikroElektronika/uart.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   UART mikroBUS API for STM32F051 Discovery Kit
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
#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
 extern "C" {
#endif 


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define TX_PIN  GPIO_Pin_2 
#define RX_PIN  GPIO_Pin_3
#define TX_PIN_SOURCE   GPIO_PinSource2
#define RX_PIN_SOURCE   GPIO_PinSource3
#define UART_PORT GPIOA

#define _UART_5_BIT_DATA 0x00
#define _UART_6_BIT_DATA 0x01
#define _UART_7_BIT_DATA 0x02
#define _UART_8_BIT_DATA 0x04

#define _UART_NOPARITY    0x08
#define _UART_EVENPARITY  0x10
#define _UART_ODDPARITY   0x20
#define _UART_PARITY_ONE  0x40
#define _UART_PARITY_ZERO 0x80

#define _UART_ONE_STOPBIT  0x01
#define _UART_TWO_STOPBITS 0x02

#define _UART_LOW_SPEED   0x00
#define _UART_HI_SPEED    0x01


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UART2_Init(unsigned long baud_rate);
void UART2_Init_Advanced(unsigned long baud_rate, unsigned long freq_Khz, unsigned int data_bits, unsigned int parity, unsigned int stop_bits, unsigned int high_low);
unsigned UART2_Data_Ready(void);
char UART2_Tx_Idle(void);
unsigned UART2_Read(void);
void UART2_Read_Text(char *Output, char *Delimiter, char Attempts);
void UART2_Write(unsigned char _data);
void UART2_Write_Text(char * UART_text);
void UART_Set_Active(unsigned (*read_ptr)(void), void (*write_ptr)(unsigned char _data), unsigned (*ready_ptr)(void), char(*tx_idle_ptr)(void));
int getch(void);
int putchar(int chr);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


