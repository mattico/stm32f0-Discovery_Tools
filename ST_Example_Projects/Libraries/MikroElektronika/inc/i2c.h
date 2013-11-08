/**
  ******************************************************************************
  * @file    MikroElektronika/i2c.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief    I2C mikroBUS API for STM32F051 Discovery Kit
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
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
 extern "C" {
#endif 


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Pin definition and settings*/
/* I2C1 */
#define I2C1_SCL_PIN          GPIO_Pin_6 
#define I2C1_SDA_PIN          GPIO_Pin_7 
#define I2C1_SCL_PIN_SOURCE   GPIO_PinSource6
#define I2C1_SDA_PIN_SOURCE   GPIO_PinSource7
#define I2C1_PORT             GPIOB
/* I2C2 */
#define I2C2_SCL_PIN          GPIO_Pin_6 
#define I2C2_SDA_PIN          GPIO_Pin_7 
#define I2C2_SCL_PIN_SOURCE   GPIO_PinSource6
#define I2C2_SDA_PIN_SOURCE   GPIO_PinSource7
#define I2C2_PORT             GPIOF



#define _I2C_BITRATE_STANDARD_MODE 0x00
#define _I2C_BITRATE_FAST_MODE 0x01

#define _I2C_DIR_MASTER_RECEIVE  0x00
#define _I2C_DIR_MASTER_TRANSMIT 0x01

#define _I2C_MASTER_MODE_SINGLE_RECEIVE       0x00
#define _I2C_MASTER_MODE_BURST_RECEIVE_START  0x01
#define _I2C_MASTER_MODE_BURST_RECEIVE_CONT   0x02
#define _I2C_MASTER_MODE_BURST_RECEIVE_FINISH 0x03

#define _I2C_MASTER_MODE_SINGLE_SEND          0x00
#define _I2C_MASTER_MODE_BURST_SEND_START     0x01
#define _I2C_MASTER_MODE_BURST_SEND_CONT      0x02
#define _I2C_MASTER_MODE_BURST_SEND_FINISH    0x03


/* Exported functions ------------------------------------------------------- */ 
void I2C0_Init(void);
void I2C1_Init(void);
void I2C0_Init_Advanced(unsigned long bitrate);
void I2C1_Init_Advanced(unsigned long bitrate);
void I2C0_Enable(void);
void I2C1_Enable(void);
void I2C0_Disable(void);
void I2C1_Disable(void);
void I2C0_Master_Slave_Addr_Set(char slave_addr, char dir);
void I2C1_Master_Slave_Addr_Set(char slave_addr, char dir);
char I2C0_Master_Bus_Busy(void);
char I2C1_Master_Bus_Busy(void);
char I2C0_Master_Busy(void);
char I2C1_Master_Busy(void);
char I2C0_Read(unsigned char *dat, unsigned char mode);
char I2C1_Read(unsigned char *dat, unsigned char mode);
unsigned I2C0_Write(unsigned char dat, unsigned char mode);
unsigned I2C1_Write(unsigned char dat, unsigned char mode);
char I2C0_ReadNBytesFromAddr(unsigned char *dat, unsigned char nbytes, unsigned char addr, unsigned char slaveRead, unsigned char slaveWrite);
char I2C1_ReadNBytesFromAddr(unsigned char *dat, unsigned char nbytes, unsigned char addr, unsigned char slaveRead, unsigned char slaveWrite);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


