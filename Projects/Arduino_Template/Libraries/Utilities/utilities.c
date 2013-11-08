/**
  ******************************************************************************
  * @file    Common/utilities.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   This code has been developped for STM32F051 Discovery Kit.
*                      Functions for general purpose.
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
#include "utilities.h"
#include "stm32f0xx_conf.h"


/* Private define ------------------------------------------------------------*/
#define  MS_DELAY_HSI (1000)
#define  SECOND_DELAY (1000*MS_DELAY_HSI)

// LEDs definition: LD3~LD4
#define  GPIO_LED_LD3   GPIO_Pin_9
#define  GPIO_LED_LD4   GPIO_Pin_8


/* Private consts ------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
uint32_t G_tickValue=0;
void (*delay)(uint32_t) = delay_ms; //Delay function


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void Init_LED(void);
static void Init_CS(void);
static void Init_RESET(void);
static void staticDelay(uint32_t);

/**
  * @brief  DISCOVERY_Configuration This function configure discovery kit clock and default interrupt 
  * @param  None
  * @retval None
  */
void DISCOVERY_Configuration(void){

  uint32_t LSE_Delay;
 
  #ifdef DEBUG
    debug();
  #endif

  
  // Initialize variables
  LSE_Delay = 0;
 
  
  // Init LEDs LD3&LD4, and user button
  Init_LED();

  //Chip Select init
  Init_CS();

  //Reset pin init
  Init_RESET();

  // Wait till HSE is ready - HSE_RDY @ bit 17
  while((RCC->CR & (1L<<17)) == 0)
  {
    if((RCC->CR & (1<<17)) == 0)
      staticDelay(1000);
    if((RCC->CR & (1<<17)) == 0)
      staticDelay(SECOND_DELAY);
    if((RCC->CR & (1<<17)) == 0)
      // turn on blue LED LD4
      TURNONLD4;
    if((RCC->CR & (1<<17)) == 0)
      staticDelay(SECOND_DELAY);
    if((RCC->CR & (1<<17)) == 0)
      // turn off blue LED LD4
      TURNOFFLD4;
  }
  
  TURNOFFLD4;
    // HSE passes
  
  // Flash no wait state
  *(uint32_t *)0x40022000 = 0x00;
  
  // for SILICON_CHIP configuration
  // PLLCLK = 8MHz * 3 = 24 MHz
  RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_2);
  
  // Enable PLL
  RCC_PLLCmd(ENABLE);
  
  
  // Wait till PLL is ready - PLL_RDY @ bit 25
  while((RCC->CR & (1L<<25)) == 0)
  {
    if((RCC->CR & (1<<25)) == 0)
      staticDelay(1000);
    if(((RCC->CR) & (1L<<25)) == 0)
      staticDelay(SECOND_DELAY);
    if((RCC->CR & (1<<25)) == 0)
      // turn on blue LED LD4
      TURNONLD4;
    if((RCC->CR & (1<<25)) == 0)
      staticDelay(SECOND_DELAY);
    if((RCC->CR & (1<<25)) == 0)
      // turn off blue LED LD4
      TURNOFFLD4;
  }

  // Select PLL as system clock source
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  // Wait till PLL is used as system clock source
  while(RCC_GetSYSCLKSource() != 0x08)
    ;

  // Enable PWR clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  // Enable access to the backup register, so LSE can be enabled
  PWR_BackupAccessCmd(ENABLE);
  

#ifdef TEST_LSE
  // Enable LSE (Low Speed External Oscillation)
  RCC_LSEConfig(RCC_LSE_ON);

  // first loop, wait for press of B1 button
  while(1)
  {
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
      delay_ms(50);
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
      {
        KeyState = 1; // pressed
        // Turn on Blue LED during 1s each time B1 is pressed
        TURNONLD4;
        delay_ms(1000);
        TURNOFFLD4;

        break;
      }
    }
    
    TURNONLD3;
    
    if(LSE_Delay & LSE_FAIL_FLAG)
      TURNONLD4;
    // delay_ms(1000);
    // delay 1s in below loop
    for(i=0; i<100; i++)
    {
      delay_ms(10);
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        if(KeyState == 0)
          break;
    }
    
    if(i<100)
      continue;
    
    if(LSE_Delay < LSE_FAIL_FLAG)
      LSE_Delay += 10;
    TURNOFFLD3;
    TURNOFFLD4;
