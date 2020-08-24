/**
  ******************************************************************************
  * @file    se_remc.h
  * @author  Epson
  * @version V1.0
  * @date    31-October-2017
  * @brief   This file contains the function prototypes for the REMC library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SE_REMC_H
#define SE_REMC_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/** @addtogroup sePeriphLibrary
  * @{
  */

/** 
  * @defgroup REMC
  * @{
  * @brief The REMC circuit generates infrared remote control output signals.
  * This circuit can also be applicable to an EL lamp drive circuit by adding a simple external circuit.
  */ 

/** @defgroup REMC_Constants
  * @{
  */

typedef enum {
  seREMC_IOSC_CLKDIV_1 = 0,                     ///< IOSC division ratio is 1/1.
  seREMC_IOSC_CLKDIV_2 = 1,                     ///< IOSC division ratio is 1/2.
  seREMC_IOSC_CLKDIV_4 = 2,                     ///< IOSC division ratio is 1/4.
  seREMC_IOSC_CLKDIV_8 = 3,                     ///< IOSC division ratio is 1/8.
  seREMC_IOSC_CLKDIV_16 = 4,                    ///< IOSC division ratio is 1/16.
  seREMC_IOSC_CLKDIV_32 = 5,                    ///< IOSC division ratio is 1/32.
  seREMC_IOSC_CLKDIV_64 = 6,                    ///< IOSC division ratio is 1/64.
  seREMC_IOSC_CLKDIV_128 = 7,                   ///< IOSC division ratio is 1/128.
  seREMC_IOSC_CLKDIV_256 = 8,                   ///< IOSC division ratio is 1/256.
  seREMC_IOSC_CLKDIV_512 = 9,                   ///< IOSC division ratio is 1/512.
  seREMC_IOSC_CLKDIV_1024 = 10,                 ///< IOSC division ratio is 1/1024.
  seREMC_IOSC_CLKDIV_2048 = 11,                 ///< IOSC division ratio is 1/2048.
  seREMC_IOSC_CLKDIV_4096 = 12,                 ///< IOSC division ratio is 1/4096
  seREMC_IOSC_CLKDIV_8192 =  13,                ///< IOSC division ratio is 1/8192.
  seREMC_IOSC_CLKDIV_16384 = 14,                ///< IOSC division ratio is 1/16384.
  seREMC_IOSC_CLKDIV_32768 = 15,                ///< IOSC division ratio is 1/32768.
} seREMC_IOSC_ClkDiv;


typedef enum {
  seREMC_OSC1_CLKDIV_1 = 0,                     ///< OSC1 division ratio is 1/1.
  seREMC_OSC1_CLKDIV_2 = 1,                     ///< OSC1 division ratio is 1/2.
  seREMC_OSC1_CLKDIV_4 = 2,                     ///< OSC1 division ratio is 1/4.
  seREMC_OSC1_CLKDIV_8 = 3,                     ///< OSC1 division ratio is 1/8.
  seREMC_OSC1_CLKDIV_16 = 4,                    ///< OSC1 division ratio is 1/16.
  seREMC_OSC1_CLKDIV_32 = 5,                    ///< OSC1 division ratio is 1/32.
  seREMC_OSC1_CLKDIV_64 = 6,                    ///< OSC1 division ratio is 1/64.
  seREMC_OSC1_CLKDIV_128 = 7,                   ///< OSC1 division ratio is 1/128.
  seREMC_OSC1_CLKDIV_256 = 8,                   ///< OSC1 division ratio is 1/256.
} seREMC_OSC1_ClkDiv;

typedef  seCLG_ClkSrc seREMC_ClkSrc;            ///< Type seREMC_ClkSrc is same as seCLG_ClkSrc


typedef enum {
    seREMC_APIF  = 1,                           ///< Compare AP interrupt.
    seREMC_DBIF  = 2,                           ///< Compare DB interrupt.
    seREMC_ALL_INT  = 3                         ///< Compare all interrupts.
} seREMC_Interrupt;

/**
  * @}
  */


/** @defgroup REMC_Types
  * @{
  */


/**
  * @brief  REMC Init structure definition
  */
typedef struct
{
  seREMC_ClkSrc ClkSrc;                 ///< Specifies the timer clock source selection.
  uint16_t ClkDivider;                  ///< Specifies the prescaler value used to divide the REMC clock.
  seState StopInSleep;                  ///< Specifies if operation stops or continues in sleep mode.
  uint16_t carr;                        ///< Carrier Waveform Register
} seREMC_InitTypeDef;


/**
  * @}
  */

/** @defgroup REMC_Functions
  * @{
  */

/**
  * @brief  Initializes the REMC peripheral according to the specified
  * parameters in the seREMC_InitStruct. Stop REMC before initializing.
  * @param  InitStruct: pointer to a initialization structure that
  * contains the configuration information for the specified REMC peripheral.
  * @ref seREMC_InitTypeDef.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seREMC_Init( seREMC_InitTypeDef* InitStruct );

/**
  * @brief  Configures REMC timer clock source and clock divider.
  * @param  clock: This parameter can be a value of @ref seREMC_ClkSrc.
  * @param  divider: This parameter can be a value of @ref seREMC_ClkDiv.
  * @retval None
  */
void seREMC_ConfigureClock( seREMC_ClkSrc clock, uint16_t divider );

/**
  * @brief  Starts REMC by supplying the operating clock.
  * @param  aplen: Sets data bit active pulse length.
  * @param  dblen: Set data bit length.
  * @retval None
  */
void seREMC_Start( uint16_t aplen, uint16_t dblen );

/**
  * @brief  Stops the REMC module.
  * @retval None
  */
void seREMC_Stop( void );


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

#endif /*SE_REMC_H */
