/**
  ******************************************************************************
  * @file    se_qspi.h
  * @author  Epson
  * @version V1.0
  * @date    21-September-2017
  * @brief   This file contains the function prototypes for the QSPI firmware
  *          library. The code in this library is based on the S1C31xxx QSPI
  *          library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SE_QSPI_H
#define SE_QSPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup sePeriphLibrary.
  * @{
  */

/** @defgroup QSPI
  * @{
  * @brief The QSPI module provides Single/Dual/Quad SPI interface as Master or Slave.
  */

/** @defgroup QSPI_Constants
  * @{
  */

typedef enum {
  seQSPI_16CLK = 0xf,         ///< Number of clocks is 16
  seQSPI_15CLK = 0xe,         ///< Number of clocks is 15
  seQSPI_14CLK = 0xd,         ///< Number of clocks is 14
  seQSPI_13CLK = 0xc,         ///< Number of clocks is 13
  seQSPI_12CLK = 0xb,         ///< Number of clocks is 12
  seQSPI_11CLK = 0xa,         ///< Number of clocks is 11
  seQSPI_10CLK = 0x9,         ///< Number of clocks is 10
  seQSPI_09CLK = 0x8,         ///< Number of clocks is 9
  seQSPI_08CLK = 0x7,         ///< Number of clocks is 8
  seQSPI_07CLK = 0x6,         ///< Number of clocks is 7
  seQSPI_06CLK = 0x5,         ///< Number of clocks is 6
  seQSPI_05CLK = 0x4,         ///< Number of clocks is 5
  seQSPI_04CLK = 0x3,         ///< Number of clocks is 4
  seQSPI_03CLK = 0x2,         ///< Number of clocks is 3
  seQSPI_02CLK = 0x1,         ///< Number of clocks is 2
  seQSPI_01CLK = 0x0,         ///< Number of clocks is 1
} seQSPI_Clocks;

typedef enum {
  seQSPI_MSB_FST  = 0,         ///< Specify the data format (input/output permutation) MSB first.
  seQSPI_LSB_FST  = 1,         ///< Specify the data format (input/output permutation) LSB first.
} seQSPI_Format;

typedef enum {
  seQSPI_POL_LOW  = 0,         ///< Output is low when clock is off.
  seQSPI_POL_HIGH = 1,         ///< Output is high when clock is off.
} seQSPI_Polarity;

typedef enum {
  seQSPI_PH_RISE = 0,          ///< Triggers on positive edge of clock.
  seQSPI_PH_FALL = 1,          ///< Triggers on negative edge of clock.
} seQSPI_Phase;

typedef enum {
  seQSPI_MODE_SLAVE  = 0,      ///< Specifies the QSPI mode slave.
  seQSPI_MODE_MASTER = 1,      ///< Specifies the QSPI mode master.
} seQSPI_OperMode;

typedef enum {
  seQSPI_MODE_SINGLE  = 0,     ///< Single transfer mode.
  seQSPI_MODE_DUAL    = 1,     ///< Dual transfer mode.
  seQSPI_MODE_QUAD    = 2,     ///< Quad transfer mode.
} seQSPI_TransferMode;

typedef enum {
    seQSPI_BSY    = 0x0080U,     ///< Transfer busy/slave selected.
    seQSPI_MMABSY = 0x0040U,     ///< MMA mode access status
    seQSPI_OEIF   = 0x0008U,     ///< Overrun error interrupt.
    seQSPI_TENDIF = 0x0004U,     ///< End-of-transmission interrupt.
    seQSPI_RBFIF  = 0x0002U,     ///< Receive buffer full interrupt.
    seQSPI_TBEIF  = 0x0001U,     ///< Transmit buffer empty interrupt.
    seQSPI_ALL_IF = seQSPI_OEIF | seQSPI_TENDIF | seQSPI_RBFIF | seQSPI_TBEIF  ///< All interrupt flags
} seQSPI_IntFlag;
//#define seQSPI_FLGS(a) ((seQSPI_InterruptFlags)((a))) ///< Combination of any of the seQSPI_IntFlag enumerations.

typedef enum {
  seQSPI_OEIE   = 0x0008U,     ///< Overrun error interrupt.
  seQSPI_TENDIE = 0x0004U,     ///< End-of-transmission interrupt.
  seQSPI_RBFIE  = 0x0002U,     ///< Receive buffer full interrupt.
  seQSPI_TBEIE  = 0x0001U,     ///< Transmit buffer empty interrupt.
  seQSPI_ALL_IE = seQSPI_OEIE | seQSPI_TENDIE | seQSPI_RBFIE | seQSPI_TBEIE  ///< All interrupt enables
} seQSPI_Interrupt;

typedef enum {
  seQSPI_Output = 0,           ///< Specifies IO direction as output.
  seQSPI_Input  = 1,           ///< Specifies IO direction as input.
} seQSPI_IO;

