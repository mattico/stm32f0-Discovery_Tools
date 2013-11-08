/**
  ******************************************************************************
  * @file    MKI108V2/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Mems DIP24 Eval Board example for STM32F051 Discovery Kit
  *          MKI108V2 Board connected to STM32F051 Discovery Kit.
  * IDC10 COG2x16 LCD BOARD can be used to display result (option).
  * Results can also be accessible/displayed with STMStudio
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
#define COG2x16_LCD	  1	   //Comment / uncomment this line depending of LCD used.

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "main.h"
#include "mems_eval_i2c.h"
#include "lsm303dlhc_driver.h"
#include "stm32f0xx_conf.h"

#ifdef  COG2x16_LCD		   
#include "COG2x16_LCD.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DELAY_SCREEN 1000

//define for example1,2,3,4 
/**********UNCOMMENT TO SELECT EXAMPLE TO RUN*******/
#define __EXAMPLE1__H 
//#define __EXAMPLE2__H 
//#define __EXAMPLE3__H 
//#define __EXAMPLE4__H 

//Acceleration values for Example 1
#define G_ACC_SOFT_THRESHOLD 10  //Threshold for acceleration

//Magneto value for Example 3
#define G_MAG_SOFT_THRESHOLD 0	 //Threshold for magneto

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/

/*****Global variables to be visible from STMStudio*****/
 uint8_t G_response;

 //Acceleration AXIS Values for Example 1
 i16_t G_AXIS_X = 0;
 i16_t G_AXIS_Y = 0;
 i16_t G_AXIS_Z = 0;

 //Magneto AXIS Values for Example 3
 i16_t G_AXIS_M_X = 0;
 i16_t G_AXIS_M_Y = 0;
 i16_t G_AXIS_M_Z = 0;

//Temperature value	 for Example 4
 i16_t G_temperature=0;
  

/**
  * @brief   Main program 
  * @param  None
  * @retval None
  */
