/**
  ******************************************************************************
  * @file    BUZZ_Click/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief  BUZZ click example for STM32F051 Discovery Kit
  *          Play with Buzzer
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

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define SCALE_LENGTH 12    // define scale length
#define SCALE_COUNT 3      // define number os scales
// note intervals in miliseconds
#define FULL_NOTE 1200
#define HALF_DOT_NOTE 900
#define HALF_NOTE 600
#define QUARTER_DOT_NOTE 450
#define QUARTER_NOTE 300
#define EIGHTH_DOT_NOTE 225
#define EIGHTH_NOTE 150
#define SIXTINTH_DOT_NOTE 112
#define SIXTINTH_NOTE 75

/* Global variables ----------------------------------------------------------*/
// Scale and note definitions
const unsigned int Scales[3][12] = {
 {523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988},
 {1046, 1108, 1174, 1244, 1318, 1397, 1480,1568, 1662,1760, 1865, 1975},
 {2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951}};


/**
  * @brief   Sound_Play  
  * @param  Note to play, delay for note duration
  * @retval None
  */
void Sound_Play(uint16_t note,  uint16_t delay){
  TIM1->ARR = (SystemCoreClock / note ) - 1;
  delay_ms(delay);
}


/**
  * @brief   Ode_to_Joy  The first several notes of famous Beethoven's tune
  * @param  None
  * @retval None
  */
void Ode_to_Joy(void){
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  Sound_Play(Scales[0][7],  QUARTER_NOTE);       // G
  Sound_Play(Scales[0][7],  QUARTER_NOTE);       // G
  Sound_Play(Scales[0][8],  QUARTER_NOTE);       // A
  Sound_Play(Scales[0][10], QUARTER_NOTE);       // B
  Sound_Play(Scales[0][10], QUARTER_NOTE);       // B
  Sound_Play(Scales[0][8],  QUARTER_NOTE);       // A
  Sound_Play(Scales[0][7],  QUARTER_NOTE);       // G
  Sound_Play(Scales[0][5],  QUARTER_NOTE);       // F
  Sound_Play(Scales[0][3],  QUARTER_NOTE);       // D#
  Sound_Play(Scales[0][3],  QUARTER_NOTE);       // E
  Sound_Play(Scales[0][5],  QUARTER_NOTE);       // F
  Sound_Play(Scales[0][7],  QUARTER_NOTE);       // G
  Sound_Play(Scales[0][7],  QUARTER_DOT_NOTE);   // G.
  Sound_Play(Scales[0][5],  EIGHTH_NOTE);        // F
  Sound_Play(Scales[0][5],  QUARTER_DOT_NOTE);   // F.
  TIM_CtrlPWMOutputs(TIM1, DISABLE);  
}



/**
  * @brief   Test_scales  Play musical scale
  * @param  None
  * @retval None
  */
void Test_scales(void){
  unsigned short note = 0, scale = 0;

  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  for (scale = 0; scale < SCALE_COUNT; scale++)
    for (note = 0; note < SCALE_LENGTH; note++)
      Sound_Play(Scales[scale][note], 200);

  TIM_CtrlPWMOutputs(TIM1, DISABLE);
}


/**
  * @brief   Main program 
  * @param  None
  * @retval None
  */
int main(void){
  
  DISCOVERY_Configuration(); //STM32 discovery Board configuration (clock etc..)

  PWM_Config();
  /* TIM1 Main Output Disable */
  TIM_CtrlPWMOutputs(TIM1, DISABLE);

 
  Test_scales();                  // Play "Test Scales"

  delay_ms(50);
  
  Ode_to_Joy();                   // Play "Ode To Joy"
 
  
  while(1){            // Endless loop
   delay_ms(50);
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
