/**
  ******************************************************************************
  * @file    se_port.h
  * @author  Epson
  * @version V1.0  
  * @date    25-September-2017
  * @brief   This file contains the function prototypes for the PORT library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SE_PORT_H
#define SE_PORT_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup sePeriphLibrary
  * @{
  */

/**
  * @defgroup PORT
  * @{
  * @brief The PORT (I/O Ports) circuit.
  * This circuit allows the selection of I/O functions on processor pins between
  * GPIO, and several alternate functions.
  */

/** @defgroup PORT_Constants
  * @{
  */

typedef enum {
  sePORT_CLKDIV_1 = 0,            ///< division ratio is 1/1.
  sePORT_CLKDIV_2 = 1,            ///< division ratio is 1/2.
  sePORT_CLKDIV_4 = 2,            ///< division ratio is 1/4.
  sePORT_CLKDIV_8 = 3,            ///< division ratio is 1/8.
  sePORT_CLKDIV_16 = 4,           ///< division ratio is 1/16.
  sePORT_CLKDIV_32 = 5,           ///< division ratio is 1/32.
  sePORT_CLKDIV_64 = 6,           ///< division ratio is 1/64.
  sePORT_CLKDIV_128 = 7,          ///< division ratio is 1/128.
  sePORT_CLKDIV_256 = 8,          ///< division ratio is 1/256.
  sePORT_CLKDIV_512 = 9,          ///< division ratio is 1/512.
  sePORT_CLKDIV_1024 = 10,        ///< division ratio is 1/1024.
  sePORT_CLKDIV_2048 = 11,        ///< division ratio is 1/2048.
  sePORT_CLKDIV_4096 = 12,        ///< division ratio is 1/4096
  sePORT_CLKDIV_8192 =  13,       ///< division ratio is 1/8192.
  sePORT_CLKDIV_16384 = 14,       ///< division ratio is 1/16384.
  sePORT_CLKDIV_32768 = 15,       ///< division ratio is 1/32768.
} sePORT_ClkDiv;

typedef  seCLG_ClkSrc sePORT_ClkSrc;   ///< Type sePORT_ClkSrc is same as seCLG_ClkSrc

typedef enum {
  sePORT_P00 = 0,                      ///< P00
  sePORT_P01 = 1,                      ///< P01
  sePORT_P02 = 2,                      ///< P02
  sePORT_P03 = 3,                      ///< P03
  sePORT_P04 = 4,                      ///< P04
  sePORT_P05 = 5,                      ///< P05
  sePORT_P06 = 6,                      ///< P06
  sePORT_P07 = 7,                      ///< P07
  sePORT_P10 = 8,                      ///< P10
  sePORT_P11 = 9,                      ///< P11
} sePORT_Id;

typedef enum {
  sePORT_P0 = ( 0 ),			///< Port number Px0.
  sePORT_P1 = ( 1 ),			///< Port number Px1.
  sePORT_P2 = ( 2 ),			///< Port number Px2.
  sePORT_P3 = ( 3 ),			///< Port number Px3.
  sePORT_P4 = ( 4 ),			///< Port number Px4.
  sePORT_P5 = ( 5 ),			///< Port number Px5.
  sePORT_P6 = ( 6 ),			///< Port number Px6.
  sePORT_P7 = ( 7 ),			///< Port number Px7.
} sePORT_PortNumber;

typedef enum {
  sePORT_G0 = ( 0 ),			///< Port group number P0x.
  sePORT_G1 = ( 1 ),			///< Port group number P1x.
} sePORT_PortGroup;

typedef enum {
  sePORT_ALT_0 = ( 0 ),                ///< Port Alt Function 0
  sePORT_ALT_1 = ( 1 ),                ///< Port Alt Function 1
  sePORT_ALT_2 = ( 2 ),                ///< Port Alt Function 2
  sePORT_ALT_3 = ( 3 ),                ///< Port Alt Function 3
} sePORT_AltFunc;


typedef enum {
  sePORT_DATA_LOW = ( 0 ),		///< Port Data:LOW.
  sePORT_DATA_HIGH= ( 1 ),		///< Port Data:HIGH.
} sePORT_Data;

typedef enum {
  sePORT_INT_EDGE_RISING  = ( 0 ),	///< Input signal:Rising edge.
  sePORT_INT_EDGE_FALLING = ( 1 ),	///< Input signal:Falling edge.
} sePORT_Edge;



/**
  * @}
  */

/** @defgroup PORT_Types
  * @{
  */


/**
  * @brief  PORT Init structure definition
  */

