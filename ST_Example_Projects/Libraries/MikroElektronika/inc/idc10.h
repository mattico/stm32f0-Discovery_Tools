/**
  ******************************************************************************
  * @file    MikroElektronika/idc10.h 
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
/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef __IDC10_H
#define __IDC10_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define P0             GPIO_Pin_0     
#define P1             GPIO_Pin_1     
#define P2             GPIO_Pin_2     
#define P3             GPIO_Pin_3     
#define P4             GPIO_Pin_4     
#define P5             GPIO_Pin_5     
#define P6             GPIO_Pin_6     
#define P7             GPIO_Pin_7     

#define INPUT    0x00
#define OUTPUT   0x01


#define  P0_HIGH   GPIO_HIGH(GPIOC, P0)
#define  P0_LOW    GPIO_LOW(GPIOC, P0)
#define  P0_TOGGLE GPIO_TOGGLE(GPIOC, P0)
#define  P1_HIGH   GPIO_HIGH(GPIOC, P1)
#define  P1_LOW    GPIO_LOW(GPIOC, P1)
#define  P1_TOGGLE GPIO_TOGGLE(GPIOC, P1)
#define  P2_HIGH   GPIO_HIGH(GPIOC, P2)
#define  P2_LOW    GPIO_LOW(GPIOC, P2)
#define  P2_TOGGLE GPIO_TOGGLE(GPIOC, P2)
#define  P3_HIGH   GPIO_HIGH(GPIOC, P3)
#define  P3_LOW    GPIO_LOW(GPIOC, P3)
#define  P3_TOGGLE GPIO_TOGGLE(GPIOC, P3)
#define  P4_HIGH   GPIO_HIGH(GPIOC, P4)
#define  P4_LOW    GPIO_LOW(GPIOC, P4)
#define  P4_TOGGLE GPIO_TOGGLE(GPIOC, P4)
#define  P5_HIGH   GPIO_HIGH(GPIOC, P5)
#define  P5_LOW    GPIO_LOW(GPIOC, P5)
#define  P5_TOGGLE GPIO_TOGGLE(GPIOC, P5)
#define  P6_HIGH   GPIO_HIGH(GPIOC, P6)
#define  P6_LOW    GPIO_LOW(GPIOC, P6)
#define  P6_TOGGLE GPIO_TOGGLE(GPIOC, P6)
#define  P7_HIGH   GPIO_HIGH(GPIOC, P7)
#define  P7_LOW    GPIO_LOW(GPIOC, P7)
#define  P7_TOGGLE GPIO_TOGGLE(GPIOC, P7)


/* Exported functions ------------------------------------------------------- */ 
void IDC10_Output_Init(void);
void IDC10_Input_Init(void);
void IDC10_Init(uint32_t Px, uint16_t Type);
void IDC10_Write(uint8_t Val);
uint8_t IDC10_Read(void);


#ifdef __cplusplus
}
#endif

#endif /* __IDC10_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


