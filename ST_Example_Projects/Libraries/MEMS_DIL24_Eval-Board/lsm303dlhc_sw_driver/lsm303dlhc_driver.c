/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : LSM303DLHC.c
* Author             : MSH Application Team
* Author             : Fabio Tota
* Version            : $Revision:$
* Date               : $Date:$
* Description        : LSM303DLHC driver file
*                      
* HISTORY:
* Date               |	Modification                    |	Author
* 02/08/2011         |	Initial Revision                |	Fabio Tota

********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "lsm303dlhc_driver.h"
#include "mems_eval_i2c.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name		: ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*			: I2C or SPI reading functions					
* Input			: Register Address
* Output		: Data REad
* Return		: None
*******************************************************************************/
u8_t ReadReg(u8_t deviceAddr, u8_t Reg, u8_t* Data) {
  
  //To be completed with either I2c or SPI reading function
  //*Data = SPI_Mems_Read_Reg( Reg );

  

  //RP Modif if(!I2C_BufferRead(Data, deviceAddr, Reg, 1)) 
  if(!I2C_ReadNBytesFromAddr(Data,1,Reg,deviceAddr+1,deviceAddr))
  return MEMS_ERROR;
  else  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name		: WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*			: I2C or SPI writing function
* Input			: Register Address, Data to be written
* Output		: None
* Return		: None
*******************************************************************************/
u8_t WriteReg(u8_t deviceAddress, u8_t WriteAddr, u8_t Data) {
    
  //To be completed with either I2c or SPI writing function
  //SPI_Mems_Write_Reg(Reg, Data);
  //RP Modif I2C_ByteWrite(&Data,  deviceAddress,  WriteAddr); 

  I2C_Master_Slave_Addr_Set(deviceAddress, _I2C_DIR_MASTER_TRANSMIT);
  I2C_Write(WriteAddr, _I2C_MASTER_MODE_BURST_SEND_START);  
  I2C_Write(Data, _I2C_MASTER_MODE_BURST_SEND_FINISH);

  return 1;
}


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SetODR
* Description    : Sets LSM303DLHC Output Data Rate Accelerometer
* Input          : Output Data Rate
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetODR(ODR_t ov){
  u8_t value;

  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG1_A, &value) )
    return MEMS_ERROR;

  value &= 0x0f;
  //value |= ov<<ODR_BIT;
  value |= ov<<4;

  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG1_A, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : SetODR_M
