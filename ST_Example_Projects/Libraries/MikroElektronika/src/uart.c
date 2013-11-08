/**
  ******************************************************************************
  * @file    MikroElektronika/uart.c
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_conf.h"
#include "utilities.h"
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GPIO_Configuration_USART2(void);
static void NVIC_Configuration(void);
static unsigned (*read_fcn)(void) = UART2_Read;
static void (*write_fcn)(unsigned char _data) = UART2_Write;
static unsigned (*ready_fcn)(void) = UART2_Data_Ready;
static char(*tx_idle_fcn)(void) = UART2_Tx_Idle;
static USART_TypeDef* USARTx = USART2;


/**
  * @brief  UART2_Init  Configures and initializes the UART module.
  * @param  Serial baud rate communication speed
  * @retval None
  */
void UART2_Init(unsigned long baud_rate){
  /*The internal UART module module is set to :
     default Tx and Rx pins. 
     115200 baud 
     8-bit data, no parity. 
     1 STOP bit. 
     */
  USART_InitTypeDef USART_InitStructure;

   
  GPIO_Configuration_USART2();
 
     
  USART_InitStructure.USART_BaudRate = baud_rate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
   
  
  NVIC_Configuration();
  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  USART_Cmd(USART2, ENABLE);
  
}


/**
  * @brief  UART2_Init_Advanced Configures and initializes the UART2 module with user defined settings.
  * @param  baud rate, data bit, parity bit etc..
  * @retval None
  */
void UART2_Init_Advanced(unsigned long baud_rate, unsigned long freq_Khz, unsigned int data_bits, unsigned int parity, unsigned int stop_bits, unsigned int high_low){
  USART_InitTypeDef USART_InitStructure;
  
  (void)freq_Khz;
  (void)high_low;
  
      
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  GPIO_Configuration_USART2();

  USART_InitStructure.USART_BaudRate = baud_rate;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  switch(data_bits) {
     case _UART_5_BIT_DATA :
     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
     break;
     case _UART_6_BIT_DATA :
     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
     break;
     case _UART_7_BIT_DATA : 
     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
     break;
    case _UART_8_BIT_DATA : 
    default :  
     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
     break;
  }

  
  switch(parity) {
    case _UART_NOPARITY : 
     USART_InitStructure.USART_Parity = USART_Parity_No;
     break;
    case _UART_EVENPARITY:
     USART_InitStructure.USART_Parity = USART_Parity_Even;
     break;
    case _UART_ODDPARITY  :
     USART_InitStructure.USART_Parity = USART_Parity_Odd;
     break;
    default:
     USART_InitStructure.USART_Parity = USART_Parity_No;
     break;
  }

  if(stop_bits == _UART_ONE_STOPBIT ) {
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
  } else {
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
  }


  USART_Init(USART2, &USART_InitStructure);
   
  
  NVIC_Configuration();
  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  USART_Cmd(USART2,ENABLE);

  
}


/**
  * @brief  UART2_Data_Ready is there a data to read on USART2?
  * @param  None
  * @retval 1 if data is ready for reading.   0 if there is no data in the receive register
  */
unsigned UART2_Data_Ready(void){
 return(USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
}


/**
  * @brief  UART_Data_Ready is there a data to read on selected USART?
  * @param  None
  * @retval 1  if data is ready for reading.   0 if there is no data in the receive register
  */
unsigned UART_Data_Ready(void){
 return(ready_fcn());
}


/**
  * @brief  UART2_Tx_Idle test if the transmit shift register is empty or not. Data sent on USART2?
  * @param  None
  * @retval 1 if the data has been transmitted.  0 otherwise. 
  */
char UART2_Tx_Idle(void ){
 return(USART_GetFlagStatus(USART2, USART_FLAG_TC));  
}


/**
  * @brief  UART_Tx_Idle test if the transmit shift register is empty or not.
  * @param  None
  * @retval  1 if the data has been transmitted.  0 otherwise. 
  */
char UART_Tx_Idle(void ){
 return(tx_idle_fcn());  
}


/**
  * @brief  UART2_Read read char fromp USART2
  * @param  None
  * @retval char read
  */
unsigned UART2_Read(void){
  return(getch());
}

/**
  * @brief  UART_Read Read char from selected usart
  * @param  None
  * @retval char read
  */
unsigned UART_Read(void){
  return(read_fcn());
}


/**
  * @brief  UART2_Read_Text Reads characters received via UART until the delimiter sequence is detected. The read sequence is stored in the parameter output; delimiter sequence is stored in the parameter delimiter.
  * This is a blocking call: the delimiter sequence is expected, otherwise the procedure exits (if the delimiter is not found).
  * NOT IMPLEMENTED
  * @param  Text, delimiter
  * @retval None
  */
void UART2_Read_Text(char *Output, char *Delimiter, char Attempts){
   char *l_out = Output;
   (void)l_out;
   /*NOT IMPLEMENTED*/
}


/**
  * @brief  UART2_Write Write a char on selected UART2
  * @param  None
  * @retval None
  */
void UART2_Write(unsigned char _data){
  putchar(_data);
}

/**
  * @brief  UART_Write Write a char on selected UART
  * @param  char to send
  * @retval None
  */
void UART_Write(unsigned char _data){
  write_fcn(_data);
}


/**
  * @brief  UART2_Write_Text Sends text via UART2. Text should be zero terminated.
  * @param  Text to send
  * @retval None
  */
void UART2_Write_Text(char * UART_text){
  char *l_i = UART_text;
  do{
    UART2_Write(*l_i++);
  } while(*l_i != 0);  
}


/**
  * @brief  UART_Set_Active Sets active UART module which will be used by UARTx_Data_Ready, UARTx_Read and UARTx_Write routines.
  * @param  Pointers to functions used
  * @retval None
  */
void UART_Set_Active(unsigned (*read_ptr)(), void (*write_ptr)(unsigned char _data), unsigned (*ready_ptr)(), char (*tx_idle_ptr)()){
   if(read_ptr == UART2_Read)
     USARTx = USART2;
   else
     USARTx = USART1;
     
   read_fcn = read_ptr;
   write_fcn = write_ptr;
   ready_fcn = ready_ptr;
   tx_idle_fcn = tx_idle_ptr;
}


/**
  * @brief  getch Read a chatacter from the usart
  * @param  None
  * @retval int  value of character read or -1 if no character
  */
int getch(void)
{
   return (USART_ReceiveData(USARTx));
}


/**
  * @brief  putchar Output a character to the usart
  * @param  int character to output
  * @retval int character to output
  */
int putchar(int chr)
{
   /* mask out any high bits */
   while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
      /* null */ ;
   USART_SendData(USARTx, (char)(chr & 0x7f));
 
   return chr;  
}


/**
  * @brief  GPIO_Configuration_USART2 Configures the different GPIO ports for UART.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration_USART2(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
  GPIO_PinAFConfig(UART_PORT, RX_PIN_SOURCE, GPIO_AF_1);
  GPIO_PinAFConfig(UART_PORT, TX_PIN_SOURCE, GPIO_AF_1);
   
  /* Configure UART1 pins:  Rx and Tx ----------------------------*/
  GPIO_InitStructure.GPIO_Pin =  TX_PIN | RX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_Init(UART_PORT, &GPIO_InitStructure); 
  
}


/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  
  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
