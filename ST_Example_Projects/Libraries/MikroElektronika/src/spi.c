/**
  ******************************************************************************
  * @file    MikroElektronika/spi.c 
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_conf.h"
#include "utilities.h"
#include "spi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GPIO_Configuration_SPI1(void);
static unsigned int (*read_fcn)(uint16_t) = SPI0_Read ;
static void (*write_fcn)(uint16_t) = SPI0_Write ;
static uint16_t G_SpiOneLine = 0 ; //Defult SPI config is FullDuplex (2Lines)
static uint16_t G_Spi16Bit = 0 ; //Defult SPI config is 8 bit

/**
  * @brief  SPI0_Init
  * @param  None
  * @retval None
  */
void SPI0_Init(){
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

	GPIO_Configuration_SPI1();
        CHIP1_SELECT_UNSELECT;
        
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        G_SpiOneLine = 0;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
        G_Spi16Bit = 1 ;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	
        if(G_Spi16Bit)
         SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_HF);
        else
         SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);          

	
        /* Disable SPI1 NSS output for master mode */
        SPI_SSOutputCmd(SPI1, DISABLE);

	
	/* Enable SPI1 */
    SPI_Cmd(SPI1, ENABLE);
}


/**
  * @brief  SPI0_Init_Advanced  Configures and initializes the SPI module with user defined settings.
  * Default config for 0 value as argument is: FullDuplex, Master, 8b, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_NSS_Soft or disabled for master, BaudRate Prescaler 16
  *  FirstBitMSB and CRC polynomial fixed to 7 (not tunable)
  * @param  bit rate Not USED (to be compatible with Mikroelektronika function call), SPI mode master or slave, config bits
  * @retval None
  */
void SPI0_Init_Advanced(unsigned long bit_rate_Hz, unsigned int master_slave_mode, unsigned int config)
{
    SPI_InitTypeDef  SPI_InitStructure;
    uint16_t L_configMask;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 
        GPIO_Configuration_SPI1();

	switch(master_slave_mode) {
		case _SPI_SLAVE_MODE :
			SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
		break;
		default :
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		break;
	}
        
    L_configMask = config & _SPI_MASK_DIRECTION;
	if(L_configMask == _SPI_2L_FULL_DUPLEX) {
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
          G_SpiOneLine = 0;
        } else if(L_configMask == _SPI_2L_RX){
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly;
          G_SpiOneLine = 0;
        } else if(L_configMask == _SPI_1L_RX){
	  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Rx;
          G_SpiOneLine = 1;
        } else if(L_configMask == _SPI_1L_TX){
	  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
          G_SpiOneLine = 1;
        } else {
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
          G_SpiOneLine = 0;
        }
       
    L_configMask = config & _SPI_MASK_BIT;        
	if(L_configMask == _SPI_4_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_4b;
                G_Spi16Bit = 0 ;
        } else if(L_configMask == _SPI_5_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_5b;
                G_Spi16Bit = 0 ;
        } else if(L_configMask == _SPI_6_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_6b;
                G_Spi16Bit = 0 ;
        } else if(L_configMask == _SPI_7_BIT) {
  		SPI_InitStructure.SPI_DataSize = SPI_DataSize_7b;
                G_Spi16Bit = 0 ;
        } else if(L_configMask == _SPI_8_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
                G_Spi16Bit = 0 ;
        } else if(L_configMask == _SPI_9_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_9b;
                G_Spi16Bit = 1 ;
        } else if(L_configMask == _SPI_10_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_10b;
                G_Spi16Bit = 1 ;
        } else if(L_configMask == _SPI_11_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_11b;
                G_Spi16Bit = 1 ;
        } else if(L_configMask == _SPI_12_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_12b;
                G_Spi16Bit = 1 ;
        } else if(L_configMask == _SPI_13_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_13b;
                G_Spi16Bit = 1 ;
        } else if(L_configMask == _SPI_14_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_14b;
                G_Spi16Bit = 1 ;
        } else if(L_configMask == _SPI_15_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_15b;
                G_Spi16Bit = 1 ;
        } else if(L_configMask == _SPI_16_BIT) {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
                G_Spi16Bit = 1 ;
	} else {
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
                G_Spi16Bit = 0 ;
	}
   
        
    L_configMask = config & _SPI_MASK_CLK_IDLE;
	if(L_configMask == _SPI_CLK_IDLE_LOW) {
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        } else if(L_configMask == _SPI_CLK_IDLE_HIGH) {
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
 	} else {
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
 	}


    L_configMask = config & _SPI_MASK_CLK_EDGE;
	if(L_configMask == _SPI_FIRST_CLK_EDGE) {
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
        } else if(L_configMask == _SPI_SECOND_CLK_EDGE) {
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
 	} else {
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
 	}

    L_configMask = config & _SPI_MASK_CLK_FIRST_BIT;
	if(L_configMask == _SPI_FIRST_BIT_LSB) {
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
        } else if(L_configMask == _SPI_FIRST_BIT_MSB) {
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
 	} else {
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
 	}
	       
        
    L_configMask = config & _SPI_MASK_NSS;
	if(L_configMask == _SPI_NSS_HARD) {
        	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
        } else if(L_configMask == _SPI_NSS_SOFT) {
        	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
 	} else {
        	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
 	}
                
        
    L_configMask = config & _SPI_MASK_BAUD_RATE;
	if(L_configMask == _SPI_BAUD_RATE_PRESCALER_2) {
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        } else if(L_configMask == _SPI_BAUD_RATE_PRESCALER_4) {
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
        } else if(L_configMask == _SPI_BAUD_RATE_PRESCALER_8) {
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        } else if(L_configMask == _SPI_BAUD_RATE_PRESCALER_16) {
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
        } else if(L_configMask == _SPI_BAUD_RATE_PRESCALER_32) {
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
        } else if(L_configMask == _SPI_BAUD_RATE_PRESCALER_64) {
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        } else if(L_configMask == _SPI_BAUD_RATE_PRESCALER_128) {
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
        } else if(L_configMask == _SPI_BAUD_RATE_PRESCALER_256) {
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	} else {
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
 	}
   
        
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	
        if(G_Spi16Bit)
         SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_HF);
        else
         SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);          

	 
        /* Disable SPI1 NSS output for master mode */
        if(SPI_InitStructure.SPI_Mode == SPI_Mode_Master)
         SPI_SSOutputCmd(SPI1, DISABLE);

	/* Enable SPI1 */
        SPI_Cmd(SPI1, ENABLE);	
}


