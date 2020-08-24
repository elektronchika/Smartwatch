/**
  ******************************************************************************
  * @file    se_spi.c
  * @author  Epson
  * @version V1.0
  * @date    28-September-2017
  * @brief   This file provides the SPI hardware functions.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#include "s1d13c00_hcl.h"
#include "s1d13C00_memregs.h"
#include "se_common.h"
#include "se_clg.h"
#include "se_t16.h"
#include "se_port.h"
#include "se_spi.h"
#include "se_dmac.h"


seStatus seSPI_Init( seSPI_InitTypeDef* SPI_InitStruct )
{
    seStatus fStatus = seSTATUS_NG;

    ///< Stop SPI.
    seSPI_Stop();

    ///< Configure SPI control as per the init structure
    seSetBits16( SPI_MOD, SPI_MST_bits, SPI_InitStruct->MST << 0 );
    seSetBits16( SPI_MOD, SPI_CPOL_bits, SPI_InitStruct->CPOL << 1 );
    seSetBits16( SPI_MOD, SPI_CPHA_bits, SPI_InitStruct->CPHA << 2 );
    seSetBits16( SPI_MOD, SPI_LSBFST_bits, SPI_InitStruct->LSBFST << 3  );
    seSetBits16( SPI_MOD, SPI_NOCLKDIV_bits, SPI_InitStruct->NOCLKDIV << 4 );
    seSetBits16( SPI_MOD, SPI_PUEN_bits, SPI_InitStruct->PUEN << 5 );
    seSetBits16( SPI_MOD, SPI_CHLN_bits, SPI_InitStruct->CHLN << 8  );

    fStatus = ConfigurePortsForSPI( SPI_InitStruct->MST );

    return fStatus;
}


void seSPI_InitStructForMaster( seSPI_InitTypeDef* SPI_InitStruct )
{
    SPI_InitStruct->CHLN     = seDATA_TR_LENGTH_08BIT;
    SPI_InitStruct->CPHA     = seSPI_PH_RISE;
    SPI_InitStruct->CPOL     = seSPI_POL_LOW;
    SPI_InitStruct->LSBFST   = seSPI_MSB_FST;
    SPI_InitStruct->MST      = seSPI_MODE_MASTER;
    SPI_InitStruct->NOCLKDIV = seDISABLE;
    SPI_InitStruct->PUEN     = seENABLE;
}


void seSPI_InitStructForSlave( seSPI_InitTypeDef* SPI_InitStruct )
{
    SPI_InitStruct->CHLN     = seDATA_TR_LENGTH_08BIT;
    SPI_InitStruct->CPHA     = seSPI_PH_RISE;
    SPI_InitStruct->CPOL     = seSPI_POL_LOW;
    SPI_InitStruct->LSBFST   = seSPI_MSB_FST;
    SPI_InitStruct->MST      = seSPI_MODE_SLAVE;
    SPI_InitStruct->PUEN     = seENABLE;
}


seStatus seSPI_Start( void )
{
    seStatus fStatus = seSTATUS_NG;
    int16_t spiMOD = seS1D13C00Read16( SPI_MOD );

    ///< Enable T16 (Master mode only)
    if ( (spiMOD & SPI_MST_bits) == SPI_MST_bits )
    {
        seT16_Enable( T16_1_REGS_BASE );
        if ( (spiMOD & SPI_NOCLKDIV_bits) == seDISABLE ) {
            seT16_Start( T16_1_REGS_BASE );
        }
    }

    fStatus = seSPI_Reset();

    ///< Enable SPI
    seSetBits16( SPI_CTL, SPI_MODEN_bits, SPI_MODEN_bits );

    return fStatus;
}


void seSPI_Stop( void )
{
    ///< Disable SPI.
    seSetBits16( SPI_CTL, SPI_MODEN_bits, 0 );

    ///< Disable T16 (Master mode only).
    if ( (seS1D13C00Read16(SPI_MOD) & SPI_MODEN_bits) == SPI_MODEN_bits )
    {
        seT16_Disable( T16_1_REGS_BASE );
        if ( (seS1D13C00Read16( SPI_MOD ) & SPI_NOCLKDIV_bits) == seDISABLE) {
            seT16_Stop( T16_1_REGS_BASE );
        }
    }
}


seStatus seSPI_Reset( void )
{
    seStatus fStatus = seSTATUS_OK;

    ///< Execute software reset.
    seSetBits16(SPI_CTL, SPI_SFTRST_bits, SPI_SFTRST_bits );
    while( seS1D13C00Read16( SPI_CTL ) & SPI_SFTRST_bits );

    return fStatus;
}


seStatus seSPI_TxBytes( uint8_t data[], uint32_t size )
{
    seStatus fStatus = seSTATUS_OK;

    while( (seS1D13C00Read16( SPI_INTF ) & SPI_TBEIF_bits) == 0 );

    ///< Send data.
    while ( size-- )
    {
        seS1D13C00Write16( SPI_TXD, *data++ );
        ///< Wait for transmit buffer empty.
        while( (seS1D13C00Read16( SPI_INTF ) & SPI_TBEIF_bits) == 0 );
    }

    ///< Wait for idle state.
    while( (seS1D13C00Read16( SPI_INTF ) & SPI_BSY_bits) );

    return fStatus;
}


seStatus seSPI_DmaTxHWords( uint16_t data[], uint32_t size )
{
    seStatus fStatus = seSTATUS_OK;

    if ( size <= seDMAC_NM_MAX )
    {
        uint32_t size_m1 = size-1;
        uint32_t cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC,size_m1,0UL,seDMAC_SIZE_HALF_WORD,seDMAC_SIZE_HALF_WORD,seDMAC_INC_2,seDMAC_INC_NO );

        seDMAC_SetChannel( cdata1, (uint32_t)data + size_m1 * 2, SPI_TXD, seDMAC_CH0 );
        seDMAC_Enable( seDMAC_CH0 );
        seDMAC_DisableRequestMask( seDMAC_CH0 );
        seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH0 );
        seOrBits16( SPI_TBEDMAEN, seDMAC_CH0 );
        do {
            if ( seDMAC_GetIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH0 ) )
            {
                seAndBits16( SPI_TBEDMAEN, ~seDMAC_CH0 );
                seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH0 );
            }
        } while ( seDMAC_GetMode( seDMAC_CH0 ) != seDMAC_MODE_STOP );

        seAndBits16( SPI_TBEDMAEN, ~seDMAC_CH0 );
        seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH0 );
        seDMAC_EnableRequestMask( seDMAC_CH0 );
        seDMAC_Disable( seDMAC_CH0 );
        while( seS1D13C00Read16( SPI_INTF) & SPI_BSY_bits );
    } else {
        fStatus = seSTATUS_NG;
    }

  return fStatus;
}


seStatus seSPI_TxHWords( uint16_t data[], uint32_t size )
{
    seStatus fStatus = seSTATUS_OK;

    while( (seS1D13C00Read16(SPI_INTF) & SPI_TBEIF_bits) == 0 );

    while( size-- )
    {
        ///< Send data.
        seS1D13C00Write16( SPI_TXD, *data++ );
        ///< Wait for transmit buffer empty.
        while( (seS1D13C00Read16( SPI_INTF ) & SPI_TBEIF_bits) == 0 );
    }

    ///< Wait for idle state.
    while( seS1D13C00Read16( SPI_INTF ) & SPI_BSY_bits );

    return fStatus;
}


seStatus seSPI_RxBytes( uint8_t data[], uint32_t size )
{
    uint8_t dummy = seS1D13C00Read16( SPI_RXD ); //< dummy data
    seStatus fStatus = seSTATUS_OK;

    if ( (seS1D13C00Read16(SPI_MOD) & SPI_MST_bits) == SPI_MST_bits )
    {
        ///< Set transmit dummy data(Master mode only).
        ///< Check transmit buffer empty.
        while( (seS1D13C00Read16( SPI_INTF ) & SPI_TBEIF_bits) == 0 );

        ///< Receive data (Master).
        while ( size-- )
        {
            ///< Set transmit dummy data(Master mode only).
            seS1D13C00Write16( SPI_TXD, dummy );
            ///< Check receive buffer full.
            while( (seS1D13C00Read16( SPI_INTF ) & SPI_RBFIF_bits) == 0 );
            *data++ = seS1D13C00Read16( SPI_RXD );
        }
    } else {
        ///< Receive data (Slave).
        while ( size-- )
        {
            ///< Check receive buffer full.
            while( (seS1D13C00Read16( SPI_INTF ) & SPI_RBFIF_bits) == 0 );
            *data++ = seS1D13C00Read16( SPI_RXD );
        }
    }

    ///< Wait for idle state.
    while( seS1D13C00Read16(SPI_INTF) & SPI_BSY_bits );

    return fStatus;
}


seStatus seSPI_RxHWords( uint16_t data[], uint32_t size )
{
    // RELE - check the 8/16 bitness compared to the original
    seStatus fStatus = seSTATUS_OK;
    uint8_t dummy = seS1D13C00Read8( SPI_RXD );   // dummy read;

    if ( (seS1D13C00Read16( SPI_MOD ) & SPI_MST_bits) == SPI_MST_bits )
    {
        ///< Set transmit dummy data(Master mode only).
        ///< Check transmit buffer empty.
        while( (seS1D13C00Read16( SPI_INTF ) & SPI_TBEIF_bits) == 0 );
        seS1D13C00Write8( SPI_TXD, dummy );
        while( (seS1D13C00Read16( SPI_INTF ) & SPI_TBEIF_bits) == 0 );

        ///< Receive data (Master).
        while ( size-- )
        {
            ///< Set transmit dummy data(Master mode only).
            seS1D13C00Write8( SPI_TXD, dummy );
            ///< Check receive buffer full.
            while( (seS1D13C00Read16( SPI_INTF ) & SPI_RBFIF_bits) == 0 );
            *data++ = seS1D13C00Read16( SPI_RXD );
        }
    } else {
        ///< Receive data (Slave)
        while ( size-- )
        {
            ///< Check receive buffer full.
            while( (seS1D13C00Read16(SPI_INTF) & SPI_RBFIF_bits) == 0 );
            *data++ = seS1D13C00Read16( SPI_RXD );
        }
    }

    ///< Wait for idle
    while( seS1D13C00Read16(SPI_INTF) & SPI_BSY_bits );

    return fStatus;
}

seStatus seSPI_DmaRxHWords( uint16_t data[], uint32_t size )
{
    uint16_t dummy = seS1D13C00Read16( SPI_RXD );    //< dummy read
    seStatus fStatus = seSTATUS_OK;
    uint32_t size_m1 = size-1;

    ///< Set transmit dummy data (Master mode only. Continuous data reception).
    if ( (seS1D13C00Read16( SPI_MOD ) & SPI_MST_bits) == SPI_MST_bits )
    {
        // Configure the primary data structure for the DMA channel (Ch.1) used for writing dummy byte to the TXD register
        uint32_t cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC,size_m1,0UL,seDMAC_SIZE_HALF_WORD,seDMAC_SIZE_HALF_WORD,seDMAC_INC_NO,seDMAC_INC_NO );
        seDMAC_SetChannel( cdata1, (uint32_t)&dummy, SPI_TXD, seDMAC_CH1 );
        // Configure the primary data structure for the DMA channel (Ch.2) used for reading data from the RXD
        cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC,size_m1,0UL,seDMAC_SIZE_HALF_WORD,seDMAC_SIZE_HALF_WORD,seDMAC_INC_NO,seDMAC_INC_2);
        seDMAC_SetChannel( cdata1, SPI_RXD, (uint32_t)data+size_m1*2, seDMAC_CH2 );
        seDMAC_Enable( seDMAC_CNLS( seDMAC_CH1 | seDMAC_CH2 ) );
        // Increase the priority of the DMA channel used for reading data
        seDMAC_PriorityIncrease(  seDMAC_CH2 );
        // Clear the channel request masks for both the DMA channels
        seDMAC_DisableRequestMask( seDMAC_CNLS( seDMAC_CH1 | seDMAC_CH2 ) );
        // Clear the DMA transfer completion interrupt flags
        seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CNLS( seDMAC_CH1 | seDMAC_CH2 ) );
        // Enable only the DMA transfer completion interrupt of the DMA channel used for reading
        seDMAC_EnableInt( seDMAC_TRANSF_COMPL, seDMAC_CH2 );
        // Clear pending DMA interrupts in the CPU core
        // Enable pending DMA interrupts in the CPU core

        // Enable the QSPI to issue DMA transfer requests to both the DMA channels
        seOrBits16( SPI_TBEDMAEN, seDMAC_CH1 );
        seOrBits16( SPI_RBFDMAEN, seDMAC_CH2 );

        do {
            if ( seDMAC_GetIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 ) )
            {
                seAndBits16( SPI_RBFDMAEN, ~seDMAC_CH2 );
                seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 );
            }
        } while ( (seDMAC_GetMode( seDMAC_CH2 ) != seDMAC_MODE_STOP ) );

        seAndBits16( SPI_RBFDMAEN, ~seDMAC_CH2 );
        seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 );
        seAndBits16( SPI_TBEDMAEN, ~seDMAC_CH1 );
        seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH1 );

        seDMAC_EnableRequestMask( seDMAC_CNLS( seDMAC_CH1 | seDMAC_CH2 ) );
        seDMAC_Disable( seDMAC_CNLS( seDMAC_CH1 | seDMAC_CH2 ) );
    } else {

        // Configure the primary data structure for the DMA channel (Ch.2) used for reading data from the RXD
        uint32_t cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC, size_m1, 0UL, seDMAC_SIZE_HALF_WORD, seDMAC_SIZE_HALF_WORD, seDMAC_INC_NO, seDMAC_INC_2 );
        seDMAC_SetChannel( cdata1, SPI_RXD, (uint32_t)data + size_m1 * 2, seDMAC_CH2 );
        // Enable the DMA channel
        seDMAC_Enable( seDMAC_CH2 );
        // Increase the priority of the DMA channel used for reading data
        seDMAC_PriorityIncrease( seDMAC_CH2 );
        // Clear the channel request masks for both the DMA channels
        seDMAC_DisableRequestMask( seDMAC_CH2 );
        // Clear the DMA transfer completion interrupt flags
        seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 );
        // Enable only the DMA transfer completion interrupt of the DMA channel used for reading
        seDMAC_EnableInt( seDMAC_TRANSF_COMPL, seDMAC_CH2 );
        // Clear pending DMA interrupts in the CPU core
        // Enable pending DMA interrupts in the CPU core
        // Enable the QSPI to issue DMA transfer requests to the DMA channel
        seOrBits16( SPI_RBFDMAEN,  seDMAC_CH2 );

        do {
            if ( seDMAC_GetIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 ) )
            {

                seAndBits16( SPI_RBFDMAEN,  ~seDMAC_CH2 );
                seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 );
            }
        } while ( (seDMAC_GetMode( seDMAC_CH2 ) != seDMAC_MODE_STOP ) );

        seAndBits16( SPI_RBFDMAEN,  ~seDMAC_CH2 );
        seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 );

        seDMAC_EnableRequestMask( seDMAC_CH2 );
        seDMAC_Disable( seDMAC_CH2 );
    }

    ///< wait for idle state.
    while( seS1D13C00Read16(SPI_INTF) & SPI_BSY_bits );

    return fStatus;
}


void seSPI_EnableInt( seSPI_Interrupt irq )
{
    seOrBits16( SPI_INTE, irq );
}


void seSPI_DisableInt( seSPI_Interrupt irq )
{
    seAndBits16( SPI_INTE, ~irq );
}


seInterruptStatus seSPI_GetIntFlag( seSPI_IntFlag flag )
{
    seInterruptStatus fStatus = seINTERRUPT_NOT_OCCURRED;
    uint16_t flagged = seS1D13C00Read16( SPI_INTF );

    if ( flag )
    {
        flagged &= flag;
        if ( flagged == flag ) {
            fStatus = seINTERRUPT_OCCURRED;
        }
    }

    return fStatus;
}


void seSPI_ClearIntFlag( seSPI_IntFlag flag )
{
    seS1D13C00Write16( SPI_INTF,  flag );
}


seStatus seSPI_SetBusSpeed(  uint32_t speed )
{
    seStatus fStatus = seSTATUS_NG;

    if ( speed != 0 )
    {
        if ( (seS1D13C00Read16( SPI_MOD ) & SPI_NOCLKDIV_bits) == 0 )
        {
            uint32_t spiclk = seT16_GetClk( T16_1_REGS_BASE );
            if ( spiclk )
            {
                fStatus = seSTATUS_OK;
                seS1D13C00Write16( T16_1_REGS_BASE  + T16_TR_OFFSET, spiclk/speed/2 - 1 );
            }
        }
    }

    return fStatus;
}


uint32_t seSPI_GetBusSpeed( void )
{
    uint32_t spiclk = 0;
    uint32_t spidiv = 1;

    spiclk = seT16_GetClk( T16_1_REGS_BASE );

    switch ( seS1D13C00Read16( SPI_MOD ) & SPI_NOCLKDIV_bits )
    {
        case 1:
            break;
        case 0:
            spiclk = spiclk / 2 / ( seS1D13C00Read16(T16_1_REGS_BASE + T16_TR_OFFSET) + 1);
            break;
        default:
            break;
    }

    return ( spiclk / spidiv );
}


seStatus ConfigurePortsForSPI( seSPI_OperMode OperMode )
{
    seStatus fStatus = seSTATUS_OK;

    // Initialize SDI pin
    if (fStatus == seSTATUS_OK)
        fStatus = sePORT_InitAsAltFunction( sePORT_P02, sePORT_ALT_0 );

    // Initialize SDO pin
    if (fStatus == seSTATUS_OK)
        fStatus = sePORT_InitAsAltFunction( sePORT_P03, sePORT_ALT_0 );

    // Initialize SPICLK pin
    if (fStatus == seSTATUS_OK)
        fStatus = sePORT_InitAsAltFunction( sePORT_P01, sePORT_ALT_0 );

    // Initialize SPISS pin for slave
    if ((fStatus == seSTATUS_OK) && (OperMode == seSPI_MODE_SLAVE))
    {
        fStatus = sePORT_InitAsAltFunction( sePORT_P00, sePORT_ALT_0 );
    }

  return fStatus;
}
