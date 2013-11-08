/**
  ******************************************************************************
  * @file    MikroElektronika/COG2x16_LCD.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   COG2x16 LCD API for STM32F051 Discovery Kit
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
#include "COG2x16_LCD.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void LCD_WriteChar(unsigned char ) ;

/* Global variables ----------------------------------------------------------*/
//LCD "object"
LCD_TypeDef LCD = {
  0x04,
  0x0F,
  0x18,
  LCD_Init,
  LCD_Write, 
  LCD_DataWrite,
  LCD_RegisterWrite,
  LCD_Clear,
  LCD_Home,
  LCD_Goto,
  LCD_Print,
  LCD_PrintL2,
  LCD_Scroll,
  LCD_Cursor,
  LCD_Blink,
  LCD_Display,
};


/**
  * @brief  Send a char to LCd cna be a data or a cmd depending on RS position (HIGH = data, LOW = cmd) 
  * @param  Char to write
  * @retval None
  */
static void LCD_WriteChar(unsigned char c) 
{ 
 GPIOC->ODR &= 0xFF0F;
 GPIOC->ODR |= (c & 0xF0);
 EN_HIGH;delay_10us(1); EN_LOW;
 GPIOC->ODR &= 0xFF0F;
 GPIOC->ODR |= c << 4;
 EN_HIGH;delay_10us(1); EN_LOW;
 delay_ms(2); 
} 


/**
  * @brief  LCD_Init LCD initialisation = Pins setting and LCD registers configuration
  * @param  None
  * @retval None
  */
void  LCD_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);  

  /*Set RS and EN pins as output PUSH PULL pins */
  GPIO_InitStructure.GPIO_Pin =  RS | EN ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  /*Set data pins as output OPEN DRAIN pins */
  GPIO_InitStructure.GPIO_Pin =  D4 | D5 | D6 | D7;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  RS_LOW; //Write control bytes  
  delay_ms(15); // power on delay 
  GPIOC->ODR |= 0x30;
  EN_HIGH;delay_10us(1); EN_LOW;
  delay_ms(5); 
  EN_HIGH;delay_10us(1); EN_LOW;
  delay_10us(10);
  EN_HIGH;delay_10us(1); EN_LOW;
  delay_ms(5); 
  GPIOC->ODR &= 0xFF20;
  EN_HIGH;delay_10us(1); EN_LOW;
  delay_10us(4);
  LCD_WriteChar(0x28); // 4 bit mode, 1/16 duty, 5x8 font 
  LCD_WriteChar(0x08); // display off 
  LCD_WriteChar(0x0F); // display on, blink curson on 
  LCD.controlReg = 0x0F;
  LCD_WriteChar(0x06); // entry mode   
  LCD_WriteChar(0x01); //LCD clear
  delay_ms(10); 
}


/**
  * @brief  LCD_Write Write a 0 terminating buffer of char
  * @param  Buffer of char to be written
  * @retval None
  */
void LCD_Write(unsigned char *buff) 
{ 
  RS_HIGH; //Write data 
  while(*buff) 
  LCD_WriteChar(*buff++); 
} 

/**
  * @brief  LCD_DataWrite Write a char
  * @param  char to write
  * @retval None
  */
void LCD_DataWrite(uint8_t data) {
  RS_HIGH; //Write data 
  LCD_WriteChar(data);
}

/**
  * @brief  LCD_RegisterWrite LCD Register configuration.
  * @param  LCD reg setting
  * @retval None
  */
void LCD_RegisterWrite(uint8_t regVal) {
  RS_LOW; //Write Register 
  LCD_WriteChar(regVal);
}

/**
  * @brief  LCD_Clear Clear all LCD (2 lines)
  * @param  None
  * @retval None
  */
void LCD_Clear(void) {
  LCD_RegisterWrite(0x01);
}

/**
  * @brief  LCD_Home LCD pointer to 1st line, 1st position
  * @param  None
  * @retval None
  */
void LCD_Home(void) {
  LCD_RegisterWrite(0x02);
}

/**
  * @brief  LCD_Goto LCD pointer to desired position
  * @param  position on LCD
  * @retval None
  */
void LCD_Goto(unsigned char pos) {
  RS_LOW; //Write Register 
  LCD_WriteChar(0x80+pos);
}

/**
  * @brief  LCD_Print Print LCd from a position
  * @param  LCD position 1st line, 0 terminated string to write
  * @retval None
  */
void LCD_Print(unsigned char x,unsigned char *str) 
{ 
  LCD_Goto(x); 
  LCD_Write(str); 
} 

/**
  * @brief  LCD_PrintL2 Print LCd from a position on 2nd line
  * @paramLCD position 2nd line line, 0 terminated string to write
  * @retval None
  */
void LCD_PrintL2(unsigned char x,unsigned char *str) 
{ 
  LCD_Goto(x+0x40); 
  LCD_Write(str); 
} 

/**
  * @brief  LCD_Scroll Right/left scroll LCD
  * @param  Direction SCROLL_LEFT/SCROLL_RIGHT
  * @retval None
  */
void LCD_Scroll(char Direction) 
{ 
  RS_LOW; //Write Register 
  if(Direction==SCROLL_LEFT) 
  { 
   LCD_WriteChar(0x18); //- S/C=1 R/L=0 => Scroll LEFT 
  } 
  else 
  { 
   LCD_WriteChar(0x1b); //- S/C=1 R/L=1 => Scroll RIGHT 
  } 
  delay_ms(2); 
} 

/**
  * @brief  LCD_Cursor  Cursor visible or not
  * @param  CURSOR_OFF/CURSOR_ON
  * @retval None
  */
void LCD_Cursor(char onoff) 
{ 
  RS_LOW; //Write Register 
  if(onoff==CURSOR_OFF) //- 0000 1DCB     
  { //- D= Display C= Cursor B=Blink 
    LCD.controlReg &= 0x0D;
  } 
  else 
  { 
    LCD.controlReg |= 0x02;
 } 
  LCD_WriteChar(LCD.controlReg);
  delay_ms(2); 
} 

/**
  * @brief  LCD_Blink Cursor blinking or not
  * @param  BLINK_OFF/BLINK_ON
  * @retval None
  */
void LCD_Blink(char onoff) 
{ 
  RS_LOW; //Write Register 
  if(onoff==BLINK_OFF) //- 0000 1DCB 
  { //- D= Display C= Cursor B=Blink 
   LCD.controlReg &= 0x0E;
  } 
  else 
  { 
   LCD.controlReg |= 0x01;
  } 
  LCD_WriteChar(LCD.controlReg);
  delay_ms(2); 
} 

/**
  * @brief  LCD_Display Set display or not
  * @param  DISPLAY_OFF/DISPLAY_ON
  * @retval None
  */
void LCD_Display(char onoff) 
{ 
  RS_LOW; //Write Register 
  if(onoff==DISPLAY_OFF) //- 0000 1DCB 
  { //- D= Display C= Cursor B=Blink 
   LCD.controlReg &= 0x0B;
  } 
  else 
  { 
   LCD.controlReg |= 0x04;
  } 
  LCD_WriteChar(LCD.controlReg);
  delay_ms(2); 
} 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
