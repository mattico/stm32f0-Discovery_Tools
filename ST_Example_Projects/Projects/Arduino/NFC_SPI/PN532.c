/**
  ******************************************************************************
  * @file    NFC_SPI/PN532.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @briefPN532 SPI com translated to STM32F051 Discovery Kit
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
#include "stm32f0xx_conf.h"
#include "utilities.h"
#include "stm32f0xx.h"
#include "PN532.h"
#include <stdio.h>


// PN532 library by adafruit/ladyada translated to be used with STM32F051 Discovery Kit
// MIT license

// authenticateBlock, readMemoryBlock, writeMemoryBlock contributed
// by Seeed Technology Inc (www.seeedstudio.com)

/*******************************************************************************
TAKE CARE!!!!!!!!
STM32F051 Discovery Kit J1 connector share SPI with pin11(MOSI) pin12(MISO) 
pin13(SCK) of J3 connector.
*******************************************************************************/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//Uncomment for debug
//#define PN532DEBUG 1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
char G_printChar[4]; //used to print information to USART Serial opened com


//Arduino PN532 communication stream
PN532_TypeDef PN532 = {
 begin_PN532_fcn,
 getFirmwareVersion_fcn,
 sendCommandCheckAck_fcn,
 SAMConfig_fcn,
 configurePeerAsInitiator_fcn,
 initiatorTxRx_fcn,
 configurePeerAsTarget_fcn,
 targetTxRx_fcn,
 authenticateBlock_fcn,
 readMemoryBlock_fcn,
 writeMemoryBlock_fcn,
 readPassiveTargetID_fcn,
 spi_readack_fcn,
 readspistatus_fcn,
 readspidata_fcn,
 spiwritecommand_fcn,
};



byte pn532ack[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
byte pn532response_firmwarevers[] = {0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03};

#define PN532_PACKBUFFSIZ 64
byte pn532_packetbuffer[PN532_PACKBUFFSIZ];


/**
  * @brief  begin_PN532_fcn
  * @param  None
  * @retval None
  */
void begin_PN532_fcn() {
//    SPI.Spi16Bit = 1;
    SPI.begin();
    delay(1000);   
    // not exactly sure why but we have to send a dummy command to get synced up
    pn532_packetbuffer[0] = PN532_FIRMWAREVERSION;
    sendCommandCheckAck_fcn(pn532_packetbuffer, 1,1000);
    // ignore response!
}


/**
  * @brief  getFirmwareVersion_fcn
  * @param  None
  * @retval Firmware version
  */
uint32_t getFirmwareVersion_fcn(void) {
    uint32_t response;
    pn532_packetbuffer[0] = PN532_FIRMWAREVERSION;
    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 1,1000))
        return 0;
 
    // read data packet
    readspidata_fcn(pn532_packetbuffer, 12);
    // check some basic stuff
    if (0 != strncmp((char *)pn532_packetbuffer, (char *)pn532response_firmwarevers, 6)) {
        return 0;
    }

    response = pn532_packetbuffer[6];
    response <<= 8;
    response |= pn532_packetbuffer[7];
    response <<= 8;
    response |= pn532_packetbuffer[8];
    response <<= 8;
    response |= pn532_packetbuffer[9];

    return response;
}


// default timeout of one second
/**
  * @brief  sendCommandCheckAck_fcn
  * @param  ptr to cmd, cmd length and timeout
  * @retval TRUE/FALSE
  */
boolean sendCommandCheckAck_fcn(uint8_t *cmd, uint8_t cmdlen, uint16_t timeout) {
    uint16_t timer = 0;
    // write the command
    spiwritecommand_fcn(cmd, cmdlen);

    // Wait for chip to say its ready!
    while (readspistatus_fcn() != PN532_SPI_READY) {
        if (timeout != 0) {
            timer+=10;
            if (timer > timeout)
                return FALSE;
        }
        delay(10);
    }

    // read acknowledgement
    if (!spi_readack_fcn()) {
        return FALSE;
    }

    timer = 0;
    // Wait for chip to say its ready!
    while (readspistatus_fcn() != PN532_SPI_READY) {
        if (timeout != 0) {
            timer+=10;
            if (timer > timeout)
                return FALSE;
        }
        delay(10);
    }

    return TRUE; // ack'd command
}

/**
  * @brief  SAMConfig_fcn
  * @param  None
  * @retval Result of operation
  */
