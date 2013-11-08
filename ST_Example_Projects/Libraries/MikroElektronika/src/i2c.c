/**
  ******************************************************************************
  * @file    MikroElektronika/i2c.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   c
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
#include "i2c.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "utilities.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void GPIO_Configuration_I2C2(void);
static void GPIO_Configuration_I2C1(void);

/* Private functions ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
static uint32_t g_Timeout = LONG_TIMEOUT;
static I2C_TypeDef* I2Cx = I2C2;


/**
  * @brief  I2C0_Init This function configures and initializes  I²C1 module with the standard 100kbps bitrate.
  * @param  None
  * @retval None
  */
void I2C0_Init(void){
    I2C_InitTypeDef  I2C_InitStructure;

    I2Cx = I2C1; //I2C1 port will be used
    
    /* Configure the I2C clock source. The clock is derived from the HSI */
        RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
      
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
     
        GPIO_Configuration_I2C1();
        
    /* I2C1 configuration ------------------------------------------------------*/
        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 0x00;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_Timing = 0x00212828;   //0x00210507; 0x00212834

	I2C_Init(I2C1, &I2C_InitStructure);                           
      
        I2C_Cmd(I2C1, ENABLE);
}

/**
  * @brief  I2C1_Init This function configures and initializes  I²C2 module with the standard 100kbps bitrate.
  * @param  None
  * @retval None
  */
void I2C1_Init(void){
    I2C_InitTypeDef  I2C_InitStructure;

    I2Cx = I2C2; //I2C2 port will be used
    
    /* Configure the I2C clock source. The clock is derived from the HSI */
        RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
      
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
     
        GPIO_Configuration_I2C2();
        
    /* I2C2 configuration ------------------------------------------------------*/
        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 0x00;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_Timing = 0x00212828;   //0x00210507; 0x00212834

	I2C_Init(I2C2, &I2C_InitStructure);                           
      
        I2C_Cmd(I2C2, ENABLE);
}


/**
  * @brief  I2C0_Init_Advanced This function configures and initializes I²C0 module using custom bitrate setting. 
  *  As per the I²C standard, SCL clock may be 100 kHz or 400 kHz. (NOT IMPLEMENTED)
  * @param  None
  * @retval None
  */
void I2C0_Init_Advanced(unsigned long bitrate){
}

/**
  * @brief  I2C1_Init_Advanced This function configures and initializes I²C1 module using custom bitrate setting. 
  *  As per the I²C standard, SCL clock may be 100 kHz or 400 kHz. (NOT IMPLEMENTED)
  * @param  None
  * @retval None
  */
void I2C1_Init_Advanced(unsigned long bitrate){
}


/**
  * @brief  I2C_Enable This routine enables I²C Master block.
  * @param  None
  * @retval None
  */
void I2C_Enable(void){
  I2C_Cmd(I2Cx, ENABLE);
}


/**
  * @brief  I2C_Disable This routine disables I²C Master block.
  * @param  None
  * @retval None
  */
void I2C_Disable(void){
  I2C_Cmd(I2Cx, DISABLE);
}


/**
  * @brief  I2C_Master_Slave_Addr_Set Sets the address that the I²C Master will place on the bus, i.e. Slave address.
  * @param  Slave address and read or write to set CR2 bit10
  * @retval None
  */
void I2C_Master_Slave_Addr_Set(char slave_addr, char dir){
  
  if(dir == _I2C_DIR_MASTER_RECEIVE) {
    I2Cx->CR2 |= I2C_CR2_RD_WRN ; //Set CR2 Bit 10  R request
  } else if(dir == _I2C_DIR_MASTER_TRANSMIT){
    I2Cx->CR2 &= ~I2C_CR2_RD_WRN; //Reset CR2 Bit 10 W request
  }
  
  I2Cx->CR2 |= (unsigned char)slave_addr; //Set CR2 Bit0..7 to slave addresse
  
}


/**
  * @brief  I2C_Master_Bus_Busy Indicates whether or not the I²C bus is busy.
  * @param  None
  * @retval 0 Not busy
  */
char I2C_Master_Bus_Busy(void){
  return(I2Cx->ISR & 0x8000);
}


/**
  * @brief  I2C_Master_Busy Indicates whether or not the I²C Master is busy.
  * @param  None
  * @retval 0 Not busy
  */
char I2C_Master_Busy(void){
  //Pending Rx/Tx if Receive data register not empty or Transmit data register
  // not empty
   return((I2Cx->ISR & 0x04)||!(I2Cx->ISR & 0x01)); 
}