typedef enum {
  seQSPI_24BIT_ADDR = 0,       ///< Specifies IO direction as output.
  seQSPI_32BIT_ADDR = 1,       ///< Specifies IO direction as input.
} seQSPI_AddrMode;

#define seQSPI_INTS(a) ((seQSPI_Interrupts)((a))) ///< Combination of any of the seQSPI_Interrupt enumerations.

/**
  * @}
  */


/** @defgroup QSPI_Types
  * @{
  */


/**
  * @brief  QSPI Init structure definition
  */

typedef struct {
    seQSPI_Clocks          CHDL; ///< Set a number of clocks to drive the serial data lines.
    seQSPI_Clocks          CHLN; ///< Set a number of clocks for data transfer.
    seState                PUEN; ///< Set input pin pull-up/down.
    seState            NOCLKDIV; ///< Set master mode operating clock.
    seQSPI_Format        LSBFST; ///< Set MSB first/LSB first.
    seQSPI_Phase           CPHA; ///< Set clock phase.
    seQSPI_Polarity        CPOL; ///< Set clock polarity.
    seQSPI_OperMode         MST; ///< Set master/slave mode.
    seQSPI_TransferMode    TMOD; ///< Set transfer mode (single/dual/quad).

    //====== If DMA transfer use ======//
    seQSPI_Clocks          TCSH; ///< Set slave select signal negation period.
    uint16_t              RMADR; ///< Set an offset, remapping start address high reg.
    seQSPI_Clocks         DUMDL; ///< Set dummy cycle drive length.
    seQSPI_Clocks         DUMLN; ///< Set dummy cycle length.
    seQSPI_TransferMode DATTMOD; ///< Set data cycle transfer mode.
    seQSPI_TransferMode DUMTMOD; ///< Set dummy cycle transfer mode.
    seQSPI_TransferMode ADRTMOD; ///< Set address cycle transfer mode.
    seQSPI_AddrMode      ADRCYC; ///< Set 24- or 32-bit address cycle.
    uint8_t              XIPACT; ///< Set XIP activation mode byte.
    uint8_t              XIPEXT; ///< Set XIP termination mode byte.

    //====== If interrupt use ======//
    seQSPI_Interrupt       INTE; ///< Set interrupt enable

} seQSPI_InitTypeDef;


/**
  * @}
  */

/** @defgroup QSPI_Functions
  * @{
  */


/**
  * @brief  This function initializes the QSPI peripheral as Master according to the specified
  *   parameters in the QSPI_InitStruct.
  * @param  QSPI_InitStruct: pointer to a @ref seQSPI_InitTypeDef structure that
  *   contains the configuration information for the specified QSPI peripheral.
  * @retval None
  */
void seQSPI_InitStructForMaster( seQSPI_InitTypeDef* QSPI_InitStruct );


/**
  * @brief  This function initializes the QSPI peripheral as Slave according to the specified
  *   parameters in the QSPI_InitStruct.
  * @param  QSPI_InitStruct: pointer to a @ref eQSPI_InitTypeDef structure that
  *   contains the configuration information for the specified QSPI peripheral.
  * @retval None
  */
void seQSPI_InitStructForSlave( seQSPI_InitTypeDef* QSPI_InitStruct );


/**
  * @brief  This function initializes the QSPIx peripheral according to the
  *         parameters in the QSPI_InitStruct.
  * @param  QSPI_InitStruct: pointer to a @ref seQSPI_InitTypeDef structure that
  *         contains the configuration information for the specified QSPI peripheral.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_Init( seQSPI_InitTypeDef* QSPI_InitStruct );

/**
  * @brief  This function starts or enables the specified QSPI channel.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_Start( void );

/**
  * @brief  This function stops or disables the QSPI channel.
  * @retval None
  */
void seQSPI_Stop( void );

