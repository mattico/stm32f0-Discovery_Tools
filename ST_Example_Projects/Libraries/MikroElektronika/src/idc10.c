/**
  ******************************************************************************
  * @file    MikroElektronika/idc10.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   IDC10 API for STM32F051 Discovery Kit
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
#include "idc10.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/

/**
  * @brief  IDC10_Output_Init initialize IDC10 port as ouptut port
  * @param  None
  * @retval None
  */
void IDC10_Output_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);  

  /*Set pins as output pins */
  GPIO_InitStructure.GPIO_Pin =  P0 | P1 | P2 | P3 | P4 | P5 | P6 | P7 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
 }

/**
  * @brief  IDC10_Input_Init initialize IDC10 port as input port
  * @param  None
  * @retval None
  */
void IDC10_Input_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);  

  /*Set pins as output pins */
  GPIO_InitStructure.GPIO_Pin =  P0 | P1 | P2 | P3 | P4 | P5 | P6 | P7 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
 }


/**
  * @brief  IDC10_Init Initialize IDC10 pin depending on given type (input/output)
  * @param  Pin number, pin type INPUT/OUTPUT
  * @retval None
  */
void IDC10_Init(uint32_t Px, uint16_t Type) {
 GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);  

  /*Set pins as output pins */
  GPIO_InitStructure.GPIO_Pin =  Px ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  
  if(Type == INPUT) {
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  }
  if(Type == OUTPUT) {
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  }
  
  GPIO_Init(GPIOC, &GPIO_InitStructure);    
}

/**
  * @brief  IDC10_Write Write a 8 bits value on IDC10 port
  * @param  8 bits val to write
  * @retval None
  */
void IDC10_Write(uint8_t Val) {
  uint16_t L_Value;
  L_Value = GPIO_ReadInputData(GPIOC);
  L_Value &= 0xFF00;
  L_Value |= (uint16_t)Val;
    
  GPIO_Write(GPIOC, L_Value);
}


/**
  * @brief  IDC10_Read Read 8bits value on IDC10 port 
  * @param  None
  * @retval 8bits value read
  */
uint8_t IDC10_Read(void) {
  uint16_t L_Value;
  L_Value = GPIO_ReadInputData(GPIOC);
  return((uint8_t)L_Value);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