/**
  * @brief  I2C_Read Receives a byte from the I²C bus.
  * @param  pointer for data emplacement, receiption mode 
  * @retval 0 OK -  0xFF KO
  */
char I2C_Read(unsigned char *dat, unsigned char mode){
  g_Timeout = LONG_TIMEOUT;  
 
  switch(mode) {
    case _I2C_MASTER_MODE_SINGLE_RECEIVE :
    default:
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_AUTOEND | I2C_CR2_START ;        
      while (!(I2Cx->ISR & I2C_ISR_RXNE)) // Wait for end of addr to read 
      {
        if((g_Timeout--) == 0) 
          return 0xFF;
      }
      *dat = (uint8_t)I2Cx->RXDR;   
      g_Timeout = LONG_TIMEOUT;     
      while (!(I2Cx->ISR & I2C_ISR_STOPF)) // Wait for stopf flag 
      {
       if((g_Timeout--) == 0) 
         return 0xFF;
      }         
      I2Cx->ICR = I2C_ICR_STOPCF; // Clear STOPF flag 
       
      break;
      
    case _I2C_MASTER_MODE_BURST_RECEIVE_START :
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_RELOAD | I2C_CR2_START ;        
      while (!(I2Cx->ISR & I2C_ISR_RXNE)) // Wait for end of addr to read 
      {
        if((g_Timeout--) == 0) 
          return 0xFF;
      }
      *dat = (uint8_t)I2Cx->RXDR;   
      g_Timeout = LONG_TIMEOUT;     
      while (!(I2Cx->ISR & I2C_ISR_TCR))  
      {
       if((g_Timeout--) == 0) 
         return 0xFF;
      }                           
      break;
    case _I2C_MASTER_MODE_BURST_RECEIVE_CONT :
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_RELOAD;  
      *dat = (uint8_t)I2Cx->RXDR;   
       while (!(I2Cx->ISR & I2C_ISR_TCR))  
      {
       if((g_Timeout--) == 0) 
         return 0xFF;
      }                                  
      break;
    case _I2C_MASTER_MODE_BURST_RECEIVE_FINISH:
      I2Cx->CR2 |=(uint32_t)(1 << 16) | I2C_CR2_AUTOEND;
      *dat = (uint8_t)I2Cx->RXDR;   
       while (!(I2Cx->ISR & I2C_ISR_TCR))  
      {
       if((g_Timeout--) == 0) 
         return 0xFF;
      }                                  
      I2Cx->CR2 &= ~I2C_CR2_RELOAD;    //The last one
      
      g_Timeout = LONG_TIMEOUT;     
      while (!(I2Cx->ISR & I2C_ISR_STOPF)) // Wait for stopf flag 
      {
       if((g_Timeout--) == 0) 
         return 0xFF;
      }   
      I2Cx->ICR = I2C_ICR_STOPCF;  // Clear STOPF flag
      
      break;
  }

  I2Cx->CR2&= ~I2C_CR2_NBYTES; //Clear NBYTES

  return(0); 
}

/**
  * @brief  I2C_ReadNBytesFromAddr
  * @param  pointer for data emplacement, nb of bytes to read, address read start, slae address when read, slave address for write
  * @retval 0 OK -  0xFF KO
  */
char I2C_ReadNBytesFromAddr(unsigned char *dat, unsigned char nbytes, unsigned char addr, unsigned char slaveRead, unsigned char slaveWrite){
  unsigned char L_nbRead = 0;
  
  g_Timeout = LONG_TIMEOUT; 
  
  I2Cx->CR2 = (uint32_t)(slaveWrite) | (uint32_t)(1 << 16) | I2C_CR2_START ;
  
  /* Wait until TXIS flag is set */
   while (!(I2Cx->ISR & I2C_ISR_TXIS))
  {
    if((g_Timeout--) == 0) 
      return(0xFF);
  }
  
  /* Send memory address */
  I2Cx->TXDR = (uint8_t)(addr);
  
  g_Timeout = LONG_TIMEOUT; 
   while (!(I2Cx->ISR & I2C_ISR_TC))
  {
    if((g_Timeout--) == 0) 
      return(0xFF);
  }
  
  I2Cx->CR2 = (uint32_t)(slaveRead) | (uint32_t)(nbytes << 16) | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN |  I2C_CR2_START;
  while (L_nbRead != nbytes)
  {
    g_Timeout = LONG_TIMEOUT; 
    while (!(I2Cx->ISR & I2C_ISR_RXNE))
    {
      if((g_Timeout--) == 0) 
        return(0xFF);
    }
    
    /* Read data from RXDR */
    *dat++= (uint8_t)I2Cx->RXDR; 
    L_nbRead++;
  }  
  g_Timeout = LONG_TIMEOUT;     
  while (!(I2Cx->ISR & I2C_ISR_STOPF)) // Wait for stopf flag 
  {
   if((g_Timeout--) == 0) 
     return 0xFF;
  }   
  I2Cx->ICR = I2C_ICR_STOPCF;  // Clear STOPF flag  
  
  return(L_nbRead);
}


