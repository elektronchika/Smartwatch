/**
  ******************************************************************************
  * @file    se_dmac.h
  * @author  Epson
  * @version V1.0
  * @date    30-November-2017
  * @brief   This file contains the function prototypes for the DMAC library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SE_DMAC_H
#define SE_DMAC_H


#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup sePeriphLibrary
  * @{
  */

/** @defgroup DMAC
  * @{
  * @brief The Direct Memory Access (DMAC) module is designed to move data between peripherals and memory without core participation.
  * It speeds up transfers and saves power.
  * The channels have separate controls and flags. Each channel has one or more transfer descriptors stored
  * in chip's memory. Descriptors contain details of the data transfers.
  *
  */

/** @defgroup DMAC_Constants
  * @{
  */

typedef enum {
   seDMAC_CH_NONE = 0U,                   ///< None of the implemented channels
   seDMAC_CH0     = 1U,                   ///< Implemented channel 0
   seDMAC_CH1     = 2U,                   ///< Implemented channel 1
   seDMAC_CH2     = 4U,                   ///< Implemented channel 2
   seDMAC_CH3     = 8U,                   ///< Implemented channel 2
   seDMAC_CH_ALL  = 0xfU                  ///< All implemented channels
} seDMAC_CHANNEL;

 typedef enum {
   seDMAC_INC_1 = 0,                     ///< Set the increment value to one
   seDMAC_INC_2 = 1,                     ///< Set the increment value to two
   seDMAC_INC_4 = 2,                     ///< Set the increment value to four
   seDMAC_INC_NO = 3,                    ///< Set the increment value to no increment
} seDMAC_Inc;

typedef enum {
   seDMAC_SIZE_BYTE = 0,                  ///< Set the size to one byte
   seDMAC_SIZE_HALF_WORD = 1,             ///< Set the size to two bytes
   seDMAC_SIZE_WORD = 2,                  ///< Set the size to four bytes
   seDMAC_SIZE_RESERVED = 3,              ///< Size value is reserved
} seDMAC_Size;

typedef enum {
   seDMAC_MODE_ALTERNT_PERIF_SCATTER = 7,  ///< Set transfer mode to Peripheral scatter-gather transfer (for alternate data structure)
   seDMAC_MODE_PRIMARY_PERIF_SCATTER = 6,  ///< Set transfer mode to Peripheral scatter-gather transfer (for primary data structure)
   seDMAC_MODE_ALTERNT_MEM_SCATTER = 5,    ///< Set transfer mode to Memory scatter-gather transfer (for alternate data structure)
   seDMAC_MODE_PRIMARY_MEM_SCATTER = 4,    ///< Set transfer mode to Memory scatter-gather transfer (for primary data structure)
   seDMAC_MODE_PING_PONG = 3,              ///< Set transfer mode to to Ping-pong transfer
   seDMAC_MODE_AUTO_REQ = 2,               ///< Set transfer mode to Auto-request transfer
   seDMAC_MODE_BASIC = 1,                  ///< Set transfer mode to Basic transfer
   seDMAC_MODE_STOP = 0,                   ///< Set transfer mode to Stop
} seDMAC_Mode;

typedef enum {
  seDMAC_ERR_INT = 1,                      ///< Interrupt cause is an error
  seDMAC_TRANSF_COMPL = 2,                 ///< Interrupt cause is transfer completion
  seDMAC_ALL_INT = 3                       ///< All possible causes of interrupt
} seDMAC_InterruptSrc;

#define seDMAC_CNLS(a) ((seDMAC_CHANNEL)((a)))                                                                 ///< Combination of any of the DMAC channels
#define seDMAC_IDX( chan ) (((chan) == seDMAC_CH0)? 0 : ((chan) == seDMAC_CH1)?1:( (chan) == seDMAC_CH2 )?2:3 )///<  Mapping index for a channel descriptor

#define  seDMAC_IntFlag seDMAC_CHANNEL     ///< Interrupt flags (same as channels)