boolean SAMConfig_fcn(void) {
    pn532_packetbuffer[0] = PN532_SAMCONFIGURATION;
    pn532_packetbuffer[1] = 0x01; // normal mode;
    pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
    pn532_packetbuffer[3] = 0x01; // use IRQ pin!

    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 4,1000))
        return FALSE;

    // read data packet
    readspidata_fcn(pn532_packetbuffer, 8);

    return  (pn532_packetbuffer[5] == 0x15);
}

/**
  * @brief  configurePeerAsInitiator_fcn
  * @param  baud rate
  * @retval Result of operation
  */
uint32_t configurePeerAsInitiator_fcn(uint8_t baudrate /* Any number from 0-2. 0 for 106kbps or 1 for 201kbps or 2 for 424kbps */) {

    pn532_packetbuffer[0] = PN532_INJUMPFORDEP;
    pn532_packetbuffer[1] = 0x01; //Active Mode
    pn532_packetbuffer[2] = baudrate; // Use 1 or 2. //0 i.e 106kps is not supported yet
    pn532_packetbuffer[3] = 0x01; //Indicates Optional Payload is present

    //Polling request payload
    pn532_packetbuffer[4] = 0x00; 
    pn532_packetbuffer[5] = 0xFF; 
    pn532_packetbuffer[6] = 0xFF; 
    pn532_packetbuffer[7] = 0x00; 
    pn532_packetbuffer[8] = 0x00; 

    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 9,1000))
        return FALSE;

    // read data packet
    readspidata_fcn(pn532_packetbuffer, 19+6);

#ifdef PN532DEBUG
    Serial.println("");
    // check the response
    Serial.println("PEER_INITIATOR");
    for(uint8_t i=0;i<19+6;i++)
    {
        sprintf(G_printChar,"%c ",pn532_packetbuffer[i]);   
        Serial.print(G_printChar);
    }
    Serial.println(" ");
#endif

return (pn532_packetbuffer[7] == 0x00); //No error

}

/**
  * @brief  initiatorTxRx_fcn
  * @param  ptr to data in/out
  * @retval Result of operation
  */
uint32_t initiatorTxRx_fcn(char *DataOut,char *DataIn)
{
    uint8_t iter;
#ifdef PN532DEBUG
    uint8_t i;
#endif
    pn532_packetbuffer[0] = PN532_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 0x01; //Target 01

    for(iter=(2+0);iter<(2+16);iter++)
    {
        pn532_packetbuffer[iter] = DataOut[iter-2]; //pack the data to send to target
    }

    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 18,1000))
        return FALSE;

    // read data packet
    readspidata_fcn(pn532_packetbuffer, 18+6);

#ifdef PN532DEBUG
    Serial.println("");
    // check the response
    Serial.println("INITIATOR_TXRX");
    for(i=0;i<18+6;i++)
    {
        sprintf(G_printChar,"%c ",pn532_packetbuffer[i]);   
        Serial.print(G_printChar);
    }
    Serial.println(" ");
#endif

   for(iter=8;iter<(8+16);iter++)
   {
       DataIn[iter-8] = pn532_packetbuffer[iter]; //data received from target
   }

return (pn532_packetbuffer[7] == 0x00); //No error
}

/**
  * @brief  configurePeerAsTarget_fcn
  * @param  None
  * @retval Result of operation
  */
uint32_t configurePeerAsTarget_fcn(void) {
    uint8_t iter;
#ifdef PN532DEBUG
    uint8_t i;
#endif

    byte pbuffer[38] =      { PN532_TGINITASTARGET, 
                             0x00,
                             0x08, 0x00, //SENS_RES
                             0x12, 0x34, 0x56, //NFCID1
                             0x40, //SEL_RES

                             0x01, 0xFE, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, // POL_RES
                             0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 
                           
                             0xFF, 0xFF,
                            
                             0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, //NFCID3t: Change this to desired value

                             0x00, 0x00 //Length of general and historical bytes
                             };

    for(iter=0;iter<38;iter++)
    {
        pn532_packetbuffer[iter] = pbuffer[iter];
    }

    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 38,1000))
        return FALSE;

    // read data packet
    readspidata_fcn(pn532_packetbuffer, 18+6);

#ifdef PN532DEBUG
    Serial.println("");
    // check some basic stuff

    Serial.println("PEER_TARGET");
    for( i=0;i<18+6;i++)
    {
        sprintf(G_printChar,"%c ",pn532_packetbuffer[i]);   
        Serial.print(G_printChar);
    }
    Serial.println(" ");
#endif

return (pn532_packetbuffer[23] == 0x00); //No error as it received all response
}

/**
  * @brief  targetTxRx_fcn
  * @param  ptr data in/out
  * @retval Result of operation
  */
