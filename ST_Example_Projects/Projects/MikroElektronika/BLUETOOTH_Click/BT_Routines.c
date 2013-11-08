/**
  ******************************************************************************
  * @file    BLUETOOTH_Click/BT_Routines.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   
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
//  Configure BlueTooth-Click module
#include "BT_Routines.h"
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

// responses to parse
extern const char BT_CMD;
extern const char BT_AOK;
extern const char BT_CONN;
extern const char BT_END;

/**
  * @brief  BT_Configure 
  * @param  None
  * @retval None
  */
void  BT_Configure(void) {
  do {
    UART2_Write_Text("$$$");                  // Enter command mode
    delay_ms(500);
  } while (BT_Get_Response() != BT_CMD);

  do {
    UART2_Write_Text("SN,BlueTooth-Click");   // Name of device
    UART2_Write(13);                          // CR
    delay_ms(500);
  } while (BT_Get_Response() != BT_AOK);

   do {
    UART2_Write_Text("SO,Slave");             // Extended status string
    UART2_Write(13);                          // CR
    delay_ms(500);
  } while (BT_Get_Response() != BT_AOK);

  do {
    UART2_Write_Text("SM,0");                 // Set mode (0 = slave, 1 = master, 2 = trigger, 3 = auto, 4 = DTR, 5 = ANY)
    UART2_Write(13);                          // CR
    delay_ms(500);
  } while (BT_Get_Response() != BT_AOK);

  do {
    UART2_Write_Text("SA,1");                 // Authentication (1 to enable, 0 to disable)
    UART2_Write(13);                          // CR
    delay_ms(500);
  } while (BT_Get_Response() != BT_AOK);

  do {
    UART2_Write_Text("SP,1234");              // Security pin code (mikroe)
    UART2_Write(13);                          // CR
    delay_ms(500);
  } while (BT_Get_Response() != BT_AOK);

  do {
    UART2_Write_Text("---");                  // Security pin code (mikroe)
    UART2_Write(13);                          // CR
    delay_ms(500);
  } while (BT_Get_Response() != BT_END);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