#define seDMAC_NM_MAX     0x3FF            ///< Maximum transfer count minus 1
#define seDMAC_RP_MAX     0xF              ///< Maximum R_power

#define seDMAC_cdata( cc, nm, Rp, ss, ds, si, di ) \
 ((cc)&0x7) | ((nm)&seDMAC_NM_MAX)<<4 | ((Rp)&seDMAC_RP_MAX)<<14 | ((ss)&3)<<24 | ((si)&3)<<26 | ((ds)&3)<<28 | (uint32_t)((di)&3)<<30  ///< DMAC control data 


/**
  * @}
  */


/** @defgroup DMAC_Types
  * @{
  */

/**
  * @brief  DMAC Control Data definition
  */

typedef  union {
  uint32_t  ctrldata;                         ///< Configuration details for the transfer
  struct {
    uint32_t  cycle_ctrl  :  3;               ///< Transfer Mode
    uint32_t  RES_3       :  1;               ///< Must write 0 to this bit
    uint32_t  n_minus_1   : 10;               ///< Transfer Total
    uint32_t  R_power     :  4;               ///< Transfer Size Select
    uint32_t  RES_23_18   :  6;               ///< Must write 0 to this field
    uint32_t  src_size    :  2;               ///< Source Data Size Select
    uint32_t  src_inc     :  2;               ///< Source Address Increment Mode
    uint32_t  dest_size   :  2;               ///< Destination Data Size Select
    uint32_t  dest_inc    :  2;               ///< Destination Address Increment Mode
  } ctrldata_b;                               ///< Bit field access
 } seDMAC_CtrlData;


/**
  * @brief  DMAC Descriptors structure definition
  */

typedef struct {
  uint32_t transfer_source_end_pointer;         ///< The address of the last source data in the transfer
  uint32_t transfer_destination_end_pointer;    ///< The last destination address of the transfer
  uint32_t control_data;                        ///< Configuration details for the transfer @ref seDMAC_CtrlData
  uint32_t request_num;                         ///< Firmware can use this byte for number of requests tracking
} seDMAC_DataStruct;


/**
  * @}
  */


/** @defgroup DMAC_Functions
  * @{
  */

/**
  * @brief  Initializes DMAC descriptors.
  * @param  dma_data_struc_ptr: Address of an allocated buffer for use for DMAC descriptors. Must be aligned.
  * @param  chnls: This parameter declares the maximum number of requested channels.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seDMAC_Init( uint32_t dma_data_struc_ptr, int chnls );

/**
  * @brief  Initializes a DMAC channel.
  * @note This routine does not check if the channel was already active.
  * @param  ctrl_data: This parameter is a DMA channel control data 32-bit value.
  * @param  transf_src_end: This parameter is a source end address.
  * @param  transf_dest_end: This parameter is a destination end address.
  * @param  chan: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seDMAC_SetChannel( uint32_t ctrl_data, uint32_t transf_src_end, uint32_t transf_dest_end, seDMAC_CHANNEL chan );


/**
  * @brief  Starts DMAC channel.
  * @param  chan: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval Status: can be a value of @ref seStatus
  */
void seDMAC_Start( seDMAC_CHANNEL chan );


/**
  * @brief  Enables interrupt(s).
  * @param  src: This parameter is a value of @ref seDMAC_InterruptSrc.
  * @param  flag: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_EnableInt( seDMAC_InterruptSrc src, seDMAC_IntFlag flag  );


/**
  * @brief  Disables interrupt(s).
  * @param  src: This parameter is a value of @ref seDMAC_InterruptSrc.
  * @param  flag: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_DisableInt( seDMAC_InterruptSrc src, seDMAC_IntFlag flag  );


/**
  * @brief  Returns the status of interrupt flag for selected interrupt sources.
  * Use caution when using this function with a combination of interrupt sources.
  * @param  src: This parameter is a value of @ref seDMAC_InterruptSrc.
  * @param  flag: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval InterruptStatus: can be a value of @ref seInterruptStatus.
  */
seInterruptStatus seDMAC_GetIntFlag( seDMAC_InterruptSrc src, seDMAC_IntFlag flag  );


