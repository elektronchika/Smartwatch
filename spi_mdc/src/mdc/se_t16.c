/**
  ******************************************************************************
  * @file    se_t16.c
  * @author  Epson
  * @version V1.0
  * @date    26-September-2017
  * @brief   This file contains the functions for the T16 library.
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

   
void seT16_InitStruct( seT16_InitTypeDef* T16_InitStruct )
{
    T16_InitStruct->ClkSrc          = seCLG_IOSC;
    T16_InitStruct->ClkDivider      = seT16_IOSC_CLKDIV_1;
    T16_InitStruct->CounterMode     = seT16_RepeatMode;
    T16_InitStruct->Period          = 0xE650;
}


seStatus seT16_Init( uint32_t T16BaseAddr, seT16_InitTypeDef* T16_InitStruct )
{
    seStatus fResult = seSTATUS_NG;

    ///< Check if selected clock is enabled
    switch( T16_InitStruct->ClkSrc )
    {
      case seCLG_IOSC:
        if ( (seS1D13C00Read16( SYS_CTRL ) & SYS_IOSCEN_bits) == SYS_IOSCEN_bits ) {
            fResult = seSTATUS_OK;
        }
        break;

      case seCLG_OSC1:
        if ( (seS1D13C00Read16( CLG_OSC ) & CLG_OSC1EN_bits) == CLG_OSC1EN_bits ) {
            fResult = seSTATUS_OK;
        }
        break;

      default:
        break;
    }

    if ( fResult == seSTATUS_OK )
    {
        ///< Stop T16 Ch.n Timer.
        seT16_Stop( T16BaseAddr );

        ///< Stop supplying operating clock
        seT16_Disable( T16BaseAddr );

        ///< Configure the T16 Ch.n operating clock.
        seT16_ConfigureClock( T16BaseAddr, T16_InitStruct->ClkSrc, T16_InitStruct->ClkDivider );

        ///< Enable count operation clock; Start supplying operating clock.
        seT16_Enable( T16BaseAddr );

        ///< Select operation mode; Repeat mode or One-shot mode.
        seT16_ConfigureCounterMode( T16BaseAddr, T16_InitStruct->CounterMode );

        seT16_DisableInt( T16BaseAddr );
        seT16_ClearIntFlag( T16BaseAddr );

        ///< Preset reload data to counter.
        seT16_SetCounter( T16BaseAddr, T16_InitStruct->Period );

        seT16_Enable( T16BaseAddr );
  }

  return fResult;
}


void seT16_Stop( uint32_t T16BaseAddr )
{
    ///< Stop Timer
    seSetBits16( T16BaseAddr + T16_CTL_OFFSET, T16_PRUN_bits, 0 );
    ///< Clear interrupt flag.
    seSetBits16( T16BaseAddr + T16_INTF_OFFSET, T16_UFIF_bits, T16_UFIF_bits );
}


void seT16_Start( uint32_t T16BaseAddr )
{
    ///< Clear interrupt flag.
    seS1D13C00Write16( T16BaseAddr + T16_INTF_OFFSET, T16_UFIF_bits );

    ///< Enable Timer
    if ( (seS1D13C00Read16( T16BaseAddr + T16_CTL_OFFSET) & T16_MODEN_bits) == T16_MODEN_bits ) {
        seSetBits16( T16BaseAddr + T16_CTL_OFFSET, T16_PRUN_bits, T16_PRUN_bits );
    }
}


void seT16_Enable( uint32_t T16BaseAddr )
{
    seSetBits16( T16BaseAddr + T16_CTL_OFFSET, T16_MODEN_bits, T16_MODEN_bits );
}


void seT16_Disable( uint32_t T16BaseAddr )
{
    seSetBits16( T16BaseAddr + T16_CTL_OFFSET, T16_MODEN_bits, 0 );
}


void seT16_ConfigureClock( uint32_t T16BaseAddr, seT16_ClkSrc clock, uint16_t divider )
{
    seSetBits16( T16BaseAddr + T16_CLK_OFFSET, T16_CLKSRC_bits, clock );
    seSetBits16( T16BaseAddr + T16_CLK_OFFSET, T16_CLKDIV_bits, divider << 4 );
}


uint32_t seT16_GetClk( uint32_t T16BaseAddr )
{
    uint32_t t16clk = 0;
    uint32_t t16div = 1;

    uint32_t clksrc = (seS1D13C00Read16( T16BaseAddr + T16_CLK_OFFSET ) && T16_CLKSRC_bits);
    uint32_t clkdiv = (seS1D13C00Read16( T16BaseAddr + T16_CLK_OFFSET ) && T16_CLKDIV_bits) >> 4;

    switch( clksrc )
    {
      case seCLG_IOSC:
        t16div = 1 << clkdiv;
        break;
      case seCLG_OSC1:
        if (clkdiv <= seT16_OSC1_CLKDIV_256)
        {
          t16div = 1 << clkdiv;
        }
        break;
      default:
        break;
    }

    switch( clksrc )
    {
      case seCLG_OSC1:
         if (seS1D13C00Read16(CLG_OSC1) & 0x0800)  // Check OSC1SELCR bit
             t16clk = seCLG_INTOSC1_FREQUENCY;
         else
             t16clk = seCLG_EXTOSC1_FREQUENCY;
        break;
      case seCLG_IOSC:
        t16clk = seCLG_GetIoscFreqSel();
        break;
      default:
        break;
  }

  return ( t16clk / t16div );
}


void seT16_ConfigureCounterMode( uint32_t T16BaseAddr, seT16_CounterMode mode )
{
    seSetBits16( T16BaseAddr + T16_MOD_OFFSET, T16_TRMD_bits, mode << 2 );
}


seStatus seT16_SetCounter( uint32_t T16BaseAddr, uint16_t counter )
{
    seStatus fResult = seSTATUS_OK;

    ///< Set the value for the counter to be set to
    seS1D13C00Write16( T16BaseAddr + T16_TR_OFFSET, counter );

    ///< Check if the timer is already enabled
    if ( (seS1D13C00Read16( T16BaseAddr + T16_CTL_OFFSET ) & T16_MODEN_bits) == 1) {
        ///< Presets the reload data stored in the T16_0TR register to the counter.
        seSetBits16( T16BaseAddr + T16_CTL_OFFSET, T16_PRESET_bits, T16_PRESET_bits );
    }

    // Wait for the preset operation to complete
    while( seS1D13C00Read16( T16BaseAddr + T16_CTL_OFFSET) & T16_PRESET_bits );

    return fResult;
}


uint16_t seT16_GetCounter( uint32_t T16BaseAddr )
{
    return ( seS1D13C00Read16( T16BaseAddr + T16_TC_OFFSET ));
}


void seT16_EnableInt( uint32_t T16BaseAddr )
{
    seSetBits16( T16BaseAddr + T16_INTE_OFFSET, T16_UFIE_bits, T16_UFIE_bits );
}


void seT16_DisableInt( uint32_t T16BaseAddr )
{
    seSetBits16( T16BaseAddr + T16_INTE_OFFSET, T16_UFIE_bits, 0 );
}


seInterruptStatus seT16_GetIntFlag( uint32_t T16BaseAddr )
{
    return ( ((seS1D13C00Read16( T16BaseAddr + T16_INTF_OFFSET) & T16_UFIF_bits) == 1) ? seINTERRUPT_OCCURRED : seINTERRUPT_NOT_OCCURRED );
}


void seT16_ClearIntFlag( uint32_t T16BaseAddr )
{
    seSetBits16( T16BaseAddr + T16_INTF_OFFSET, T16_UFIF_bits, T16_UFIF_bits );
}
