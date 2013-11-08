/**
  ******************************************************************************
  * @file    Arduino/spi.c 
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_conf.h"
#include "utilities.h"
#include "gpio.h"
#include "spi.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/*Arduino SPI communication stream*/
SPI_ArduinoTypeDef SPI = {
  0,
  0,
  begin_spi_fcn,
  read_spi_fcn,
  write_spi_fcn,
};


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GPIO_Configuration_SPI(void);
static void SPI_Arduino_Init(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SPI_Arduino_Init
  * @param  None
  * @retval None
  */
static void SPI_Arduino_Init(void){
	/*!<Configures and initializes the SPI0 module with default settings:
	Master mode. 
	8-bit data mode. 
	Serial clock set to 3 MHz. (ref clock = 48Mhz BaudRate prescaler = 16)
	Clock idle state low. 
	MSB bit first
	Serial output data changes on the first clock edge transition. 
	*/
	SPI_InitTypeDef  SPI_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    GPIO_Configuration_SPI();
    digitalWrite(CS, HIGH); //Unselect componant
    
    if(SPI.SpiOneLine == 0){
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    } else if(SPI.SpiOneLine == 1) {
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly;
    } else if(SPI.SpiOneLine == 2) {
      SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Rx;
    } else if(SPI.SpiOneLine == 3) {
      SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    } else {
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    }
    
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    if(SPI.Spi16Bit) {
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    } else {
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    }
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB; 
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
	
    if(SPI.Spi16Bit)
     SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_HF);
    else
     SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);          
	
    /* Disable SPI1 NSS output for master mode */
    SPI_SSOutputCmd(SPI1, DISABLE);
    /* Enable SPI1 */
    SPI_Cmd(SPI1, ENABLE);
}


/**
  * @brief  begin_spi_fcn default spi config
  * @param  None
  * @retval None
  */
void begin_spi_fcn() {
  pinSetMode(CS, OUTPUT_PP); //Config CS 
  SPI_Arduino_Init();
  pinSetMode(0, INPUT_AF); //pin 0 is Rx
  pinSetMode(1, OUTPUT_AF); //pin 1 is Tx
  pinSetMode(SCK, OUTPUT_AF); //SPI pins config as alternate functions
  pinSetMode(MOSI, OUTPUT_AF);
  pinSetMode(MISO, INPUT_AF); 
}

/**
  * @brief  read_spi_fcn
  * @param  buffer : Cmd to send
  * @retval rcv char/word
  */
unsigned int read_spi_fcn(uint16_t buffer){
  unsigned int L_result =0;
    if(SPI.Spi16Bit)
     SPI1->DR = buffer;
    else
      *(__IO uint8_t *) ((uint32_t)SPI1 + 0x0C) = (unsigned char)buffer; 
    while((SPI1->SR & 0x02) == 0);
    if(SPI.SpiOneLine) // If one line only for SPI
      SPI1->CR1 &= SPI_Direction_Rx;  // Configure line for receive
    else
      while((SPI1->SR & 0x01) == 0 || (SPI1->SR & 0x80) );
    L_result = SPI1->DR;
    if(SPI.SpiOneLine) // If one line only for SPI
      SPI1->CR1 |= SPI_Direction_Tx; // Configure line for transmit
    return(L_result);
}


/**
  * @brief  write_spi_fcn Send a char or word on SPI
  * @param char/word to send
  * @retval none
  */
void write_spi_fcn(uint16_t data_out){
    while((SPI1->SR & 0x02) == 0);
    if(SPI.SpiOneLine) // If one line only for SPI
      SPI1->CR1 |= SPI_Direction_Tx; // Configure line for transmit
    if(SPI.Spi16Bit)
     SPI1->DR = data_out;
    else
     *(__IO uint8_t *) ((uint32_t)SPI1 + 0x0C) = (unsigned char)data_out; 
}


/**
  * @brief  GPIO_Configuration_SPI spi GPIO pins configuration
  * @param none
  * @retval none
  */
static void GPIO_Configuration_SPI(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_PinTypeDef  L_PinSCKInfo,L_PinMOSIInfo,L_PinMISOInfo;
     
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  

  L_PinSCKInfo = getPinDef(SCK);
  GPIO_PinAFConfig(L_PinSCKInfo.GPIOx, L_PinSCKInfo.PinNum, GPIO_AF_0);
  L_PinMISOInfo = getPinDef(MISO);
  GPIO_PinAFConfig(L_PinMISOInfo.GPIOx, L_PinMISOInfo.PinNum, GPIO_AF_0);
  L_PinMOSIInfo = getPinDef(MOSI);
  GPIO_PinAFConfig(L_PinMOSIInfo.GPIOx, L_PinMOSIInfo.PinNum, GPIO_AF_0);
    
  /* Configure SPI pins:  SCK, MISO and MOSI ----------------------------*/
  GPIO_InitStructure.GPIO_Pin =  L_PinSCKInfo.PinNum | L_PinMISOInfo.PinNum | L_PinMOSIInfo.PinNum;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(L_PinMOSIInfo.GPIOx, &GPIO_InitStructure);  
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