/**
  * @brief  Clears DMAC interrupt(s).
  * @param  src: This parameter is a value of @ref seDMAC_InterruptSrc.
  * @param  flag: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_ClearIntFlag( seDMAC_InterruptSrc src, seDMAC_IntFlag flag  );


/**
  * @brief  Enables masking of the Peripheral devices.
  * @param  chnls: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_EnableRequestMask( seDMAC_CHANNEL chnls );


/**
  * @brief  Disables masking of the Peripheral devices.
  * @param  chnls: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_DisableRequestMask( seDMAC_CHANNEL chnls );


/**
  * @brief  Enables DMAC channel.
  * @param  chnls: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_Enable( seDMAC_CHANNEL chnls );


/**
  * @brief  Disables DMAC channel.
  * @param  chnls: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_Disable( seDMAC_CHANNEL chnls );


/**
  * @brief  Enables DMAC's alternate desriptors.
  * @param  chnls: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_AlternateEnable( seDMAC_CHANNEL chnls );


/**
  * @brief  Enables DMAC's primary desriptors.
  * @param  chnls: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_AlternateDisable( seDMAC_CHANNEL chnls );

/**
  * @brief  Causes DMAC channel priority increase.
  * @param  chnls: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_PriorityIncrease( seDMAC_CHANNEL chnls );

/**
  * @brief  Causes DMAC channel priority decrease.
  * @param  chnls: This parameter is a value of @ref seDMAC_CHANNEL.
  * @retval None
  */
void seDMAC_PriorityDecrease( seDMAC_CHANNEL chnls );


/**
  * @brief  Sets DMAC channel's data structure pointer.
  * @param  dma_data_struc_ptr: This parameter shall be a valid aligned memory pointer.
  * @retval None
  */
void seDMAC_SetDataStrucPtr( uint32_t dma_data_struc_ptr );


/**
  * @brief  Returns DMAC channel's data structure pointer.
  * @retval Ptr: A valid aligned memory pointer.
  */
uint32_t seDMAC_GetDataStrucPtr( void );


/**
  * @brief  Returns DMAC channel's alternate data structure pointer.
  * @retval Ptr: A valid aligned memory pointer.
  */
uint32_t seDMAC_GetAltDataStrucPtr( void );


/**
  * @brief  Gets DMAC mode from the valid data structure.
  * Returns mode seDMAC_MODE_STOP if DMA structures are not initialized.
  * @param  chan: is a value of @ref seDMAC_CHANNEL.
  * @retval Mode Value matching @ref seDMAC_Mode
  */
uint32_t seDMAC_GetMode( seDMAC_CHANNEL chan );


/**
  * @brief  Gets DMAC Total number of units to transmit minus 1 from the data structure.
  * @param  chan: is a value of @ref seDMAC_CHANNEL.
  * @retval units: Total number of units to transmit minus 1.
  */
uint32_t seDMAC_GetNMinus1( seDMAC_CHANNEL chan );


/**
  * @brief  For Peripheral-to-Memory DMAC transfer this function generates DMAC control data and sets it in the DMAC channel descriptor.
  * Returns a register pointer further used to trigger DMAC transfer.
  * @param  periph: Pointer to a peripheral.
  * @param  daddress: Destination address. The size of the transfer defined in the function based on the peripheral type.
  * @param  size_m1: Transfer size minus one.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval Ptr: The offset to the register used to trigger DMAC transfer.
  */
uint32_t seDMAC_ConfigPeriphToMem( uint32_t periph,  uint32_t daddress, uint32_t size_m1, seDMAC_CHANNEL chan );


/**
  * @brief  For Memory-to-Peripheral DMAC transfer this function generates DMAC control data and sets it in the DMAC channel descriptor.
  * Returns a register pointer further used to enable DMAC request.
  * @param  saddress: Source address. The size of the transfer defined in the function based on the peripheral type.
  * @param  periph: Pointer to a peripheral.
  * @param  size_m1: Transfer size minus one.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval Ptr: The offset to the register used to trigger DMAC transfer.
  */