typedef struct
{
    sePORT_ClkSrc ClkSrc;               ///< Specifies the timer clock source selection.
    uint16_t ClkDivider;                ///< Specifies the prescaler value used to divide the PORT clock.
} sePORT_InitTypeDef;

/**
  * @brief  PORT Peripheral function definition
  */

typedef struct
{
  sePORT_Id       portID;              ///< Port identifier
  sePORT_AltFunc  AltFunc;             ///< Specifies the alternate function number
} sePORT_PeriphPortDef;


/**
  * @}
  */

/** @defgroup PORT_Functions
  * @{
  */

/**
  * @brief  Initializes the PORT peripheral according to the specified
  * parameters in the sePORT_InitStruct. Stop PORT before initializing.
  * @param  InitStruct: pointer to a sePORT_InitTypeDef structure that
  * contains the configuration information for the specified PORT peripheral.
  * @ref sePORT_InitTypeDef.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus sePORT_Init( sePORT_InitTypeDef* InitStruct );


/**
  * @brief  Configures PORT timer clock source and clock divider.
  * @param  clock: This parameter can be a value of @ref sePORT_ClkSrc.
  * @param  divider: This parameter can be a value of @ref sePORT_ClkDiv.
  * @note   If Chattering filter is enabled application shall clear interrupts when changing the clock settings.
  * @retval None
  */
void sePORT_ConfigureClock( sePORT_ClkSrc clock, uint16_t divider );


/**
  * @brief  Initialize for using a port as a general-purpose input port.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval None
  */
void sePORT_InitAsInput( sePORT_Id portId );

/**
  * @brief  Enables the pull-up resistor.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval None
  */
void sePORT_EnablePullUpResistor( sePORT_Id portId );

/**
  * @brief  Enables the pull-down resistor.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval None
  */
void sePORT_EnablePullDownResistor( sePORT_Id portId );

/**
  * @brief  Disables built-in resistor.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval None
  */
void sePORT_DisableBuiltInResistor( sePORT_Id portId );

/**
  * @brief  Initialize for using a port as a general-purpose output port.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval None
  */
void sePORT_InitAsOutput( sePORT_Id portId );

/**
  * @brief  Enable port interrupt.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @param  edge: This parameter can be a value of @ref sePORT_Edge.
  * @retval None
  */
void sePORT_EnableInt( sePORT_Id portId, sePORT_Edge edge );

/**
  * @brief  Disable port interrupt.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval None
  */
void sePORT_DisableInt( sePORT_Id portId );

/**
  * @brief  Get port interrupt.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval InterruptStatus: can be a value of @ref seInterruptStatus.
  */
seInterruptStatus sePORT_GetIntFlag( sePORT_Id portId );

/**
  * @brief  Clear port interrupt.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval None
  */
void sePORT_ClearIntFlag( sePORT_Id portId );

/**
  * @brief  Get port output value.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval value: can be a value of @ref sePORT_Data.
  */
sePORT_Data sePORT_GetOutput( sePORT_Id portId );

/**
  * @brief  Set port output data.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @param  value: can be a value of @ref sePORT_Data.
  * @retval None
  */
void sePORT_SetOutput( sePORT_Id portId, sePORT_Data data );

/**
  * @brief  Get port input value.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval value: can be a value of @ref sePORT_Data.
  */
sePORT_Data sePORT_GetInput( sePORT_Id portId );

/**
  * @brief  Enable port Chattering Filter.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @note Application shall clear interrupts when changing Chattering Filter.
  * @retval None
  */
void sePORT_EnableChatteringFilter( sePORT_Id portId );

/**
  * @brief  Disable port Chattering Filter.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @note Application shall clear interrupts when changing Chattering Filter.
  * @retval None
  */
void sePORT_DisableChatteringFilter( sePORT_Id portId );

/**
  * @brief  Gets the Chattering Filter.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval State: can be a value of @ref seState
  */
seState sePORT_GetChatteringFilter( sePORT_Id portId );

/**
  * @brief  Initialize for using a port as HiZ that is GPIO with input and
  *         output disabled, also disables the Interrupt
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @retval None
  */
void sePORT_InitAsHiZ( sePORT_Id portId );

/**
  * @brief  Initialize for using a port as an alternate function.
  * @param  portId: This parameter can be a value of @ref sePORT_Id.
  * @param  funcNo: This parameter can be a value of @ref sePORT_AltFunc.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus sePORT_InitAsAltFunction( sePORT_Id portId, sePORT_AltFunc funcNo );


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

#endif /*SE_PORT_H */
