/**
  ******************************************************************************
  * @file    se_snda.h
  * @author  Epson
  * @version V1.0
  * @date    21-October-2017
  * @brief   This file contains the function prototypes for the SND library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SE_SND_H
#define SE_SND_H

#ifdef __cplusplus
 extern "C" {
#endif
 

/** @addtogroup sePeriphLibrary
  * @{
  */


/** @defgroup SND
  * @{
  * @brief SND is a sound generator that generates melodies and buzzer signals.
  */ 

/** @defgroup SND_Constants
  * @{
  */
      
typedef  seCLG_ClkSrc seSND_ClkSrc;     ///< Type seSND_ClkSrc is same as seCLG_ClkSrc

typedef enum {
  seSND_OSC1_CLKDIV_1   = 0,            ///< OSC1 division ratio is 1/1.
} seSND_OSC1_ClkDiv;

typedef enum {
  seSND_SingleDrive = 1,                ///< Single Pin Drive
  seSND_DirectDrive = 0,                ///< Direct Drive
} seSND_DriveMode;

typedef enum {
    seSND_NormalBuzzer = 0,             ///< Normal buzzer mode
    seSND_OneShotBuzzer = 1,            ///< One-shot buzzer mode
    seSND_Melody = 2                    ///< Melody mode
} seSND_ModeSel;

typedef enum {
  seSND_ED_INT  = 1,                    ///< Sound output completion interrupt
  seSND_EM_INT  = 2,                    ///< Sound buffer empty interrupt
  seSND_ALL_INT = 3                     ///< All interrupts
} seSND_InterruptSrc;

/**
  * @}
  */

/** @defgroup SND_Types
  * @{
  */


/**
  * @brief  SND initialization structure definition
  */
typedef struct {
    seSND_ClkSrc ClkSrc;                ///< Specifies the timer clock source selection.
    uint16_t ClkDivider;                ///< Specifies the prescaler value used to divide the SND clock.
    seSND_DriveMode DriveMode;          ///< Specifies Pin Drive Mode
    seDMAC_CHANNEL DMAChannel;          ///< Specifies DMA channel
    seSND_InterruptSrc EnableInt;       ///< Specifies Interrupts to be enabled
} seSND_InitTypeDef;

/**
  * @}
  */

/** @defgroup SND_Functions
  * @{
  */

/**
  * @brief  Fills each seSND_InitTypeDef member with its default value.
  * @param  SND_InitStruct: pointer to an seSND_InitTypeDef structure which will be initialized.
  * @retval None
  */
void seSND_InitStruct( seSND_InitTypeDef* SND_InitStruct );


/**
  * @brief  Initializes the SND peripheral according to the specified
  *         parameters in the seSND_InitStruct.
  * @note   This function configures the module, and module's interrupts.
  *         It clears module's interrupts but does not enable interrupt
  *         from the module to CPU. This function enables module start by
  *         supplying operating clock.
  * @param  InitStruct: pointer to a seSND_InitTypeDef structure that
  *         contains the configuration information for the specified SND
  *         peripheral.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSND_Init( seSND_InitTypeDef* SND_InitStruct );


/**
  * @brief  Starts SND in normal mode.
  * @param  frequency: This parameter defines frequency.
  * @param  duty_ratio: This parameter defines duty ratio.
  * @retval None
  */
void seSND_Start( uint16_t frequency, uint16_t duty_ratio );


/**
  * @brief  Starts SND in one-shot mode.
  * @param  frequency: This parameter defines frequency.
  * @param  duty_ratio: This parameter defines duty ratio.
  * @param  duration: This parameter defines duty duration.
  * @retval None
  */
void seSND_StartOneShot( uint16_t frequency, uint16_t duty_ratio, uint16_t duration );


/**
  * @brief  Starts SND in melody mode.
  * @param  data: This parameter defines data of music.
  * @param  size: This parameter defines size of music data by number of words.
  * @param  tempo: This parameter defines tempo of playing.
  * @retval None
  */
void seSND_StartMelody( const uint16_t * data, uint32_t size, uint16_t tempo );


/**
  * @brief  Stops SND.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seSND_Stop( void );


/**
  * @brief  Enables SND by start supplying operating clock.
  * @retval None
  */
void seSND_Enable( void );


/**
  * @brief  Disables SND by stop supplying operating clock.
  * @retval None
  */
void seSND_Disable( void );


/**
  * @brief  Configures clock source and clock divider.
  * @param  clock: This parameter can be a value of @ref seSND_ClkSrc.
  * @param  divider: This parameter can be a value of @ref seSND_ClkDiv.
  * @retval None
  */
void seSND_ConfigureClock( seSND_ClkSrc clock, uint16_t divider );


/**
  * @brief  Discovers SND clock from registers.
  * @retval SND clock.
  */
uint32_t seSND_GetClk( void );


/**
  * @brief  Enables SND interrupt.
  * @param  irq: This parameter can be a value of @seSND_InterruptSrc.
  * @retval None
  */
void seSND_EnableInt( seSND_InterruptSrc irq );


/**
  * @brief  Disables SND interrupt.
  * @param  irq: This parameter can be a value of @seSND_InterruptSrc.
  * @retval None
  */
void seSND_DisableInt( seSND_InterruptSrc irq );


/**
  * @brief  Returns SND interrupt status.
  * @param  irq: This parameter can be a value of @ref seSND_InterruptSrc.
  * @retval InterruptStatus: see @ref seInterruptStatus
  */
seInterruptStatus seSND_GetIntFlag( seSND_InterruptSrc irq );


/**
  * @brief  Clears SND interrupt.
  * @retval None
  */
void seSND_ClearIntFlag( void );


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

#endif /*SE_SND_H */