/**
  * @brief  I2C_Write Sends data byte via the I²C bus.
  * @param  data to send, write mode type
  * @retval 0 OK -  0xFF KO
  */
unsigned I2C_Write(unsigned char dat, unsigned char mode){
  g_Timeout = LONG_TIMEOUT;  
  
  switch(mode) {
    case _I2C_MASTER_MODE_SINGLE_SEND :
    default:
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_AUTOEND | I2C_CR2_START ;        
      while (!(I2Cx->ISR & I2C_ISR_TXIS)) // Wait for end of sent addr 
      {
        if((g_Timeout--) == 0) return 0xFF;
      }
      I2Cx->TXDR = (uint8_t)dat;
      g_Timeout = LONG_TIMEOUT;     
      while (!(I2Cx->ISR & I2C_ISR_STOPF)) // Wait for stopf flag 
      {
       if((g_Timeout--) == 0) return 0xFF;
      }         
      I2Cx->ICR = I2C_ICR_STOPCF; // Clear STOPF flag 
      break;
    case _I2C_MASTER_MODE_BURST_SEND_START :
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_RELOAD | I2C_CR2_START ;  
      while (!(I2Cx->ISR & I2C_ISR_TXIS))// Wait for end of sent addr 
      {
        if((g_Timeout--) == 0) return 0xFF;
      }
      I2Cx->TXDR = (uint8_t)(dat); // Load char to sent  
      g_Timeout = LONG_TIMEOUT;     
      while (!(I2Cx->ISR & I2C_ISR_TCR)) //Wait for autoreload 
      {
        if((g_Timeout--) == 0) 
          return 0xFF;
      } 
      break;
    case _I2C_MASTER_MODE_BURST_SEND_CONT :
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_RELOAD;  
      /* Send char */
      I2Cx->TXDR = (uint8_t)(dat);     
      while (!(I2Cx->ISR & I2C_ISR_TCR)) //Wait for autoreload 
      {
        if((g_Timeout--) == 0) 
          return 0xFF;
      } 
      break;
    case _I2C_MASTER_MODE_BURST_SEND_FINISH :
      I2Cx->CR2 |=(uint32_t)(1 << 16) | I2C_CR2_AUTOEND;
      g_Timeout = LONG_TIMEOUT;     
      while (!(I2Cx->ISR & I2C_ISR_TXIS)) //Wait for end of last send 
      {
        if((g_Timeout--) == 0) 
          return 0xFF;
      } 
      
      I2Cx->TXDR = (uint8_t)dat; // Data to send

      I2Cx->CR2 &= ~I2C_CR2_RELOAD;    //The last one
      
      g_Timeout = LONG_TIMEOUT;     
      while (!(I2Cx->ISR & I2C_ISR_STOPF)) // Wait for stopf flag 
      {
       if((g_Timeout--) == 0) 
         return 0xFF;
      }   
      I2Cx->ICR = I2C_ICR_STOPCF;  // Clear STOPF flag
      break;
  }

  I2Cx->CR2&= ~I2C_CR2_NBYTES; //Clear NBYTES
  return(I2Cx->ISR & 0x700);
}

/**
  * @brief  I2C1_Enable Enable I2C1
  * @param  None
  * @retval None
  */
void I2C1_Enable(void){
  I2C_Enable();
}

/**
  * @brief  I2C1_Enable Enable I2C0
  * @param  None
  * @retval None
  */
void I2C0_Enable(void){
  I2C_Enable();
}

/**
  * @brief  I2C1_Disable Disable I2C1
  * @param  None
  * @retval None
  */
void I2C1_Disable(void){
  I2C_Disable();
}

/**
  * @brief  I2C0_Disable Disable I2C0
  * @param  None
  * @retval None
  */
void I2C0_Disable(void){
  I2C_Disable();
}

/**
  * @brief  I2C1_Master_Slave_Addr_Set Sets the address that the I²C1 Master will place on the bus, i.e. Slave address.
  * @param  Slave address and read or write to set CR2 bit10
  * @retval None
  */
void I2C1_Master_Slave_Addr_Set(char slave_addr, char dir){
  I2C_Master_Slave_Addr_Set(slave_addr, dir);
}

