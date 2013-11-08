/**
  ******************************************************************************
  * @file    BLUETOOTH_Click/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler and 
*                      peripherals interrupt service routine.
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
#include "stm32f0xx_it.h"
#include "main.h"

// responses to parse
extern const char BT_CMD;
extern const char BT_AOK;
extern const char BT_CONN;
extern const char BT_END;

extern char g_txt[];
extern char g_CMD_mode;
extern char g_BT_state;
extern char g_response_rcvd;
extern char g_responseID, g_response;
extern uint16_t g_DataReady;


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
   if(G_tickValue)
     G_tickValue--;
}


/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
  static uint16_t l_i = 0; //Counter to fill buffer
  uint16_t l_tmp; //Received char from USART2
  
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    l_tmp  = USART_ReceiveData(USART2);

    if (g_CMD_mode){
    /* The responses expected from the EasyBT module:
    CMD
    AOK
    AOK
    AOK
    AOK
    AOK
    END
    SlaveCONNECTmikroE
    EasyBlueTooth
    mikroE ...
    EasyBlueTooth*/

    // Process reception through state machine
    // We are parsing CMD<cr><lf>, AOK<cr><lf>, CONN<cr> and END<cr><lf> responses
    switch (g_BT_state) {
      case  0: {
                g_response = 0;                   // Clear response
                if (l_tmp == 'C')                 // We have 'C', it could be CMD<cr><lf>  or CONN
                  g_BT_state = 1;                 // Expecting 'M' or 'N'
                if (l_tmp == 'A')                 // We have 'A', it could be AOK<cr><lf>
                  g_BT_state = 11;                // expecting 'O'
                if (l_tmp == 'E')                 // We have 'E', it could be END<cr><lf>
                  g_BT_state = 31;                // expecting 'N'
                break;                          // ...
      }

      case  1: {
                if (l_tmp == 'M')
                  g_BT_state = 2;
                else if (l_tmp == 'O')
                  g_BT_state = 22;
                else
                  g_BT_state = 0;
                break;
      }

      case  2: {
                if (l_tmp == 'D') {
                  g_response = BT_CMD;           // CMD
                  g_BT_state = 40;
                }
                else
                  g_BT_state = 0;
                break;
      }

      case 11: {
                if (l_tmp == 'O')
                  g_BT_state = 12;
                else
                  g_BT_state = 0;
                break;
      }

      case 12: {
                if (l_tmp == 'K'){
                  g_response = BT_AOK;            // AOK
                  g_BT_state = 40;
                }
                else
                  g_BT_state = 0;
                break;
      }

      case 22: {
                if (l_tmp == 'N')
                  g_BT_state = 23;
                else
                  g_BT_state = 0;
                break;
      }

      case 23: {
                if (l_tmp == 'N') {
                  g_response = BT_CONN;           // SlaveCONNECTmikroE
                  g_response_rcvd = 1;
                  g_responseID = g_response;
                }
                g_BT_state = 0;
                break;
      }

      case 31: {
                if (l_tmp == 'N')
                  g_BT_state = 32;
                else
                  g_BT_state = 0;
                break;
      }

      case 32: {
                if (l_tmp == 'D') {
                  g_response = BT_END;           // END
                  g_BT_state = 40;
                }
                else
                  g_BT_state = 0;
                break;
      }

      case 40: {
                if (l_tmp == 13)
                  g_BT_state = 41;
                else
                  g_BT_state = 0;
                break;
      }

      case 41: {
                if (l_tmp == 10){
                  g_response_rcvd = 1;
                  g_responseID = g_response;
                }
                g_BT_state = 0;
                break;
      }

      default: {
                g_BT_state = 0;
                break;
      }
    }
  }
  else {
    if (l_tmp == 13) {
      g_txt[l_i] = 0;      // Puting 0 at the end of the string
      g_DataReady = 1;     // Data is received
      l_i = 0;           // Next data will be next string      
    }
    else {
      g_txt[l_i] = l_tmp;  // Moving the data received from UART to string txt[]
      if(l_i < BT_MAX_RCV_SIZE)  // test for buffer overflow!
       l_i++;               // Increment counter      
    }
  }    
 }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
