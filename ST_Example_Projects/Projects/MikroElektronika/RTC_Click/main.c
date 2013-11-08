/**
  ******************************************************************************
  * @file    RTC_Click/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   RTC click example for STM32F051 Discovery Kit
  *          Initialise RTC and continuously ask for Time
  *          Default feature = IT every second
  *          Print day/month and hours:Mn:Sec on IDC10 COG2*16 LCD Board
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
#include <stdio.h>
#include "main.h"
#include "i2c.h"
#include "stm32f0xx_conf.h"
#include "utilities.h"
#include "COG2x16_LCD.h"


/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// RTC Definitions
#define RTC_ADDR_WRITE 0xA0  //Write address  R/W =0
#define RTC_ADDR_READ  0xA1  //Read address R/W = 1

/* Global variables ----------------------------------------------------------*/
// Module variables
unsigned char hours, minutes, seconds, day, month, year;    // Global date/time variables



/**************************************************************************************************
* PCF8583 Functions
**************************************************************************************************/


/**
  * @brief   RTC_Read Read data from RTC PCF8583 //Not used
  * @param  addres of RTC register
  * @retval value of of RTC register
  */
unsigned char RTC_Read(unsigned char addr){
  unsigned char value = 0;

  I2C1_Master_Slave_Addr_Set(RTC_ADDR_WRITE, _I2C_DIR_MASTER_TRANSMIT);
  I2C1_Write(addr, _I2C_MASTER_MODE_BURST_SEND_START);  
  I2C1_Master_Slave_Addr_Set(RTC_ADDR_READ, _I2C_DIR_MASTER_RECEIVE);
  I2C1_Write(value, _I2C_MASTER_MODE_BURST_SEND_FINISH);

  I2C1_Read(&value, 0)  ;
  
  return value;
}

/**
  * @brief   RTC_Write  Write data from RTC PCF8583
  * @param  addres of RTC register, value of of RTC register
  * @retval None
  */
void RTC_Write(unsigned char addr, unsigned char value){
  I2C1_Master_Slave_Addr_Set(RTC_ADDR_WRITE, _I2C_DIR_MASTER_TRANSMIT);
  I2C1_Write(addr, _I2C_MASTER_MODE_BURST_SEND_START);  
  I2C1_Write(value, _I2C_MASTER_MODE_BURST_SEND_FINISH);
}

/**
  * @brief   Read_Time Read time from RTC PCF8583 //Not used
  * @param  pointer to variables where RTC data will be stored
  * @retval variables with RTC data
  */
void Read_Time(unsigned char *p_hours, unsigned char *p_minutes, unsigned char *p_seconds,
               unsigned char *p_date, unsigned char *p_month){
 
  I2C1_Master_Slave_Addr_Set(RTC_ADDR_WRITE, _I2C_DIR_MASTER_TRANSMIT);
  I2C1_Write(0x02, _I2C_MASTER_MODE_BURST_SEND_START);  
  I2C1_Master_Slave_Addr_Set(RTC_ADDR_READ, _I2C_DIR_MASTER_RECEIVE);
  I2C1_Write(0x02, _I2C_MASTER_MODE_BURST_SEND_START);

  I2C1_Read(p_seconds,0);
  I2C1_Read(p_minutes,0);
  I2C1_Read(p_hours,0);
  I2C1_Read(p_date,0);
  I2C1_Read(p_month,0);
}

/**
  * @brief   Write_Time Write time to RTC PCF8583
  * @param  variables with RTC data
  * @retval None
  */
