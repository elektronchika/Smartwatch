/**
 ******************************************************************************
 * @file    se_port.c
 * @author  Epson
 * @version V1.0
 * @date    25-September-2017
 * @brief   This file contains the PORT support functions for the API library.
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
#include "se_qspi.h"


//-------------- Valid PORT pins --------------------------------------------
const uint8_t ValidPortTable[2] =
{
    0xFF,   // Ports P07 - P00
    0x03,   // Ports P11 - P10
};


//-------------- Valid PORT alternate functions -----------------------------
// Define valid alternate function settings for each Port pin
// These values are discrete bits only for the table not for the register
#define altfunc0   0x01
#define altfunc1   0x02
const uint8_t ValidAltFunctionTable[10] =
{               // Port0
  ( altfunc0 ), // P00
  ( altfunc0 ), // P01
  ( altfunc0 ), // P02
  ( altfunc0 ), // P03
  ( altfunc0 ), // P04
  ( altfunc0 ), // P05
  ( altfunc0 ), // P06
  ( altfunc0 ), // P07
                // Port1
  ( altfunc0 | altfunc1 ), // P10
  ( altfunc0 | altfunc1 ), // P11
};


seStatus sePORT_Init( sePORT_InitTypeDef* InitStruct )
{
    sePORT_ConfigureClock( InitStruct->ClkSrc, InitStruct->ClkDivider );
    return seSTATUS_OK;
}


void sePORT_ConfigureClock( sePORT_ClkSrc clock, uint16_t divider )
{
    switch (clock)
    {
    case seCLG_IOSC:
        seSetBits16( PORT_CLK, PORT_CLKSRC_bits, 0 );
        seSetBits16( PORT_CLK, PORT_CLKDIV_bits, divider << 4 );
        break;
    case seCLG_OSC1:
        seSetBits16( PORT_CLK, PORT_CLKSRC_bits, PORT_CLKSRC_bits );
        seSetBits16( PORT_CLK, PORT_CLKDIV_bits, divider << 4 );
        break;
    default:
        break;
    }
}


void sePORT_InitAsInput( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1 << portNo))
    {
        /// Disable pull-up or pull-down resistor.
        seAndBits16( PORT_P0RCTL + portOffset, ~(0x0001 << portNo) );
        // Enable GPIO function.
        seAndBits16( PORT_P0MODSEL + portOffset, ~(0x0001 << portNo) );
        /// Enable input port.
        seOrBits16( PORT_P0IOEN + portOffset, (0x0100 << portNo) );
        /// Disable output port.
        seAndBits16( PORT_P0IOEN + portOffset, ~(0x0001<<portNo) );
    }
}


void sePORT_EnablePullUpResistor( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1 << portNo))
    {
        seOrBits16( PORT_P0RCTL + portOffset, (0x0101 << portNo) );
    }
}


void sePORT_EnablePullDownResistor( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1<<portNo))
    {
        /// Select pull-down resistor.
        seAndBits16( PORT_P0RCTL + portOffset, ~(0x0100 << portNo));
        /// Enable pull-down resistor.
        seOrBits16( PORT_P0RCTL + portOffset, (0x0001 << portNo));
    }
}


void sePORT_DisableBuiltInResistor( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1<<portNo))
    {
        /// Disable pull-up or pull-down resistor.
        seAndBits16( PORT_P0RCTL + portOffset, ~(0x0001 << portNo));
    }
}


void sePORT_InitAsOutput( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1 << portNo))
    {
        // Enable GPIO function.
        seAndBits16( PORT_P0MODSEL + portOffset, ~(0x0001 << portNo));

        /// Disable input port.
        seAndBits16( PORT_P0IOEN + portOffset, ~(0x0100 << portNo));

        /// Enable output port.
        seOrBits16( PORT_P0IOEN + portOffset, (0x0001 << portNo));
    }
}


void sePORT_EnableInt( sePORT_Id portId, sePORT_Edge edge )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1<<portNo))
    {
        /// Set interrupt edge.
        if ( edge == sePORT_INT_EDGE_FALLING ) {
            seOrBits16( PORT_P0INTCTL + portOffset, (0x0100 << portNo)); // Falling edge.
        } else {
            seAndBits16( PORT_P0INTCTL + portOffset, ~(0x0100<<portNo)); // Rising edge.
        }
        // Clear interrupt flag.
        seS1D13C00Write16( PORT_P0INTF + portOffset, (0x0001 << portNo) );

        // Enable interrupt.
        seOrBits16( PORT_P0INTCTL + portOffset, (0x0001 << portNo));
    }
}


void sePORT_DisableInt( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1<<portNo))
    {
        seAndBits16( PORT_P0INTCTL + portOffset, ~(0x0001 << portNo));
    }
}


seInterruptStatus sePORT_GetIntFlag( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    seInterruptStatus result = seINTERRUPT_NOT_OCCURRED;

    if ((ValidPortTable[groupNo]) & (1<<portNo))
    {
        volatile uint16_t flag;
        flag = seS1D13C00Read16( PORT_P0INTF + portOffset );
        if ( flag & ( 0x0001 << portNo ) ) {
            result = seINTERRUPT_OCCURRED;
        }
    }

    return result;
}


void sePORT_ClearIntFlag( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1<<portNo)) {
        seS1D13C00Write16( PORT_P0INTF + portOffset, (0x0001 << portNo));
    }
}


sePORT_Data sePORT_GetOutput( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    sePORT_Data result = sePORT_DATA_LOW;

    if ((ValidPortTable[groupNo]) & (1<<portNo))
    {
        volatile uint16_t data;
        data = seS1D13C00Read16( PORT_P0DAT + portOffset );

        // Get output data.
        data = ( data & ( 0x0100 << portNo ) );

        if ( data != 0 ) {
          result = sePORT_DATA_HIGH;
        }
    }

  return result;
}


void sePORT_SetOutput( sePORT_Id portId, sePORT_Data data )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1 << portNo))
    {
        if ( data == sePORT_DATA_HIGH ) {
            seOrBits16( PORT_P0DAT + portOffset, (0x0100 << portNo));
        } else {
          seAndBits16( PORT_P0DAT + portOffset, ~(0x0100<<portNo));
        }
    }
}


sePORT_Data sePORT_GetInput( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;
    sePORT_Data result = sePORT_DATA_LOW;

    if ((ValidPortTable[groupNo]) & (1<<portNo))
    {
        volatile uint16_t data;
        data = seS1D13C00Read16( PORT_P0DAT + portOffset );

        data = ( data & ( 0x0001 << portNo ) );

        if ( data != 0 ) {
            result = sePORT_DATA_HIGH;
        }
    }

    return result;
}


void sePORT_EnableChatteringFilter( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1 << portNo))
    {
        seOrBits16( PORT_P0CHATEN + portOffset, (0x0001 << portNo));
    }
}


void sePORT_DisableChatteringFilter( sePORT_Id portId )
{
    uint8_t groupNo = (portId>>3);
    uint8_t portNo = (portId%8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1<<portNo)) {
        seAndBits16( PORT_P0CHATEN + portOffset, ~(0x0001<<portNo));
    }
}


seState sePORT_GetChatteringFilter( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo  = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;
    seState ChatteringFilter = seDISABLE;

    if ((ValidPortTable[groupNo]) & (1<<portNo))
    {
        if ( seS1D13C00Read16(PORT_P0CHATEN + portOffset) & (0x0001 << portNo) ) {
          ChatteringFilter = seENABLE;
        }
    }

    return ChatteringFilter;
}


void sePORT_InitAsHiZ( sePORT_Id portId )
{
    uint8_t groupNo = (portId >> 3);
    uint8_t portNo = (portId % 8);
    uint16_t portOffset = groupNo * 0x10;

    if ((ValidPortTable[groupNo]) & (1<<portNo))
    {
        // Disable both the Input and Output,
        // Input is in the high 16bits and output in the low 16bits
        seAndBits16( PORT_P0IOEN + portOffset, ~(0x0101<<portNo));

        // Clear the INTCTL bit to disable the Interrupt
        seS1D13C00Write16( PORT_P0INTF + portOffset, (0x0001 << portNo));

        // Clear the MODSEL bit to switch function to GPIO
        seAndBits16( PORT_P0MODSEL + portOffset, ~(0x0001 << portNo));
    }
}


seStatus sePORT_InitAsAltFunction( sePORT_Id portId, sePORT_AltFunc funcNo )
{
    seStatus fResult = seSTATUS_NG;
    uint8_t groupNo = (portId>>3);
    uint8_t portNo = (portId % 8);
    uint16_t tmp;
    uint16_t index = (groupNo << 3) + portNo; // goupNo * 8 plus portNo gets index
    uint16_t portOffset = groupNo * 0x10;
    uint8_t validAlt;
    uint8_t requestedAlt = (1<<funcNo);

    validAlt = ValidAltFunctionTable[index];

    // If the requested Alternate Function is valid, set it up
    if (requestedAlt & validAlt)
    {
        fResult = seSTATUS_OK;

        // Disable both the Input and Output
        // Input is in the high 16bits and output in the low 16bits
        seAndBits16( PORT_P0IOEN + portOffset, ~(0x0101 << portNo) );

        // Clear the MODSEL bit to switch function to GPIO temporarily
        seAndBits16( PORT_P0MODSEL + portOffset, ~(0x0001 << portNo) );

        // Setup the requested Alternate Function
        // FNCSEL has 2 bits per port
        tmp = seS1D13C00Read16( PORT_P0FNCSEL + portOffset );
        tmp &= ~(0x0003 << (portNo * 2));
        tmp |= ((uint16_t)(funcNo) << (portNo * 2));
        seS1D13C00Write16( PORT_P0FNCSEL + portOffset, tmp );

        // Switch to Alternate Function mode for that pin
        seOrBits16( PORT_P0MODSEL + portOffset, (0x0001 << portNo) );
    }

    return fResult;
}
