/**
  ******************************************************************************
  * @file    Arduino/serial.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Arduino serial communication API for STM32F051 Discovery Kit
  *       Interrupt receive function is USART2_IRQHandler    
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
#include "serial.h"
#include "gpio.h"

/* Private typedef -----------------------------------------------------------*/



/* Private define ------------------------------------------------------------*/
/*Arduino Serial communication stream*/
Serial_TypeDef Serial = {
  0,
  begin_fcn,
  available_fcn,
  flush_fcn,
  read_fcn,
  peek_fcn,
  write_fcn,
  print_fcn,
  println_fcn,
};

//Reception definitions
static char rcv_buff[BUFFER_RCV_SIZE]; //Reception buffer
static char* pt_rcv_read = rcv_buff;   //Circular read ptr
static char* pt_rcv_write = rcv_buff;  //Circular write ptr
static volatile uint16_t nb_rcv_buff_char = 0;  //Nb of char to read in rcv_buff
static uint16_t rcv_buff_overload=0;   //Nb of char refused cause rcv_buff is full (overload)
static char rcv_buff_full=0;           //rcv_buff is full. pt_rcv_write


/* Private function prototypes -----------------------------------------------*/
static void GPIO_Configuration_USART2(void);
static void NVIC_Configuration(void);
static void UART2_Init(uint32_t baud_rate);
static USART_TypeDef* USARTx = USART2;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  UART2_Init  Configures and initializes the UART module.
  * @param  Serial baud rate communication speed
  * @retval None
  */
static void UART2_Init(uint32_t baud_rate){
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
  * @brief  putchar Output a character to the usart
  * @param  int character to output
  * @retval None
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
  * @brief  begin_fcn Configure Arduino communication port
  * @param  baudRate = usart speed
  * @retval None
  */
void begin_fcn(uint32_t baudRate) {
  UART2_Init(baudRate);
  Serial.baudRate = baudRate;
  pinSetMode(0, INPUT_AF); //pin 0 is Rx
  pinSetMode(1, OUTPUT_AF); //pin 1 is Tx
}

/**
  * @brief  available_fcn Return the nb of received char
  * @param  None
  * @retval Nb of char to process from rcv_buffer 
  */
uint16_t available_fcn(void){
  return(nb_rcv_buff_char);
}


/**
  * @brief  flush_fcn Reset all rcv_buff parameters
  * @param  None
  * @retval None
  */
void flush_fcn(void){
  pt_rcv_read = rcv_buff;   //reset circular read ptr
  pt_rcv_write = rcv_buff;  //reset circular write ptr
  nb_rcv_buff_char = 0; //reset Nb of char to read in rcv_buff
  rcv_buff_overload=0;  //reset Nb of char refused cause rcv_buff is full (overload)
  rcv_buff_full=0;         //reset rcv_buff is full.   
}


/**
  * @brief  read_fcn Return the first not read received char 
  * @param  None
  * @retval rcv char
  */
char read_fcn(void){
  char l_readChar = (char)-1; //Init with no char to read result
  if(nb_rcv_buff_char){
    //If there is char to read
    l_readChar = *pt_rcv_read++;
    nb_rcv_buff_char--;
    rcv_buff_full = 0; //At least 1 char space free in rcv_buff
    if(pt_rcv_read > &rcv_buff[BUFFER_RCV_SIZE]) {
      //pointer rollover rcv_buffer
      pt_rcv_read = rcv_buff;
    }    
  }
  return(l_readChar);
}


/**
  * @brief  peek_fcn Return the first char from buff but doesn't move read ptr
  * @param  None
  * @retval 1st rcv char
  */
char peek_fcn(void){
  char l_readChar = (char)-1; //Init with no char in rcv buff
  if(nb_rcv_buff_char){
    //If there is char to peek
    l_readChar = *pt_rcv_read;
  }
  return(l_readChar);
}


/**
  * @brief  write_fcn Send a char on usart
  * @param  char to send
  * @retval None
  */
void write_fcn(unsigned char byte){
  putchar(byte);
}

/**
  * @brief  print_fcn Send a string on usart
  * @param  string to send
  * @retval None
  */
void print_fcn(char *string){
  char *l_i = string;
  do{
    putchar(*l_i++);
  } while(*l_i != 0);  
}

/**
  * @brief  println_fcn Send a string on usart with carriage return
  * @param  string to send
  * @retval None
  */
void println_fcn(char *string){
  print_fcn(string);
  write_fcn('\r');
  write_fcn('\n');  
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
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  
  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
  uint16_t l_tmp; //Received char from USART2
  
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    //Read one byte from the receive data register
    l_tmp  = USART_ReceiveData(USART2);
    if(rcv_buff_full){
      //rcv buffer is full ==> trash rcv char
      rcv_buff_overload++;
    } else {
      *pt_rcv_write++ = (char)l_tmp; //put rcv char in buffer
      nb_rcv_buff_char++; //One char more to be read in buffer
      if(pt_rcv_write > &rcv_buff[BUFFER_RCV_SIZE]) {
        //pointer rollover rcv_buffer
        pt_rcv_write = rcv_buff;
      }
      if(pt_rcv_write == pt_rcv_read) {
        //Test if rcv buffer is full
        rcv_buff_full = 1;
      }      
    }
  }
  USART_ClearITPendingBit(USART2, USART_IT_ORE);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
