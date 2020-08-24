/**
  ******************************************************************************
  * @file    se_clg.c
  * @author  Epson
  * @version V1.0
  * @date    19-September-2017
  * @brief   This file contains the functions for the CLG library.
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


/**
  * Static functions declarations
**/
static void StartOsc1( void );
static void StopOsc1( void );
static void EnableOsc1StopDetection( void );
static void DisableOsc1StopDetection( void );

static void StartIosc( void );
static void StopIosc( void );


seStatus seCLG_Init( seCLG_InitTypeDef* InitStruct )
{
    seStatus fResult = seSTATUS_OK;
    return fResult;
}


uint32_t seCLG_GetIoscFreqSel( void )
{
    uint32_t freq = seCLG_IOSC_FREQUENCY_8;
    uint16_t asynIOSCFQ = (seS1D13C00Read16(SYS_CTRL) & SYS_IOSCFQ_bits ) >> 1;

    switch ( asynIOSCFQ )
    {
        case seCLG_IOSC_IOSCFQ_8:
            freq = seCLG_IOSC_FREQUENCY_8;
            break;

        case seCLG_IOSC_IOSCFQ_12:
            freq = seCLG_IOSC_FREQUENCY_12;
            break;

        case seCLG_IOSC_IOSCFQ_16:
            freq = seCLG_IOSC_FREQUENCY_16;
            break;

        case seCLG_IOSC_IOSCFQ_20:
            freq = seCLG_IOSC_FREQUENCY_20;
            break;

        default:
            break;
  }

  return freq;
}


void seCLG_SetIoscFreqSel( seCLG_IOSC_IoscFq freq )
{
    seSetBits16( SYS_CTRL, SYS_IOSCFQ_bits, freq << 1 );
}


seStatus seCLG_Start( seCLG_ClkSrc clock )
{
    seStatus fResult = seSTATUS_NG;

    switch ( clock )
    {
    case seCLG_IOSC:
        StartIosc();
        fResult = seSTATUS_OK;
        break;

    case seCLG_OSC1:
        StartOsc1();
        fResult = seSTATUS_OK;
        break;

    default:
        break;
    }

    return fResult;
}


seStatus seCLG_Stop( seCLG_ClkSrc clock )
{
    seStatus fResult = seSTATUS_NG;

    switch ( clock )
    {
    case seCLG_IOSC:
        StopIosc();
        fResult = seSTATUS_OK;
        break;

    case seCLG_OSC1:
        StopOsc1();
        fResult = seSTATUS_OK;
        break;

    default:
        break;
    }

    return fResult;
}


seStatus seCLG_SetStopDetection( seCLG_ClkSrc clock, seState StopDetectionEn )
{
    seStatus fResult = seSTATUS_NG;

    switch ( clock )
    {
    case seCLG_OSC1:
        if ( StopDetectionEn ) {
            EnableOsc1StopDetection();
        }
        else {
            DisableOsc1StopDetection();
        }
        fResult = seSTATUS_OK;
        break;

    case seCLG_IOSC:
    default:
        break;
    }

    return fResult;
}


void seCLG_SetTrimAdjust( seCLG_ClkSrc clock, uint16_t trim )
{
    switch( clock )
    {
    case seCLG_IOSC:
        seSetBits16( SYS_CTRL, SYS_IOSCAJ_bits, trim << 8 );
        break;

    case seCLG_OSC1:
        seSetBits16( CLG_OSC1TRM, CLG_OSC1SAJ_bits, trim );
        break;
    }

}


void seCLG_EnableInt( seCLG_Interrupt irq )
{
    seOrBits16( CLG_INTE, irq );
}


void seCLG_DisableInt( seCLG_Interrupt irq )
{
//    CLG->INTE &= ~irq;
    seAndBits16( CLG_INTE, ~irq );

}