//    delay_ms(1000);
    // delay 1s in below loop
    
    for(i=0; i<100; i++)
    {
      delay_ms(10);
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        if(KeyState == 0)
          break;
    }
    
    if(i<100)
      continue;
    
    if(LSE_Delay < LSE_FAIL_FLAG)
      LSE_Delay += 10;
    
    // LSE_FAIL_FLAG = 0x80
    if(LSE_Delay >= LSE_FAIL_FLAG)
      ;
    else if(LSE_Delay >= 60)
    {
      // check whether LSE is ready, with 6 seconds timeout
      if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        // set flag: LSE FAIL
        LSE_Delay |= LSE_FAIL_FLAG;
      else
        // set flag: LSE PASS
        LSE_Delay |= LSE_PASS_FLAG;
      // Disable LSE
      RCC_LSEConfig(RCC_LSE_OFF);
    }
  }
#else
  LSE_Delay |= LSE_PASS_FLAG;  
#endif
  //Update SystemCoreClock depending on RCC config
  SystemCoreClockUpdate ();
}


/**
  * @brief  delay_ms  delay for some time in ms unit(accurate)
  * @param  n_ms is how many ms of time to delay
  * @retval None
  */
void delay_ms(uint32_t n_ms)
{
  // SysTick interrupt each 1000 Hz with HCLK equal to 32MHz
  // - 30 to compensate the overhead of this sub routine
  SysTick_Config(8000*PLL_MUL_X - 30);
  // Enable the SysTick Counter

  G_tickValue = n_ms;
  while(G_tickValue == n_ms)
    ;

  // SysTick interrupt each 1000 Hz with HCLK equal to 32MHz
  SysTick_Config(8000*PLL_MUL_X);
  while(G_tickValue != 0)
    ;
}

/**
  * @brief  delay_10us delay for some time in 10us unit(accurate)
  * @param  n_10us is how many 10us of time to delay
  * @retval None
  */
void delay_10us(uint32_t n_10us)
{
  // Enable the SysTick Counter
  G_tickValue = n_10us;
  // SysTick interrupt each 10us with HCLK equal to 24MHz
  // - 30 to compensate the overhead of this sub routine
  SysTick_Config(80*PLL_MUL_X - 30);
  while(G_tickValue == n_10us)
    ;

  // SysTick interrupt each 10us with HCLK equal to 24MHz
  SysTick_Config(80 * PLL_MUL_X);
  while(G_tickValue != 0)
    ;
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Init_LED LEDs initialization and User button
  * @param  None
  * @retval None
  */
static void Init_LED(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // Enable GPIOA, GPIOC clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_LED_LD3 | GPIO_LED_LD4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   

  // Init User/Wakeup button B1
  GPIO_InitStructure.GPIO_Pin = GPIO_BUTTON_B1;		
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/**
  * @brief  Init_CS Chip Select pin/port init
  * @param  None
  * @retval None
  */
static void Init_CS(void){
 GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure PA11 pin as CS1 ----------------------------*/
 /* Configure PA12 pin as CS2 ----------------------------*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  

  GPIO_InitStructure.GPIO_Pin =  CS1_PIN | CS2_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(CS_PORT, &GPIO_InitStructure);
    
}


/**
  * @brief  Init_RESET Reset pin/port init
  * @param  None
  * @retval None
  */
static void Init_RESET(void){
 GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure PB13 pin as RESET ----------------------------*/
  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  RESET_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(RESET_PORT, &GPIO_InitStructure);
    
}

/**
  * @brief  staticDelay delay for some time in counts(rough)
  * @param  counts is how many counts of time to delay
  * @retval None
  */
static void staticDelay(uint32_t counts)
{
  while(counts-- != 0)
    ;
}


/**
  * @brief  INT1_EXTI_Config Configure PB12 as external interrupt
  * @param  None
  * @retval None
  */
void INT1_EXTI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* Configure PB12 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI12 Line to PB12 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);

  /* Configure EXTI12 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI12 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  INT2_EXTI_Config Configure PB11 as external interrupt
  * @param  None
  * @retval None
  */
void INT2_EXTI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* Configure PB11 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI11 Line to PB11 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);

  /* Configure EXTI11 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI11 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  INT_EXTI_Config  Configure PB12 as external interrupt (MikroBus INt pin)
  * @param  None
  * @retval None
  */
void INT_EXTI_Config(void)
{
  INT1_EXTI_Config();
}



/**
  * @brief  PWM_Config  Configure PA8 as Timer for PWM control
  * @param  None
  * @retval None
  */
void PWM_Config(void)
{
  GPIO_InitTypeDef         GPIO_InitStructure;  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
  uint16_t TimerPeriod = 0;
  uint16_t Channel1Pulse = 0;
    
  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
 
  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  /* Configure PA8 pin as TIM1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  /* Connect TIM1 Channels to PA8 Alternate Function 2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
 
  
  /**********************************/
  /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / 17570 ) - 1;
  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
  Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);

  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);


  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
