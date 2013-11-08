/**
  ******************************************************************************
  * @file    BLUETOOTH_Click/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   BLUETOOTH example for STM32F051 Discovery Kit
  * IDC10 COG2x16 LCD BOARD can be used instead of UART2 to display result.
  * cf RTC example to used LCD driver API
  * Bluetooth connection key is 1234
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
#include "uart.h"
#include "stm32f0xx_conf.h"
#include "BT_Routines.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
// responses to parse
const char BT_CMD  = 1;
const char BT_AOK  = 2;
const char BT_CONN = 3;
const char BT_END  = 4;

char g_txt[BT_MAX_RCV_SIZE];
char g_CMD_mode;

char g_BT_state;
char g_response_rcvd;
char g_responseID, g_response = 0;

unsigned short g_DataReady;
unsigned short i;


/**
  * @brief  BT_Get_Response Get BlueTooth response, if there is any
  * @param  None
  * @retval 0 no answer
  */
char BT_Get_Response() {
  if (g_response_rcvd) {
    g_response_rcvd = 0;
    return g_responseID;
  }
  else
    return 0;
}

/**
  * @brief  Main program 
  * @param  None
  * @retval None
  */
int main(void)
{
  // Initialize variables
  g_CMD_mode = 1;
  g_BT_state = 0;
  g_response_rcvd = 0;
  g_responseID = 0;
  g_response = 0;
  g_DataReady = 0;

  
  DISCOVERY_Configuration(); //STM32 discovery Board configuration (clock etc..)
    
  UART2_Init(115200); //USART config
   
  RESET_PIN_LOW; //Reset BT module
  delay_ms(200);
  RESET_PIN_HIGH;
  
  // Configure BlueTooth-Click module
  BT_Configure();

  //  Wait until connected
  while (BT_Get_Response() != BT_CONN);

  g_CMD_mode = 0;  
  g_DataReady = 0;                // Data not received

  UART2_Write_Text("Bluetooth Click Connected!");         //  Send message on connection
  UART2_Write(13);              // CR
  
  while (1) {
    while (!g_DataReady)          // Wait while the data is received
      ;
    g_DataReady = 0;              // Data not received
    
    // !!!!! PROCESS - PROCESS - PROCESS - PROCESS  !!!!!
    // !!!!! Here received data are in g_txt buffer.!!!!!

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
