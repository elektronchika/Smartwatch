/**
  ******************************************************************************
  * @file    se_i2c.h
  * @author  Epson
  * @version V1.0
  * @date    27-November-2017
  * @brief   This file contains the function prototypes for the I2C library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SE_I2C_H
#define SE_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup sePeriphLibrary
  * @{
  */


/** @defgroup I2C
  * @{
  * @brief The I2C module is a subset of the I2C bus interface
  */

/** @defgroup I2C_Constants
  * @{
  */ 
typedef  seCLG_ClkSrc seI2C_ClkSrc;

typedef enum {
  seI2C_IOSC_CLKDIV_1 = 0,                      ///< IOSC division ratio is 1/1.
  seI2C_IOSC_CLKDIV_2 = 1,                      ///< IOSC division ratio is 1/2.
  seI2C_IOSC_CLKDIV_4 = 2,                      ///< IOSC division ratio is 1/4.
  seI2C_IOSC_CLKDIV_8 = 3,                      ///< IOSC division ratio is 1/8.
} seI2C_IOSC_ClkDiv;

typedef enum {
  seI2C_OSC1_CLKDIV_1 = 0,                      ///< OSC1 division ratio is 1/1.
} seI2C_OSC1_ClkDiv;

typedef enum {
  seI2C_MODE_SLAVE  = 0,                        ///< Specifies the I2C mode slave
  seI2C_MODE_MASTER = 1,                        ///< Specifies the I2C mode master
} seI2C_mode;                                   ///< Master Mode Select

typedef enum {
  seI2C_7BIT_SLV_ADDR  = 0,                     ///< Specifies the I2C mode slave
  seI2C_10BIT_SLV_ADDR = 1,                     ///< Specifies the I2C mode master
} seI2C_AddrMode;                               ///< Own addressing mode select

typedef union {
  seI2C_IOSC_ClkDiv iosc;
  seI2C_OSC1_ClkDiv osc1;
} seI2C_ClkDiv;


typedef enum {
  seI2C_SDALOW    = 0x1000U,                    ///< SDA low flag
  seI2C_SCLLOW    = 0x0800U,                    ///< SCL low flag
  seI2C_BSY       = 0x0400U,                    ///< Busy flag
  seI2C_TR        = 0x0200U,                    ///< Transmitter/Receiver flag  
  seI2C_BYTEENDIF = 0x0080U,                    ///< End of transfer flag
  seI2C_GCIF      = 0x0040U,                    ///< General call address reception flag
  seI2C_NACKIF    = 0x0020U,                    ///< NACK reception flag
  seI2C_STOPIF    = 0x0010U,                    ///< STOP condition flag
  seI2C_STARTIF   = 0x0008U,                    ///< START condition flag
  seI2C_ERRIF     = 0x0004U,                    ///< Error detection flag
  seI2C_RBFIF     = 0x0002U,                    ///< Receive buffer full flag
  seI2C_TBEIF     = 0x0001U,                    ///< Transmit buffer empty flag
  seI2C_ALL_IF    = seI2C_BYTEENDIF | seI2C_GCIF | seI2C_NACKIF |
                    seI2C_STOPIF | seI2C_STARTIF | seI2C_ERRIF | 
                    seI2C_RBFIF | seI2C_TBEIF   ///< All interrupt flags


} seI2C_IntFlag;
#define seI2C_FLGS(a) ((seI2C_IntFlag)((a)))    ///< Combination of any of the seI2C_IntFlag enumerations.

typedef enum {
  seI2C_BYTEENDIE = 0x0080U,                    ///< End of transfer interrupt
  seI2C_GCIE      = 0x0040U,                    ///< General call address reception interrupt
  seI2C_NACKIE    = 0x0020U,                    ///< NACK reception interrupt
  seI2C_STOPIE    = 0x0010U,                    ///< STOP condition interrupt
  seI2C_STARTIE   = 0x0008U,                    ///< START condition interrupt
  seI2C_ERRIE     = 0x0004U,                    ///< Error detection interrupt
  seI2C_RBFIE     = 0x0002U,                    ///< Receive buffer full interrupt
  seI2C_TBEIE     = 0x0001U,                    ///< Transmit buffer empty interrupt
  seI2C_ALL_IE    = seI2C_BYTEENDIE | seI2C_GCIE | seI2C_NACKIE |
                    seI2C_STOPIE | seI2C_STARTIE | seI2C_ERRIE | 
                    seI2C_RBFIE | seI2C_TBEIE   ///< All interrupt enables
} seI2C_Interrupt;    
#define seI2C_INTS(a) ((seI2C_Interrupt)((a)))  ///< Combination of any of the seI2C_Interrupt enumerations.
    
/**
  * @}
  */ 
   
   
/** @defgroup I2C_Types
  * @{
  */

   
/** 
  * @brief  I2C Init structure definition  
  */

