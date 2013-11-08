/**
  ******************************************************************************
  * @file    NFC_SPI/PN532.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   PN532 SPI com
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

// PN532 library by adafruit/ladyada translated to be used with STM32F051 Discovery Kit
// MIT license

// authenticateBlock, readMemoryBlock, writeMemoryBlock contributed
// by Seeed Technology Inc (www.seeedstudio.com)

/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef _PN532_SPI_H_
#define _PN532_SPI_H_
#ifdef __cplusplus

extern "C"{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "spi.h"
#include "serial.h"

/* Exported macros -----------------------------------------------------------*/



/* Exported constants --------------------------------------------------------*/
#define PN532_PREAMBLE 0x00
#define PN532_STARTCODE1 0x00
#define PN532_STARTCODE2 0xFF
#define PN532_POSTAMBLE 0x00

#define PN532_HOSTTOPN532 0xD4

#define PN532_FIRMWAREVERSION 0x02
#define PN532_GETGENERALSTATUS 0x04
#define PN532_SAMCONFIGURATION  0x14
#define PN532_INLISTPASSIVETARGET 0x4A
#define PN532_INDATAEXCHANGE 0x40
#define PN532_INJUMPFORDEP 0x56
#define PN532_TGINITASTARGET 0x8C
#define PN532_TGGETDATA 0x86
#define PN532_TGSETDATA 0x8E

#define PN532_MIFARE_READ 0x30
#define PN532_MIFARE_WRITE 0xA0

#define PN532_AUTH_WITH_KEYA 0x60
#define PN532_AUTH_WITH_KEYB 0x61


#define PN532_WAKEUP 0x55

#define  PN532_SPI_STATREAD 0x02
#define  PN532_SPI_DATAWRITE 0x01
#define  PN532_SPI_DATAREAD 0x03
#define  PN532_SPI_READY 0x01

#define PN532_MIFARE_ISO14443A 0x0

#define KEY_A	1
#define KEY_B	2



/* Exported typedef -------------------------------------------------------------------*/

/*
 * PN532 struct definition
 */
typedef struct
{
void (*begin)();
uint32_t (*getFirmwareVersion)(void);
boolean (*sendCommandCheckAck)(uint8_t*, uint8_t , uint16_t );
boolean (*SAMConfig)(void);
uint32_t (*configurePeerAsInitiator)(uint8_t baudrate /* Any number from 0-2. 0 for 106kbps or 1 for 201kbps or 2 for 424kbps */);
uint32_t (*initiatorTxRx)(char*,char*);
uint32_t (*configurePeerAsTarget)(void);
uint32_t (*targetTxRx)(char*,char*);
uint32_t (*authenticateBlock)(uint8_t  /*1 or 2*/,uint32_t  /*Card NUID*/, uint8_t  /*0 to 63*/,uint8_t /*Either KEY_A or KEY_B */, uint8_t* );
uint32_t (*readMemoryBlock)(uint8_t  /*1 or 2*/,uint8_t  /*0 to 63*/, uint8_t* );
uint32_t (*writeMemoryBlock)(uint8_t /*1 or 2*/,uint8_t  /*0 to 63*/, uint8_t* );
uint32_t (*readPassiveTargetID)(uint8_t);
boolean (*spi_readack)(void);
uint8_t (*readspistatus)(void);
void (*readspidata)(uint8_t*, uint8_t);
void (*spiwritecommand)(uint8_t*, uint8_t);
}PN532_TypeDef;
 
extern PN532_TypeDef PN532;

/* function prototypes -------------------------------------------------------*/
void begin_PN532_fcn(void);
uint32_t getFirmwareVersion_fcn(void);
boolean sendCommandCheckAck_fcn(uint8_t*, uint8_t , uint16_t );
boolean SAMConfig_fcn(void);
uint32_t configurePeerAsInitiator_fcn(uint8_t baudrate /* Any number from 0-2. 0 for 106kbps or 1 for 201kbps or 2 for 424kbps */);
uint32_t initiatorTxRx_fcn(char*,char*);
uint32_t configurePeerAsTarget_fcn(void);
uint32_t targetTxRx_fcn(char*,char*);
uint32_t authenticateBlock_fcn(uint8_t  /*1 or 2*/,uint32_t  /*Card NUID*/, uint8_t  /*0 to 63*/,uint8_t /*Either KEY_A or KEY_B */, uint8_t* );
uint32_t readMemoryBlock_fcn(uint8_t  /*1 or 2*/,uint8_t  /*0 to 63*/, uint8_t* );
uint32_t writeMemoryBlock_fcn(uint8_t /*1 or 2*/,uint8_t  /*0 to 63*/, uint8_t* );
uint32_t readPassiveTargetID_fcn(uint8_t);
boolean spi_readack_fcn(void);
uint8_t readspistatus_fcn(void);
void readspidata_fcn(uint8_t*, uint8_t);
void spiwritecommand_fcn(uint8_t*, uint8_t);


#ifdef __cplusplus
}
#endif


#endif /* _PN532_SPI_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



