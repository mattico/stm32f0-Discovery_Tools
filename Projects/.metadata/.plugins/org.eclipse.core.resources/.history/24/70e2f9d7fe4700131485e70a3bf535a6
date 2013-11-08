/**
  ******************************************************************************
  * @file    Arduino/gpio.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Arduino GPIO API for STM32F051 Discovery Kit
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
#include "gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
static GPIO_PinTypeDef ArduinoPort[22] = {
  {GPIO_Pin_3,GPIOA,NOT_CONFIGURED,0}, 
  {GPIO_Pin_2,GPIOA,NOT_CONFIGURED,0}, 
  {GPIO_Pin_12,GPIOB,NOT_CONFIGURED,0}, 
  {GPIO_Pin_11,GPIOB,NOT_CONFIGURED,0}, 
  {GPIO_Pin_7,GPIOA,NOT_CONFIGURED,0}, 
  {GPIO_Pin_9,GPIOB,NOT_CONFIGURED,0}, 
  {GPIO_Pin_8,GPIOB,NOT_CONFIGURED,0}, 
  {GPIO_Pin_6,GPIOA,NOT_CONFIGURED,0}, 
  {GPIO_Pin_5,GPIOA,NOT_CONFIGURED,0}, 
  {GPIO_Pin_4,GPIOA,NOT_CONFIGURED,0}, 
  {GPIO_Pin_11,GPIOA,NOT_CONFIGURED,0},
  {GPIO_Pin_5,GPIOB,NOT_CONFIGURED,0}, 
  {GPIO_Pin_4,GPIOB,NOT_CONFIGURED,0}, 
  {GPIO_Pin_3,GPIOB,NOT_CONFIGURED,0}, 
  {GPIO_Pin_0,GPIOC,NOT_CONFIGURED,0}, 
  {GPIO_Pin_1,GPIOC,NOT_CONFIGURED,0}, 
  {GPIO_Pin_2,GPIOC,NOT_CONFIGURED,0}, 
  {GPIO_Pin_3,GPIOC,NOT_CONFIGURED,0}, 
  {GPIO_Pin_4,GPIOC,NOT_CONFIGURED,0}, 
  {GPIO_Pin_5,GPIOC,NOT_CONFIGURED,0}, 
  {GPIO_Pin_7,GPIOF,NOT_CONFIGURED,0}, 
  {GPIO_Pin_6,GPIOF,NOT_CONFIGURED,0}, 
};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GPIO_InitTypeDef 		GPIO_InitStructure;
static EXTI_InitTypeDef 		EXTI_InitStructure;
static NVIC_InitTypeDef 		NVIC_InitStructure;
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef        TIM_OCInitStructure;
static ADC_InitTypeDef          ADC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
static void pinConfig(uint16_t pin, uint16_t mode);
/* Private functions ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/


/**
  * @brief  pinConfig Arduino pin configuration
  * @param  INPUT or OUTPUT and variations
  * @retval None
  */