uint32_t seDMAC_ConfigMemToPeriph( uint32_t saddress, uint32_t periph, uint32_t size_m1, seDMAC_CHANNEL chan );


/**
  * @brief  For Peripheral-to-Memory DMAC transfer this function configures a DMAC channel
  * and generates a DMAC request.
  * Returns a register pointer further used to disable DMAC request.
  * @param  periph: Pointer to a peripheral.
  * @param  daddress: Destination address. The size of the transfer defined in the function based on the peripheral type.
  * @param  transfcount: Transfer size minus one.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval Ptr: The offset to the register used to trigger DMAC transfer.
  */
uint32_t seDMAC_NonBlockTransfPeriphToMem( uint32_t periph, uint32_t daddress, uint32_t transfcount, seDMAC_CHANNEL chan ) ;


/**
  * @brief  For Memory-to-Peripheral DMAC transfer this function configures a DMAC channel
  * and generates a DMAC request.
  * Returns a register pointer further used to disable DMAC request.
  * @param  saddress: Source address. The size of the transfer defined in the function based on the peripheral type.
  * @param  periph: Pointer to a peripheral.
  * @param  transfcount: Transfer size minus one.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval Ptr: The offset to the register used to trigger DMAC transfer.
  */
uint32_t  seDMAC_NonBlockTransfMemToPeriph( uint32_t saddress, uint32_t periph, uint32_t transfcount, seDMAC_CHANNEL chan ) ;

/**
  * @brief  Memory fill 8-bit DMAC transfer.
  * @param  dstaddr: Destination address.
  * @param  nbytes: Number of bytes to fill/tranfer.
  * @param  fillbyte: Byte value to fill.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval None.
  */
void seDMAC_MemFill8 (uint32_t dstaddr, uint32_t nbytes, uint8_t fillbyte, seDMAC_CHANNEL chan);

/**
  * @brief  Memory fill 16-bit DMAC transfer.
  * @param  dstaddr: Destination address.
  * @param  nhwords: Number of half-words (16-bit) to fill/tranfer.
  * @param  fillhword: Half-word value to fill.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval None.
  */
void seDMAC_MemFill16 (uint32_t dstaddr, uint32_t nhwords, uint16_t fillhword, seDMAC_CHANNEL chan);

/**
  * @brief  Memory fill 32-bit DMAC transfer.
  * @param  dstaddr: Destination address.
  * @param  nwords: Number of words (32-bit) to fill/tranfer.
  * @param  fillword: Word value to fill.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval None.
  */
void seDMAC_MemFill32 (uint32_t dstaddr, uint32_t nwords, uint32_t fillword, seDMAC_CHANNEL chan);

/**
  * @brief  Memory-to-Memory 8-bit DMAC transfer.
  * @param  srcaddr: Source address.
  * @param  dstaddr: Destination address.
  * @param  nbytes: Number of bytes to transfer.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval None.
  */
void seDMAC_MemCpy8 (uint32_t srcaddr, uint32_t dstaddr, uint32_t nbytes, seDMAC_CHANNEL chan);

/**
  * @brief  Memory-to-Memory 16-bit DMAC transfer.
  * @param  srcaddr: Source address.
  * @param  dstaddr: Destination address.
  * @param  nhwords: Number of half-words (16-bit) to transfer.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval None.
  */
void seDMAC_MemCpy16 (uint32_t srcaddr, uint32_t dstaddr, uint32_t nhwords, seDMAC_CHANNEL chan);

/**
  * @brief  Memory-to-Memory 32-bit DMAC transfer.
  * @param  srcaddr: Source address.
  * @param  dstaddr: Destination address.
  * @param  nwords: Number of words (32-bit) to transfer.
  * @param  chan: DMAC channel can be @ref seDMAC_CHANNEL
  * @retval None.
  */
void seDMAC_MemCpy32 (uint32_t srcaddr, uint32_t dstaddr, uint32_t nwords, seDMAC_CHANNEL chan);


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
#endif	// SE_DMAC_H
