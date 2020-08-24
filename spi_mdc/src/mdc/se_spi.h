/**
  ******************************************************************************
  * @file    se_spi.h
  * @author  Epson
  * @version V1.0
  * @date    28-September-217
  * @brief   This file contains function prototypes for the SPI firmware library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SE_SPI_H
#define SE_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup sePeriphLibrary.
  * @{
  */ 

/** @defgroup SPI
  * @{
  * @brief The SPI module provides an SPI interface as Master or Slave.
  */

/** @defgroup SPI_Constants
  * @{
  */ 

typedef enum {
    seDATA_TR_LENGTH_16BIT = 0xf,    ///< SPI data length is 16 bits
    seDATA_TR_LENGTH_15BIT = 0xe,    ///< SPI data length is 15 bits
    seDATA_TR_LENGTH_14BIT = 0xd,    ///< SPI data length is 14 bits
    seDATA_TR_LENGTH_13BIT = 0xc,    ///< SPI data length is 13 bits
    seDATA_TR_LENGTH_12BIT = 0xb,    ///< SPI data length is 12 bits
    seDATA_TR_LENGTH_11BIT = 0xa,    ///< SPI data length is 11 bits
    seDATA_TR_LENGTH_10BIT = 0x9,    ///< SPI data length is 10 bits
    seDATA_TR_LENGTH_09BIT = 0x8,    ///< SPI data length is 9 bits
    seDATA_TR_LENGTH_08BIT = 0x7,    ///< SPI data length is 8 bits
    seDATA_TR_LENGTH_07BIT = 0x6,    ///< SPI data length is 7 bits
    seDATA_TR_LENGTH_06BIT = 0x5,    ///< SPI data length is 6 bits
    seDATA_TR_LENGTH_05BIT = 0x4,    ///< SPI data length is 5 bits
    seDATA_TR_LENGTH_04BIT = 0x3,    ///< SPI data length is 4 bits
    seDATA_TR_LENGTH_03BIT = 0x2,    ///< SPI data length is 3 bits
    seDATA_TR_LENGTH_02BIT = 0x1,    ///< SPI data length is 2 bits
} seSPI_DataTransferLength;

typedef enum {
    seSPI_LSB_FST  = 1,        ///< Specify the data format (input/output permutation) LSB first
    seSPI_MSB_FST  = 0,        ///< Specify the data format (input/output permutation) MSB first
} seSPI_Format;

typedef enum {
    seSPI_POL_LOW  = 0,        ///< Output is low when clock off.
    seSPI_POL_HIGH = 1,        ///< Output is high when clock off.
} seSPI_Polarity;

typedef enum {
    seSPI_PH_RISE = 0,         ///< Triggers on positive edge of clock.
    seSPI_PH_FALL = 1,         ///< Triggers on negative edge of clock.
} seSPI_Phase;

typedef enum {
    seSPI_MODE_SLAVE  = 0,     ///< Specifies the SPI mode slave
    seSPI_MODE_MASTER = 1,     ///< Specifies the SPI mode master
} seSPI_OperMode;

typedef enum {
    seSPI_BSY    = 0x0080U,    ///< Transfer Busy/Slave Selected
    seSPI_OEIF   = 0x0008U,    ///< Overrun error interrupt
    seSPI_TENDIF = 0x0004U,    ///< End-of-transmission interrupt
    seSPI_RBFIF  = 0x0002U,    ///< Receive buffer full interrupt
    seSPI_TBEIF  = 0x0001U,    ///< Transmit buffer empty interrupt
    seSPI_ALL_IF = seSPI_OEIF | seSPI_TENDIF | seSPI_RBFIF | seSPI_TBEIF  ///< All interrupt flags
} seSPI_IntFlag;
#define seSPI_FLGS(a) ((seSPI_InterruptFlags)((a)))///< Combination of any of seSPI_IntFlag enumerations

typedef enum {
    seSPI_OEIE   = 0x0008U,    ///< Overrun error interrupt
    seSPI_TENDIE = 0x0004U,    ///< End-of-transmission interrupt
    seSPI_RBFIE  = 0x0002U,    ///< Receive buffer full interrupt
    seSPI_TBEIE  = 0x0001U,    ///< Transmit buffer empty interrupt
    seSPI_ALL_IE = seSPI_OEIE | seSPI_TENDIE | seSPI_RBFIE | seSPI_TBEIE  ///< All interrupt enables
} seSPI_Interrupt;

#define seSPI_INTS(a) ((seSPI_Interrupts)((a))) ///< Combination of any of seSPI_Interrupt enumerations

/**
  * @}
  */

/** @defgroup SPI_Types
  * @{
  */

/**
  * @brief  SPI Init structure definition
  */

typedef struct {
  seSPI_DataTransferLength       CHLN; ///< Specifies the data transfer bit length
  seState                        PUEN; ///< Enables/Disables input pin pull-up/down
  seState                    NOCLKDIV; ///< Select master mode operating clock
  seSPI_Format                 LSBFST; ///< Select MSB first/LSB first
  seSPI_Phase                    CPHA; ///< Select clock phase
  seSPI_Polarity                 CPOL; ///< Select clock polarity
  seSPI_OperMode                  MST; ///< Select master/slave mode
} seSPI_InitTypeDef;


