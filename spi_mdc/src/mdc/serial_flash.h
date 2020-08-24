/**
  ******************************************************************************
  * @file    serial_flash.h
  * @author  Epson
  * @version V1.0
  * @date    11-April-2018
  * @brief   This file provides routines for accessing the onboard serial flash
  *          on the S5U13C00P00Cx00 board.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SERIAL_FLASH_H
#define SERIAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @defgroup seSerFlashLib seSerFlashLib
  * @{
  * @brief The S1D13C00 Serial Flash library provides routines to access a serial flash device attached to the QSPI
  * peripheral of the S1D13C00.
  */ 
   
/** @defgroup SERFLASH_Constants
  * @{
  */ 
   
typedef enum {
   FLMODE_SINGLE                = 0U,                 ///< Single data line mode
   FLMODE_DUAL                  = 1U,                 ///< Dual data lines mode
   FLMODE_QUAD                  = 2U                  ///< Quad data lines mode
} seMDC_SERFLASH_DATAMODE;


#define FLASH_WRITE_IN_PROGRESS         0x01          ///< Serial flash write in progress 

#define S25FL127S_MFGID                 0x01          ///< Cypress/Spansion S25FL127S Manufacturer ID
#define S25FL127S_DEVID                 0x2018        ///< Cypress/Spansion S25FL127S Device ID
#define IS25LP128_MFGID                 0x9D          ///< ISSI IS25LP128 Manufacturer ID
#define IS25LP128_DEVID                 0x6018        ///< ISSI IS25LP128 Device ID
#define MX25R6435F_MFGID                0xC2          ///< Macronix MX25R6435F Manufacturer ID
#define MX25R6435F_DEVID                0x2817        ///< Macronix MX25R6435F Device ID

#define CMD_READ_ID_SINGLE_MODE         0x9F          ///< Read ID command
#define CMD_READ_STATUS_REG             0x05          ///< Read Status Register command
#define CMD_WRITE_STATUS_REG            0x01          ///< Write Status Register command
#define CMD_READ_CONFIG_REG_S25FL127S   0x35          ///< Read Configuration Register command for S25FL127S
#define CMD_READ_CONFIG_REG_MX25R6435F  0x15          ///< Read Configuration Register command for MX25R6435F
#define CMD_WRITE_ENABLE                0x06          ///< Write Enable command
#define CMD_CHIP_ERASE                  0xC7          ///< Chip Erase command
#define CMD_BLOCK_ERASE                 0xD8          ///< Block Erase commmand
#define CMD_READ_SINGLE_IO              0x0B          ///< Read Single I/O command
#define CMD_READ_DUAL_IO                0xBB          ///< Read Dual I/O command
#define CMD_READ_QUAD_IO                0xEB          ///< Read Quad I/O command
#define CMD_PAGE_PROG                   0x02          ///< Page Program Single I/O command
#define CMD_QUAD_PROG                   0x32          ///< Page Program Quad Output command
#define CMD_QUAD_PROG_IO                0x38          ///< Page Program Quad I/O command
#define CMD_ENTER_DEEP_PWRDOWN          0xB9          ///< Enter Deep Power Down command
#define CMD_ISSI_EXIT_DEEP_PWRDOWN      0xAB          ///< Exit Deep Power Down command for ISSI

#define CMD_QIOEN                       0x35          ///< Quad I/O Enable command for IS25LP128
#define CMD_QIODI                       0xF5          ///< Quad I/O Disable command for IS25LP128

/**
  * @}
  */   // SERFLASH_Constants


/** @defgroup SERFLASH_Functions
  * @{
  */


/**
  * @brief  Routine to read the Manufacturer and Device ID of the serial flash device
  * @param  mfc_id: Pointer to 8-bit variable to store the Manufacturer ID read from device
  * @param  dev_id: Pointer to 16-bit variable to store the Device ID read from device
  * @retval Status: can be a value of @ref seStatus
  */
seStatus ReadFlashID( uint8_t * mfc_id, uint16_t * dev_id );

/**
  * @brief  Read the ID of the serial flash device and identify its manufacturer
  * in order to set up parameters for other routines in the library.
  * @param  mfc_id: Pointer to 8-bit variable to store the Manufacturer ID read from device
  * @param  dev_id: Pointer to 16-bit variable to store the Device ID read from device
  * @retval Status: can be a value of @ref seStatus
  */
