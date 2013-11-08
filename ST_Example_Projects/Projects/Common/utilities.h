/**
  ******************************************************************************
  * @file    Common/utilities.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   This code has been developped for STM32F051 Discovery Kit.
*                       General purpose function prototypes and definitions.
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
#ifndef __UTILITIES_H
#define __UTILITIES_H

/* Includes ------------------------------------------------------------------*/
#include "stm32F0xx.h"
#include "core_cm0.h"

/* Exported constants --------------------------------------------------------*/
extern uint32_t G_tickValue;


/* Exported macros -----------------------------------------------------------*/
#define GPIO_HIGH(a,b) 		a->BSRR = b
#define GPIO_LOW(a,b)		a->BRR = b
#define GPIO_TOGGLE(a,b) 	a->ODR ^= b 
#define  TURNONLD3   GPIO_HIGH(GPIOC, GPIO_LED_LD3)
#define  TURNOFFLD3  GPIO_LOW(GPIOC, GPIO_LED_LD3)
#define  TOGGLE_LD3  GPIO_TOGGLE(GPIOC, GPIO_LED_LD3)
#define  TURNONLD4   GPIO_HIGH(GPIOC, GPIO_LED_LD4)
#define  TURNOFFLD4  GPIO_LOW(GPIOC, GPIO_LED_LD4)
#define  TOGGLE_LD4  GPIO_TOGGLE(GPIOC, GPIO_LED_LD4)
#define  CHIP1_SELECT_SELECT     GPIO_LOW(GPIOA, GPIO_Pin_11) 
#define  CHIP1_SELECT_UNSELECT   GPIO_HIGH(GPIOA, GPIO_Pin_11) 
#define  CHIP2_SELECT_SELECT     GPIO_LOW(GPIOA, GPIO_Pin_12) 
#define  CHIP2_SELECT_UNSELECT   GPIO_HIGH(GPIOA, GPIO_Pin_12) 
#define RESET_PIN_HIGH  GPIO_HIGH(RESET_PORT, RESET_PIN)
#define RESET_PIN_LOW   GPIO_LOW(RESET_PORT, RESET_PIN)


/* Exported constants --------------------------------------------------------*/
#define  GPIO_BUTTON_B1 GPIO_Pin_0
#define  LSE_FAIL_FLAG  0x80
#define  LSE_PASS_FLAG  0x100

#define  PLL_MUL_X  3
#define GPIO_LED_LD3 GPIO_Pin_9
#define GPIO_LED_LD4 GPIO_Pin_8


/*CS pin definition */
#define CS1_PIN  GPIO_Pin_11
#define CS2_PIN  GPIO_Pin_12
#define CS_PORT  GPIOA

/*RESET pin definition */
#define RESET_PIN   GPIO_Pin_13
#define RESET_PORT  GPIOB


/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define FLAG_TIMEOUT         ((uint32_t)0x1000)
#define LONG_TIMEOUT         ((uint32_t)(10 * FLAG_TIMEOUT))



/* exported typedef -------------------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* function prototypes -------------------------------------------------------*/
void delay_ms(uint32_t n_ms);
void delay_10us(uint32_t n_10us);
void DISCOVERY_Configuration(void);
void INT_EXTI_Config(void);
void INT1_EXTI_Config(void);
void INT2_EXTI_Config(void);
void PWM_Config(void);

extern void(*delay)(uint32_t); //Func pointer to delay_ms

#endif /* __UTILITIES_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