uint32_t targetTxRx_fcn(char *DataOut,char *DataIn)
{
    uint8_t iter;
#ifdef PN532DEBUG
    uint8_t i;
#endif
///////////////////////////////////// Receiving from Initiator ///////////////////////////
    pn532_packetbuffer[0] = PN532_TGGETDATA;
    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 1,1000))
        return FALSE;

    // read data packet
    readspidata_fcn(pn532_packetbuffer, 18+6);

#ifdef PN532DEBUG
    Serial.println("");
    // check the response
    Serial.println("TARGET_RX");
    for(uint8_t i=0;i<18+6;i++)
    {
        sprintf(G_printChar,"%c ",pn532_packetbuffer[i]);   
        Serial.print(G_printChar);
    }
    Serial.println(" ");
#endif

   for( iter=8;iter<(8+16);iter++)
   {
      DataIn[iter-8] = pn532_packetbuffer[iter]; //data received from initiator
   }

///////////////////////////////////// Sending to Initiator ///////////////////////////
if(pn532_packetbuffer[7] == 0x00) //If no errors in receiving, send data.
{
    pn532_packetbuffer[0] = PN532_TGSETDATA;
    for( iter=(1+0);iter<(1+16);iter++)
    {
        pn532_packetbuffer[iter] = DataOut[iter-1]; //pack the data to send to target
    }

    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 17,1000))
        return FALSE;

    // read data packet
    readspidata_fcn(pn532_packetbuffer, 2+6);

#ifdef PN532DEBUG
    Serial.println("");
    // check the response
    Serial.println("TARGET_TX");
    for( i=0;i<2+6;i++)
    {
        sprintf(G_printChar,"%c ",pn532_packetbuffer[i]);   
        Serial.print(G_printChar);
    }
    Serial.println(" ");
#endif

return (pn532_packetbuffer[7] == 0x00); //No error
}
    return FALSE;
}

/**
  * @brief  authenticateBlock_fcn
  * @param  cf function prototype
  * @retval Success/Error
  */
uint32_t authenticateBlock_fcn(uint8_t cardnumber /*1 or 2*/,uint32_t cid /*Card NUID*/, uint8_t blockaddress /*0 to 63*/,uint8_t authtype/*Either KEY_A or KEY_B */, uint8_t * keys) {
    pn532_packetbuffer[0] = PN532_INDATAEXCHANGE;
    pn532_packetbuffer[1] = cardnumber;  // either card 1 or 2 (tested for card 1)
    if(authtype == KEY_A)
    {
        pn532_packetbuffer[2] = PN532_AUTH_WITH_KEYA;
    }
    else
    {
        pn532_packetbuffer[2] = PN532_AUTH_WITH_KEYB;
    }
    pn532_packetbuffer[3] = blockaddress; //This address can be 0-63 for MIFARE 1K card

    pn532_packetbuffer[4] = keys[0];
    pn532_packetbuffer[5] = keys[1];
    pn532_packetbuffer[6] = keys[2];
    pn532_packetbuffer[7] = keys[3];
    pn532_packetbuffer[8] = keys[4];
    pn532_packetbuffer[9] = keys[5];

    pn532_packetbuffer[10] = ((cid >> 24) & 0xFF);
    pn532_packetbuffer[11] = ((cid >> 16) & 0xFF);
    pn532_packetbuffer[12] = ((cid >> 8) & 0xFF);
    pn532_packetbuffer[13] = ((cid >> 0) & 0xFF);

    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 14,1000))
        return FALSE;

    // read data packet
    readspidata_fcn(pn532_packetbuffer, 2+6);

#ifdef PN532DEBUG
    for( iter=0;iter<14;iter++)
    {
        sprintf(G_printChar,"%c ",pn532_packetbuffer[iter]);   
        Serial.print(G_printChar);
    }
    Serial.println("");
    // check some basic stuff

    Serial.println("AUTH");
    for( i=0;i<2+6;i++)
    {
        sprintf(G_printChar,"%c ",pn532_packetbuffer[i]);   
        Serial.print(G_printChar);
        Serial.println(" ");
    }
#endif

    if((pn532_packetbuffer[6] == 0x41) && (pn532_packetbuffer[7] == 0x00))
    {
  	return TRUE;
    }
    else
    {
  	return FALSE;
    }

}

/**
  * @brief  readMemoryBlock_fcn
  * @param  Num of card, address, buffer for data read
  * @retval Success/Error
  */
