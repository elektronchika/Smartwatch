/**
  ******************************************************************************
  * @file    se_t16.h
  * @author  Epson
  * @version V1.0
  * @date    26-September-2017
  * @brief   This file contains function prototypes for the T16 library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SE_T16_H
#define SE_T16_H

#ifdef __cplusplus
 extern "C" {
#endif
 

/** @addtogroup sePeriphLibrary
  * @{
  */

/** @defgroup T16
  * @{
  * @brief T16 is a 16 bit timer
  */ 

/** @defgroup T16_Constants 
  * @{
  */

typedef  seCLG_ClkSrc seT16_ClkSrc;             ///< Type seT16_ClkSrc is same as seCLG_ClkSrc

typedef enum {
  seT16_IOSC_CLKDIV_1 = 0,                      ///< IOSC division ratio is 1/1.
  seT16_IOSC_CLKDIV_2 = 1,                      ///< IOSC division ratio is 1/2.
  seT16_IOSC_CLKDIV_4 = 2,                      ///< IOSC division ratio is 1/4.
  seT16_IOSC_CLKDIV_8 = 3,                      ///< IOSC division ratio is 1/8.
  seT16_IOSC_CLKDIV_16 = 4,                     ///< IOSC division ratio is 1/16.
  seT16_IOSC_CLKDIV_32 = 5,                     ///< IOSC division ratio is 1/32.
  seT16_IOSC_CLKDIV_64 = 6,                     ///< IOSC division ratio is 1/64.
  seT16_IOSC_CLKDIV_128 = 7,                    ///< IOSC division ratio is 1/128.
  seT16_IOSC_CLKDIV_256 = 8,                    ///< IOSC division ratio is 1/256.
  seT16_IOSC_CLKDIV_512 = 9,                    ///< IOSC division ratio is 1/512.
  seT16_IOSC_CLKDIV_1024 = 10,                  ///< IOSC division ratio is 1/1024.
  seT16_IOSC_CLKDIV_2048 = 11,                  ///< IOSC division ratio is 1/2048.
  seT16_IOSC_CLKDIV_4096 = 12,                  ///< IOSC division ratio is 1/4096.
  seT16_IOSC_CLKDIV_8192 = 13,                  ///< IOSC division ratio is 1/8192.
  seT16_IOSC_CLKDIV_16384 = 14,                 ///< IOSC division ratio is 1/16384.
  seT16_IOSC_CLKDIV_32768 = 15                  ///< IOSC division ratio is 1/32768.
} seT16_IOSC_ClkDiv;

typedef enum {
  seT16_OSC1_CLKDIV_1 = 0,                      ///< OSC1 division ratio is 1/1.
  seT16_OSC1_CLKDIV_2 = 1,                      ///< OSC1 division ratio is 1/2.
  seT16_OSC1_CLKDIV_4 = 2,                      ///< OSC1 division ratio is 1/4.
  seT16_OSC1_CLKDIV_8 = 3,                      ///< OSC1 division ratio is 1/8.
  seT16_OSC1_CLKDIV_16 = 4,                     ///< OSC1 division ratio is 1/16.
  seT16_OSC1_CLKDIV_32 = 5,                     ///< OSC1 division ratio is 1/32.
  seT16_OSC1_CLKDIV_64 = 6,                     ///< OSC1 division ratio is 1/64.
  seT16_OSC1_CLKDIV_128 = 7,                    ///< OSC1 division ratio is 1/128.
  seT16_OSC1_CLKDIV_256 = 8,                    ///< OSC1 division ratio is 1/256.
} seT16_OSC1_ClkDiv;

typedef enum {
  seT16_RepeatMode,                             ///< Timer operates in Repeat Mode. Select this mode
                                                ///< to generate periodic underflow interrupts or when using
                                                ///< the timer to output a clock to a peripheral circuit.
  seT16_OneShotMode                             ///< Timer operates in One-shot Mode. Select this mode to
                                                ///< stop the counter after an interrupt has occurred once.
} seT16_CounterMode;