void Write_Time(unsigned char c_hours, unsigned char c_minutes, unsigned char c_seconds,
                unsigned char c_date, unsigned char c_month){
                  
  I2C1_Master_Slave_Addr_Set(RTC_ADDR_WRITE, _I2C_DIR_MASTER_TRANSMIT);
  I2C1_Write(0x00, _I2C_MASTER_MODE_BURST_SEND_START);
  
  I2C1_Write(0x80, _I2C_MASTER_MODE_BURST_SEND_CONT);
  I2C1_Write(0x00, _I2C_MASTER_MODE_BURST_SEND_CONT);
  I2C1_Write(c_seconds, _I2C_MASTER_MODE_BURST_SEND_CONT);
  I2C1_Write(c_minutes, _I2C_MASTER_MODE_BURST_SEND_CONT);
  I2C1_Write(c_hours, _I2C_MASTER_MODE_BURST_SEND_CONT);
  I2C1_Write(c_date, _I2C_MASTER_MODE_BURST_SEND_CONT);
  I2C1_Write(c_month, _I2C_MASTER_MODE_BURST_SEND_FINISH);
                  
  I2C1_Write(0x00, _I2C_MASTER_MODE_BURST_SEND_START);
  I2C1_Write(0x00, _I2C_MASTER_MODE_BURST_SEND_FINISH);                 
}


/**
  * @brief  Get_Time  Get Time from PCF8583P
  * @param  data buffer
  * @retval None
  */
void Get_Time(void){
 unsigned char L_nbBytesRead = 0;
 unsigned char L_data[6];
    L_nbBytesRead = I2C1_ReadNBytesFromAddr(L_data, 5, 0x02, RTC_ADDR_READ, RTC_ADDR_WRITE);
    if(L_nbBytesRead == 5) {
     seconds = L_data[0];
     minutes = L_data[1];
     hours = L_data[2];
     year = L_data[3];
     day  = L_data[3];
     month = L_data[4];          
    } else {
     seconds = 0;
     minutes = 0;
     hours = 0;
     year = 0;
     day  = 0;
     month = 0;     
    }
 }

/**
  * @brief   Show_Time  Show on the LCD display
  * @param  variables with RTC data
  * @retval None
  */
void Show_Time(void){
  char L_buff[17]; 
  seconds  =  ((seconds & 0xF0) >> 4)*10 + (seconds & 0x0F);  // Transform seconds
  minutes  =  ((minutes & 0xF0) >> 4)*10 + (minutes & 0x0F);  // Transform months
  hours    =  ((hours & 0xF0)  >> 4)*10  + (hours & 0x0F);    // Transform hours
  year     =   (day & 0xC0) >> 6;                             // Transform year
  day      =  ((day & 0x30) >> 4)*10    + (day & 0x0F);       // Transform day
  month    =  ((month & 0x10)  >> 4)*10 + (month & 0x0F);     // Transform month
  sprintf(L_buff,"%02d/%02d",day,month);
  LCD.Home();
  LCD.Print(0,(unsigned char*)L_buff);
  sprintf(L_buff,"%02dH%02dMN%02dS",hours,minutes,seconds);
  LCD.PrintL2(4,(unsigned char*)L_buff);
}

/**
  * @brief   
  * @param  None
  * @retval None
  */
int main(void){
  
  DISCOVERY_Configuration(); //STM32 discovery Board configuration (clock etc..)

  TURNOFFLD4;         // Turn off LED4

  I2C1_Init();   //I2C communication + GPIO init

  LCD.Init(); //Init LCD
  LCD.Cursor(CURSOR_OFF); //Cursor not visible
  LCD.Blink(BLINK_OFF); //Cursor not visible
  delay_ms(100);  

  INT_EXTI_Config();  //Configure INT pin for external interrupt.
                      // Call EXTI4_15_IRQHandler function every seconde
                      // it Toggle Discovery LED4
    
  // write TIME: 14:03:00 and DATE: 01.03.2011
  //Uncomment to set time/date
  //Write_Time(0x14, 0x03, 0x00, 0x40 | 0x01, 0x03); //Set Time

  RTC_Write(0,0x00); //Default Setting INt toggle 1Hz (1 IT/s)
 
  while(1){            // Endless loop
    Get_Time();        // Get time from PCF8583P
    Show_Time();       // Extract Time from values and print on LCD
    delay_ms(500);
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
