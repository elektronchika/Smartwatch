/**
  ******************************************************************************
  * @file    se_common.h
  * @author  Epson
  * @date    21-November-2015
  * @brief   This file contains common definitions and function prototypes
  *          for the S1D13C00 peripheral library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017 - 2018. All rights reserved.
  ******************************************************************************
  */
#ifndef SE_COMMON_H
#define SE_COMMON_H

/*! \mainpage S1D13C00 Software Libraries
   \section periph_sec sePeriphLibrary
   The S1D13C00 contains several peripherals including the Memory Display Controller (MDC).
   A set of library routines is available for each peripheral.
   The sePeriphLibrary/ folder contains low-level routines for each peripheral in the S1D13C00.
   All the routines access the S1D13C00 using "seS1D13C00Write*()" and "seS1D13C00Read*()" routines
   to access the registers and memory of the S1D13C00.

   \section gfx_sec seGraphicsLibrary
   The seGraphicsLibrary/ folder contains graphics routines for drawing and display text on the
   memory display.

   \section serflash_sec seSerFlashLib
   The seSerFlashLib/ folder contains routines for accessing a serial flash memory device which is
   attached to the QSPI interface of the S1D13C00.

  * @defgroup sePeriphLibrary sePeriphLibrary
  * @{
  * @brief The S1D13C00 Peripheral library is a set of drivers offering customers uniform access to chip peripherals.
  *        The driver is responsible for modules initialization, features configuration, modules access.
  */


/** @defgroup Common
  * @{
  * @brief Common definitions, data structures, and functions prototypes for the peripheral library.
  */

/** @defgroup Common_Constants
  * @{
  */

    
typedef enum {
  seDISABLE = 0,                                    ///< Module Disable state.
  seENABLE = !seDISABLE                             ///< Module Enable state.
} seState;

typedef enum {
  seSTATUS_NG = 0,                                  ///< Represents state of failure (No Good).
  seSTATUS_OK = !seSTATUS_NG                        ///< Represents state of success (Ok).
} seStatus;

typedef enum {
  seINTERRUPT_NOT_OCCURRED = 0,                     ///< Interrupt did not occur.
  seINTERRUPT_OCCURRED = !seINTERRUPT_NOT_OCCURRED  ///< Interrupt did occur.
} seInterruptStatus;

typedef enum {
  seWRITE_PROTECT_ON  = 0x00,                       ///< Write-protect on.
  seWRITE_PROTECT_OFF = 0x96                        ///< Write-Protect off.
} seWriteProtect;

/**
  * @}
  */ // Common_Constants



/** @defgroup Common_Functions
  * @{
  */


/**
  * @brief Registers with "R/WP" designation are write-protected.
  *        Write protection can be enabled or disabled using this routine.
  * @param protect: On or off value, can be a value of @ref seWriteProtect
  * @retval None
  */
void seProtectSys( seWriteProtect protect );

/**
  * @brief This routine logically ANDs the specified bits with the contents of the specified register.
  * @param addr: Address of the register to change
  * @param value: Bit value to be ANDed with the register contents
  * @retval None
  */
void seAndBits16( uint32_t addr, uint16_t value );

/**
  * @brief This routine logically ORs the specified bits with the contents of the specified register.
  * @param addr: Address of the register to change
  * @param value: Bit value to be ORed with the register contents
  * @retval None
  */
void seOrBits16( uint32_t addr, uint16_t bits );


/**
  * @brief This routine sets the value of one or more bits in an 8-bit register.
  * @param addr: Address of the register to change
  * @param bits: Mask of the bits to change
  * @param value: Value to set the bits to
  * @retval None
  */
void seSetBits8( uint32_t addr, uint8_t bits, uint8_t value );


/**
  * @brief This routine sets the value of one or more bits in a 16-bit register.
  * @param addr: Address of the register to change
  * @param bits: Bit mask of the bits to change
  * @param value: Value to set the bits to
  * @retval None
  */
void seSetBits16( uint32_t addr, uint16_t bits, uint16_t value );


/**
  * @brief This routine sets the value of one or more bits in a 32-bit register.
  * @param addr: Address of the register to change
  * @param bits: Bit mask of the bits to change
  * @param value: Value to set the bits to
  * @retval None
  */
void seSetBits32( uint32_t addr, uint32_t bits, uint32_t value );


/**
  * @brief This routine clamps the given 16-bit value between a low and high value.
  * @param value: The given value to be clamped
  * @param low: Minimum value
  * @param high: Maximum value
  * @retval clampval: clamped value 
  */
int16_t seClamp16( int16_t value, int16_t low, int16_t high );


/**
  * @brief This routine clamps the given 32-bit value between a low and high value.
  * @param value: The given value to be clamped
  * @param low: Minimum value
  * @param high: Maximum value
  * @retval clampval: clamped value
  */
int32_t seClamp32( int32_t value, int32_t low, int32_t high );

/**
  * @brief This routine converts an 8-bit value to 2-digit hex string.
  * @param value: The given value to be converted
  * @param outstr: Pointer to output string buffer
  * @retval outstr: Pointer to output string buffer
  */
char *uint8tohexstr(uint8_t value, char *outstr);

/**
  * @brief This routine converts a 16-bit value to 4-digit hex string.
  * @param value: The given value to be converted
  * @param outstr: Pointer to string buffer
  * @retval outstr: Pointer to output string buffer
  */
char *uint16tohexstr(uint16_t value, char *outstr);

/**
  * @brief This routine converts a 32-bit value to 8-digit hex string.
  * @param value: The given value to be converted
  * @param outstr: Pointer to string buffer
  * @retval outstr: Pointer to output string buffer
  */
char *uint32tohexstr(uint32_t value, char *outstr);

/**
  * @}
  */  // Common_Functions
/**
  * @}
  */ // Common

/**
  * @}
  */ // sePeriphLibrary

#endif //SE_COMMON_H