* Description    : Sets LSM303DLHC Output Data Rate Magnetometer
* Input          : Output Data Rate
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetODR_M(ODR_M_t ov){
  u8_t value;

  if( !ReadReg(MAG_I2C_ADDRESS, CRA_REG_M, &value) )
    return MEMS_ERROR;

  value &= 0x80; //bit<6,5,1,0> must be =0 for correct working
  value |= ov<<ODR_M;

  if( !WriteReg(MAG_I2C_ADDRESS, CRA_REG_M, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetTemperature
* Description    : Sets LSM303DLHC Output Temperature
* Input          : MEMS_ENABLE, MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetTemperature(State_t state){
  u8_t value;

  if( !ReadReg(MAG_I2C_ADDRESS, CRA_REG_M, &value) )
    return MEMS_ERROR;

  value &= 0x7F;
  value |= state<<TEMP_EN;

  if( !WriteReg(MAG_I2C_ADDRESS, CRA_REG_M, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetGainMag
* Description    : Sets LSM303DLHC Magnetometer Gain
* Input          : GAIN_1100_M or GAIN_855_M or GAIN_670_M or GAIN_450_M....
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetGainMag(GAIN_M_t Gain){
  u8_t value;

  if( !ReadReg(MAG_I2C_ADDRESS, CRB_REG_M, &value) )
    return MEMS_ERROR;

  value &= 0x00; //bit<4-0> must be =0 for correct working
  value |= Gain<<GN_CFG;

  if( !WriteReg(MAG_I2C_ADDRESS, CRB_REG_M, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetModeMag
* Description    : Sets LSM303DLHC Magnetometer Modality
* Input          : Modality (CONTINUOUS_MODE, or SINGLE_MODE, or SLEEP_MODE)	
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetModeMag(Mode_M_t Mode){
  u8_t value;

  if( !ReadReg(MAG_I2C_ADDRESS, MR_REG_M, &value) )
    return MEMS_ERROR;

  value &= 0x00; //bit<7-3> must be =0 for correct working
  value |= Mode<<MODE_SEL_M;

  if( !WriteReg(MAG_I2C_ADDRESS, MR_REG_M, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : GetTempRaw
* Description    : Read the Temperature Values by TEMP_OUT Output Registers
* Input          : Value to empity (16 Bit two's complement)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetTempRaw(i16_t* val) {
  u8_t valueH;

  if( !ReadReg(MAG_I2C_ADDRESS, TEMP_OUT_H_M, &valueH) )
      return MEMS_ERROR;
  if(valueH & 0x80){
	valueH &= 0x7F;
	valueH = 0x7F - valueH;
    *val = 25 - (i16_t)(valueH);
  } else {
  	*val = 25 + (i16_t)(valueH);
  }
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : SetMode
* Description    : Sets LSM303DLHC Operating Mode Accelrometer
* Input          : Modality (NORMAL, LOW_POWER, POWER_DOWN)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetMode(Mode_t md) {
  u8_t value;
  u8_t value2;
  static   u8_t ODR_old_value;
 
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG1_A, &value) )
    return MEMS_ERROR;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG4_A, &value2) )
    return MEMS_ERROR;
  
  if((value & 0xF0)==0) value = value | (ODR_old_value & 0xF0); //if it comes from POWERDOWN  
    
  switch(md) {
  
  case POWER_DOWN:
    ODR_old_value = value;
    value &= 0x0F;
    break;
          
  case NORMAL:
    value &= 0xF7;
    value |= (MEMS_RESET<<LPEN);
    value2 &= 0xF7;
    value2 |= (MEMS_SET<<HR);   //set HighResolution_BIT
    break;
          
  case LOW_POWER:		
    value &= 0xF7;
    value |=  (MEMS_SET<<LPEN);
    value2 &= 0xF7;
    value2 |= (MEMS_RESET<<HR); //reset HighResolution_BIT
    break;
          
  default:
    return MEMS_ERROR;
  }
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG1_A, value) )
    return MEMS_ERROR;
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG4_A, value2) )
    return MEMS_ERROR;  
   
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetAxis
* Description    : Enable/Disable LSM303DLHC Axis
* Input          : X_ENABLE/X_DISABLE | Y_ENABLE/Y_DISABLE | Z_ENABLE/Z_DISABLE
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetAxis(Axis_t axis) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG1_A, &value) )
    return MEMS_ERROR;
  value &= 0xF8;
  value |= (0x07 & axis);
   
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG1_A, value) )
    return MEMS_ERROR;   
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetFullScale
* Description    : Sets the LSM303DLHC FullScale
* Input          : FULLSCALE_2/FULLSCALE_4/FULLSCALE_8/FULLSCALE_16
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetFullScale(Fullscale_t fs) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG4_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xCF;	
  value |= (fs<<FS);
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG4_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetBDU
* Description    : Enable/Disable Block Data Update Functionality
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetBDU(State_t bdu) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG4_A, &value) )
    return MEMS_ERROR;
 
  value &= 0x7F;
  value |= (bdu<<BDU);

  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG4_A, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetBLE
* Description    : Set Endianess (MSB/LSB)
* Input          : BLE_LSB / BLE_MSB
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetBLE(Endianess_t ble) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG4_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xBF;	
  value |= (ble<<BLE);
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG4_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetSelfTest
* Description    : Set Self Test Modality
* Input          : SELF_TEST_DISABLE/ST_0/ST_1
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetSelfTest(SelfTest_t st) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG4_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xF9;
  value |= (st<<ST);
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG4_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : HPFClick
* Description    : Enable/Disable High Pass Filter for click
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t HPFClickEnable(State_t hpfe) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG2_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xFB;
  value |= (hpfe<<HPCLICK);
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG2_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : HPFAOI1
* Description    : Enable/Disable High Pass Filter for AOI on INT_1
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t HPFAOI1Enable(State_t hpfe) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG2_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xFE;
  value |= (hpfe<<HPIS1);
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG2_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : HPFAOI2
* Description    : Enable/Disable High Pass Filter for AOI on INT_2
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t HPFAOI2Enable(State_t hpfe) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG2_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xFD;
  value |= (hpfe<<HPIS2);
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG2_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetHPFMode
* Description    : Set High Pass Filter Modality
* Input          : HPM_NORMAL_MODE_RES/HPM_REF_SIGNAL/HPM_NORMAL_MODE/HPM_AUTORESET_INT
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetHPFMode(HPFMode_t hpm) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG2_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0x3F;
  value |= (hpm<<HPM);
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG2_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetHPFCutOFF
* Description    : Set High Pass CUT OFF Freq
* Input          : HPFCF [0,3]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetHPFCutOFF(HPFCutOffFreq_t hpf) {
  u8_t value;
    
  if (hpf > 3)
    return MEMS_ERROR;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG2_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xCF;
  value |= (hpf<<HPCF);
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG2_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
  
}