uint32_t readMemoryBlock_fcn(uint8_t cardnumber /*1 or 2*/,uint8_t blockaddress /*0 to 63*/, uint8_t * block) {
    uint8_t i;
    pn532_packetbuffer[0] = PN532_INDATAEXCHANGE;
    pn532_packetbuffer[1] = cardnumber;  // either card 1 or 2 (tested for card 1)
    pn532_packetbuffer[2] = PN532_MIFARE_READ;
    pn532_packetbuffer[3] = blockaddress; //This address can be 0-63 for MIFARE 1K card

    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 4,1000))
        return FALSE;

    // read data packet
    readspidata_fcn(pn532_packetbuffer, 18+6);
    // check some basic stuff
#ifdef PN532DEBUG
    Serial.println("READ");
#endif
    for( i=8;i<18+6;i++)
    {
        block[i-8] = pn532_packetbuffer[i];
#ifdef PN532DEBUG
        sprintf(G_printChar,"%c ",pn532_packetbuffer[i]);   
        Serial.print(G_printChar);
#endif
    }
#ifdef PN532DEBUG
    Serial.println(" ");
#endif
    if((pn532_packetbuffer[6] == 0x41) && (pn532_packetbuffer[7] == 0x00))
    {
  	return TRUE; //read successful
    }
    else

    {
  	return FALSE;
    }

}

//Do not write to Sector Trailer Block unless you know what you are doing.
/**
  * @brief  writeMemoryBlock_fcn
  * @param  Num of card, address, data to write
  * @retval Success/Error
  */
uint32_t writeMemoryBlock_fcn(uint8_t cardnumber /*1 or 2*/,uint8_t blockaddress /*0 to 63*/, uint8_t * block) {
    uint8_t iter;
    pn532_packetbuffer[0] = PN532_INDATAEXCHANGE;
    pn532_packetbuffer[1] = cardnumber;  // either card 1 or 2 (tested for card 1)
    pn532_packetbuffer[2] = PN532_MIFARE_WRITE;
    pn532_packetbuffer[3] = blockaddress;

    for( iter=0; iter <16; iter++)
    {
        pn532_packetbuffer[4+iter] = block[iter];
    }

    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 20,1000))
        return FALSE;
    // read data packet
    readspidata_fcn(pn532_packetbuffer, 2+6);

#ifdef PN532DEBUG
    // check some basic stuff
    Serial.println("WRITE");
    for( i=0;i<2+6;i++)
    {
        sprintf(G_printChar,"%c ",pn532_packetbuffer[i]);   
        Serial.print(G_printChar);
    }
    Serial.println(" ");
#endif

    if((pn532_packetbuffer[6] == 0x41) && (pn532_packetbuffer[7] == 0x00))
    {
  	return TRUE; //write successful
    }
    else
    {
  	return FALSE;
    }
}

/**
  * @brief  readPassiveTargetID_fcn
  * @param  baudrate 
  * @retval Target ID
  */
uint32_t readPassiveTargetID_fcn(uint8_t cardbaudrate) {
    uint32_t cid;
	uint16_t sens_res,i;

    pn532_packetbuffer[0] = PN532_INLISTPASSIVETARGET;
    pn532_packetbuffer[1] = 1;  // max 1 cards at once (we can set this to 2 later)
    pn532_packetbuffer[2] = 0; //RP cardbaudrate;

    if (! sendCommandCheckAck_fcn(pn532_packetbuffer, 3,1000))
        return 0x0;  // no cards read

    // read data packet
    readspidata_fcn(pn532_packetbuffer, 20);
    // check some basic stuff

    Serial.print("Found "); 
    sprintf(G_printChar,"%c ",pn532_packetbuffer[7]);   
    Serial.print(G_printChar);
  
    Serial.println(" tags");
    if (pn532_packetbuffer[7] != 1)
        return 0;
    
    sens_res = pn532_packetbuffer[9];
    sens_res <<= 8;
    sens_res |= pn532_packetbuffer[10];
    Serial.print("Sens Response: 0x");  
    sprintf(G_printChar,"%02X",sens_res);   
    Serial.print(G_printChar);
    Serial.println("");
    Serial.print("Sel Response: 0x");  
    sprintf(G_printChar,"%02X",pn532_packetbuffer[11]);   
    Serial.print(G_printChar);
    Serial.println("");
    cid = 0;
    for ( i=0; i< pn532_packetbuffer[12]; i++) {
        cid <<= 8;
        cid |= pn532_packetbuffer[13+i];
        Serial.print(" 0x"); 
        sprintf(G_printChar,"%02X",pn532_packetbuffer[13+i]);   
        Serial.print(G_printChar);
    }

#ifdef PN532DEBUG
    Serial.println("TargetID");
    for( i=0;i<20;i++)
    {
        sprintf(G_printChar,"%c ",pn532_packetbuffer[i]);   
        Serial.print(G_printChar);
    }
    Serial.println(" ");
#endif  
    return cid;
}


