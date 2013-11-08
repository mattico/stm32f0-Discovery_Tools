/**
  ******************************************************************************
  * @file    DIGIPOT_Click/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   DIGIPOT example for STM32F051 Discovery Kit
  *          DIGIPOT value is continuously rooling from Min value to Max value.
  * IDC10 COG2x16 LCD BOARD can be used to display information.
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
/* Includes ------------------------------------------------------------------*/   
#include "main.h"
#include "spi.h"
#include "stm32f0xx_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
uint16_t g_potValue = 0x0000;

/**
  * @brief   Main program 
  * @param  None
  * @retval None
  */
int main(void)
{
unsigned int l_config = 0; //SPI configuration mask
unsigned int l_status = 0; //staus of SPI request
unsigned char l_counter = 0x00; //From 0 to 0xFF to set DIGIPOT from 0 to max

    
    DISCOVERY_Configuration(); //STM32 discovery Board configuration (clock etc..)
    
    //SPI configuration. Port and functions
    l_config = _SPI_16_BIT | _SPI_1L_TX | _SPI_BAUD_RATE_PRESCALER_128; //other settings are default one
    SPI0_Init_Advanced(0,_SPI_MASTER_MODE,l_config);
    
    SPI_Set_Active(SPI0_Read, SPI0_Write); // SPI read/write 16bit access
  
 
    CHIP1_SELECT_SELECT;          // Select PotentioMeter chip  
    SPI_Write(0x400F);            // Set TCON reg for Wipper0 config
    CHIP1_SELECT_UNSELECT;        // Deselect PotentioMeter chip
 
   
    while(1){
      CHIP1_SELECT_SELECT;        // Select PotentioMeter chip              
      do{
       l_status = SPI_Read(0x5C00);  // Wait for end of EEPROM access
      } while((l_status & 0x10) == 0x10);


      g_potValue = 0x0000;        // DIGIPOT value on RAM Wiper0
      g_potValue |= l_counter;
      SPI_Write(g_potValue);      // Write DIGITPOT value and get status
      delay_ms(50);               // Wait for 50ms
   
      g_potValue = 0x2000;        // DIGIPOT value on EEPROM Wiper0
      g_potValue |= l_counter;
      SPI_Write(g_potValue);
      CHIP1_SELECT_UNSELECT;      // Deselect PotentioMeter chip
    
   
      l_counter++;                // Inc DIGIPOT value
      delay_ms(50);               // Wait for 50ms
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