int main(void)
{
 uint8_t buffer[18]; 
 uint8_t position=0, old_position=0;


 
    DISCOVERY_Configuration(); //STM32 discovery Board configuration (clock etc..)
    TURNOFFLD3;         // Turn off Discovery LED3
    TURNOFFLD4;         // Turn off Discovery LED4

    I2C1_Init();        //I2C1 GPIO and communication init
    
    GPIO_HIGH(GPIOA, GPIO_Pin_11); //CS = 1 I2C Mode selected

#ifdef  COG2x16_LCD
    delay_ms(100);  
    LCD.Init();             //Init LCD (Mikroelektronika LCD board)
    LCD.Cursor(CURSOR_OFF); //Cursor not visible
    LCD.Blink(BLINK_OFF);   //Cursor not visible
    delay_ms(100); 
    LCD.Clear();  
#endif

    INT_EXTI_Config();  //Configure INT pin for external interrupt.
                      // Call EXTI4_15_IRQHandler 

  	//set ODR_ACCELEROMETER (turn ON device)
 	G_response = SetODR(ODR_25Hz);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_ODR_OK      ");
        LCD.Print(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
  	}
#endif
 	//set ODR_MAGNETOMETER (turn ON device)
 	G_response = SetODR_M(ODR_220Hz_M);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_ODR_M_OK    ");
        LCD.PrintL2(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        }
#endif
    //set PowerMode 
    G_response = SetMode(NORMAL);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_MODE_OK     ");
        LCD.Print(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        
  	}
#endif
 	//set MagnetometerMode 
 	G_response = SetModeMag(CONTINUOUS_MODE);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_MODE_MAG_OK ");
        LCD.PrintL2(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        
  	}
#endif
 //set Fullscale
 G_response = SetFullScale(FULLSCALE_2);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_FULLSCALE_OK ");
        LCD.Print(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        
  	}
#endif
  //set Magnetometer Gain
 G_response = SetGainMag(GAIN_450_M);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_GAIN_MAG_OK ");
        LCD.PrintL2(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        
  	}
#endif
 //set axis Enable
 G_response = SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_AXIS_OK     ");
        LCD.Print(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        
  	}
#endif
  
/******Example 1******/ 
#ifdef __EXAMPLE1__H 
#ifdef  COG2x16_LCD
	LCD.Clear();
#endif
  	while(1){
      AccAxesRaw_t L_data;
	  delay_ms(100); //get ACC value avery 100ms only 
  	  //get Acceleration Raw data  
  	  G_response = GetAccAxesRaw(&L_data);
      if(G_response==1){
	    TOGGLE_LD4;
		//Change value only if over software threshold to avoid blinking
		if(((L_data.AXIS_X >= G_AXIS_X) && (L_data.AXIS_X - G_ACC_SOFT_THRESHOLD >= G_AXIS_X)) || 
		   ((L_data.AXIS_X < G_AXIS_X) && (L_data.AXIS_X + G_ACC_SOFT_THRESHOLD <= G_AXIS_X)))
		   G_AXIS_X = L_data.AXIS_X;	

		if(((L_data.AXIS_Y >= G_AXIS_Y) && (L_data.AXIS_Y - G_ACC_SOFT_THRESHOLD >= G_AXIS_Y)) || 
		   ((L_data.AXIS_Y < G_AXIS_Y) && (L_data.AXIS_Y + G_ACC_SOFT_THRESHOLD <= G_AXIS_Y)))
		   G_AXIS_Y = L_data.AXIS_Y;	

		if(((L_data.AXIS_Z >= G_AXIS_Z) && (L_data.AXIS_Z - G_ACC_SOFT_THRESHOLD >= G_AXIS_Z)) || 
		   ((L_data.AXIS_Z < G_AXIS_Z) && (L_data.AXIS_Z + G_ACC_SOFT_THRESHOLD <= G_AXIS_Z)))
		   G_AXIS_Z = L_data.AXIS_Z;	

#ifdef  COG2x16_LCD
    	sprintf((char*)buffer, "X:%6d", G_AXIS_X);
    	LCD.Print(0,(unsigned char*)buffer);
    	sprintf((char*)buffer, "Y:%6dZ:%6d",G_AXIS_Y, G_AXIS_Z);
    	LCD.PrintL2(0,(unsigned char*)buffer);
#endif
      }
 	}
#endif /* __EXAMPLE1__H  */ 

 
/******Example 2******/
#ifdef __EXAMPLE2__H
	//set Interrupt Threshold 
 	G_response = SetInt1Threshold(20);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_THRESHOLD_OK");
        LCD.PrintL2(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        
  	}
#endif
	//set Interrupt configuration (all enabled)
 	G_response = SetInt1Configuration(INT_ZHIE_ENABLE | INT_ZLIE_ENABLE |
                                INT_YHIE_ENABLE | INT_YLIE_ENABLE |
                                INT_XHIE_ENABLE | INT_XLIE_ENABLE ); 
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_INT_CONF_OK ");
        LCD.Print(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        
  	}
#endif
 	//set Interrupt Mode
 	G_response = SetIntMode(INT_MODE_6D_POSITION);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_INT_MODE    ");
        LCD.PrintL2(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        
  	}
	LCD.Clear();
#endif

 	while(1){
  	  //get 6D Position
  	  G_response = Get6DPosition(&position);
	  if((G_response==1) && (old_position!=position)){
    	TOGGLE_LD4;
#ifdef  COG2x16_LCD
    	sprintf((char*)buffer,"Position:");
    	LCD.Print(0,(unsigned char*)buffer);
    	switch (position){
    	case UP_SX:   sprintf((char*)buffer,"UP_SX  ");   break;
    	case UP_DX:   sprintf((char*)buffer,"UP_DX  ");   break;
    	case DW_SX:   sprintf((char*)buffer,"DW_SX  ");   break;              
    	case DW_DX:   sprintf((char*)buffer,"DW_DX  ");   break; 
    	case TOP:     sprintf((char*)buffer,"TOP    ");   break; 
    	case BOTTOM:  sprintf((char*)buffer,"BOTTOM ");   break; 
    	default:      sprintf((char*)buffer,"unknown");   break;
    	}
    	LCD.PrintL2(8,(unsigned char*)buffer);   
#endif
  		old_position = position;
  	  }
 	}
#endif /*__EXAMPLE2__H */ 
 
 
/******Example 3******/ 
#ifdef __EXAMPLE3__H 
#ifdef  COG2x16_LCD
	LCD.Clear();
#endif
  	while(1){
      MagAxesRaw_t L_dataM;
	  delay_ms(100); //get data value every 100ms only 
  	  //get Magnetometer Raw data  
  	  G_response = GetMagAxesRaw(&L_dataM);
  	  if(G_response==1){
	    TOGGLE_LD4;
		//Change value only if over software threshold to avoid blinking
		if(((L_dataM.AXIS_X >= G_AXIS_M_X) && (L_dataM.AXIS_X - G_MAG_SOFT_THRESHOLD >= G_AXIS_M_X)) || 
		   ((L_dataM.AXIS_X < G_AXIS_M_X) && (L_dataM.AXIS_X + G_MAG_SOFT_THRESHOLD <= G_AXIS_M_X)))
		   G_AXIS_M_X = L_dataM.AXIS_X;	

		if(((L_dataM.AXIS_Y >= G_AXIS_M_Y) && (L_dataM.AXIS_Y - G_MAG_SOFT_THRESHOLD >= G_AXIS_M_Y)) || 
		   ((L_dataM.AXIS_Y < G_AXIS_M_Y) && (L_dataM.AXIS_Y + G_MAG_SOFT_THRESHOLD <= G_AXIS_M_Y)))
		   G_AXIS_M_Y = L_dataM.AXIS_Y;	

		if(((L_dataM.AXIS_Z >= G_AXIS_M_Z) && (L_dataM.AXIS_Z - G_MAG_SOFT_THRESHOLD >= G_AXIS_M_Z)) || 
		   ((L_dataM.AXIS_Z < G_AXIS_M_Z) && (L_dataM.AXIS_Z + G_MAG_SOFT_THRESHOLD <= G_AXIS_M_Z)))
		   G_AXIS_M_Z = L_dataM.AXIS_Z;	

#ifdef  COG2x16_LCD
    	sprintf((char*)buffer, "xM:%5d", G_AXIS_M_X);
    	LCD.Print(0,(unsigned char*)buffer);
    	sprintf((char*)buffer, "yM:%5dzM:%5d",G_AXIS_M_Y, G_AXIS_M_Z);
    	LCD.PrintL2(0,(unsigned char*)buffer);
#endif
	    old_position = position;
  	  }
 	}
#endif /* __EXAMPLE3__H  */  
 
 /******Example 4******/ 
#ifdef __EXAMPLE4__H 
 	//set temperature 
 	G_response = SetTemperature(MEMS_ENABLE);
#ifdef  COG2x16_LCD
 	if(G_response==1){
        sprintf((char*)buffer,"SET_TEMP_OK     ");
        LCD.PrintL2(0,(unsigned char*)buffer);
        delay_ms(DELAY_SCREEN);  
        
  	}
	LCD.Clear();
#endif
 	while(1){
	  delay_ms(400); //get data value every 400ms only 
  	  //get temperature data value (DegC)  
  	  G_response = GetTempRaw(&G_temperature);
  	  if(G_response==1){
	  	LCD.Clear();
	    TOGGLE_LD4;

#ifdef  COG2x16_LCD
    	sprintf((char*)buffer,"Temperature:");
    	LCD.Print(0,(unsigned char*)buffer);

	//    snprintf((char*)buffer,8, "%f", temperature);
	    snprintf((char*)buffer,8, "%d", G_temperature);
    	LCD.PrintL2(8,(unsigned char*)buffer);   
#endif
  	  }
 	}
#endif /* __EXAMPLE4__H  */ 

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