/**
  * @brief  Performs a software reset of the QSPI interface.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_Reset( void );

/**
  * @brief  This function sends data through the QSPIx peripheral.
  * @param  data: Pointer to data to be transmitted.
  * @param  size: Data size in number of half words.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_TxHWords( uint16_t data[], uint32_t size );


/**
  * @brief  This function sends data by DMA through the QSPIx peripheral.
  * @note: DMAC must be initialized prior calling of this function.
  * DMA Channel 0 is used by this function. It must be available for the duration of the function call.
  * @param  data: Pointer to data to be transmitted.
  * @param  size: Data size in number of half words.
  * @param  localaddr: Local RAM address to copy data from host.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_DmaTxHWords( uint16_t data[], uint32_t size, uint32_t localaddr );


/**
  * @brief  This function sends data through the QSPIx peripheral.
  * @param  data: Pointer to data to be transmitted.
  * @param  size: Data size in number of bytes.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_TxBytes( uint8_t data[], uint32_t size );


/**
  * @brief  This function sends single data value through the QSPIx peripheral.
  * @param  value: Data value to be transmitted.
  * @param  count: Data size in number of bytes.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_TxValue( uint8_t value, uint32_t count );

/**
  * @brief  This function returns the received data by the QSPIx peripheral.
  * @param  data: The received data pointer.
  * @param  size: Data size in number of half words.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_RxHWords( uint16_t data[], uint32_t size );

/**
  * @brief  This function returns the received data from the QSPIx peripheral by DMA.
  * @note: DMAC must be initialized prior calling of this function.
  * DMA Channels 1,3 are used by this function. They must be available for duration of the function call.
  * @param  data: The received data pointer.
  * @param  size: Data size in number of half words.
  * @param  localaddr: Local RAM address to copy data from host.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_DmaRxHWords( uint16_t data[], uint32_t size, uint32_t localaddr );

/**
  * @brief  This function returns the received data from the QSPIx peripheral.
  * @param  data: The received data pointer.
  * @param  size: Data size in number of bytes.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_RxBytes( uint8_t data[], uint32_t size );

/**
  * @brief  This function returns the received data by DMA from the QSPIx peripheral in memory mapped mode.
  * @note: DMAC must be initialized prior calling of this function.
  * DMA Channel 2 is used by this function. They must be available for duration of the function call.
  * @param  data: The received data pointer.
  * @param  size: Data size in number of long words.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_DmaRxMmaWords( uint32_t offset, uint32_t data[], uint32_t size );

/**
  * @brief  This function sets QSPI mode (single, dual, quad).
  * @param  mode: Sets single, dual or quad, see @ref seQSPI_TransferMode.
  * @param  chln: Number of clocks (seQSPI_01CLK is prohibited) for data transfer, see @ref seQSPI_Clocks.
  * @param  chdl: Number of clocks to drive the serial data lines, see @ref seQSPI_Clocks.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_SetMode( seQSPI_TransferMode mode, seQSPI_Clocks chln, seQSPI_Clocks chdl );

/**
  * @brief  This function enables QSPI channel interrupt.
  * @param  irq: Interrupt to be enabled, see @ref seQSPI_Interrupt.
  * @retval None
  */
void seQSPI_EnableInt( seQSPI_Interrupt irq );

/**
  * @brief  This function disables QSPI channel interrupt.
  * @param  irq: Interrupt to be disabled, see @ref seQSPI_Interrupt.
  * @retval None
  */
void seQSPI_DisableInt( seQSPI_Interrupt irq );

/**
  * @brief  This function gets QSPI channel interrupt flag.
  * @param  flag: Interrupt to check, see @ref seQSPI_IntFlag.
  * @retval seInterruptStatus see @ref seInterruptStatus.
  */
seInterruptStatus seQSPI_GetIntFlag( seQSPI_IntFlag flag );

/**
  * @brief  This function clears QSPI channel interrupt flag.
  * @param  flag: Interrupt to clear, see @ref seQSPI_IntFlag.
  * @retval None
  */
void seQSPI_ClearIntFlag( seQSPI_IntFlag flag );

/**
  * @brief  This function configures QSPICLKn frequency [Hz] (= baud rate [bps]).
  * @param  speed: QSPICLKn frequency [Hz].
  * @retval Status: can be a value of @ref seStatus
  */
seStatus  seQSPI_SetBusSpeed( uint32_t speed );

/**
  * @brief  This function discovers QSPICLKn frequency [Hz] (= baud rate [bps]).
  * @retval QSPICLKn frequency [Hz].
  */
uint32_t seQSPI_GetBusSpeed( void );

/**
  * @brief  This function sets the mapping mode for reading from External flash.
  * @param  raddr: Remapping address.
  * @param  rcmd: Read command defined by flash type used for mapping.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_SetMasterRxMMA( uint32_t raddr, uint16_t flash_rcmd );

/**
  * @brief  This function ends mapping mode for reading from External flash.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_ClearMasterRxMMA( void );

/**
  * @brief  This function terminates QSPI module.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seQSPI_TermMasterTx( void );

/**
  * @brief  This function configures I/O for Rx or Tx direction (Master mode only).
  * @param  QSPIx: QSPIx_CH0 to select the QSPI peripheral.
  * @retval None
  */
void seQSPI_SetIO( seQSPI_IO direction );

/**
  * @brief  This function  asserts Chip Slave Select 0 (Master mode only)
  *         It controls a native QSPI slave select line that can be used with MMA.
  * @retval None
  */
void seQSPI_ASSERT_MST_CS0( void );

/**
  * @brief  This function negates Chip Slave Select 0 (Master mode only)
  *         It controls a native QSPI slave select line that can be used with MMA.
  * @retval None
  */
void seQSPI_NEGATE_MST_CS0( void );

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

#endif // SE_QSPI_H