/**
  * @brief  SPI Channel definition
  */

typedef struct {
    uint32_t                T16BaseAddr; ///< Base Address for the T16 port
} seSPI_ChannelDef;

/**
  * @}
  */


/** @defgroup SPI_Functions
  * @{
  */


/**
  * @brief  Initializes the SPI peripheral as Master according to the specified
  *   parameters in the SPI_InitStruct.
  * @param  SPI_InitStruct: pointer to a @ref seSPI_InitTypeDef structure that
  *   contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void seSPI_InitStructForMaster( seSPI_InitTypeDef* SPI_InitStruct );

/**
  * @brief  Initializes the SPI peripheral as Slave according to the specified
  *   parameters in the SPI_InitStruct.
  * @param  SPI_InitStruct: pointer to a @ref seSPI_InitTypeDef structure that
  *   contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void seSPI_InitStructForSlave( seSPI_InitTypeDef* SPI_InitStruct );

/**
  * @brief  Initializes the SPI peripheral according to the parameters in the SPI_InitStruct.
  * @param  SPI_InitStruct: pointer to a @ref seSPI_InitTypeDef structure that
  *   contains the configuration information for the specified SPI peripheral.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSPI_Init( seSPI_InitTypeDef* SPI_InitStruct );

/**
  * @brief  Starts the specified SPI channel.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSPI_Start( void );

/**
  * @brief  Stops the specified SPI channel.
  * @retval None
  */
void seSPI_Stop( void );

/**
  * @brief  Software reset of the specified SPI channel.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSPI_Reset( void );

/**
  * @brief  Sends a data through the SPI peripheral.
  * @param  data: Data to be transmitted.
  * @param  size: Data size in number of words.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSPI_TxHWords( uint16_t data[], uint32_t size );

/**
  * @brief  Sends a data through the SPI peripheral.
  * @param  data: Data to be transmitted.
  * @param  size: Data size in number of words.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSPI_DmaTxHWords( uint16_t data[], uint32_t size );

/**
  * @brief  Sends a data through the SPI peripheral.
  * @param  data: Data to be transmitted.
  * @param  size: Data size in number of bytes.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSPI_TxBytes( uint8_t data[], uint32_t size );

/**
  * @brief  Returns the received data by the SPI peripheral.
  * @param  data: The received data.
  * @param  size: Data size in number of words.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSPI_RxHWords( uint16_t data[], uint32_t size );

/**
  * @brief  Returns the received data by the SPI peripheral.
  * @param  data: The received data.
  * @param  size: Data size in number of words.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSPI_DmaRxHWords( uint16_t data[], uint32_t size );

/**
  * @brief  Returns the received data by the SPI peripheral.
  * @param  data: The received data.
  * @param  size: Data size in number of bytes.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSPI_RxBytes( uint8_t data[], uint32_t size );

/**
  * @brief  Enables SPI channel interrupt.
  * @param  irq: interrupt to enable, see @ref seSPI_IntFlag.
  * @retval None
  */
void seSPI_EnableInt( seSPI_Interrupt irq );

/**
  * @brief  Disables SPI channel interrupt.
  * @param  irq: interrupt to disable, see @ref seSPI_IntFlag.
  * @retval None
  */
void seSPI_DisableInt( seSPI_Interrupt irq );

/**
  * @brief  Gets SPI channel interrupt flag.
  * @param  flag: interrupt to check, see @ref seSPI_IntFlag.
  * @retval seInterruptStatus see @ref seInterruptStatus
  */
seInterruptStatus seSPI_GetIntFlag( seSPI_IntFlag flag );

/**
  * @brief  Clears SPI channel interrupt flag.
  * @param  flag: interrupt to clear, see @ref seSPI_IntFlag.
  * @retval None
  */
void seSPI_ClearIntFlag( seSPI_IntFlag flag );

/**
  * @brief  Configures SPICLKn frequency [Hz] (= baud rate [bps])
  * @param  speed: SPICLKn frequency [Hz]
  * @retval Status: can be a value of @ref seStatus
  */
seStatus  seSPI_SetBusSpeed( uint32_t speed );

/**
  * @brief  Discovers SPICLKn frequency [Hz] (= baud rate [bps])
  * @retval SPICLKn: frequency [Hz]
  */
uint32_t seSPI_GetBusSpeed( void );

/**
  * @brief  Function allocates GPIO for Chip Slave Select 0
  *         and enables it.
  *         This function should be defined by user.
  * @retval None
  */
extern void seSPI_ENABLE_MST_CS0( void );

/**
  * @brief  Function asserts Chip Slave Select 0 (Master mode only).
  *         This function should be defined by user.
  *         The function should be implemented by using GPIO.
  * @retval None
  */
extern void seSPI_ASSERT_MST_CS0( void );

/**
  * @brief  Function negates Chip Slave Select 0 (Master mode only).
  *         This function should be defined by user.
  *         The function should be implemented by using GPIO.
  * @retval None
  */
extern void seSPI_NEGATE_MST_CS0( void );

/**
  * @brief  Configures ports for this module. Override this function to configure specific ports.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus ConfigurePortsForSPI( seSPI_OperMode OperMode );

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
#endif // SE_SPI_H