static void pinConfig(uint16_t pin, uint16_t mode){
  uint16_t L_oldType = ArduinoPort[pin].PinMode; //Does the pin already configured
  uint8_t L_setExtIt = 0;    //Do we have to set ExtIT
  uint8_t L_resetExtIt = 0;  //Do we have to reset ExtIT
  uint8_t L_setTimer = 0;    //Do we have to set Timer for PWM mode
  uint8_t L_resetTimer = 0;  //Do we have to reset Timer cause PWM mode no more used
  
  if(L_oldType == NOT_CONFIGURED) {
    //Enable GPIOx clock cause Pin has not already been configured
    if(ArduinoPort[pin].GPIOx == GPIOA ) {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);      
    } else if(ArduinoPort[pin].GPIOx == GPIOB) {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    } else if(ArduinoPort[pin].GPIOx == GPIOC) {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    } else if(ArduinoPort[pin].GPIOx == GPIOF) {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
    } else {
      //Error pin is not connected to STM32
      return;
    }
  }
  if((L_oldType & 0x0001) && !(mode & 0x0001) ) { 
    //Pin has already been configured as INPUT and OUTPUT config is requested
    if((pin == 2)||(pin == 3)) {
      L_resetExtIt = 1; //Reset External Interrupt settings. Cause when pin2/3 are input, ExtIT is automatically configured. 
    }
  }
  if(!(L_oldType & 0x0001) && (mode & 0x0001) && (L_oldType != NOT_CONFIGURED)) { 
    //Pin has already been configured as OUTPUT and INPUT config is requested
    if((pin == 3)||(pin == 5)||(pin == 6)||(pin == 9)||(pin == 10)||(pin == 11)) {
      L_resetTimer = 1; //Reset PWM timer settings.  This pins are dedicated to PWM when output. There are changed to INPUT.
    }    
  }

  //What is the STM32 pin to configure regarding Arduino pin num. Get it from mapping from Arduino toSTM32
  GPIO_InitStructure.GPIO_Pin = ArduinoPort[pin].PinNum;
  //Default GPIO speed clock
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  if(mode & 0x0001) { //Requested mode is : INPUT
    //If mode bit 0 = 1 (odd). Request INPUT mode for Pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    switch(mode) {
      case INPUT: 
      case INPUT_NOPULL: 
        //Default GPIO configured as No pull
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        L_setExtIt = 1; //ExtIT can be configured
        break;
      case INPUT_AF: 
        //Alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        break;
      case INPUT_AN: 
        //Analog function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        break;
      case INPUT_PU: 
        //GPIO configured as pull_up
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        if(L_oldType == NOT_CONFIGURED)
          L_setExtIt = 1; //ExtIT can be configured if not already done
        break;
      case INPUT_PD: 
        //GPIO configured as pull_down
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        if(L_oldType == NOT_CONFIGURED)
          L_setExtIt = 1; //ExtIT can be configured if not already done
        break;
      default:
        break;
    }
  } else { //Requested mode is : OUTPUT
    //Request OUTPUT mode for Pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    switch(mode) {
      case OUTPUT: 
      case OUTPUT_OD: 
        //Default GPIO configured as Open Drain
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;     
        break;
      case OUTPUT_AF: 
        //Alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        L_setTimer = 1; //Timer for PWM can be configured
        break;
      case OUTPUT_AN: 
        //Analog function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;          
        break;
      case OUTPUT_PP: 
        //GPIO configured as Output PushPull
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             
        break;
      default:
        break;
    }
  }
  //GPIO Pin Configuration
  GPIO_Init(ArduinoPort[pin].GPIOx, &GPIO_InitStructure);    
  //NOW: Pin has been configured whith requested mode.
  ArduinoPort[pin].PinMode = mode;  //log it!
  
  //Test for Configuration of External interrupt and Timers. Set/Reset if needed!
  if(L_setExtIt) {
   //Configure External Interrupt for Pin2 or Pin3 only
   if((pin == 2)||(pin == 3)) {
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    if(pin == 2) {
    /* Connect EXTI12 Line to PB12 pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);
    /* Configure EXTI12 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    } else {
    /* Connect EXTI11 Line to PB11 pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);
    /* Configure EXTI11 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    }  
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    /* Enable and set EXTI11/EXTI12 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);     
   }
  }
  if(L_resetExtIt) {
   //Reset ExtIt for Pin2 or pin3 only
   if(pin == 2) {
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);      
   }    
   if(pin == 3) {
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);            
   }    
  }
  if(L_resetTimer) {
    //Reset Timer request
    switch(pin){
    // TIMx counter Disable 
    // TIMx Main Output Disable 
    // TIMx clock Disable
      case 3:
        TIM_Cmd(TIM2, DISABLE);
        TIM_CtrlPWMOutputs(TIM2, DISABLE);     
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);   
        break;
      case 5:
        TIM_Cmd(TIM17, DISABLE);
        TIM_CtrlPWMOutputs(TIM17, DISABLE);     
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, DISABLE);   
        break;
      case 6:
        TIM_Cmd(TIM16, DISABLE);
        TIM_CtrlPWMOutputs(TIM16, DISABLE);     
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, DISABLE);   
        break;
      case 9:
        TIM_Cmd(TIM14, DISABLE);
        TIM_CtrlPWMOutputs(TIM14, DISABLE);     
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, DISABLE);   
        break;
      case 10:
        TIM_Cmd(TIM1, DISABLE);
        TIM_CtrlPWMOutputs(TIM1, DISABLE);     
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, DISABLE);   
        break;
      case 11:
        TIM_Cmd(TIM3, DISABLE);
        TIM_CtrlPWMOutputs(TIM3, DISABLE);     
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);   
        break;
      default:
        break;
    }
  }
  if(L_setTimer) {
   //Configure Timer for PWM
   TIM_TimeBaseStructure.TIM_Prescaler = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseStructure.TIM_Period = (SystemCoreClock / 1000 ) - 1; //Default frequency is 1KHz
   ArduinoPort[pin].pwmPeriod = (SystemCoreClock / 1000 ) - 1;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_OCInitStructure.TIM_Pulse = 0; //(uint16_t) (((uint32_t) 2 * (ArduinoPort[pin].pwmPeriod - 1)) / 10); //Default duty cycle at 0%
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    
    switch(pin){
    // TIMx clock enable
    // TIMx Main Output Enable 
    // TIMx clock Enable
      case 3:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
        //Connect TIM Channels to Port Alternate Function 
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_2);        
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
        TIM_OC4Init(TIM2, &TIM_OCInitStructure);
        //enable Frequ and pulse update
        TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
        TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Repetitive); 
        TIM_Cmd(TIM2, ENABLE);
        TIM_CtrlPWMOutputs(TIM2, ENABLE);     
        break;
      case 5:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);   
        //Connect TIM Channels to Port Alternate Function 
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_2);        
        TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);
        TIM_OC1Init(TIM17, &TIM_OCInitStructure);
        //enable Frequ and pulse update
        TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Enable);
        TIM_SelectOnePulseMode(TIM17, TIM_OPMode_Repetitive); 
        TIM_Cmd(TIM17, ENABLE);
        TIM_CtrlPWMOutputs(TIM17, ENABLE);     
        break;
      case 6:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);   
        //Connect TIM Channels to Port Alternate Function 
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2);        
        TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
        TIM_OC1Init(TIM16, &TIM_OCInitStructure);
        //enable Frequ and pulse update
        TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);
        TIM_SelectOnePulseMode(TIM16, TIM_OPMode_Repetitive); 
        TIM_Cmd(TIM16, ENABLE);
        TIM_CtrlPWMOutputs(TIM16, ENABLE);     
        break;
      case 9:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);   
        //Connect TIM Channels to Port Alternate Function 
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_4);        
        TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
        TIM_OC1Init(TIM14, &TIM_OCInitStructure);
        //enable Frequ and pulse update
        TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
        TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Repetitive); 
        TIM_Cmd(TIM14, ENABLE);
        TIM_CtrlPWMOutputs(TIM14, ENABLE);     
        break;
      case 10:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   
        //Connect TIM Channels to Port Alternate Function 
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_2);        
        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
        TIM_OC4Init(TIM1, &TIM_OCInitStructure);
        //enable Frequ and pulse update
        TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
        TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Repetitive); 
        TIM_Cmd(TIM1, ENABLE);
        TIM_CtrlPWMOutputs(TIM1, ENABLE);     
        break;
      case 11:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   
        //Connect TIM Channels to Port Alternate Function 
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_1);        
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);
        //enable Frequ and pulse update
        TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
        TIM_SelectOnePulseMode(TIM3, TIM_OPMode_Repetitive); 
        TIM_Cmd(TIM3, ENABLE);
        TIM_CtrlPWMOutputs(TIM3, ENABLE);     
        break;
      default:
        break;
    } 
  }

}

/**
  * @brief  pinMode Arduino pin configuration
  * @param  INPUT or OUTPUT
  * @retval None
  */
void pinMode(uint16_t pin, uint16_t mode){
  /* Check the parameters */
  assert_param(IS_ARDUINO_PIN(pin));
  assert_param(IS_ARDUINO_MODE(mode));
  pinConfig(pin, mode);
}


/**
  * @brief  pinSetMode Arduino pin mode configuration
  * This function is called to update pin mode when pin configuration in done
  *  from foreign function. Take care on use!!
  * @param  INPUT or OUTPUT
  * @retval None
  */
void pinSetMode(uint16_t pin, uint16_t mode){
  /* Check the parameters */
  assert_param(IS_ARDUINO_PIN(pin));
  assert_param(IS_ARDUINO_MODE(mode));
  ArduinoPort[pin].PinMode = mode;  
}


/**
  * @brief  digitalRead Read GPIO Arduino pin value 
  * @param  pin number
  * @retval HIGH or LOW
  */
uint16_t digitalRead(uint16_t pin){
  uint16_t L_oldType = ArduinoPort[pin].PinMode; //Does the pin already configured?
  uint8_t L_pinVal;
  
  /* Check the parameters */
  assert_param(IS_ARDUINO_PIN(pin));
  
  //Verify Pin configuration. Is it already configured?
  if(L_oldType == NOT_CONFIGURED) {
    //Pin is not configured : Config pin as INPUT
    pinMode(pin, INPUT);
  }
  //If PinMode bit 0 =1 (odd) Pin is already configured as INPUT
  if(L_oldType & 0x01) {
    L_pinVal = GPIO_ReadInputDataBit(ArduinoPort[pin].GPIOx, ArduinoPort[pin].PinNum);
  } else {
    L_pinVal = GPIO_ReadOutputDataBit(ArduinoPort[pin].GPIOx, ArduinoPort[pin].PinNum);
  }
  //Ok, Pin is configured. then return its value!
  if(L_pinVal == (uint8_t)Bit_SET) {
    return(HIGH);
  } else {
    return(LOW);
  }
}

/**
  * @brief  digitalWrite Write value to GPIO Arduino pin 
  * @param  pin number, value to write HIGH or LOW
  * @retval None
  */
void digitalWrite(uint16_t pin, uint16_t value) {
  uint16_t L_oldType = ArduinoPort[pin].PinMode; //Does the pin already configured
  /* Check the parameters */
  assert_param(IS_ARDUINO_PIN(pin));

  
  //If pin has not been configured or configured as INPUT only
  if(L_oldType == INPUT || L_oldType == NOT_CONFIGURED) { 
    //Set the pull_up/down regarding value to write
    if(value == HIGH) {
      pinConfig(pin,INPUT_PU);
    }
    if(value == LOW) {
      pinConfig(pin,INPUT_PD);
    }    
  }
  if(!(L_oldType & 0x0001)) {
    //If pin is configured as OUTPUT mode but not AF or AN
    if(L_oldType != OUTPUT_AF && L_oldType != OUTPUT_AN) {
      if(value == HIGH) {
        GPIO_SetBits(ArduinoPort[pin].GPIOx, ArduinoPort[pin].PinNum);
      }
      if(value == LOW) {
        GPIO_ResetBits(ArduinoPort[pin].GPIOx, ArduinoPort[pin].PinNum);
      }     
    }
  }
    
}


/**
  * @brief  analogRead Read GPIO Arduino pin value 
  * @param  pin number
  * @retval 0 to max STM32 ADC value
  */
uint16_t analogRead(uint16_t pin){
  uint16_t L_oldType = ArduinoPort[pin].PinMode; //Does the pin already configured?
  uint32_t L_adcChannel;
  uint16_t L_ADCConvertedValue=0;
  /* Check the parameters */
  assert_param(IS_ADC_PIN(pin));
  switch(pin){
      //pin to configure or to get value from
      case A0:
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        L_adcChannel = ADC_Channel_10;
        break;
      case A1:
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        L_adcChannel = ADC_Channel_11;
        break;
      case A2:
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        L_adcChannel = ADC_Channel_12;
       break;
      case A3:
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        L_adcChannel = ADC_Channel_13;
        break;
      case A4:
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        L_adcChannel = ADC_Channel_14;
        break;
      case A5:
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        L_adcChannel = ADC_Channel_15;
        break;
      default:
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        L_adcChannel = ADC_Channel_10;
        break;
  }
  if(L_oldType != INPUT_AN) {
  //Pin was not correctly configured! Do it first!
    //ADC Config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_DeInit(ADC1);
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 

    /* GPIOC config for ADC */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    //Config pin as AN    
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //Config ADC
    ADC_Init(ADC1, &ADC_InitStructure); 
    /* ADC Calibration */
    ADC_GetCalibrationFactor(ADC1);
    ADC_Cmd(ADC1, ENABLE);      
    /* Wait the ADCEN falg */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
     //Pin is configured as ADC INPUT now.
    ArduinoPort[pin].PinMode = INPUT_AN;
  }
  //Select channel to scan
  ADC_ChannelConfig(ADC1, L_adcChannel , ADC_SampleTime_239_5Cycles);  
  // ADC1 regular Software Start Conv 
  ADC_StartOfConversion(ADC1);
  // Wait for end of conversion 
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  //Get value
  L_ADCConvertedValue = ADC_GetConversionValue(ADC1);
  //Stop conversion and return ADC value.
  ADC_StopOfConversion(ADC1);
  return(L_ADCConvertedValue);  
}


/**
  * @brief  analogWrite generate pwm output based on value  
  * @param  pin number, value = % of max possible output 
  * @retval None
  */
void analogWrite(uint16_t pin, uint16_t value){
  uint16_t L_oldType = ArduinoPort[pin].PinMode; //Does the pin already configured?
  uint16_t ChannelPulse;
  /* Check the parameters */
  assert_param(IS_PWM_PIN(pin));

  if(L_oldType != OUTPUT_AF){
  //Pin was not correctly configured! Do it first!
    pinConfig(pin, OUTPUT_AF);
  }
  
  ChannelPulse = (uint16_t) (((uint32_t)value * (ArduinoPort[pin].pwmPeriod - 1)) / 100);
   switch(pin){
      case 3:
        TIM2->CCR4 = ChannelPulse;
        break;
      case 5:
        TIM17->CCR1 = ChannelPulse;
        break;
      case 6:
        TIM16->CCR1 = ChannelPulse;
        break;
      case 9:
        TIM14->CCR1 = ChannelPulse;
        break;
      case 10:
        TIM1->CCR4 = ChannelPulse;
        break;
      case 11:
        TIM3->CCR2 = ChannelPulse;
        break;
      default:
        break;
    } 
   
}


/**
  * @brief  pwmFrequency set pwm output frequency  
  * @param  pin number,  frequency in Hz
  * @retval None
  */
void pwmFrequency(uint16_t pin, uint16_t frequency){
  uint16_t L_oldType = ArduinoPort[pin].PinMode; //Does the pin already configured?
  /* Check the parameters */
  assert_param(IS_PWM_PIN(pin));

  if(L_oldType != OUTPUT_AF){
  //Pin was not correctly configured! Do it first!
    pinConfig(pin, OUTPUT_AF);
  }
  
  //Set pwm frequency 
   ArduinoPort[pin].pwmPeriod = (SystemCoreClock / frequency ) - 1;
   switch(pin){
      case 3:
        TIM2->ARR = ArduinoPort[pin].pwmPeriod;
        break;
      case 5:
        TIM17->ARR = ArduinoPort[pin].pwmPeriod;
        break;
      case 6:
        TIM16->ARR = ArduinoPort[pin].pwmPeriod;
        break;
      case 9:
        TIM14->ARR = ArduinoPort[pin].pwmPeriod;
        break;
      case 10:
        TIM1->ARR = ArduinoPort[pin].pwmPeriod;
        break;
      case 11:
        TIM3->ARR = ArduinoPort[pin].pwmPeriod;
        break;
      default:
        break;
    }       
}


/**
  * @brief  getPinDef return Arduino pin definition for given pin number
  * @param  pin Number 
  * @retval Pin definition
  */
GPIO_PinTypeDef getPinDef(uint16_t pin) {
  //Return Arduino pin config
  return(ArduinoPort[pin]);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
