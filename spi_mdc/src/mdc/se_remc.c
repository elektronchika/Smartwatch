/**
 ******************************************************************************
 * @file    se_remc.c
 * @author  Epson
 * @version V1.0
 * @date    31-October-2017
 * @brief   This file contains the functions for the REMC firmware library.
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
#include "se_port.h"
#include "se_remc.h"


seStatus seREMC_Init( seREMC_InitTypeDef* InitStruct )
{
    seStatus fResult = seSTATUS_NG;

    // Configure REMO pin
    fResult = sePORT_InitAsAltFunction( sePORT_P06, sePORT_ALT_0 );

    // Configure CLPLS pin
    fResult = sePORT_InitAsAltFunction( sePORT_P07, sePORT_ALT_0 );

    seREMC_ConfigureClock( InitStruct->ClkSrc, InitStruct->ClkDivider );

    seSetBits16( REMC_DBCTL, REMC_REMCRST_bits, REMC_REMCRST_bits );    // Reset REMC.
    seS1D13C00Write16( REMC_DBCTL, 1 );      // ----.----.---0.00-1 OINV=0(norm) BUFEN=0(disable) TRMD=0(repeat) MODEN=1
    seS1D13C00Write16( REMC_CARR, InitStruct->carr );                   // Set carrier waveform.
    seSetBits16( REMC_CCTL, REMC_CARREN_bits, REMC_CARREN_bits );       // Enable carrier modulation.
    seS1D13C00Write16( REMC_INTF, seREMC_ALL_INT );                     // DBIF=1 APIF=1  Clear interrupt flag.
    seS1D13C00Write16( REMC_INTE, REMC_DBIF_bits );                     // DBIF=1 APIF=0  Enable interrupt.
  
    return fResult;
}


void seREMC_ConfigureClock( seREMC_ClkSrc clock, uint16_t divider )
{
    seSetBits16( REMC_CLK, REMC_CLKSRC_bits, clock );
    seSetBits16( REMC_CLK, REMC_CLKDIV_bits, divider << 4);
}


void seREMC_Start( uint16_t aplen, uint16_t dblen )
{
    seS1D13C00Write16( REMC_DBCTL, 0x0001 );    // REMOINV=0(norm) BUFEN=0(disable) TRMD=0(repeat) MODEN=1
    seS1D13C00Write16( REMC_APLEN, aplen );     // Set data bit active pulse length.
    seS1D13C00Write16( REMC_DBLEN, dblen );     // Set data bit length.
    seS1D13C00Write16( REMC_DBCTL, 0x0001 );    // REMCRST=1  Reset REMC.
    seS1D13C00Write16( REMC_DBCTL, 0x0009 );    // REMOINV=0(norm) BUFEN=1(enable) TRMD=0(repeat) MODEN=1  Enable compare buffer.
    seS1D13C00Write16( REMC_DBCTL, 0x0309 );    // PRESET=1 PRUN=1 REMOINV=0(norm) BUFEN=1(enable) TRMD=0(repeat) MODEN=1  Preset and start counter.
}


void seREMC_Stop( void )
{
    seS1D13C00Write16( REMC_DBCTL, 0x0009 );    // PRUN=0 REMOINV=0(norm) BUFEN=1(enable) TRMD=0(repeat) MODEN=1  Stop counter.
    seS1D13C00Write16( REMC_DBCTL, 0x0008 );    // PRUN=0 REMOINV=0(norm) BUFEN=1(enable) TRMD=0(repeat) MODEN=0  Disable counter clock.
}
