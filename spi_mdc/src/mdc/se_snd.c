/**
  ******************************************************************************
  * @file    se_snd.c
  * @author  Epson
  * @version V1.0
  * @date    21-October-2017
  * @brief   This file contains the functions for the SND library.
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
#include "se_dmac.h"
#include "se_snd.h"
   
void seSND_InitStruct( seSND_InitTypeDef* SND_InitStruct )
{
    SND_InitStruct->ClkSrc       = seCLG_OSC1;
    SND_InitStruct->ClkDivider   = seSND_OSC1_CLKDIV_1;
    SND_InitStruct->DriveMode    = seSND_SingleDrive;
    SND_InitStruct->DMAChannel   = seDMAC_CH_NONE;
    SND_InitStruct->EnableInt    = (seSND_InterruptSrc)0; ///< No interrupt
}


seStatus seSND_Init( seSND_InitTypeDef* SND_InitStruct )
{
    seStatus fResult = seSTATUS_NG;

    ///< Check if selected clock is enabled
    switch (SND_InitStruct->ClkSrc)
    {
      case seCLG_IOSC:
          if ( seS1D13C00Read16( SYS_CTRL ) & SYS_IOSCEN_bits ) {
              fResult = seSTATUS_OK;
          }
          break;

      case seCLG_OSC1:
          if ( seS1D13C00Read16( CLG_OSC ) & CLG_OSC1EN_bits ) {
                fResult = seSTATUS_OK;
          }
          break;

      default:
          break;
    }

    if ( fResult == seSTATUS_OK )
    {
        ///< Stop supplying operating clock
        seSND_Disable();

        ///< Configure the SND operating clock.
        seSND_ConfigureClock( SND_InitStruct->ClkSrc, SND_InitStruct->ClkDivider );

        ///< Enable count operation clock; Start supplying operating clock.
        seSND_Enable();

        // Program P10 and P11 as BZOUT and #BZOUT
        seS1D13C00Write8( PORT_P1FNCSEL, 0x00 );   // Select function 00b
        seS1D13C00Write8( PORT_P1MODSEL, 0x03 );   // Enable peripheral function


        ///< Set output pin drive mode
        seSetBits16( SND_SEL, SND_SINV_bits, SND_InitStruct->DriveMode << 2 );

        ///< Clear interrupt flag.
        seSND_ClearIntFlag();

        ///< Enable interrupt.
        seSND_EnableInt( SND_InitStruct->EnableInt );
    }

    return fResult;
}


void seSND_Enable( void )
{
    seSetBits16( SND_CTL, SND_MODEN_bits, SND_MODEN_bits );
}


void seSND_Disable( void )
{
    seSetBits16( SND_CTL, SND_MODEN_bits, 0 );
}


seStatus seSND_Stop( void )
{
    seStatus fStatus = seSTATUS_OK;

    ///< Stop buzzer output
    seSetBits16( SND_CTL, SND_SSTP_bits, SND_SSTP_bits );

    ///< Wait for output to stop
    while( seS1D13C00Read16( SND_CTL ) & SND_SSTP_bits );

    ///< Clear interrupt flag.
    seS1D13C00Write16( SND_INTF, 1 );

    return fStatus;
}


void seSND_Start( uint16_t frequency, uint16_t duty_ratio )
{
    // Clear any unwanted interrupts
    seS1D13C00Write16( SND_INTF, 0x00FF );

    if ( (seS1D13C00Read16( SND_CTL ) & SND_MODEN_bits) != 0 )
    {
        while ( (seS1D13C00Read16( SND_INTF ) & SND_SBSY_bits) & SND_SBSY_bits ) {
            asm(" nop");
        }

        ///< Set Buzzer Normal Mode
        seSetBits16( SND_SEL, SND_MODSEL_bits, 0 );
        ///< Set Buzzer Output
        seSetBits16( SND_DAT, SND_SLEN_bits, duty_ratio << 8 );
        seSetBits16( SND_DAT, SND_SFRQ_bits, frequency );
    }
}


void seSND_StartOneShot( uint16_t frequency, uint16_t duty_ratio, uint16_t duration )
{
    // Clear any unwanted interrupts
    seS1D13C00Write16( SND_INTF, 0x00FF );

    if ( (seS1D13C00Read16( SND_CTL ) & SND_MODEN_bits) == SND_MODEN_bits )
    {
        while ( (seS1D13C00Read16( SND_INTF ) & SND_SBSY_bits) == SND_SBSY_bits ) {
            asm(" nop");
        }

        ///< One-shot buzzer mode
        seSetBits16( SND_SEL, SND_MODSEL_bits, 1 );
        seSetBits16( SND_SEL, SND_STIM_bits, duration << 8 );

        ///< Start buzzer output
        seSetBits16( SND_DAT, SND_SLEN_bits, duty_ratio << 8 );
        seSetBits16( SND_DAT, SND_SFRQ_bits, frequency );
    }
}


void seSND_StartMelody( const uint16_t * data, uint32_t size, uint16_t tempo )
{
    if ( (seS1D13C00Read16( SND_CTL ) & SND_MODEN_bits) == SND_MODEN_bits )
    {
        uint32_t i;

        while ( (seS1D13C00Read16( SND_INTF ) & SND_SBSY_bits) == SND_SBSY_bits ) {
            asm(" nop");
        }

        ///< Melody buzzer mode
        seSetBits16( SND_SEL, SND_MODSEL_bits, 2 );
        seSetBits16( SND_SEL, SND_STIM_bits, tempo << 8 );

        for ( i = 0; i < size; i++ )
        {
            seS1D13C00Write16( SND_DAT, data[i] );
            while (( seS1D13C00Read16( SND_INTF ) & SND_EMIF_bits) == 0 ) {
                asm(" nop");
            }
        }
    }
}


void seSND_ConfigureClock( seSND_ClkSrc clock, uint16_t divider )
{
    if ( (seS1D13C00Read16( SND_CTL ) & SND_MODEN_bits) == 0 )
    {
        seSetBits16( SND_CLK, SND_CLKSRC_bits, clock );
        seSetBits16( SND_CLK, SND_CLKDIV_bits, divider << 4 );
    }
}


uint32_t seSND_GetClk( void )
{
    uint32_t sndclk = 0;
    uint32_t snddiv = 1;

    uint16_t clk = seS1D13C00Read16( SND_CLK );
    uint16_t clksrc = clk & SND_CLKSRC_bits;
    uint16_t clkdiv = (clk & SND_CLKDIV_bits) >> 5;

    switch ( clksrc )
    {
      case seCLG_IOSC:
        sndclk = seCLG_GetIoscFreqSel();
        snddiv = 1 << clkdiv;
        break;
      case seCLG_OSC1:
      default:
         if (seS1D13C00Read16(CLG_OSC1) & 0x0800)  // Check OSC1SELCR bit
             sndclk = seCLG_INTOSC1_FREQUENCY;
         else
             sndclk = seCLG_EXTOSC1_FREQUENCY;
         snddiv = 1;
         break;
    }

    return ( sndclk / snddiv );
}


void seSND_EnableInt( seSND_InterruptSrc irq )
{
    seOrBits16( SND_INTE, irq );
}


void seSND_DisableInt( seSND_InterruptSrc irq )
{
    seAndBits16( SND_INTE, ~irq );
}


seInterruptStatus seSND_GetIntFlag( seSND_InterruptSrc irq )
{
    seInterruptStatus fResult = seINTERRUPT_NOT_OCCURRED;
    uint16_t flagged = seS1D13C00Read16( SND_INTF );

    if ( flagged & irq )
    {
        fResult = seINTERRUPT_OCCURRED;
    }

    return fResult;
}


void seSND_ClearIntFlag( void )
{
    seS1D13C00Write16( SND_INTF, SND_EDIF_bits );
}
