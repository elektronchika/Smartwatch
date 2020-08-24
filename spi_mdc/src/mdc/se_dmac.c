/**
  ******************************************************************************
  * @file    se_dmac.c
  * @author  Epson
  * @version V1.0
  * @date    30-November-2017
  * @brief   This file contains the functions for the DMAC library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#include <string.h>

#include "s1d13c00_hcl.h"
#include "s1d13C00_memregs.h"
#include "se_common.h"
#include "se_dmac.h"


seStatus seDMAC_Init( uint32_t dma_data_struc_ptr, int chnls )
{
    seStatus fResult = seSTATUS_NG;
    uint8_t tmp[128];

    seS1D13C00Write8( DMAC_CFG, 1 );    // enable controller

    uint32_t statreg = seS1D13C00Read32( DMAC_STAT );
    int dmachannels = (statreg >> 16) & 0x0F;
    if ( chnls <= dmachannels && !(dma_data_struc_ptr & 0x7F) )
    {
        // Set the pointer to the DMA control data and clear
        // the memory at that location.
        seS1D13C00Write32( DMAC_CPTR, dma_data_struc_ptr );
        memset( (void*)tmp, 0, 0x80 );
        seS1D13C00Write( dma_data_struc_ptr, tmp, sizeof(tmp) );

        seS1D13C00Write8( DMAC_RMCLR, seDMAC_CH_ALL );
        seS1D13C00Write8( DMAC_PRCLR, seDMAC_CH_ALL );
        seS1D13C00Write8( DMAC_PACLR, seDMAC_CH_ALL );
        seS1D13C00Write8( DMAC_ENCLR, seDMAC_CH_ALL );

        fResult = seSTATUS_OK;
    }

    return fResult;
}


uint32_t seDMAC_GetDataStrucPtr( void )
{
    return seS1D13C00Read32( DMAC_CPTR );
}


void seDMAC_SetDataStrucPtr( uint32_t dma_data_struc_ptr )
{
    seS1D13C00Write32( DMAC_CPTR, dma_data_struc_ptr );
}


uint32_t seDMAC_GetAltDataStrucPtr( void )
{
    return seS1D13C00Read32( DMAC_ACPTR );
}


seStatus seDMAC_SetChannel( uint32_t ctrl_data, uint32_t transf_src_end, uint32_t transf_dst_end, seDMAC_CHANNEL chan )
{
    seStatus fResult = seSTATUS_NG;
    uint32_t index = seDMAC_IDX( chan );
    uint32_t base_ptr = seS1D13C00Read32( DMAC_CPTR );

    if ( base_ptr )
    {
        seS1D13C00Write32( base_ptr + index * 16 + 0, transf_src_end );
        seS1D13C00Write32( base_ptr + index * 16 + 4, transf_dst_end );
        seS1D13C00Write32( base_ptr + index * 16 + 8, ctrl_data );

        fResult = seSTATUS_OK;
    }

    return fResult;
}


uint32_t seDMAC_GetMode( seDMAC_CHANNEL chan )
{
    uint32_t mode = seDMAC_MODE_STOP;
    uint32_t index = seDMAC_IDX( chan );
    uint32_t base_ptr = seS1D13C00Read32( DMAC_CPTR );
    seDMAC_CtrlData ctrl_data;

    if ( base_ptr )
    {
        seS1D13C00Read( (base_ptr + index * 16 + 8), (uint8_t *) &ctrl_data, sizeof(ctrl_data) );
        mode = ctrl_data.ctrldata_b.cycle_ctrl;
    }

    return mode;
}


uint32_t seDMAC_GetNMinus1( seDMAC_CHANNEL chan )
{
    uint32_t nm1 = 0;
    uint32_t index = seDMAC_IDX( chan );
    uint32_t base_ptr = seS1D13C00Read32( DMAC_CPTR );
    seDMAC_CtrlData ctrl_data;

    if ( base_ptr )
    {
        seS1D13C00Read( (base_ptr + index * 16 + 8), (uint8_t *) &ctrl_data, sizeof(ctrl_data) );
        nm1 = ctrl_data.ctrldata_b.n_minus_1;
    }

    return nm1;
}


void seDMAC_Start( seDMAC_CHANNEL chan )
{
  seS1D13C00Write32( DMAC_SWREQ, chan );
}


void seDMAC_EnableInt( seDMAC_InterruptSrc src, seDMAC_IntFlag flag )
{
    if ( (seDMAC_ERR_INT & src) == seDMAC_ERR_INT ) {
        seSetBits8( DMAC_ERRIESET, 0x0F, flag );
    }
    if  ( (seDMAC_TRANSF_COMPL & src) == seDMAC_TRANSF_COMPL ) {
        seSetBits8( DMAC_ENDIESET, 0x0F, flag );
    }
}


void seDMAC_DisableInt( seDMAC_InterruptSrc src, seDMAC_IntFlag flag  )
{
    if ( (seDMAC_ERR_INT & src) == seDMAC_ERR_INT ) {
        seSetBits8( DMAC_ERRIECLR, 0x0F, flag );
    }
    if  ( (seDMAC_TRANSF_COMPL & src) == seDMAC_TRANSF_COMPL ) {
        seSetBits8( DMAC_ENDIECLR, 0x0F, flag );
    }
}


seInterruptStatus seDMAC_GetIntFlag( seDMAC_InterruptSrc src, seDMAC_IntFlag flag  )
{
    seInterruptStatus status = seINTERRUPT_NOT_OCCURRED;

    if ( ( seDMAC_TRANSF_COMPL & src ) == seDMAC_TRANSF_COMPL ) {
        status = ( seS1D13C00Read8( DMAC_ENDIF ) & flag ) ? seINTERRUPT_OCCURRED : seINTERRUPT_NOT_OCCURRED;
    }
    if  ( ( status == seINTERRUPT_NOT_OCCURRED ) && ( seDMAC_ERR_INT & src ) == seDMAC_ERR_INT ) {
        status = ( seS1D13C00Read32( DMAC_ERRIF) & flag ) ? seINTERRUPT_OCCURRED : seINTERRUPT_NOT_OCCURRED;
    }

    return status;
}


void seDMAC_ClearIntFlag( seDMAC_InterruptSrc src, seDMAC_IntFlag flag  )
{
    if ( (src & seDMAC_TRANSF_COMPL) == seDMAC_TRANSF_COMPL ) {
        seSetBits8( DMAC_ENDIF, 0x0F, flag );
    }
    if  ( (src & seDMAC_ERR_INT) == seDMAC_ERR_INT ) {
        seSetBits8( DMAC_ERRIF, 0x0F, flag );
    }
}


void seDMAC_EnableRequestMask( seDMAC_CHANNEL chnls )
{
    seS1D13C00Write32( DMAC_RMSET, chnls );
}


void seDMAC_DisableRequestMask( seDMAC_CHANNEL chnls )
{
    seS1D13C00Write32( DMAC_RMCLR, chnls );
}


void seDMAC_Enable( seDMAC_CHANNEL chnls )
{
    seS1D13C00Write32( DMAC_ENSET, chnls );
}


void seDMAC_Disable( seDMAC_CHANNEL chnls )
{
    seS1D13C00Write32( DMAC_ENCLR, chnls );
}


void seDMAC_AlternateEnable( seDMAC_CHANNEL chnls )
{
  seS1D13C00Write8( DMAC_PASET, chnls );
}


void seDMAC_AlternateDisable( seDMAC_CHANNEL chnls )
{
    seS1D13C00Write8( DMAC_PACLR, chnls );
}


void seDMAC_PriorityIncrease( seDMAC_CHANNEL chnls )
{
    seS1D13C00Write32( DMAC_PRSET, chnls );
}


void seDMAC_PriorityDecrease( seDMAC_CHANNEL chnls )
{
    seS1D13C00Write32( DMAC_PRCLR, chnls );
}


// Data Transmission
uint32_t seDMAC_ConfigMemToPeriph( uint32_t saddress, uint32_t periph, uint32_t size_m1, seDMAC_CHANNEL chan )
{
    uint32_t cdata1 = 0;
    volatile uint32_t enable_trigger = 0;

    switch ( periph )
    {
    case C00_SND:
        enable_trigger = (uint32_t)SND_EMDMAEN;
        cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC, size_m1, 0UL, seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD, seDMAC_INC_2, seDMAC_INC_NO );
        seDMAC_SetChannel( cdata1, (uint32_t)saddress + size_m1 * 2, (uint32_t)SND_DAT, chan );
        break;

    case C00_SPI:
        enable_trigger = (uint32_t)SPI_TBEDMAEN;
        cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC, size_m1, 0UL, seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD, seDMAC_INC_2, seDMAC_INC_NO );
        seDMAC_SetChannel( cdata1, (uint32_t)saddress + size_m1 * 2, (uint32_t)SPI_TXD, chan );
        break;

    case C00_I2C:
        enable_trigger = (uint32_t)I2C_TBEDMAEN;
        cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC, size_m1, 0UL, seDMAC_SIZE_BYTE, seDMAC_SIZE_BYTE, seDMAC_INC_1, seDMAC_INC_NO );
        seDMAC_SetChannel( cdata1, (uint32_t)saddress + size_m1, (uint32_t)I2C_TXD, chan );
        break;

    case C00_QSPI:
        enable_trigger = (uint32_t)QSPI_TBEDMAEN;
        cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC, size_m1, 0UL, seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD, seDMAC_INC_2, seDMAC_INC_NO );
        seDMAC_SetChannel( cdata1, (uint32_t)saddress + size_m1 * 2, (uint32_t)QSPI_TXD, chan );
        break;

    default:
        break;
    }

    return enable_trigger;
}


// Data Reception
uint32_t seDMAC_ConfigPeriphToMem( uint32_t periph,  uint32_t daddress, uint32_t size_m1, seDMAC_CHANNEL chan )
{
    uint32_t cdata1 = 0;
    volatile uint32_t enable_trigger = 0;

    switch ( periph )
    {
    case C00_SPI:
      enable_trigger = (uint32_t)SPI_RBFDMAEN;
      cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC, size_m1, 0, seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD, seDMAC_INC_NO, seDMAC_INC_2 );
      seDMAC_SetChannel( cdata1, (uint32_t)SPI_RXD, daddress + size_m1 * 2, chan );
      break;

    case C00_I2C:
      enable_trigger = (uint32_t)I2C_RBFDMAEN;
      cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC, size_m1, 0, seDMAC_SIZE_BYTE, seDMAC_SIZE_BYTE, seDMAC_INC_NO, seDMAC_INC_1 );
      seDMAC_SetChannel( cdata1, (uint32_t)I2C_RXD, daddress + size_m1, chan );
      break;

    case C00_QSPI:
      enable_trigger = (uint32_t)QSPI_RBFDMAEN;
      cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC, size_m1, 0, seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD, seDMAC_INC_NO, seDMAC_INC_2 );
      seDMAC_SetChannel( cdata1, (uint32_t)QSPI_RXD, daddress + size_m1 * 2, chan );
      break;
    default: break;
  }

  return enable_trigger;
}


uint32_t seDMAC_NonBlockTransfPeriphToMem( uint32_t periph, uint32_t daddress, uint32_t transfcount, seDMAC_CHANNEL chan )
{
    volatile uint32_t enable_trigger = 0;

    if ( transfcount <= seDMAC_NM_MAX )
    {
        uint32_t size_m1 = transfcount-1;
        enable_trigger = seDMAC_ConfigPeriphToMem( periph, daddress, size_m1, chan );

        if ( enable_trigger )
        {
            seDMAC_Enable( chan );
            seDMAC_DisableRequestMask( chan );
            seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, chan );
            seSetBits16( enable_trigger, 0x000F, chan );
        }
    }

    return enable_trigger;
}


uint32_t seDMAC_NonBlockTransfMemToPeriph( uint32_t saddress, uint32_t periph, uint32_t transfcount, seDMAC_CHANNEL chan )
{
    volatile uint32_t enable_trigger = 0;

    if ( transfcount <= seDMAC_NM_MAX )
    {
        enable_trigger = seDMAC_ConfigMemToPeriph( saddress, periph, (transfcount-1), chan );

        if ( enable_trigger )
        {
            seDMAC_Enable( chan );
            seDMAC_DisableRequestMask( chan );
            seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, chan );
            seSetBits16( enable_trigger, 0x000F, chan );
        }
    }

  return enable_trigger;
}


void seDMAC_MemFill8 (uint32_t dstaddr, uint32_t nbytes, uint8_t fillbyte, seDMAC_CHANNEL chan)
{
    uint32_t k, m, cdata1;

    ///< Configure the primary data structure for the DMA channel
    seS1D13C00Write8( DMAC_CFG, 1 );    // enable controller
    seDMAC_DisableRequestMask( chan );

    seDMAC_Enable( chan );
    seSetBits8( DMAC_ENDIF, chan, chan );

    m = nbytes / 1024;
    k = 0;
    for (k = 0; k < m; k++)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, 1024-1, 0UL,
                               seDMAC_SIZE_BYTE, seDMAC_SIZE_BYTE,
                               seDMAC_INC_NO, seDMAC_INC_1 );
        seDMAC_SetChannel( cdata1, ( uint32_t ) &(fillbyte), ( uint32_t )(dstaddr+(k*1024)+1024-1), chan );

        seSetBits8( DMAC_ENSET, chan, chan );
        seDMAC_Start(chan);

        // Wait transfer complete
        while((seS1D13C00Read8(DMAC_ENDIF) & chan) == 0);
        seSetBits8( DMAC_ENDIF, chan, chan );

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    if ((nbytes % 1024) != 0)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, (nbytes%1024)-1, 0UL,
                               seDMAC_SIZE_BYTE, seDMAC_SIZE_BYTE,
                               seDMAC_INC_NO, seDMAC_INC_1 );

        seDMAC_SetChannel( cdata1, ( uint32_t ) &(fillbyte), ( uint32_t )(dstaddr+(k*1024)+(nbytes%1024)-1), chan );

        seSetBits8( DMAC_ENSET, chan, chan );
        seDMAC_Start(chan);

        // Wait transfer complete
        while((seS1D13C00Read8(DMAC_ENDIF) & chan) == 0);
        seSetBits8( DMAC_ENDIF, chan, chan );

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    seDMAC_Disable(  chan );
}




void seDMAC_MemFill16 (uint32_t dstaddr, uint32_t nhwords, uint16_t fillhword, seDMAC_CHANNEL chan)
{
    uint32_t k, m, cdata1;

    ///< Configure the primary data structure for the DMA channel
    seS1D13C00Write8( DMAC_CFG, 1 );    // enable controller
    seDMAC_DisableRequestMask( chan );

    seDMAC_Enable( chan );
    seSetBits8( DMAC_ENDIF, chan, chan );

    m = nhwords / 1024;
    k = 0;
    for (k = 0; k < m; k++)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, 1024-1, 0UL,
                               seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD,
                               seDMAC_INC_NO, seDMAC_INC_2 );
        seDMAC_SetChannel( cdata1, ( uint32_t ) &(fillhword), ( uint32_t )(dstaddr+(k*2048)+2048-2), chan );

        seSetBits8( DMAC_ENSET, chan, chan );
        seDMAC_Start(chan);

        // Wait transfer complete
        while((seS1D13C00Read8(DMAC_ENDIF) & chan) == 0);
        seSetBits8( DMAC_ENDIF, chan, chan );

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    if ((nhwords % 1024) != 0)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, (nhwords%1024)-1, 0UL,
                               seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD,
                               seDMAC_INC_NO, seDMAC_INC_2 );

        seDMAC_SetChannel( cdata1, ( uint32_t ) &(fillhword), ( uint32_t )(dstaddr+(k*2048)+(nhwords%1024)*2-2), chan );

        seSetBits8( DMAC_ENSET, chan, chan );
        seDMAC_Start(chan);

        // Wait transfer complete
        while((seS1D13C00Read8(DMAC_ENDIF) & chan) == 0);
        seSetBits8( DMAC_ENDIF, chan, chan );

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    seDMAC_Disable(  chan );
}




void seDMAC_MemFill32 (uint32_t dstaddr, uint32_t nwords, uint32_t fillword, seDMAC_CHANNEL chan)
{
    uint32_t k, m, cdata1;

    ///< Configure the primary data structure for the DMA channel
    seS1D13C00Write8( DMAC_CFG, 1 );    // enable controller
    seDMAC_DisableRequestMask( chan );

    seDMAC_Enable( chan );
    seSetBits8( DMAC_ENDIF, chan, chan );

    m = nwords / 1024;
    k = 0;
    for (k = 0; k < m; k++)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, 1024-1, 0UL,
                               seDMAC_SIZE_WORD, seDMAC_SIZE_WORD,
                               seDMAC_INC_NO, seDMAC_INC_4 );
        seDMAC_SetChannel( cdata1, ( uint32_t ) &(fillword), ( uint32_t )(dstaddr+(k*4096)+4096-4), chan );

        seSetBits8( DMAC_ENSET, chan, chan );
        seDMAC_Start(chan);

        // Wait transfer complete
        while((seS1D13C00Read8(DMAC_ENDIF) & chan) == 0);
        seSetBits8( DMAC_ENDIF, chan, chan );

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    if ((nwords % 1024) != 0)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, (nwords%1024)-1, 0UL,
                               seDMAC_SIZE_WORD, seDMAC_SIZE_WORD,
                               seDMAC_INC_NO, seDMAC_INC_4 );

        seDMAC_SetChannel( cdata1, ( uint32_t ) &(fillword), ( uint32_t )(dstaddr+(k*4096)+(nwords%1024)*4-4), chan );

        seSetBits8( DMAC_ENSET, chan, chan );
        seDMAC_Start(chan);

        // Wait transfer complete
        while((seS1D13C00Read8(DMAC_ENDIF) & chan) == 0);
        seSetBits8( DMAC_ENDIF, chan, chan );

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    seDMAC_Disable(  chan );
}




void seDMAC_MemCpy8 (uint32_t srcaddr, uint32_t dstaddr, uint32_t nbytes, seDMAC_CHANNEL chan)
{
    uint32_t k, m, cdata1;

    ///< Configure the primary data structure for the DMA channel
    seS1D13C00Write8(DMAC_CFG, 0x01);    // Enable DMAC
    seDMAC_DisableRequestMask( seDMAC_CH2 );

    seS1D13C00Write8(DMAC_ENSET, chan);     // Enable CHx
    seS1D13C00Write8(DMAC_ENDIF, chan);     // Clear CHx completion interrupt flag
    seS1D13C00Write8(DMAC_ENDIESET, chan);  // Enable CHx completion interrupt enable

    m = nbytes / 1024;
    k = 0;
    for (k = 0; k < m; k++)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, 1024-1, 0UL,
                               seDMAC_SIZE_BYTE, seDMAC_SIZE_BYTE,
                               seDMAC_INC_1, seDMAC_INC_1 );
        seDMAC_SetChannel( cdata1, ( uint32_t )(srcaddr+(k*1024)+1024-1), ( uint32_t )(dstaddr+(k*1024)+1024-1), chan );

        seS1D13C00Write8(DMAC_ENSET, chan);     // Enable CHx
        seDMAC_Start(chan);

        // Wait transfer complete interrupt
        while((seS1D13C00Read16(SYS_INTS) & SYS_DMACINT_bits) == 0);
        seS1D13C00Write8(DMAC_ENDIF, chan);     // Clear CHx completion interrupt flag

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    if ((nbytes % 1024) != 0)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, (nbytes%1024)-1, 0UL,
                               seDMAC_SIZE_BYTE, seDMAC_SIZE_BYTE,
                               seDMAC_INC_1, seDMAC_INC_1 );

        seDMAC_SetChannel( cdata1, ( uint32_t )(srcaddr+(k*1024)+(nbytes%1024)-1), ( uint32_t )(dstaddr+(k*1024)+(nbytes%1024)-1), chan );

        seS1D13C00Write8(DMAC_ENSET, chan);     // Enable CHx
        seDMAC_Start(chan);

        // Wait transfer complete interrupt
        while((seS1D13C00Read16(SYS_INTS) & SYS_DMACINT_bits) == 0);
        seS1D13C00Write8(DMAC_ENDIF, chan);     // Clear CHx completion interrupt flag

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    seDMAC_DisableInt( seDMAC_TRANSF_COMPL, chan   );
    seDMAC_EnableRequestMask( chan );
    seDMAC_Disable( chan );
}


void seDMAC_MemCpy16 (uint32_t srcaddr, uint32_t dstaddr, uint32_t nhwords, seDMAC_CHANNEL chan)
{
    uint32_t k, m, cdata1;

    ///< Configure the primary data structure for the DMA channel
    seS1D13C00Write8(DMAC_CFG, 0x01);    // Enable DMAC
    seDMAC_DisableRequestMask( seDMAC_CH2 );

    seS1D13C00Write8(DMAC_ENSET, chan);     // Enable CHx
    seS1D13C00Write8(DMAC_ENDIF, chan);     // Clear CHx completion interrupt flag
    seS1D13C00Write8(DMAC_ENDIESET, chan);  // Enable CHx completion interrupt enable

    m = nhwords / 1024;
    k = 0;
    for (k = 0; k < m; k++)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, 1024-1, 0UL,
                               seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD,
                               seDMAC_INC_2, seDMAC_INC_2 );
        seDMAC_SetChannel( cdata1, ( uint32_t )(srcaddr+(k*2048)+2048-2), ( uint32_t )(dstaddr+(k*2048)+2048-2), chan );

        seS1D13C00Write8(DMAC_ENSET, chan);     // Enable CHx
        seDMAC_Start(chan);

        // Wait transfer complete interrupt
        while((seS1D13C00Read16(SYS_INTS) & SYS_DMACINT_bits) == 0);
        seS1D13C00Write8(DMAC_ENDIF, chan);     // Clear CHx completion interrupt flag

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    if ((nhwords % 1024) != 0)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, (nhwords%1024)-1, 0UL,
                               seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD,
                               seDMAC_INC_2, seDMAC_INC_2 );

        seDMAC_SetChannel( cdata1, ( uint32_t )(srcaddr+(k*2048)+(nhwords%1024)*2-2), ( uint32_t )(dstaddr+(k*2048)+(nhwords%1024)*2-2), chan );

        seS1D13C00Write8(DMAC_ENSET, chan);     // Enable CHx
        seDMAC_Start(chan);

        // Wait transfer complete interrupt
        while((seS1D13C00Read16(SYS_INTS) & SYS_DMACINT_bits) == 0);
        seS1D13C00Write8(DMAC_ENDIF, chan);     // Clear CHx completion interrupt flag

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    seDMAC_DisableInt( seDMAC_TRANSF_COMPL, chan   );
    seDMAC_EnableRequestMask( chan );
    seDMAC_Disable( chan );
}


void seDMAC_MemCpy32 (uint32_t srcaddr, uint32_t dstaddr, uint32_t nwords, seDMAC_CHANNEL chan)
{
    uint32_t k, m, cdata1;

    ///< Configure the primary data structure for the DMA channel
    seS1D13C00Write8(DMAC_CFG, 0x01);    // Enable DMAC
    seDMAC_DisableRequestMask( seDMAC_CH2 );

    seS1D13C00Write8(DMAC_ENSET, chan);     // Enable CHx
    seS1D13C00Write8(DMAC_ENDIF, chan);     // Clear CHx completion interrupt flag
    seS1D13C00Write8(DMAC_ENDIESET, chan);  // Enable CHx completion interrupt enable

    m = nwords / 1024;
    k = 0;
    for (k = 0; k < m; k++)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, 1024-1, 0UL,
                               seDMAC_SIZE_WORD, seDMAC_SIZE_WORD,
                               seDMAC_INC_4, seDMAC_INC_4 );
        seDMAC_SetChannel( cdata1, ( uint32_t )(srcaddr+(k*4096)+4096-4), ( uint32_t )(dstaddr+(k*4096)+4096-4), chan );

        seS1D13C00Write8(DMAC_ENSET, chan);     // Enable CHx
        seDMAC_Start(chan);

        // Wait transfer complete interrupt
        while((seS1D13C00Read16(SYS_INTS) & SYS_DMACINT_bits) == 0);
        seS1D13C00Write8(DMAC_ENDIF, chan);     // Clear CHx completion interrupt flag

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    if ((nwords % 1024) != 0)
    {
        cdata1 = seDMAC_cdata( seDMAC_MODE_AUTO_REQ, (nwords%1024)-1, 0UL,
                               seDMAC_SIZE_WORD, seDMAC_SIZE_WORD,
                               seDMAC_INC_4, seDMAC_INC_4 );

        seDMAC_SetChannel( cdata1, ( uint32_t )(srcaddr+(k*4096)+(nwords%1024)*4-4), ( uint32_t )(dstaddr+(k*4096)+(nwords%1024)*4-4), chan );

        seS1D13C00Write8(DMAC_ENSET, chan);     // Enable CHx
        seDMAC_Start(chan);

        // Wait transfer complete interrupt
        while((seS1D13C00Read16(SYS_INTS) & SYS_DMACINT_bits) == 0);
        seS1D13C00Write8(DMAC_ENDIF, chan);     // Clear CHx completion interrupt flag

        // Check that DMA is stopped
        while ( (seDMAC_GetMode( chan ) != seDMAC_MODE_STOP ) );
    }

    seDMAC_DisableInt( seDMAC_TRANSF_COMPL, chan   );
    seDMAC_EnableRequestMask( chan );
    seDMAC_Disable( chan );
}

