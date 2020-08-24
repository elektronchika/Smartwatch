/**
******************************************************************************
* @file    se_clg.h
* @author  Epson
* @version V1.0
* @date    18-September-2017
* @brief   This file contains the function prototypes for the CLG library.
*
* Despite IOSC control being moved to the host asynchronous register control
* of this clock source has been retained in the CLG library.
******************************************************************************
* @attention
*
* Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
******************************************************************************
*/

#ifndef SE_CLG_H
#define SE_CLG_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup sePeriphLibrary
  * @{
  */

/** @defgroup CLG
  * @{
  * @brief CLG module is the clock generator that controls the clock sources.
  */

/** @defgroup CLG_Constants
  * @{
  */

typedef enum {
    seCLG_IOSC  = 0,              ///< IOSC is the CLK source.
    seCLG_OSC1  = 1               ///< OSC1 is the CLK source.
} seCLG_ClkSrc;

typedef enum {
    seCLG_IOSC_CLKDIV_1   = 0,    ///< IOSC division ratio of 1/1.
    seCLG_IOSC_CLKDIV_2   = 1,    ///< IOSC division ratio of 1/2.
    seCLG_IOSC_CLKDIV_4   = 2,    ///< IOSC division ratio of 1/4.
    seCLG_IOSC_CLKDIV_8   = 3     ///< IOSC division ratio of 1/8.
} seCLG_IOSC_ClkDiv;

typedef enum {
    seCLG_OSC1_CLKDIV_1   = 0,    ///< OSC1 division ratio of 1/1.
    seCLG_OSC1_CLKDIV_2   = 1,    ///< OSC1 division ratio of 1/2.
} seCLG_OSC1_ClkDiv;

typedef enum {
  seCLG_OSC1STPIF  = 0x0020U,     ///< OSC1 stop interrupt flag.
  seCLG_OSC1STAIF  = 0x0002U      ///< OSC1 stable interrupt flag.
} seCLG_IntFlag;



typedef enum {
  seCLG_OSC1STPIE  = 0x0020U,     ///< OSC1 stop interrupt enable.
  seCLG_OSC1STAIE  = 0x0002U,     ///< OSC1 stable interrupt enable.
} seCLG_Interrupt;


typedef enum {
    seCLG_IOSC_IOSCFQ_8  = 0,     ///< IOSC frequency is 8 MHz.
    seCLG_IOSC_IOSCFQ_12 = 1,     ///< IOSC frequency is 12 MHz.
    seCLG_IOSC_IOSCFQ_16 = 2,     ///< IOSC frequency is 16 MHz.
    seCLG_IOSC_IOSCFQ_20 = 3      ///< IOSC frequency is 20 MHz.
} seCLG_IOSC_IoscFq;

#define seCLG_IOSC_FREQUENCY_8   ( 8000000UL)     ///< 8MHz frequency value
#define seCLG_IOSC_FREQUENCY_12  (12000000UL)     ///< 12MHz frequency value
#define seCLG_IOSC_FREQUENCY_16  (16000000UL)     ///< 16MHz frequency value
#define seCLG_IOSC_FREQUENCY_20  (20000000UL)     ///< 20MHz frequency value

#define seCLG_INTOSC1_FREQUENCY  (32000UL)        ///< Internal OSC1 32kHz frequency value
#define seCLG_EXTOSC1_FREQUENCY  (32768UL)        ///< External OSC1 32.768kHz frequency value

/**
  * @}
  */  // CLG_Constants



/** @defgroup CLG_Types
  * @{
  */

typedef union {
  seCLG_IOSC_ClkDiv  IOSC_ClkDiv;        ///< IOSC clock divide
  seCLG_OSC1_ClkDiv  OSC1_ClkDiv;        ///< OSC1 clock divide
} seCLG_ClkDiv;

/**
  * @brief  CLG Init structure definition.
  */

typedef struct
{
    seCLG_ClkSrc ClkSrc;                 ///< Specifies the clock source.
    seCLG_ClkDiv ClkDiv;                 ///< Specifies the clock source divide.
} seCLG_InitTypeDef;


/**
  * @}
  */ // CLG_Types


/** @defgroup CLG_Functions
  * @{
  */

/**
  * @brief  Initializes the CLG peripheral according to the specified
  *   parameters in the InitStruct parameter.
  * @param  InitStruct: Pointer to a @ref seCLG_InitTypeDef structure that
  *   contains the configuration information for the specified CLG peripheral.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seCLG_Init( seCLG_InitTypeDef * InitStruct );


/**
  * @brief  Sets the IOSC frequency.
  * @param freq: New frequency can be a value of @ref seCLG_IOSC_IoscFq.
  * @retval None
  */
void seCLG_SetIoscFreqSel( seCLG_IOSC_IoscFq freq );


/**
  * @brief  Gets the IOSC frequency.
  * @retval Freq: Actual value of IOSC frequency in Hz.
  */
uint32_t seCLG_GetIoscFreqSel( void );


/**
  * @brief  Starts Clock.
  * @param  clock: The clock to start (of type @ref seCLG_ClkSrc).
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seCLG_Start( seCLG_ClkSrc clock );


/**
  * @brief  Stops Clock.
  * @param  clock: The clock @ref seCLG_ClkSrc to stop (of type @ref seCLG_ClkSrc).
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seCLG_Stop( seCLG_ClkSrc clock );


/**
  * @brief  Enables/Disables OSC1 oscillator stop detection function.
  * @note The oscillation stop detection function restarts the OSC1 oscillator
  * circuit when it detects the oscillation was stopped (due to under adverse
  * environments, etc.).
  * @param  clock: This parameter can be a value of @ref seCLG_ClkSrc.
  * @param  StopDetectionEn: @ref seState. This enables/disables oscilator stop detection function.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seCLG_SetStopDetection( seCLG_ClkSrc clock, seState StopDetectionEn );


/**
  * @brief  Set the manual trim adjust value.
  * @param  clock: This parameter can be a value of @ref seCLG_ClkSrc.
  * @param  trim:  A value between 0 and 63.
  * @retval None
  */
void seCLG_SetTrimAdjust( seCLG_ClkSrc clock, uint16_t trim );

/**
  * @brief  Enable CLG interrupt(s).
  * @param  irq: This parameter can be a value of @ref seCLG_Interrupt.
  * @retval None
  */
void seCLG_EnableInt( seCLG_Interrupt irq );

/**
  * @brief  Disable CLG interrupt(s).
  * @param  irq: This parameter can be a value of @ref seCLG_Interrupt.
  * @retval None
  */
void seCLG_DisableInt( seCLG_Interrupt irq );

/**
  * @brief  Returns CLG interrupt flag(s).
  * @param flag: This parameter can be a value of @ref seCLG_IntFlag.
  * @retval InterruptStatus: can be a value of @ref seInterruptStatus
  */
seInterruptStatus seCLG_GetIntFlag( seCLG_IntFlag flag );

/**
  * @brief  Clears CLG interrupt(s).
  * @param  flag: This parameter can be a value of @ref seCLG_IntFlag.
  * @retval None
  */
void seCLG_ClearIntFlag( seCLG_IntFlag flag );


/**
  * @}
  */ // CLG_Functions

/**
  * @}
  */ // CLG

/**
  * @}
  */ // sePeriphLibrary

#ifdef __cplusplus
}
#endif
#endif	// SE_CLG_H