/**
  * @brief  SPI0_Read  Reads one word from the SPI bus.
  * @param  SPI cmd value
  * @retval SPI read value
  */
unsigned int SPI0_Read(uint16_t buffer){
  unsigned int L_result =0;
    if(G_Spi16Bit)
     SPI1->DR = buffer;
    else
      *(__IO uint8_t *) ((uint32_t)SPI1 + 0x0C) = (unsigned char)buffer; 
    while((SPI1->SR & 0x02) == 0);
    if(G_SpiOneLine) // If one line only for SPI
      SPI1->CR1 &= SPI_Direction_Rx;  // Configure line for receive
    else
      while((SPI1->SR & 0x01) == 0 || (SPI1->SR & 0x80) );
    L_result = SPI1->DR;
    if(G_SpiOneLine) // If one line only for SPI
      SPI1->CR1 |= SPI_Direction_Tx; // Configure line for transmit
    return(L_result);
}


/**
  * @brief  SPI0_Write Writes one word via the SPI bus.
  * @param  data to write
  * @retval None
  */
void SPI0_Write(uint16_t data_out){
    while((SPI1->SR & 0x02) == 0);
    if(G_SpiOneLine) // If one line only for SPI
      SPI1->CR1 |= SPI_Direction_Tx; // Configure line for transmit
    if(G_Spi16Bit)
     SPI1->DR = data_out;
    else
     *(__IO uint8_t *) ((uint32_t)SPI1 + 0x0C) = (unsigned char)data_out; 
}


/**
  * @brief  SPI_Set_Active Sets the active SPI module which will be used by the SPIx_Read and SPIx_Write routines.
  * @param  functions for read and write
  * @retval None
  */
void SPI_Set_Active(unsigned int(*read_ptr)(uint16_t), void(*write_ptr)(uint16_t)){
	read_fcn = read_ptr;
	write_fcn = write_ptr;
}


/**
  * @brief  SPI_Read Reads one word or byte from the SPI bus.
  * This is a generic routine which uses the active SPI module previously activated by the SPI_Set_Active routine.
  * @param  SPI cmd value
  * @retval SPI read value
  */
unsigned int SPI_Read(uint16_t buffer){
  return(read_fcn(buffer));
}


/**
  * @brief  SPI_Write Writes one word or byte via the SPI bus.
  * This is a generic routine which uses the active SPI module previously activated by the SPI_Set_Active routine.
  * @param  data to write on bus
  * @retval None
  */
void SPI_Write(uint16_t data_out){
  write_fcn(data_out);
}


/**
  * @brief  GPIO_Configuration_SPI1 Configures the different GPIO ports for SPI1.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration_SPI1(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  

  GPIO_PinAFConfig(SPI0_PORT, SCK_PIN, GPIO_AF_0);
  GPIO_PinAFConfig(SPI0_PORT, MISO_PIN, GPIO_AF_0);
  GPIO_PinAFConfig(SPI0_PORT, MOSI_PIN, GPIO_AF_0);
    
  /* Configure SPI1 pins:  SCK, MISO and MOSI ----------------------------*/
  GPIO_InitStructure.GPIO_Pin =  SCK_PIN | MISO_PIN | MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(SPI0_PORT, &GPIO_InitStructure);

  /*Set NSS pin as input pin */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SPI0_PORT, &GPIO_InitStructure);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