/************** high level SPI */


/**
  * @brief  spi_readack_fcn Acknoledge PN532
  * @param  None
  * @retval Result of compare with PN532 Ack
  */
boolean spi_readack_fcn() {
    uint8_t ackbuff[6];

    readspidata_fcn(ackbuff, 6);

    return (0 == strncmp((char *)ackbuff, (char *)pn532ack, 6));
}

/************** mid level SPI */
/**
  * @brief  readspistatus_fcn Status read
  * @param  None
  * @retval Status 
  */
uint8_t readspistatus_fcn(void) {
	uint8_t x;
    digitalWrite(CS, LOW);
    delay(3); 
    // read byte
    x = (uint8_t)SPI.read(PN532_SPI_STATREAD);
    digitalWrite(CS, HIGH);
    return x;
}

/**
  * @brief  readspidata_fcn read SPI data 
  * @param  buff for read data, nb of char 
  * @retval None
  */
void readspidata_fcn(uint8_t* buff, uint8_t n) {
    uint8_t i;
  
    digitalWrite(CS, LOW);
    delay(3); 
    SPI.read(PN532_SPI_DATAREAD);

#ifdef PN532DEBUG
    Serial.print("Reading: ");
#endif
    for ( i=0; i<n; i++) {
        delay(2);
        buff[i] = (uint8_t)SPI.read(0);
#ifdef PN532DEBUG
        sprintf(G_printChar,"%02X",buff[i]);   
        Serial.print(" 0x"); Serial.print(G_printChar);
#endif
    }

#ifdef PN532DEBUG
    Serial.println(" ");
#endif
    digitalWrite(CS, HIGH);
}

/**
  * @brief  spiwritecommand_fcn Send a cmd string
  * @param  cmd to send, length of cmd
  * @retval None
  */
void spiwritecommand_fcn(uint8_t* cmd, uint8_t cmdlen) {
    uint8_t checksum,i;
   	uint8_t cmdlen_1;
	uint8_t checksum_1;

    cmdlen++;

#ifdef PN532DEBUG
    Serial.print("\nSending: ");
#endif

    digitalWrite(CS, LOW);
    delay(3);    // or whatever the delay is for waking up the board
    SPI.read(PN532_SPI_DATAWRITE);

    checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE2;
    SPI.read(PN532_PREAMBLE);
    SPI.read(PN532_PREAMBLE);
    SPI.read(PN532_STARTCODE2);
    SPI.read(cmdlen);
    
    cmdlen_1=~cmdlen + 1;
    SPI.read(cmdlen_1);

    SPI.read(PN532_HOSTTOPN532);
    checksum += PN532_HOSTTOPN532;

#ifdef PN532DEBUG
    sprintf(G_printChar,"%02X",PN532_PREAMBLE);   
    Serial.print(" 0x"); Serial.print(G_printChar);
    sprintf(G_printChar,"%02X",PN532_PREAMBLE);   
    Serial.print(" 0x"); Serial.print(G_printChar);
    sprintf(G_printChar,"%02X",PN532_STARTCODE2);   
    Serial.print(" 0x"); Serial.print(G_printChar);
    sprintf(G_printChar,"%02X",cmdlen);   
    Serial.print(" 0x"); Serial.print(G_printChar);
    sprintf(G_printChar,"%02X",cmdlen_1);   
    Serial.print(" 0x"); Serial.print(G_printChar);
    sprintf(G_printChar,"%02X",PN532_HOSTTOPN532);   
    Serial.print(" 0x"); Serial.print(G_printChar);
#endif

    for ( i=0; i<cmdlen-1; i++) {
        SPI.read(cmd[i]);
        checksum += cmd[i];
#ifdef PN532DEBUG
        sprintf(G_printChar,"%02X",cmd[i]);   
        Serial.print(" 0x"); Serial.print(G_printChar);
#endif
    }
    checksum_1=~checksum;
    SPI.read(checksum_1);
    SPI.read(PN532_POSTAMBLE);
    digitalWrite(CS, HIGH);

#ifdef PN532DEBUG
    sprintf(G_printChar,"%02X",checksum_1);   
    Serial.print(" 0x"); Serial.print(G_printChar);
    sprintf(G_printChar,"%02X",PN532_POSTAMBLE);   
    Serial.print(" 0x"); Serial.print(G_printChar);
    Serial.println("");
#endif
} 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