seInterruptStatus seCLG_GetIntFlag( seCLG_IntFlag flag )
{
    seInterruptStatus fResult = seINTERRUPT_NOT_OCCURRED;
    uint16_t flagged = seS1D13C00Read16( CLG_INTF );

    if ( flag )
    {
        flagged &= flag;
        if ( flagged == flag ) {
            fResult = seINTERRUPT_OCCURRED;
        }
    }

    return fResult;
}


void seCLG_ClearIntFlag( seCLG_IntFlag flag )
{
    seS1D13C00Write16( CLG_INTF, flag );
}


/**
  * Static functions
**/


/**
  * @brief  Start Osc1.
  */
static void StartOsc1( void )
{
    uint16_t reg;

    seProtectSys( seWRITE_PROTECT_OFF );
    seS1D13C00Write16( CLG_OSC1, 0x5092 );
    seProtectSys( seWRITE_PROTECT_ON );
    seS1D13C00Write8( CLG_INTF, 0xFF );  // Clear interrupts
    seS1D13C00Write8( CLG_OSC, 0x02 );   // Enable OSC1
    do {
        reg = seS1D13C00Read16( CLG_INTF );
    } while ( (reg & CLG_OSC1STAIE_bits ) == 0x00 );
}


/**
  * @brief  Stop Osc1.
  */
static void StopOsc1( void )
{
    ///< Stop Osc1.
    seSetBits16( CLG_OSC, CLG_OSC1EN_bits, 0 );

    ///< Disable interrupt.
    seSetBits16( CLG_OSC, CLG_OSC1STAIE_bits, 0 );

    ///< Clear interrupt flag.
    seCLG_ClearIntFlag( (seCLG_IntFlag) (seCLG_OSC1STAIF | seCLG_OSC1STPIF ));
}


/**
  * @brief  Enable oscillation stop detection function in Osc1.
  */
static void EnableOsc1StopDetection( void )
{
    ///< Disable write-protect.
    seProtectSys( seWRITE_PROTECT_OFF );

    seSetBits16( CLG_OSC1, CLG_OSDRB_bits, CLG_OSDRB_bits );    ///< Enable Osc1 restart function.

    seSetBits16( CLG_OSC1, CLG_OSDEN_bits, CLG_OSDEN_bits );    ///< Enable oscillation stop detection function.

    ///< Clear interrupt flag.
    seCLG_ClearIntFlag( seCLG_OSC1STPIF );

    ///< Enable write-protect.
    seProtectSys( seWRITE_PROTECT_ON );
}


/**
  * @brief  Enable oscillation stop detection function in Osc1.
  */
static void DisableOsc1StopDetection( void )
{
    ///< Disable write-protect.
    seProtectSys( seWRITE_PROTECT_OFF );

    seSetBits16( CLG_OSC1, CLG_OSDRB_bits, 0 ); /// Disable Osc1 restart function.
    seSetBits16( CLG_OSC1, CLG_OSDEN_bits, 0 ); /// Disable oscillation stop detection function.

    ///< Disable interrupt.
    seSetBits16( CLG_INTE, CLG_OSC1STPIE_bits, 0 );

    ///< Clear interrupt flag.
    seCLG_ClearIntFlag( seCLG_OSC1STPIF );

    ///< Enable write-protect.
    seProtectSys( seWRITE_PROTECT_ON );
}


/**
  * @brief  Start Iosc.
  */
static void StartIosc( void )
{
    uint16_t reg;
    uint16_t iosc_fq = seCLG_IOSC_IOSCFQ_20;
    uint16_t iosc_aj = 39;

    reg = seS1D13C00Read16( SYS_CTRL );

    seS1D13C00Write16( SYS_CTRL, (0x0011 | (iosc_fq << 1) | (iosc_aj << 8)));

    // Wait for IOSC to stabilize
    do {
        reg = seS1D13C00Read16( SYS_CTRL );
    } while( (reg & SYS_IOSCSTA_bits) != SYS_IOSCSTA_bits );
}


/**
  * @brief  Stop Iosc.
  */
static void StopIosc( void )
{
    ///< Stop Iosc.
    seSetBits16( SYS_CTRL, SYS_IOSCEN_bits, 0 );
}