typedef struct
{  
  seI2C_mode            I2C_mode;         ///< Specifies Master/Slave mode, a value of @ref I2C_mode 
  
  /* Master mode */
  seI2C_ClkSrc          ClkSrc;           ///< Specifies the i2c clock source selection.    
  uint16_t              ClkDivider;       ///< Specifies the prescaler value used to divide the i2c clock.   
  uint16_t              BRT;              /**< Specifies the baud rate( bps = CLK_I2C / (BRT + 3) x 2 ). 
                                               Note: The I2C bus transfer rate is limited to 100 kbit/s in 
                                               standard mode or 400 kbit/s in fast mode. Do not set a 
                                               transfer rate exceeding the limit. */
  /* Slave mode */
  seI2C_AddrMode        AddrMode;         ///< Specifies if 7-bit or 10-bit address, a seI2C_AddrMode value.
  uint16_t              SlaveAddr;        ///< Specifies the slave address. This parameter can be a 7-bit or 10-bit address.
  seState               RespGenCalls;     ///< Specifies if slave is to response to master's general calls.
  
} seI2C_InitTypeDef;

/** 
  * @brief  I2C Channel definition
  */

typedef struct { 
  sePORT_PeriphPortDef           SCL;   ///< SCL pin port definition
  sePORT_PeriphPortDef           SDA;   ///< SDA pin port definition
} seI2C_ChannelDef;


/**
  * @}
  */ 


/** @defgroup I2C_Functions
  * @{
  */

/**
  * @brief  Fills each I2C_InitStruct member with its default value for Master mode.
  * @param  I2C_InitStruct: Pointer to an @ref seI2C_InitTypeDef structure which will be initialized.
  * @retval None
  */    
void seI2C_InitStructForMaster( seI2C_InitTypeDef* I2C_InitStruct );

/**
  * @brief  Fills each I2C_InitStruct member with its default value for Slave mode.
  * @param  I2C_InitStruct: Pointer to an @ref seI2C_InitTypeDef structure which will be initialized.
  * @retval None
  */
void seI2C_InitStructForSlave( seI2C_InitTypeDef* I2C_InitStruct );

/**
  * @brief  Initializes the I2Cx peripheral according to the parameters in the I2C_InitStruct.
  * @param  I2C_InitStruct: Pointer to a @ref seI2C_InitTypeDef structure that
  *   contains the configuration information for the I2C peripheral.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seI2C_Init( seI2C_InitTypeDef* I2C_InitStruct );

/**
  * @brief  Enables the I2C channel.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seI2C_Enable( void );

/**
  * @brief  Disables the I2C channel.
  * @retval None
  */
void seI2C_Disable( void );

/**
  * @brief  Sends a data byte through the I2Cx peripheral if Master mode.
  * @param  address: I2C adddres, can be 10-bit address.
  * @param  data: Data to be transmitted.
  * @param  size: Number of bytes in data to send.
  * @param  stop_pending: if zero - send stop condition, if non zero - stop pending.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seI2C_MstSendData( uint16_t address, uint8_t data[], uint32_t size, uint32_t stop_pending );

/**
  * @brief  Returns the most recent received data by the I2Cx peripheral if Master mode.
  * @param  address: I2C adddres, can be 10-bit address.
  * @param  data: ptr to place recieved data.
  * @param  size: Number of bytes in data to receive.
  * @param  stop_pending: if zero - send stop condition, if non zero - stop is pending
  * @retval Status: can be a value of @ref seStatus.  Received data is stored in data[].
  */
seStatus seI2C_MstReceiveData( uint16_t address, uint8_t data[], uint32_t size, uint32_t stop_pending );

/**
  * @brief  Sends a data byte through the I2Cx peripheral if Slave mode.
  * @param  data: Data to be transmitted.
  * @param  size: Number of bytes in data to send.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seI2C_SlvSendData( uint8_t data[], uint32_t size );

/**
  * @brief  Returns the most recent received data by the I2Cx peripheral if Slave mode.
  * @param  data: ptr to be place received data.
  * @param  size: Number of bytes in data to receive.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seI2C_SlvReceiveData( uint8_t data[], uint32_t size );

/**
  * @brief  Resets the I2C channel.
  * @retval Status: can be a value of @ref seStatus.  Received data is stored in data[].
  */
seStatus seI2C_Reset( void );

/**
  * @brief  Enables I2C channel interrupt.
  * @param  irq: The interrupt to enable, can be value of @ref seI2C_Interrupt.
  * @retval None
  */
void seI2C_EnableInt( seI2C_Interrupt irq );

/**
  * @brief  Disables I2C channel interrupt.
  * @param  irq: The interrupt to disable, can be value of @ref seI2C_Interrupt.
  * @retval None
  */
void seI2C_DisableInt( seI2C_Interrupt irq );

/**
  * @brief  Gets I2C channel interrupt flag.
  * @param  flag: The interrupt flag, can be value of @ref seI2C_IntFlag.
  * @retval InterruptStatus: can be a value of @ref seInterruptStatus
  */
seInterruptStatus seI2C_GetIntFlag( seI2C_IntFlag flag );

/**
  * @brief  Clears I2C channel interrupt flag.
  * @param  flag: The interrupt flag, can be value of @ref seI2C_IntFlag.
  * @retval None
  */
void seI2C_ClearIntFlag( seI2C_IntFlag flag );

/**
  * @brief  Configures ports for the I2C module. 
  * @param  I2CCHx: I2C channel definition of type @ref seI2C_ChannelDef
  * @retval Status: can be a value of @ref seStatus
  */
seStatus ConfigurePortsForI2C( void );

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif
#endif // SE_I2C_H

