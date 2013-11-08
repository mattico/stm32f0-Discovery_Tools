/**
  ******************************************************************************
  * @file    ADC_Click/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   ADC Click example for STM32F051 Discovery Kit
  *          Get ADC Click value for channel 0 to 3.
  * IDC10 COG2x16 LCD BOARD can be used to display result.
  * cf RTC example to used LCD driver API
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
  
/*************************************
Master send 2 bytes: 000001SDDD000000
1st byte is 1 for start bit, S to differentiate differential and single ended op.
D is 1 when channel address is more 3 to 7
2nd byte is DD000000 where DD is channel address.

Config is 16bit, MSB first. ADC Click is a 4 channels board.
Master sends : 00000110 DD000000  = 0x0600 for channel 0
                                  = 0x0640 for channel 1 etc...
Board sends :  xxxxxxxx xxxxvvvv  = hexa XXXV value where x or X is don't care bit/byte
                                    v or V is ADC channel value bits 11 to 8 

Master then sends : 00000000 00000000 Don't care about this value
                                      It is just to generate clock
Board sends : vvvvvvvv xxxxxxxx = hexa VVXX value where VV is ADC value bits 7 to 0

Don't forget to connect not used channel to GND to get 0 as answer.
*************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "stm32f0xx_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
uint16_t g_adcValue = 0x0000; //ADC value 0..4095
uint16_t g_channel = 0;


/**
  * @brief  getADC   Get ADC values
  * @param  Channel nb
  * @retval Adc Value
  */
uint16_t getADC(uint16_t channel) {    // Returns 0..4095
  uint16_t l_tmp, l_result;

  if (channel > 3) {
    channel = channel << 2; // build request to send 
    channel |= 0xE0;        // for channel 3 to 7 (not used for ADC click)
  } else {
    channel = channel << 2;
    channel |= 0x60;        // For channel 0 to 2
  }
  
  channel = channel << 8;   // Send 0x60 before 0xD0 
  
  CHIP1_SELECT_SELECT;            // Select MCP3204  
  l_tmp = SPI_Read(channel);      // Ask for value
  l_result = (l_tmp & 0x0F) << 8; // Get ADC value bits 11 ... 8
  l_tmp = SPI_Read(0);            // Ask for remaining value
  l_tmp = l_tmp >>8;              // First receive byte is remaining value
  
  l_result |= l_tmp;              // Compute result.

  
  CHIP1_SELECT_UNSELECT;              // Deselect MCP3204
  return (l_result & 0x0FFF);              // Returns 12-bit ADC value
}


/**
  * @brief  Main program 
  * @param  None
  * @retval None
  */
int main(void)
{
   unsigned int l_config = 0; //SPI configuration mask
    
    DISCOVERY_Configuration(); //STM32 discovery Board configuration (clock etc..)
    
    //SPI configuration. Port and functions
    l_config = _SPI_BAUD_RATE_PRESCALER_128 | _SPI_16_BIT; //other settings are default one
    SPI0_Init_Advanced(0,_SPI_MASTER_MODE,l_config);
    
    SPI_Set_Active(SPI0_Read, SPI0_Write); // SPI read/write function select (SPI0 / SPI21)

  
    while(1){
      /* Continuously ask (every 50ms) for channel 0 to 3 ADC click value */            
      for(g_channel = 0; g_channel <4 ; g_channel++) {
        g_adcValue = getADC(g_channel);
        delay_ms(50);               // Wait for 50ms
      }
      
    }
}



#ifdef  DEBUG
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