seStatus IdentifyFlash( uint8_t * mfc_id, uint16_t * dev_id );

/**
  * @brief  Set the serial flash data transfer mode (single/dual/quad).
  * @param  mode: Data mode (of type @ref seMDC_SERFLASH_DATAMODE) to set for the serial flash.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus SetFlashMode( seMDC_SERFLASH_DATAMODE mode );

/**
  * @brief  Get the serial flash data transfer mode (single/dual/quad).
  * @retval datamode: value of serial flash data mode of type @ref seMDC_SERFLASH_DATAMODE
  */
seMDC_SERFLASH_DATAMODE GetFlashMode( void );

/**
  * @brief  Enable writing of serial flash.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus EnableFlashWrite( void );

/**
  * @brief  Erase whole serial flash.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus EraseFlash( void );

/**
  * @brief  Erase a sector in serial flash.
  * @param  addr: base address of the sector
  * @retval Status: can be a value of @ref seStatus
  */
seStatus EraseFlashSector( uint32_t addr );

/**
  * @brief  Program a page in serial flash.
  * @param  flash_addr: base address of the page
  * @param  data: pointer to array of bytes to write to the page
  * @param  nBytes: number of bytes to write (must be less than page size)
  * @retval Status: can be a value of @ref seStatus
  */
seStatus ProgramFlash( uint32_t flash_addr, uint8_t data[], uint32_t nBytes );

/**
  * @brief  Read data from serial flash.
  * @param  flash_addr: base address of the block of data to read
  * @param  data: pointer to array of bytes to store the data read
  * @param  nBytes: number of bytes to read
  * @retval Status: can be a value of @ref seStatus
  */
seStatus ReadFlash( uint32_t flash_addr, uint8_t data[], uint32_t nBytes );

/**
  * @brief  Wait while serial flash is busy.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus WaitFlashBusy( void );

/**
  * @brief  Read serial flash Status Register.
  * @param  val: pointer to 8-bit value to store status read from serial flash
  * @retval Status: can be a value of @ref seStatus
  */
seStatus ReadFlashStatusReg( uint8_t * val );

/**
  * @brief  Write serial flash Status Register.
  * @param  val: 8-bit value to write to Status Register 
  * @retval Status: can be a value of @ref seStatus
  */
seStatus WriteFlashStatusReg( uint8_t val );

/**
  * @brief  Read serial flash Configuration Register.
  * @param  cfg1: pointer to 8-bit value to store first configuration byte read from serial flash
  * @param  cfg2: pointer to 8-bit value to store second configuration byte read from serial flash
  * @retval Status: can be a value of @ref seStatus
  */
seStatus ReadFlashCfgReg( uint8_t * cfg1, uint8_t * cfg2 );

/**
  * @brief  Write serial flash Configuration Register.
  * @param  cfg1: 8-bit value to write to first configuration byte of serial flash
  * @param  cfg2: 8-bit value to write to second configuration byte of serial flash
  * @retval Status: can be a value of @ref seStatus
  */
seStatus WriteFlashCfgReg( uint8_t cfg1, uint8_t cfg2 );

/**
  * @brief  Put serial flash in deep power down state.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus FlashEnterDeepPowerDown( void );

/**
  * @brief  Exit the serial flash from deep power down state.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus FlashExitDeepPowerDown( void );

/**
  * @brief  Cancel XIP mode of serial flash.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus FlashCancelXIP( void );

/**
  * @brief  Program a page in serial flash using DMAC.
  * @param  flash_addr: base address of the page
  * @param  data: pointer to array of 16-bit word to write to the page
  * @param  nWords: number of 16-bit words to write (must be less than page size)
  * @retval Status: can be a value of @ref seStatus
  */
seStatus ProgramFlashDma( uint32_t flash_addr, uint16_t data[], uint32_t nWords );

/**
  * @brief  Read data from serial flash.
  * @param  flash_addr: base address of the block of data to read
  * @param  data: pointer to array of 16-bit words to store the data read
  * @param  nWords: number of 16-bit words to read
  * @retval Status: can be a value of @ref seStatus
  */
seStatus ReadFlashDma( uint32_t flash_addr, uint16_t data[], uint32_t nWords );

/**
  * @}
  */   // SERFLASH_Functions

/**
  * @}
  */   // seSerFlashLib

#ifdef __cplusplus
}
#endif
#endif // SERIAL_FLASH_H
