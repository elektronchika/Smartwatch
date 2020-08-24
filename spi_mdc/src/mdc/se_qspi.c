/**
  ******************************************************************************
  * @file    se_qspi.c
  * @author  Epson
  * @version V1.0
  * @date    21-September-2017
  * @brief   This file provides the QSPI hardware functions as defined in
  *          se_qspi.h. The code in this library is based on the S1C31xxx QSPI
  *          library.
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
#include "se_dmac.h"
#include "se_qspi.h"



seStatus seQSPI_Init( seQSPI_InitTypeDef* QSPI_InitStruct )
{
    seStatus fStatus = seSTATUS_NG;

    ///< Stop QSPI.
    seQSPI_Stop();

    ///< Setup QSPI
    seSetBits16( QSPI_MOD, QSPI_MST, QSPI_InitStruct->MST << 0 );               ///< Set master/slave mode.
    seSetBits16( QSPI_MOD, QSPI_CPOL, QSPI_InitStruct->CPOL << 1 );             ///< Set clock polarity.
    seSetBits16( QSPI_MOD, QSPI_CPHA, QSPI_InitStruct->CPHA << 2 );             ///< Set clock phase.
    seSetBits16( QSPI_MOD, QSPI_LSBFST, QSPI_InitStruct->LSBFST << 3 );         ///< Set MSB first/LSB first.
    seSetBits16( QSPI_MOD, QSPI_NOCLKDIV, QSPI_InitStruct->NOCLKDIV << 4);      ///< Set master mode operating clock.
    seSetBits16( QSPI_MOD, QSPI_PUEN, QSPI_InitStruct->PUEN << 5 );             ///< Enable/Disable pull-up/down.
    seSetBits16( QSPI_MOD, QSPI_TMOD, QSPI_InitStruct->TMOD << 6 );             ///< Set Transfer Mode
    seSetBits16( QSPI_MOD, QSPI_CHLN, QSPI_InitStruct->CHLN << 8);              ///< Set number of clocks for data transfer
    seSetBits16( QSPI_MOD, QSPI_CHDL, QSPI_InitStruct->CHDL << 12);             ///< Set the number of clocks to drive the serial data lines

    ///< If using memory mapped access mode
    seSetBits16( QSPI_MMACFG1, QSPI_TCSH, QSPI_InitStruct->TCSH );              ///< Set slave select signal negation period
    seSetBits16( QSPI_RMADRH,  QSPI_RMADR, QSPI_InitStruct->RMADR << 4 );       ///< Set remapping address
    seSetBits16( QSPI_MMACFG2, QSPI_ADRCYC, QSPI_InitStruct->ADRCYC << 1 );     ///< Set 24 or 32-bit address cycle
    seSetBits16( QSPI_MMACFG2, QSPI_ADRTMOD, QSPI_InitStruct->ADRTMOD << 2 );   ///< Set address cycle transfer mode
    seSetBits16( QSPI_MMACFG2, QSPI_DUMTMOD, QSPI_InitStruct->DUMTMOD << 4 );   ///< Set dummy cycle transfer mode
    seSetBits16( QSPI_MMACFG2, QSPI_DATTMOD, QSPI_InitStruct->DATTMOD << 6 );   ///< Set data cycle transfer mode
    seSetBits16( QSPI_MMACFG2, QSPI_DUMLN, QSPI_InitStruct->DUMLN << 8 );       ///< Set dummy cycle length
    seSetBits16( QSPI_MMACFG2, QSPI_DUMDL, QSPI_InitStruct->DUMDL << 12 );      ///< Set dummy cycle drive length
    seSetBits16( QSPI_MB, QSPI_XIPEXT, QSPI_InitStruct->XIPEXT << 0 );          ///< Set XIP termination mode byte
    seSetBits16( QSPI_MB, QSPI_XIPACT, QSPI_InitStruct->XIPACT << 8 );          ///< Set XIP activation mode byte

    ///< Execute software reset.
    fStatus = seQSPI_Reset();

    ///< Enable QSPI
    seSetBits16( QSPI_CTL, QSPI_MODEN, QSPI_MODEN );

    ///< Set the following bits when using interrupt
    seS1D13C00Write16( QSPI_INTF, 0xFFFF );                         ///< Clear ALL interrupt flags
    seS1D13C00Write16( QSPI_INTE, QSPI_InitStruct->INTE );          ///< Enable configured interrupts.

    return fStatus;
}


void seQSPI_InitStructForMaster( seQSPI_InitTypeDef* QSPI_InitStruct )
{
    ///< Set QSPI variables
    QSPI_InitStruct->CHLN     = seQSPI_08CLK;
    QSPI_InitStruct->CHDL     = seQSPI_08CLK;
    QSPI_InitStruct->CPHA     = seQSPI_PH_RISE;
    QSPI_InitStruct->CPOL     = seQSPI_POL_LOW;
    QSPI_InitStruct->LSBFST   = seQSPI_MSB_FST;
    QSPI_InitStruct->MST      = seQSPI_MODE_MASTER;
    QSPI_InitStruct->TMOD     = seQSPI_MODE_SINGLE;
    QSPI_InitStruct->NOCLKDIV = seDISABLE;
    QSPI_InitStruct->PUEN     = seENABLE;

    //====== If using MMA mode ======//
    QSPI_InitStruct->TCSH     = seQSPI_08CLK;
    QSPI_InitStruct->RMADR    = 0;
    QSPI_InitStruct->DUMDL    = seQSPI_08CLK;
    QSPI_InitStruct->DUMLN    = seQSPI_08CLK;
    QSPI_InitStruct->DATTMOD  = seQSPI_MODE_SINGLE;
    QSPI_InitStruct->DUMTMOD  = seQSPI_MODE_SINGLE;
    QSPI_InitStruct->ADRTMOD  = seQSPI_MODE_SINGLE;
    QSPI_InitStruct->ADRCYC   = seQSPI_24BIT_ADDR;
    QSPI_InitStruct->XIPACT   = 0;
    QSPI_InitStruct->XIPEXT   = 0;

    //====== If interrupt use ======//
    QSPI_InitStruct->INTE     = (seQSPI_Interrupt)0; // No interrupt
}


void seQSPI_InitStructForSlave( seQSPI_InitTypeDef* QSPI_InitStruct )
{
    QSPI_InitStruct->CHLN     = seQSPI_08CLK;
    QSPI_InitStruct->CHDL     = seQSPI_08CLK; ///< CHDL should be equal to CHLN for Slave
    QSPI_InitStruct->CPHA     = seQSPI_PH_RISE;
    QSPI_InitStruct->CPOL     = seQSPI_POL_LOW;
    QSPI_InitStruct->LSBFST   = seQSPI_MSB_FST;
    QSPI_InitStruct->MST      = seQSPI_MODE_SLAVE;
    QSPI_InitStruct->TMOD     = seQSPI_MODE_SINGLE;
    QSPI_InitStruct->PUEN     = seENABLE;

    //====== If using MMA mode ======//
    QSPI_InitStruct->TCSH     = seQSPI_08CLK;
    QSPI_InitStruct->RMADR    = 0;
    QSPI_InitStruct->DUMDL    = seQSPI_08CLK;
    QSPI_InitStruct->DUMLN    = seQSPI_08CLK;
    QSPI_InitStruct->DATTMOD  = seQSPI_MODE_SINGLE;
    QSPI_InitStruct->DUMTMOD  = seQSPI_MODE_SINGLE;
    QSPI_InitStruct->ADRTMOD  = seQSPI_MODE_SINGLE;
    QSPI_InitStruct->ADRCYC   = seQSPI_24BIT_ADDR;
    QSPI_InitStruct->XIPACT   = 0;
    QSPI_InitStruct->XIPEXT   = 0;

    //====== If interrupt use ======//
    QSPI_InitStruct->INTE     = (seQSPI_Interrupt)0; // No interrupt
}


seStatus seQSPI_Start( void )
{
    seStatus fStatus = seSTATUS_NG;

    ///< Enable T16 (Master mode only)
    if ( (seS1D13C00Read16( QSPI_MOD ) & QSPI_MST) == QSPI_MST)
    {
        seT16_Enable( T16_2_REGS_BASE );
        if ( (seS1D13C00Read16( QSPI_MOD ) & QSPI_NOCLKDIV) == seDISABLE )
        {
            seT16_Start( T16_2_REGS_BASE );
        }
    }

    ///< Execute software reset.
    fStatus = seQSPI_Reset();

    ///< Enable QSPI
    seSetBits16( QSPI_CTL, QSPI_MODEN,  QSPI_MODEN );

    return fStatus;
}

void seQSPI_Stop( void )
{
    uint16_t tmp;

    /// Disable QSPI.
    seSetBits16( QSPI_CTL, QSPI_MODEN, 0 );

    /// Disable T16 (Master mode only)
    tmp = seS1D13C00Read16( QSPI_CTL );
    if ( (tmp && QSPI_MODEN) == QSPI_MODEN )
    {
        seT16_Disable( T16_2_REGS_BASE );
        seT16_Stop( T16_2_REGS_BASE );
    }
}

seStatus seQSPI_Reset( void )
{
    seStatus fStatus = seSTATUS_OK;

    ///< Software reset then wait for the status bit
    seSetBits16( QSPI_CTL, QSPI_SFTRST, QSPI_SFTRST );
    while( seS1D13C00Read16(QSPI_CTL) & QSPI_SFTRST );

    return fStatus;
}

seStatus seQSPI_TxValue( uint8_t value, uint32_t count )
{
    seStatus fStatus = seSTATUS_OK;

    while ( count-- )
    {
        while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_TBEIF) == 0 );
        seS1D13C00Write16( QSPI_TXD, value );
    }

    while( seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_BSY );

    return fStatus;
}


seStatus seQSPI_TxBytes( uint8_t data[], uint32_t size )
{
    seStatus fStatus = seSTATUS_OK;

    while ( size-- )
    {
        while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_TBEIF) == 0 );

        seS1D13C00Write16( QSPI_TXD, *data++ );
    }

    while( seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_BSY );

    return fStatus;
}


seStatus seQSPI_TxHWords( uint16_t data[], uint32_t size )
{
    seStatus fStatus = seSTATUS_OK;

    while ( size-- )
    {
        while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_TBEIF) == 0 );
        seS1D13C00Write16( QSPI_TXD, *data++ );
    }

    while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_BSY) == 0 );

  return fStatus;
}


void seQSPI_SetIO( seQSPI_IO direction )
{
    seSetBits16( QSPI_CTL, QSPI_DIR, direction << 3 );
}

seStatus seQSPI_RxBytes( uint8_t data[], uint32_t size )
{
    seStatus fStatus = seSTATUS_OK;
    uint8_t dummy = seS1D13C00Read16( QSPI_RXD ); //< dummy read

    if ((seS1D13C00Read16( QSPI_MOD ) && 1) == 1)
    {
        ///< Master mode only. Continuous data reception.
        if ( size == 0 )
        {
            size = 1;		// If size is zero we still read 1 byte.
        }
        ///< Check transmit buffer empty.
        while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_TBEIF) == 0 );
        ///< Receive data.
        do {
            ///< Set transmit dummy data (Master mode only)
            seS1D13C00Write16( QSPI_TXD, dummy );
            ///< Check receive buffer full.
            while( (seS1D13C00Read16( QSPI_INTF) & QSPI_INTF_RBFIF) == 0 );
            ///< Read receive data.
            *data++ = seS1D13C00Read16( QSPI_RXD );
        } while ( --size );
    } else {
        // Slave mode only. Continuous data reception.
        ///< Receive data.
        while ( size-- )
        {
            ///< Check receive buffer full.
            while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_RBFIF) == 0 );
            ///< Read receive data.
            *data++ = seS1D13C00Read16( QSPI_RXD );
        }
    }

    ///< wait for idle state.
    while( seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_BSY );

  return fStatus;
}

seStatus seQSPI_RxHWords( uint16_t data[], uint32_t size )
{
    seStatus fStatus = seSTATUS_OK;
    uint8_t dummy = seS1D13C00Read8( QSPI_RXD ); //< dummy read

    if ((seS1D13C00Read16( QSPI_MOD ) && 1) == 1)
    {
        ///< Master mode only. Continuous data reception.

        // If size is zero we still read 1 byte.
        if ( size == 0 )
        {
            size = 1;
        }
        ///< Check transmit buffer empty.
        while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_TBEIF) == 0 );

        ///< Receive data.
        do {
            ///< Set transmit dummy data(Master mode only).
            seS1D13C00Write16( QSPI_TXD, dummy);
            ///< Check receive buffer full.
            while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_RBFIF) == 0 );
            ///< Read receive data.
            *data++ = seS1D13C00Read16( QSPI_RXD );
        } while ( --size );

    } else {
        // Slave mode only. Continuous data reception.

        ///< Receive data.
        while ( size-- )
        {
            ///< Check receive buffer full.
            while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_RBFIF) == 0 );
            ///< Read receive data.
            *data++ = seS1D13C00Read16( QSPI_RXD );
        }
    }

    ///< wait for idle state.
    while( seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_BSY );

    return fStatus;
}


seStatus seQSPI_DmaTxHWords( uint16_t data[], uint32_t size, uint32_t localaddr ) {

  seStatus fStatus = seSTATUS_OK;
  uint16_t rdval;

  seS1D13C00Write(localaddr, (uint8_t*) data, size*2);  // Write to local buffer in S1D13C00

  if ( size <= seDMAC_NM_MAX ) {
    uint32_t size_m1 = size-1;
    uint32_t cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC,size_m1,0UL,seDMAC_SIZE_HALF_WORD,seDMAC_SIZE_HALF_WORD,seDMAC_INC_2,seDMAC_INC_NO );
    seDMAC_SetChannel( cdata1, localaddr+size_m1*2, QSPI_TXD, seDMAC_CH0 );
    seDMAC_Enable( seDMAC_CH0 );
    seDMAC_DisableRequestMask( seDMAC_CH0 );
    seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH0 );
    rdval = seS1D13C00Read16(QSPI_TBEDMAEN);
    rdval |= seDMAC_CH0;
    seS1D13C00Write16(QSPI_TBEDMAEN, rdval);
    do {
      while(seDMAC_GetIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH0 ) == seINTERRUPT_NOT_OCCURRED);
      if ( seDMAC_GetIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH0 ) ) {
        rdval  &= ~seDMAC_CH0;
        seS1D13C00Write16(QSPI_TBEDMAEN, rdval);
        seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH0 );
      }
    } while (  seDMAC_GetMode( seDMAC_CH0 ) != seDMAC_MODE_STOP );

    seDMAC_EnableRequestMask( seDMAC_CH0 );
    seDMAC_Disable( seDMAC_CH0 );
    while( seS1D13C00Read16(QSPI_INTF) & QSPI_INTF_BSY );
  } else {
    fStatus = seSTATUS_NG;
  }

  return fStatus;
}


seStatus seQSPI_DmaRxHWords( uint16_t data[], uint32_t size, uint32_t localaddr ) {

  seStatus fStatus = seSTATUS_OK;
  uint16_t rbfdmaen, tbedmaen;

  if ( size <= seDMAC_NM_MAX ) {
    uint32_t size_m1 = size-1;
    uint32_t cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC,size_m1,0UL,seDMAC_SIZE_HALF_WORD,seDMAC_SIZE_HALF_WORD,seDMAC_INC_NO,seDMAC_INC_NO );
    seDMAC_SetChannel( cdata1, 0x20017FF0, QSPI_TXD, seDMAC_CH1 );
    cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC,size_m1,0UL,seDMAC_SIZE_HALF_WORD,seDMAC_SIZE_HALF_WORD,seDMAC_INC_NO,seDMAC_INC_2 );
    seDMAC_SetChannel( cdata1, QSPI_RXD, localaddr+size_m1*2, seDMAC_CH3 );
    seDMAC_Enable( seDMAC_CH1 );
    seDMAC_Enable( seDMAC_CH3 );
    seDMAC_DisableRequestMask( seDMAC_CH1 );
    seDMAC_DisableRequestMask( seDMAC_CH3 );
    seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH1 );
    seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH3 );
    rbfdmaen = seS1D13C00Read16(QSPI_RBFDMAEN);
    rbfdmaen |= seDMAC_CH3;
    seS1D13C00Write16(QSPI_RBFDMAEN, rbfdmaen);
    tbedmaen = seS1D13C00Read16(QSPI_TBEDMAEN);
    tbedmaen |= seDMAC_CH1;
    seS1D13C00Write16(QSPI_TBEDMAEN, tbedmaen);
    do {
      while(seDMAC_GetIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH1 ) == seINTERRUPT_NOT_OCCURRED);
      if ( seDMAC_GetIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH1 ) ) {
        tbedmaen  &= ~seDMAC_CH1;
        seS1D13C00Write16(QSPI_TBEDMAEN, tbedmaen);
        seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH1 );
      }
    } while (  seDMAC_GetMode( seDMAC_CH1 ) != seDMAC_MODE_STOP );

    while( seS1D13C00Read16(QSPI_INTF) & QSPI_INTF_BSY );
  } else {
    fStatus = seSTATUS_NG;
  }

  seS1D13C00Read(localaddr, (uint8_t *) data, size*2);

  seDMAC_EnableRequestMask( seDMAC_CH1 );
  seDMAC_EnableRequestMask( seDMAC_CH3 );
  seDMAC_Disable( seDMAC_CH1 );
  seDMAC_Disable( seDMAC_CH3 );

  return fStatus;
}

seStatus seQSPI_DmaRxMmaWords( uint32_t offset, uint32_t data[], uint32_t size ) {

    uint16_t rdval;
    uint32_t size_m1 = size-1;

  ///< Configure the primary data structure for the DMA channel (Ch.2)
  uint32_t cdata1 = seDMAC_cdata( seDMAC_MODE_BASIC, size_m1, 0UL,
                                 seDMAC_SIZE_WORD, seDMAC_SIZE_WORD,
                                 seDMAC_INC_4, seDMAC_INC_4 );
  seDMAC_SetChannel( cdata1,(offset&0x000FFFFFU)+size_m1*4,( uint32_t )data+size_m1*4, seDMAC_CH2 );
  seDMAC_Enable( seDMAC_CH2 );
  seDMAC_DisableRequestMask( seDMAC_CH2 );
  seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 );
  ///< Enable the QSPI to issue DMA transfer requests to fifo
  rdval = seS1D13C00Read16(QSPI_FRLDMAEN);
  rdval |= seDMAC_CH2;
  seS1D13C00Write16(QSPI_FRLDMAEN, rdval);
  ///< Issue a software DMA transfer request
  seDMAC_Start( seDMAC_CH2 );
  do {
    if ( seDMAC_GetIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 ) ) {
      rdval  &= ~seDMAC_CH2;
      seS1D13C00Write16(QSPI_FRLDMAEN, rdval);
      seDMAC_ClearIntFlag( seDMAC_TRANSF_COMPL, seDMAC_CH2 );
    }
  } while ( (seDMAC_GetMode( seDMAC_CH2 ) != seDMAC_MODE_STOP ) );

  seDMAC_EnableRequestMask( seDMAC_CH2 );
  seDMAC_Disable(  seDMAC_CH2 );

  return seSTATUS_OK;
}



void seQSPI_EnableInt( seQSPI_Interrupt irq )
{
    seOrBits16( QSPI_INTE, irq );
}

void seQSPI_DisableInt( seQSPI_Interrupt irq )
{
    seAndBits16( QSPI_INTE, ~irq );
}

seInterruptStatus seQSPI_GetIntFlag( seQSPI_IntFlag flag )
{
    seInterruptStatus fStatus = seINTERRUPT_NOT_OCCURRED;
    uint16_t flagged = seS1D13C00Read16( QSPI_INTF );

    if ( flag )
    {
        flagged &= flag;
        if ( flagged == flag )
        {
            fStatus = seINTERRUPT_OCCURRED;
        }
    }

    return fStatus;
}

void seQSPI_ClearIntFlag( seQSPI_IntFlag flag )
{
    seS1D13C00Write16( QSPI_INTF, flag );
}

seStatus seQSPI_SetBusSpeed( uint32_t speed )
{
    seStatus fStatus = seSTATUS_NG;

    if ( speed != 0 )
    {
        if ( (seS1D13C00Read16( QSPI_MOD ) & QSPI_NOCLKDIV) == 0 )
        {
            uint32_t qspiclk = 0;
            qspiclk = seT16_GetClk( T16_2_REGS_BASE );
            if ( qspiclk && qspiclk/2 >= speed )
            {
                fStatus = seSTATUS_OK;
                seS1D13C00Write16( T16_2_REGS_BASE + T16_TR_OFFSET, qspiclk/speed/2 - 1);
            }
            else
            {
                seS1D13C00Write16( T16_2_REGS_BASE + T16_TR_OFFSET, 0 ); // Limit bus speed to max possible with this clocks
            }
          }
      }

      return fStatus;
}


uint32_t seQSPI_GetBusSpeed( void )
{
    uint32_t qspiclk = 0;
    uint32_t qspidiv = 1;
    uint32_t t;

    qspiclk = seT16_GetClk( T16_2_REGS_BASE );

    switch ( (seS1D13C00Read16( QSPI_MOD) & QSPI_NOCLKDIV) >> 4)
    {
      case 1:
        break;
      case 0:
        t = seS1D13C00Read16( T16_2_REGS_BASE + T16_TR_OFFSET );
        qspiclk = qspiclk/2/( t + 1) ;
        break;
      default:
        break;
    }

    return ( qspiclk / qspidiv );
}

seStatus seQSPI_SetMasterRxMMA( uint32_t raddr, uint16_t flash_rcmd )
{
    seStatus fStatus = seSTATUS_OK;
    uint16_t rdval;

    rdval = seS1D13C00Read16( QSPI_MMACFG2 );
    if (( rdval & QSPI_MMAEN) != 0)
    {
        fStatus = seSTATUS_NG;
    } else {
        seQSPI_ASSERT_MST_CS0();
        seQSPI_SetMode(seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK);
        seQSPI_SetIO( seQSPI_Output );
        while( (seS1D13C00Read16( QSPI_INTF) & QSPI_INTF_BSY) != 0 );
        seS1D13C00Write16( QSPI_TXD, flash_rcmd );
        while( (seS1D13C00Read16( QSPI_INTF) & QSPI_INTF_TBEIF) == 0 );
        while( (seS1D13C00Read16( QSPI_INTF) & QSPI_INTF_TENDIF) == 0 );
        seSetBits16( QSPI_INTF, QSPI_INTF_TENDIF, QSPI_INTF_TENDIF );
        if ( fStatus == seSTATUS_OK ) {
          seQSPI_SetMode(seQSPI_MODE_QUAD, seQSPI_02CLK, seQSPI_02CLK);
          seS1D13C00Write16( QSPI_RMADRH, 0xFFF0 & (raddr >> 16));
          seS1D13C00Write16( QSPI_MMACFG2, rdval | 1);  // Enable MMA
/*
          seS1D13C00Write16( QSPI_MMACFG2, 0x15A9);
                          // MMAEN = 1;
                          // ADRCYC = 0;
                          // ADRTMOD = 2;  // quad
                          // DATTMOD = 2;  // quad
                          // DUMTMOD = 2;  // quad
                          // DUMLN = 5;    // 6 clocks
                          // DUMDL = 1;    // 2 clocks
*/			  
        }
    }

    return fStatus;
}