/**
  * @brief  I2C0_Master_Slave_Addr_Set Sets the address that the I²C2 Master will place on the bus, i.e. Slave address.
  * @param  Slave address and read or write to set CR2 bit10
  * @retval None
  */
void I2C0_Master_Slave_Addr_Set(char slave_addr, char dir){
  I2C_Master_Slave_Addr_Set(slave_addr, dir);
}

/**
  * @brief  I2C1_Master_Bus_Busy is I2C1 bus busy?
  * @param  None
  * @retval 0 Not busy
  */
char I2C1_Master_Bus_Busy(void){
  return (I2C_Master_Bus_Busy());
}

/**
  * @brief  I2C0_Master_Bus_Busy is I2C0 bus busy?
  * @param  None
  * @retval 0 Not busy
  */
char I2C0_Master_Bus_Busy(void){
  return (I2C_Master_Bus_Busy());
}

/**
  * @brief  I2C1_Master_Busy is I2C1 master busy?
  * @param  None
  * @retval 0 Not busy
  */
char I2C1_Master_Busy(void){
  return (I2C_Master_Busy());
}

/**
  * @brief  I2C0_Master_Busy is I2C0 master busy?
  * @param  None
  * @retval 0 Not busy
  */
char I2C0_Master_Busy(void){
  return (I2C_Master_Busy());
}

/**
  * @brief  I2C1_Read Receives a byte from the I²C1 bus.
  * @param  pointer for data emplacement, receiption mode 
  * @retval 0 OK -  0xFF KO
  */
char I2C1_Read(unsigned char *dat, unsigned char mode){
  return(I2C_Read(dat,mode));
}

/**
  * @brief  I2C0_Read Receives a byte from the I²C2 bus.
  * @param  pointer for data emplacement, receiption mode 
  * @retval 0 OK -  0xFF KO
  */
char I2C0_Read(unsigned char *dat, unsigned char mode){
  return(I2C_Read(dat,mode));
}

/**
  * @brief  I2C1_Write Sends data byte via the I²C1 bus.
  * @param  data to send, write mode type
  * @retval 0 OK -  0xFF KO
  */
unsigned I2C1_Write(unsigned char dat, unsigned char mode){
  return(I2C_Write(dat,mode));
}

/**
  * @brief  I2C0_Write Sends data byte via the I²C2 bus.
  * @param  data to send, write mode type
  * @retval 0 OK -  0xFF KO
  */
unsigned I2C0_Write(unsigned char dat, unsigned char mode){
  return(I2C_Write(dat,mode));
}

/**
  * @brief  I2C1_ReadNBytesFromAddr Read nb bytes from I2C1
  * @param  pointer for data emplacement, nb of bytes to read, address read start, slae address when read, slave address for write
  * @retval 0 OK -  0xFF KO
  */
char I2C1_ReadNBytesFromAddr(unsigned char *dat, unsigned char nbytes, unsigned char addr, unsigned char slaveRead, unsigned char slaveWrite){
  return(I2C_ReadNBytesFromAddr(dat,nbytes, addr,slaveRead,slaveWrite));
}

/**
  * @brief  I2C0_ReadNBytesFromAddr Read nb bytes from I2C0
  * @param  pointer for data emplacement, nb of bytes to read, address read start, slae address when read, slave address for write
  * @retval 0 OK -  0xFF KO
  */
char I2C0_ReadNBytesFromAddr(unsigned char *dat, unsigned char nbytes, unsigned char addr, unsigned char slaveRead, unsigned char slaveWrite){
  return(I2C_ReadNBytesFromAddr(dat,nbytes, addr,slaveRead,slaveWrite));
}


/**
  * @brief  GPIO_Configuration_I2C2 Configures the different GPIO ports for I2C2.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration_I2C2(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);  

  
  /* Configure I2C2 pins: SCL and SDA ----------------------------------------*/
  GPIO_InitStructure.GPIO_Pin =  I2C2_SCL_PIN | I2C2_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(I2C2_PORT, &GPIO_InitStructure);

}

/**
  * @brief  GPIO_Configuration_I2C1 Configures the different GPIO ports for I2C1.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration_I2C1(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  

  GPIO_PinAFConfig(I2C1_PORT, I2C1_SCL_PIN_SOURCE, GPIO_AF_1);
  GPIO_PinAFConfig(I2C1_PORT, I2C1_SDA_PIN_SOURCE, GPIO_AF_1);
 
  /* Configure I2C1 pins: SCL and SDA ----------------------------------------*/
  GPIO_InitStructure.GPIO_Pin =  I2C1_SCL_PIN | I2C1_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(I2C1_PORT, &GPIO_InitStructure);

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

