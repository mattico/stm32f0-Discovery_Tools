/**
  ******************************************************************************
  * @file    EasyLED/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief  EasyLED example for STM32F051 Discovery Kit
  * IDC10 connector         
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
#include "stm32f0xx_conf.h"
#include "utilities.h"
#include "idc10.h"


/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/

/**
  * @brief   Main program 
  * @param  None
  * @retval None
  */
int main(void){
  uint8_t i;
  
  DISCOVERY_Configuration(); //STM32 discovery Board configuration (clock etc..)

  IDC10_Output_Init();
  
  while(1){            // Endless loop 
   P0_HIGH;
   P1_LOW;
   P2_LOW;
   P3_LOW;
   P4_LOW;
   P5_LOW;
   P6_LOW;
   P7_LOW;
   delay_ms(2500);
   P1_HIGH;
   P0_LOW;
   delay_ms(350);     
   P2_HIGH;
   P1_LOW;
   delay_ms(350);     
   P3_HIGH;
   P2_LOW;
   delay_ms(350);     
   P4_HIGH;
   P3_LOW;
   delay_ms(350);     
   P5_HIGH;
   P4_LOW;
   delay_ms(350);     
   P6_HIGH;
   P5_LOW;
   delay_ms(350);     
   P7_HIGH;
   P6_LOW;
   delay_ms(2500);     
   
   for(i=0;i < 0xFF;i++){
     delay_ms(350);     
     IDC10_Write(i);
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