seStatus seQSPI_ClearMasterRxMMA(void )
{
    seStatus fStatus = seSTATUS_OK;

    seQSPI_NEGATE_MST_CS0();
    seSetBits16(QSPI_MMACFG2, QSPI_MMAEN, 0 );
    while( seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_MMABSY );
    return fStatus;
}

seStatus seQSPI_TermMasterTx( void )
{
    seStatus fStatus = seSTATUS_OK;

    while( (seS1D13C00Read16( QSPI_INTF ) & QSPI_INTF_TENDIF) == 0 );
    seS1D13C00Write16( QSPI_INTF, QSPI_INTF_TENDIF);
    seQSPI_Stop();

    return fStatus;
}

seStatus seQSPI_SetMode( seQSPI_TransferMode mode, seQSPI_Clocks chln, seQSPI_Clocks chdl )
{
    seStatus fStatus = seSTATUS_NG;

    if ( chln > 0 )
    {
        seSetBits16( QSPI_MOD, QSPI_TMOD, mode << 6 );  ///< Select Transfer Mode
        seSetBits16( QSPI_MOD, QSPI_CHLN, chln << 8 );  ///< data transfer clocks
        seSetBits16(QSPI_MOD, QSPI_CHDL, chdl << 12 );  ///< serial data lines clocks

        fStatus = seSTATUS_OK;
    }

    return fStatus;
}

void seQSPI_ASSERT_MST_CS0( void )
{
    seSetBits16( QSPI_CTL, QSPI_MSTSSO, 0 );
    // Wait...
    asm(" nop");
}

void seQSPI_NEGATE_MST_CS0( void )
{
    seSetBits16( QSPI_CTL, QSPI_MSTSSO, QSPI_MSTSSO );
  // Wait...
  asm(" nop");
}
