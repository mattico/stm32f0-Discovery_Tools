/**
  ******************************************************************************
  * @file    MikroElektronika/COG2x16_LCD.h 
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
/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef __COG2x16_LCD_H
#define __COG2x16_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/*
 * LCD struct definition
 * C++ like structure to access LCD functions 
 * LCD.Clear, LCD.Init etc
 */
typedef struct
{
  uint8_t entryReg; //0 0 0 0 0 1 I/D S  Left(I/D=1) Right(I/D=0) S=Screen
  uint8_t controlReg; //0 0 0 0 1 D  C  B  D=display C=cursor B=Blink
  uint8_t CDShiftReg; //0 0 0 1 S/C R/L x x S/C=1=cursor R/L=1=Left
  void  (*Init)();
  void  (*Write)(unsigned char* ) ;
  void  (*DataWrite)(uint8_t);
  void  (*RegisterWrite)(uint8_t);
  void  (*Clear)(void);
  void  (*Home)(void);
  void  (*Goto)(unsigned char);
  void  (*Print)(unsigned char ,unsigned char*); 
  void  (*PrintL2)(unsigned char ,unsigned char*); 
  void  (*Scroll)(char);
  void  (*Cursor)(char); 
  void  (*Blink)(char); 
  void  (*Display)(char); 
}LCD_TypeDef;
 
//When Mikroelektronika LCD board is used. LCD objet is created in the LCD.c file. 
extern LCD_TypeDef LCD;

/* Exported constants --------------------------------------------------------*/
//GPIO Pin setting
#define RS             GPIO_Pin_2     
#define EN             GPIO_Pin_3     
#define D4             GPIO_Pin_4     
#define D5             GPIO_Pin_5     
#define D6             GPIO_Pin_6     
#define D7             GPIO_Pin_7     


#define  RS_HIGH   GPIO_HIGH(GPIOC, RS)
#define  RS_LOW    GPIO_LOW(GPIOC, RS)
#define  RS_TOGGLE GPIO_TOGGLE(GPIOC, RS)
#define  EN_HIGH   GPIO_HIGH(GPIOC, EN)
#define  EN_LOW    GPIO_LOW(GPIOC, EN)
#define  EN_TOGGLE GPIO_TOGGLE(GPIOC, EN)
#define  D4_HIGH   GPIO_HIGH(GPIOC, D4)
#define  D4_LOW    GPIO_LOW(GPIOC, D4)
#define  D4_TOGGLE GPIO_TOGGLE(GPIOC, D4)
#define  D5_HIGH   GPIO_HIGH(GPIOC, D5)
#define  D5_LOW    GPIO_LOW(GPIOC, D5)
#define  D5_TOGGLE GPIO_TOGGLE(GPIOC, D5)
#define  D6_HIGH   GPIO_HIGH(GPIOC, D6)
#define  D6_LOW    GPIO_LOW(GPIOC, D6)
#define  D6_TOGGLE GPIO_TOGGLE(GPIOC, D6)
#define  D7_HIGH   GPIO_HIGH(GPIOC, D7)
#define  D7_LOW    GPIO_LOW(GPIOC, D7)
#define  D7_TOGGLE GPIO_TOGGLE(GPIOC, D7)

#define  SCROLL_LEFT 0
#define  SCROLL_RIGHT 1
#define  CURSOR_OFF 0
#define  CURSOR_ON 1
#define  BLINK_OFF 0
#define  BLINK_ON 1
#define  DISPLAY_OFF 0
#define  DISPLAY_ON 1


/* Exported functions ------------------------------------------------------- */ 
void LCD_Init(void);
void LCD_Write(unsigned char *) ;
void LCD_DataWrite(uint8_t);
void LCD_RegisterWrite(uint8_t);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_Goto(unsigned char);
void LCD_Print(unsigned char ,unsigned char*); 
void LCD_PrintL2(unsigned char ,unsigned char*); 
void LCD_Scroll(char);
void LCD_Cursor(char); 
void LCD_Blink(char); 
void LCD_Display(char); 



#ifdef __cplusplus
}
#endif

#endif /* __COG2x16_LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/




