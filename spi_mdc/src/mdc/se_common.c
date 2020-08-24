/**
  ******************************************************************************
  * @file    se_common.c
  * @author  Epson
  * @version V1.0
  * @date    18-September-2017
  * @brief   This file contains the common functions for the S1D13C00
  *          peripheral library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#include <stdio.h>

#include "s1d13c00_hcl.h"
#include "se_common.h"
#include "s1d13c00_memregs.h"

void seProtectSys( seWriteProtect protect )
{
    seS1D13C00Write16( SYS_SYSPROT, protect );
}

void seAndBits16( uint32_t addr, uint16_t bits )
{
    uint16_t reg;

    reg = seS1D13C00Read16( addr );
    reg &= bits;
    seS1D13C00Write16( addr, reg );
}

void seOrBits16( uint32_t addr, uint16_t bits )
{
    uint16_t reg;

    reg = seS1D13C00Read16( addr );
    reg |= bits;
    seS1D13C00Write16( addr, reg );
}


void seSetBits8( uint32_t addr, uint8_t bits, uint8_t value )
{
    uint8_t reg;

    /// Read the register, mask out the bits of interest in the register
    /// and also in the new value, then mask in the new value bits.
    reg = seS1D13C00Read8( addr );
    reg   &= ~bits;
    value &= bits;
    reg   |= value;
    seS1D13C00Write8( addr, reg );
}


void seSetBits16( uint32_t addr, uint16_t bits, uint16_t value )
{
    uint16_t reg;

    /// Read the register, mask out the bits of interest in the register
    /// and also in the new value, then mask in the new value bits.
    reg = seS1D13C00Read16( addr );
    reg   &= ~bits;
    value &= bits;
    reg   |= value;
    seS1D13C00Write16( addr, reg );
}


void seSetBits32( uint32_t addr, uint32_t bits, uint32_t value )
{
    uint32_t reg;

    /// Read the register, mask out the bits of interest in the register
    /// and also in the new value, then mask in the new value bits.
    reg = seS1D13C00Read32( addr );
    reg   &= ~bits;
    value &= bits;
    reg   |= value;
    seS1D13C00Write32( addr, reg );
}


int32_t seClamp32( int32_t value, int32_t low, int32_t high )
{
    int32_t tmp = value;

    if ( tmp < low ) {
        tmp = low;
    }
    else if ( tmp > high ) {
        tmp = high;
    }
    else {
    }

    return tmp;
}


int16_t seClamp16( int16_t value, int16_t low, int16_t high )
{
    int16_t tmp = value;

    if ( tmp < low ) {
        tmp = low;
    }
    else if ( tmp > high ) {
        tmp = high;
    }
    else {
    }

    return tmp;
}


void seAssert( uint8_t* file, uint32_t line )
{
    printf( "Assertion failed - %s, line %d\r\n", file, (int)line );
}


//*****************************************************************************
//
// Convert 8-bit value to hex string
//
//*****************************************************************************
char *uint8tohexstr(uint8_t value, char *outstr)
{
   uint8_t digitval;
   char *retstr;

   retstr = outstr;

   digitval = (value>>4) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = value & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   *outstr = 0;

   return retstr;
}

//*****************************************************************************
//
// Convert 16-bit value to hex string
//
//*****************************************************************************
char *uint16tohexstr(uint16_t value, char *outstr)
{
   uint16_t digitval;
   char *retstr;

   retstr = outstr;

   digitval = (value>>12) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = (value>>8) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = (value>>4) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = value & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   *outstr = 0;

   return retstr;
}

//*****************************************************************************
//
// Convert 32-bit value to hex string
//
//*****************************************************************************
char *uint32tohexstr(uint32_t value, char *outstr)
{
   uint32_t digitval;
   char *retstr;

   retstr = outstr;

   digitval = (value>>28) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = (value>>24) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = (value>>20) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = (value>>16) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = (value>>12) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = (value>>8) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = (value>>4) & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   digitval = value & 0x0F;
   if (digitval > 9)
      *(outstr++) = 'A' + (digitval - 10);
    else
      *(outstr++) = '0' + digitval;

   *outstr = 0;

   return retstr;
}