/*******************************************************************************
* Function Name  : SetFilterDataSel
* Description    : Set Filter Data Selection bypassed or sent to FIFO OUT register
* Input          : MEMS_SET, MEMS_RESET
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetFilterDataSel(State_t state) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG2_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xF7;
  value |= (state<<FDS);
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG2_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
  
}


/*******************************************************************************
* Function Name  : SetInt1Pin
* Description    : Set Interrupt1 pin Function
* Input          :  CLICK_ON_PIN_INT1_ENABLE/DISABLE    | I1_INT1_ON_PIN_INT1_ENABLE/DISABLE |              
                    I1_INT2_ON_PIN_INT1_ENABLE/DISABLE  | I1_DRDY1_ON_INT1_ENABLE/DISABLE    |              
                    I1_DRDY2_ON_INT1_ENABLE/DISABLE     | WTM_ON_INT1_ENABLE/DISABLE         |           
                    INT1_OVERRUN_ENABLE/DISABLE  
* example        : SetInt1Pin(CLICK_ON_PIN_INT1_ENABLE | I1_INT1_ON_PIN_INT1_ENABLE |              
                    I1_INT2_ON_PIN_INT1_DISABLE | I1_DRDY1_ON_INT1_ENABLE | I1_DRDY2_ON_INT1_ENABLE     |
                    WTM_ON_INT1_DISABLE | INT1_OVERRUN_DISABLE   ) 
* Note           : To enable Interrupt signals on INT1 Pad (You MUST use all input variable in the argument, as example)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt1Pin(IntPinConf_t pinConf) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG3_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0x00;
  value |= pinConf;
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG3_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetInt2Pin
* Description    : Set Interrupt2 pin Function
* Input          : CLICK_ON_PIN_INT2_ENABLE/DISABLE   | I2_INT1_ON_PIN_INT2_ENABLE/DISABLE |               
                   I2_INT2_ON_PIN_INT2_ENABLE/DISABLE | I2_BOOT_ON_INT2_ENABLE/DISABLE |                   
                   INT_ACTIVE_HIGH/LOW
* example        : SetInt2Pin(CLICK_ON_PIN_INT2_ENABLE/DISABLE | I2_INT1_ON_PIN_INT2_ENABLE/DISABLE |               
                   I2_INT2_ON_PIN_INT2_ENABLE/DISABLE | I2_BOOT_ON_INT2_ENABLE/DISABLE |                   
                   INT_ACTIVE_HIGH/LOW)
* Note           : To enable Interrupt signals on INT2 Pad (You MUST use all input variable in the argument, as example)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt2Pin(IntPinConf_t pinConf) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG6_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0x00;
  value |= pinConf;
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG6_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}                       


/*******************************************************************************
* Function Name  : SetClickCFG
* Description    : Set Click Interrupt config Function
* Input          : ZD_ENABLE/DISABLE | ZS_ENABLE/DISABLE  | YD_ENABLE/DISABLE  | 
                   YS_ENABLE/DISABLE | XD_ENABLE/DISABLE  | XS_ENABLE/DISABLE 
* example        : SetClickCFG( ZD_ENABLE | ZS_DISABLE | YD_ENABLE | 
                               YS_DISABLE | XD_ENABLE | XS_ENABLE)
* Note           : You MUST use all input variable in the argument, as example
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetClickCFG(u8_t status) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CLICK_CFG_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xC0;
  value |= status;
  
  if( !WriteReg(ACC_I2C_ADDRESS, CLICK_CFG_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}  


/*******************************************************************************
* Function Name  : SetClickTHS
* Description    : Set Click Interrupt threshold
* Input          : Click-click Threshold value [0-127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetClickTHS(u8_t ths) {
  
  if(ths>127)     return MEMS_ERROR;
  
  if( !WriteReg(ACC_I2C_ADDRESS, CLICK_THS_A, ths) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
} 


/*******************************************************************************
* Function Name  : SetClickLIMIT
* Description    : Set Click Interrupt Time Limit
* Input          : Click-click Time Limit value [0-127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetClickLIMIT(u8_t t_limit) {
  
  if(t_limit>127)     return MEMS_ERROR;
  
  if( !WriteReg(ACC_I2C_ADDRESS, TIME_LIMIT_A, t_limit) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
} 


/*******************************************************************************
* Function Name  : SetClickLATENCY
* Description    : Set Click Interrupt Time Latency
* Input          : Click-click Time Latency value [0-255]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetClickLATENCY(u8_t t_latency) {
  
  if(t_latency>255)     return MEMS_ERROR;
  
  if( !WriteReg(ACC_I2C_ADDRESS, TIME_LATENCY_A, t_latency) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
} 


/*******************************************************************************
* Function Name  : SetClickWINDOW
* Description    : Set Click Interrupt Time Window
* Input          : Click-click Time Window value [0-255]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetClickWINDOW(u8_t t_window) {
  
  if(t_window>255)     return MEMS_ERROR;
  
  if( !WriteReg(ACC_I2C_ADDRESS, TIME_WINDOW_A, t_window) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : GetClickResponce
* Description    : Get Click Interrupt Responce by CLICK_SRC REGISTER
* Input          : char to empity by Click Responce Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetClickResponce(u8_t* res) {
  u8_t value;
  
 if( !ReadReg(ACC_I2C_ADDRESS, CLICK_SRC_A, &value) ) 
   return MEMS_ERROR;

 value &= 0x7F;
 
 if((value & IA)==0) {        *res = NO_CLICK;     return MEMS_SUCCESS;}
 else {
   if (value & DCLICK){
     if (value & CLICK_SIGN){
        if (value & CLICK_Z) {*res = DCLICK_Z_N;   return MEMS_SUCCESS;}
        if (value & CLICK_Y) {*res = DCLICK_Y_N;   return MEMS_SUCCESS;}
        if (value & CLICK_X) {*res = DCLICK_X_N;   return MEMS_SUCCESS;}
     }
     else{
        if (value & CLICK_Z) {*res = DCLICK_Z_P;   return MEMS_SUCCESS;}
        if (value & CLICK_Y) {*res = DCLICK_Y_P;   return MEMS_SUCCESS;}
        if (value & CLICK_X) {*res = DCLICK_X_P;   return MEMS_SUCCESS;}
     }       
   }
   else{
     if (value & CLICK_SIGN){
        if (value & CLICK_Z) {*res = SCLICK_Z_N;   return MEMS_SUCCESS;}
        if (value & CLICK_Y) {*res = SCLICK_Y_N;   return MEMS_SUCCESS;}
        if (value & CLICK_X) {*res = SCLICK_X_N;   return MEMS_SUCCESS;}
     }
     else{
        if (value & CLICK_Z) {*res = SCLICK_Z_P;   return MEMS_SUCCESS;}
        if (value & CLICK_Y) {*res = SCLICK_Y_P;   return MEMS_SUCCESS;}
        if (value & CLICK_X) {*res = SCLICK_X_P;   return MEMS_SUCCESS;}
     }
   }
 }
 return MEMS_ERROR;
} 


/*******************************************************************************
* Function Name  : Int1LatchEnable
* Description    : Enable Interrupt 1 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t Int1LatchEnable(State_t latch) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG5_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xF7;
  value |= latch<<LIR_INT1;
  
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG5_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : ResetInt1Latch
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t ResetInt1Latch(void) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, INT1_SRC_A, &value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}

      
/*******************************************************************************
* Function Name  : SetInt1Configuration
* Description    : Interrupt 1 Configuration (whitout 6D_INT)
* Input          : INT_AND/OR | INT_ZHIE_ENABLE/DISABLE | INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt1Configuration(IntConf_t ic) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, INT1_CFG_A, &value) )
    return MEMS_ERROR;
  
  value &= 0x40; 
  value |= ic;
  
  if( !WriteReg(ACC_I2C_ADDRESS, INT1_CFG_A, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetInt2Configuration
* Description    : Interrupt 1 Configuration (whitout 6D_INT)
* Input          : INT_AND/OR | INT_ZHIE_ENABLE/DISABLE | INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt2Configuration(IntConf_t ic) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, INT2_CFG_A, &value) )
    return MEMS_ERROR;
  
  value &= 0x40; 
  value |= ic;
  
  if( !WriteReg(ACC_I2C_ADDRESS, INT2_CFG_A, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}
      
     
/*******************************************************************************
* Function Name  : SetIntMode
* Description    : Interrupt 1 Configuration mode (OR, 6D Movement, AND, 6D Position)
* Input          : INT_MODE_OR, INT_MODE_6D_MOVEMENT, INT_MODE_AND, INT_MODE_6D_POSITION
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetIntMode(Int1Mode_t int_mode) {
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, INT1_CFG_A, &value) )
    return MEMS_ERROR;
  
  value &= 0x3F; 
  value |= (int_mode<<INT_6D);
  
  if( !WriteReg(ACC_I2C_ADDRESS, INT1_CFG_A, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

    
/*******************************************************************************
* Function Name  : SetInt6D4DConfiguration
* Description    : 6D, 4D Interrupt Configuration
* Input          : INT1_6D_ENABLE, INT1_4D_ENABLE, INT1_6D_4D_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt6D4DConfiguration(INT_6D_4D_t ic) {
  u8_t value;
  u8_t value2;
  
  if( !ReadReg(ACC_I2C_ADDRESS, INT1_CFG_A, &value) )
    return MEMS_ERROR;
  if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG5_A, &value2) )
    return MEMS_ERROR;
  
  if(ic == INT1_6D_ENABLE){
      value &= 0xBF; 
      value |= (MEMS_ENABLE<<INT_6D);
      value2 &= 0xFB; 
      value2 |= (MEMS_DISABLE<<D4D_INT1);
  }
  
    if(ic == INT1_4D_ENABLE){
      value &= 0xBF; 
      value |= (MEMS_ENABLE<<INT_6D);
      value2 &= 0xFB; 
      value2 |= (MEMS_ENABLE<<D4D_INT1);
  }
  
    if(ic == INT1_6D_4D_DISABLE){
      value &= 0xBF; 
      value |= (MEMS_DISABLE<<INT_6D);
      value2 &= 0xFB; 
      value2 |= (MEMS_DISABLE<<D4D_INT1);
  }
  
  if( !WriteReg(ACC_I2C_ADDRESS, INT1_CFG_A, value) )
    return MEMS_ERROR;
  if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG5_A, value2) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : Get6DPosition
* Description    : 6D, 4D Interrupt Position Detect
* Input          : Byte to empity by POSITION_6D_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t Get6DPosition(u8_t* val){
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, INT1_SRC_A, &value) )
    return MEMS_ERROR;

  value &= 0x7F;
  
  switch (value){
  case UP_SX:   *val = UP_SX;    break;
  case UP_DX:   *val = UP_DX;    break;
  case DW_SX:   *val = DW_SX;    break;
  case DW_DX:   *val = DW_DX;    break;
  case TOP:     *val = TOP;      break;
  case BOTTOM:  *val = BOTTOM;   break;
  }
  
return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : SetInt1Threshold
* Description    : Sets Interrupt 1 Threshold
* Input          : Threshold = [0,31]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt1Threshold(u8_t ths) {
  if (ths > 127)
    return MEMS_ERROR;
  
      if( !WriteReg(ACC_I2C_ADDRESS, INT1_THS_A, ths) )
        return MEMS_ERROR;    

  return MEMS_SUCCESS;
}

     
/*******************************************************************************
* Function Name  : SetInt1Duration
* Description    : Sets Interrupt 1 Duration
* Input          : Duration value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt1Duration(IntConf_t id) {
 
  if (id > 127)
    return MEMS_ERROR;

  if( !WriteReg(ACC_I2C_ADDRESS, INT1_DURATION_A, id) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}
      

/*******************************************************************************
* Function Name  : FIFOModeEnable
* Description    : Sets Fifo Modality
* Input          : FIFO_DISABLE, FIFO_BYPASS_MODE, FIFO_MODE, FIFO_STREAM_MODE, FIFO_TRIGGER_MODE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t FIFOModeEnable(FifoMode_t fm) {
  u8_t value;  
  
  if(fm == FIFO_DISABLE) { 
     if( !ReadReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, &value) )
      return MEMS_ERROR;  
    value &= 0x1f;
    value |= (FIFO_BYPASS_MODE<<FM);                     
    if( !WriteReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, value) )           //fifo mode bypass
      return MEMS_ERROR;   
    
    if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG5_A, &value) )
      return MEMS_ERROR;                 
    value &= 0xBF;    
    if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG5_A, value) )               //fifo disable
      return MEMS_ERROR;   
  }
  
  if(fm == FIFO_BYPASS_MODE)   {  
    if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG5_A, &value) )
      return MEMS_ERROR;               
    value &= 0xBF;
    value |= MEMS_SET<<FIFO_EN;
    if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG5_A, value) )               //fifo enable
      return MEMS_ERROR;  
    
    if( !ReadReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, &value) )
      return MEMS_ERROR;   
    value &= 0x1f;
    value |= (fm<<FM);                     //fifo mode configuration   
    if( !WriteReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, value) )
      return MEMS_ERROR;
  }
  
  if(fm == FIFO_MODE)   {
    if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG5_A, &value) )
      return MEMS_ERROR;                 
    value &= 0xBF;
    value |= MEMS_SET<<FIFO_EN;
    if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG5_A, value) )               //fifo enable
      return MEMS_ERROR;  
    
    if( !ReadReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, &value) )
      return MEMS_ERROR;
    value &= 0x1f;
    value |= (fm<<FM);                      //fifo mode configuration
    if( !WriteReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, value) )
      return MEMS_ERROR;
  }
  
  if(fm == FIFO_STREAM_MODE)   {  
    if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG5_A, &value) )
      return MEMS_ERROR;                
    value &= 0xBF;
    value |= MEMS_SET<<FIFO_EN; 
    if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG5_A, value) )               //fifo enable
      return MEMS_ERROR;   
    
    if( !ReadReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, &value) )
      return MEMS_ERROR;  
    value &= 0x1f;
    value |= (fm<<FM);                      //fifo mode configuration   
    if( !WriteReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, value) )
      return MEMS_ERROR;
  }
  
  if(fm == FIFO_TRIGGER_MODE)   {  
    if( !ReadReg(ACC_I2C_ADDRESS, CTRL_REG5_A, &value) )
      return MEMS_ERROR;                 
    value &= 0xBF;
    value |= MEMS_SET<<FIFO_EN; 
    if( !WriteReg(ACC_I2C_ADDRESS, CTRL_REG5_A, value) )               //fifo enable
      return MEMS_ERROR;    
  
    if( !ReadReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, &value) )
      return MEMS_ERROR;
    value &= 0x1f;
    value |= (fm<<FM);                      //fifo mode configuration 
    if( !WriteReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, value) )
      return MEMS_ERROR;
  } 
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetTriggerInt
* Description    : Trigger event liked to trigger signal INT1/INT2
* Input          : TRIG_INT1/TRIG_INT2
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetTriggerInt(TrigInt_t tr) {
  u8_t value;  
  
  if( !ReadReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, &value) )
      return MEMS_ERROR;
    
    value &= 0xDF;
    value |= (tr<<TR); 

  if( !WriteReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetWaterMark
* Description    : Sets Watermark Value
* Input          : Watermark = [0,31]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetWaterMark(u8_t wtm) {
  u8_t value;
  
  if(wtm > 31)
    return MEMS_ERROR;  
  
  if( !ReadReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, &value) )
    return MEMS_ERROR;
                  
  value &= 0xE0;
  value |= wtm; 
  
  if( !WriteReg(ACC_I2C_ADDRESS, FIFO_CTRL_REG_A, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}

     
/*******************************************************************************
* Function Name  : GetSatusReg
* Description    : Read the status register
* Input          : char to empity by Status Reg Value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetSatusReg(u8_t* val) {
  if( !ReadReg(ACC_I2C_ADDRESS, STATUS_REG_A, val) )
      return MEMS_ERROR;
  
  return MEMS_SUCCESS;  
}

      
/*******************************************************************************
* Function Name  : GetSatusBIT
* Description    : Read the status register BIT
* Input          : STATUS_REG_ZYXOR, STATUS_REG_ZOR, STATUS_REG_YOR, STATUS_REG_XOR,
                   STATUS_REG_ZYXDA, STATUS_REG_ZDA, STATUS_REG_YDA, STATUS_REG_XDA, DATAREADY_BIT
* Output         : status register BIT
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetSatusBit(u8_t statusBIT) {
  u8_t value;  
  
  if( !ReadReg(ACC_I2C_ADDRESS, STATUS_REG_A, &value) )
      return MEMS_ERROR;
 
  switch (statusBIT){
    case STATUS_REG_ZYXOR:     if(value &= STATUS_REG_ZYXOR) return MEMS_SUCCESS;
                               else  return MEMS_ERROR; 
    case STATUS_REG_ZOR:       if(value &= STATUS_REG_ZOR) return MEMS_SUCCESS;
                               else  return MEMS_ERROR;
    case STATUS_REG_YOR:       if(value &= STATUS_REG_YOR) return MEMS_SUCCESS;
                               else  return MEMS_ERROR;                               
    case STATUS_REG_XOR:       if(value &= STATUS_REG_XOR) return MEMS_SUCCESS;
                               else  return MEMS_ERROR;   
    case STATUS_REG_ZYXDA:     if(value &= STATUS_REG_ZYXDA) return MEMS_SUCCESS;
                               else  return MEMS_ERROR; 
    case STATUS_REG_ZDA:       if(value &= STATUS_REG_ZDA) return MEMS_SUCCESS;
                               else  return MEMS_ERROR; 
    case STATUS_REG_YDA:       if(value &= STATUS_REG_YDA) return MEMS_SUCCESS;
                               else  return MEMS_ERROR; 
    case STATUS_REG_XDA:       if(value &= STATUS_REG_XDA) return MEMS_SUCCESS;
                               else  return MEMS_ERROR;                                
    
  }
return MEMS_ERROR;
}

   
/*******************************************************************************
* Function Name  : GetAccAxesRaw
* Description    : Read the Acceleration Values Output Registers
* Input          : buffer to empity by AccAxesRaw_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetAccAxesRaw(AccAxesRaw_t* buff) {
  u8_t valueL;
  u8_t valueH;
  
  if( !ReadReg(ACC_I2C_ADDRESS, OUT_X_L_A, &valueL) )
      return MEMS_ERROR;
  
  if( !ReadReg(ACC_I2C_ADDRESS, OUT_X_H_A, &valueH) )
      return MEMS_ERROR;
  
  buff->AXIS_X = (i16_t)( (valueH << 8) | valueL )/16;
  
  if( !ReadReg(ACC_I2C_ADDRESS, OUT_Y_L_A, &valueL) )
      return MEMS_ERROR;
  
  if( !ReadReg(ACC_I2C_ADDRESS, OUT_Y_H_A, &valueH) )
      return MEMS_ERROR;
  
  buff->AXIS_Y = (i16_t)( (valueH << 8) | valueL )/16;
  
   if( !ReadReg(ACC_I2C_ADDRESS, OUT_Z_L_A, &valueL) )
      return MEMS_ERROR;
  
  if( !ReadReg(ACC_I2C_ADDRESS, OUT_Z_H_A, &valueH) )
      return MEMS_ERROR;
  
  buff->AXIS_Z = (i16_t)( (valueH << 8) | valueL )/16;
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : GetMagAxesRaw
* Description    : Read the Magnetometer Values Output Registers
* Input          : buffer to empity by MagAxesRaw_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetMagAxesRaw(MagAxesRaw_t* buff) {
  u8_t valueL;
  u8_t valueH;
  
  if( !ReadReg(MAG_I2C_ADDRESS, OUT_X_L_M, &valueL) )
      return MEMS_ERROR;
  
  if( !ReadReg(MAG_I2C_ADDRESS, OUT_X_H_M, &valueH) )
      return MEMS_ERROR;
  
  buff->AXIS_X = (i16_t)( (valueH << 8) | valueL )/16;
  
  if( !ReadReg(MAG_I2C_ADDRESS, OUT_Y_L_M, &valueL) )
      return MEMS_ERROR;
  
  if( !ReadReg(MAG_I2C_ADDRESS, OUT_Y_H_M, &valueH) )
      return MEMS_ERROR;
  
  buff->AXIS_Y = (i16_t)( (valueH << 8) | valueL )/16;
  
   if( !ReadReg(MAG_I2C_ADDRESS, OUT_Z_L_M, &valueL) )
      return MEMS_ERROR;
  
  if( !ReadReg(MAG_I2C_ADDRESS, OUT_Z_H_M, &valueH) )
      return MEMS_ERROR;
  
  buff->AXIS_Z = (i16_t)( (valueH << 8) | valueL )/16;
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : GetInt1Src
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetInt1Src(u8_t* val) {
  
  if( !ReadReg(ACC_I2C_ADDRESS, INT1_SRC_A, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : GetInt1SrcBit
* Description    : Reset Interrupt 1 Latching function
* Input          : INT1_SRC_IA, INT1_SRC_ZH, INT1_SRC_ZL, INT1_SRC_YH .....
* Output         : None
* Return         : Status of BIT [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetInt1SrcBit(u8_t statusBIT) {
  u8_t value;  
  
  if( !ReadReg(ACC_I2C_ADDRESS, INT1_SRC_A, &value) )
      return MEMS_ERROR;
 
  if(statusBIT == INT_SRC_IA){
    if(value &= INT_SRC_IA)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }
  
  if(statusBIT == INT_SRC_ZH){
    if(value &= INT_SRC_ZH)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }

  if(statusBIT == INT_SRC_ZL){
    if(value &= INT_SRC_ZL)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }
  
  if(statusBIT == INT_SRC_YH){
    if(value &= INT_SRC_YH)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }

  if(statusBIT == INT_SRC_YL){
    if(value &= INT_SRC_YL)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }

  if(statusBIT == INT_SRC_XH){
    if(value &= INT_SRC_XH)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }

  if(statusBIT == INT_SRC_XL){
    if(value &= INT_SRC_XL)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  } 
return MEMS_ERROR;
}


/*******************************************************************************
* Function Name  : GetInt2SrcBit
* Description    : Reset Interrupt 2 Latching function
* Input          : INT_SRC_IA, INT_SRC_ZH, INT_SRC_ZL, INT_SRC_YH .....
* Output         : None
* Return         : Status of BIT [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetInt2SrcBit(u8_t statusBIT) {
  u8_t value;  
  
  if( !ReadReg(ACC_I2C_ADDRESS, INT2_SRC_A, &value) )
      return MEMS_ERROR;
 
  if(statusBIT == INT_SRC_IA){
    if(value &= INT_SRC_IA)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }
  
  if(statusBIT == INT_SRC_ZH){
    if(value &= INT_SRC_ZH)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }

  if(statusBIT == INT_SRC_ZL){
    if(value &= INT_SRC_ZL)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }
  
  if(statusBIT == INT_SRC_YH){
    if(value &= INT_SRC_YH)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }

  if(statusBIT == INT_SRC_YL){
    if(value &= INT_SRC_YL)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }

  if(statusBIT == INT_SRC_XH){
    if(value &= INT_SRC_XH)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }

  if(statusBIT == INT_SRC_XL){
    if(value &= INT_SRC_XL)    return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  } 
return MEMS_ERROR;
}


/*******************************************************************************
* Function Name  : GetFifoSourceReg
* Description    : Read Fifo source Register
* Input          : Byte to empity by FIFO source register value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetFifoSourceReg(u8_t* val) {
  
  if( !ReadReg(ACC_I2C_ADDRESS, FIFO_SRC_REG_A, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : GetFifoSourceBit
* Description    : Read Fifo WaterMark source bit
* Input          : FIFO_SRC_WTM, FIFO_SRC_OVRUN, FIFO_SRC_EMPTY
* Output         : None
* Return         : Status of BIT [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetFifoSourceBit(u8_t statusBIT){
  u8_t value;  
  
  if( !ReadReg(ACC_I2C_ADDRESS, FIFO_SRC_REG_A, &value) )
      return MEMS_ERROR;
 
  if(statusBIT == FIFO_SRC_WTM){
    if(value &= FIFO_SRC_WTM)     return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }
  
  if(statusBIT == FIFO_SRC_OVRUN){
    if(value &= FIFO_SRC_OVRUN)   return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  }

  if(statusBIT == FIFO_SRC_EMPTY){
    if(value &= FIFO_SRC_EMPTY)   return MEMS_SUCCESS;
    else  return MEMS_ERROR;  
  } 
return MEMS_ERROR;
}


/*******************************************************************************
* Function Name  : GetFifoSourceFSS
* Description    : Read current number of unread samples stored in FIFO
* Input          : Byte to empity by FIFO unread sample value
* Output         : None
* Return         : Status [value of FSS]
*******************************************************************************/
status_t GetFifoSourceFSS(u8_t* val){
  u8_t value;
  
  if( !ReadReg(ACC_I2C_ADDRESS, FIFO_SRC_REG_A, &value) )
    return MEMS_ERROR;
 
  value &= 0x1F;
  *val = value;
  
  return MEMS_SUCCESS;
}