/**
  * @}
  */

/** @defgroup T16_Types
  * @{
  */


/**
  * @brief  T16 Init structure definition
  */
typedef struct {
  seT16_ClkSrc ClkSrc;                  ///< Specifies the timer clock source selection.
  uint16_t ClkDivider;                  ///< Specifies the prescaler value used to divide the T16 clock.
  seT16_CounterMode CounterMode;        ///< Specifies the counter mode.
  uint16_t Period;                      ///< Specifies the period value to be loaded into the counter.
} seT16_InitTypeDef;


/**
  * @}
  */

/** @defgroup T16_Functions
  * @{
  */

/**
  * @brief  Fills each seT16_InitTypeDef member with its default value.
  * @param  T16_InitStruct: pointer to an @ref seT16_InitTypeDef structure which will be initialized.
  * @retval None
  */
void seT16_InitStruct( seT16_InitTypeDef* T16_InitStruct );

/**
  * @brief  Initializes the T16 peripheral according to the specified
  *         parameters in the seT16_InitStruct.
  * @note   This function configures the module, and module interrupts.
  *         Interrupts are cleared but are not enabled. This function
  *         enables module by start supplying operating clock.
  * @param  T16BaseAddr: pointer to T16 peripheral bloc.
  * @param  InitStruct: pointer to a @ref seT16_InitTypeDef structure that
  *         contains the configuration information for the specified T16 peripheral.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seT16_Init( uint32_t T16BaseAddr, seT16_InitTypeDef* T16_InitStruct );

/**
  * @brief  Starts Timer channel.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval None
  */
void seT16_Start( uint32_t T16BaseAddr );

/**
  * @brief  Stops Timer channel.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval None
  */
void seT16_Stop( uint32_t T16BaseAddr );

/**
  * @brief  Enables Timer channel by start supplying operating clock.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval None
  */
void seT16_Enable( uint32_t T16BaseAddr );

/**
  * @brief  Disables Timer channel by stop supplying operating clock.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval None
  */
void seT16_Disable( uint32_t T16BaseAddr );

/**
  * @brief  Configures timer clock source and clock divider.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @param  clock: This parameter can be a value of @ref seCLG_ClkSrc.
  * @param  divider: This parameter can be a value of @ref seT16_ClkDiv.
  * @retval None
  */
void seT16_ConfigureClock( uint32_t T16BaseAddr, seT16_ClkSrc clock, uint16_t divider );

/**
  * @brief  Discovers T16 clock from registers.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval T16 clock.
  */
uint32_t seT16_GetClk( uint32_t T16BaseAddr );

/**
  * @brief  Configures timer counter mode.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @param  mode: This parameter can be a value of @ref seT16_CounterMode.
  * @retval None
  */
void seT16_ConfigureCounterMode(  uint32_t T16BaseAddr, seT16_CounterMode mode );

/**
  * @brief  Sets Timer counter.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @param  counter: This parameter can be a 16-bit value.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seT16_SetCounter( uint32_t T16BaseAddr, uint16_t counter );

/**
  * @brief  Gets Timer counter value.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval 16-bit counter value.
  */
uint16_t seT16_GetCounter( uint32_t T16BaseAddr );

/**
  * @brief  Enables Timer channel interrupt.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval None
  */
void seT16_EnableInt( uint32_t T16BaseAddr );

/**
  * @brief  Disables Timer channel interrupt.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval None
  */
void seT16_DisableInt( uint32_t T16BaseAddr );

/**
  * @brief  Returns Timer interrupt flag.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval InterruptStatus: @ref seInterruptStatus
  */
seInterruptStatus seT16_GetIntFlag( uint32_t T16BaseAddr );

/**
  * @brief  Clears Timer channel interrupt.
  * @param  T16BaseAddr: Address space offset to the T16 to be controlled.
  * @retval None
  */
void seT16_ClearIntFlag( uint32_t T16BaseAddr );


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

#endif /*SE_T16_H */
