/**
 ******************************************************************************
 * @file    se_rtc.c
 * @author  Epson
 * @version V1.0
 * @date    05-October-2017
 * @brief   This file contains the functions for the RTC library.
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
#include "se_rtc.h"
   
static seStatus unprotect( void );              ///< RTC private protection mechanism
static void protect( void );                    ///< RTC private protection mechanism


seStatus seRTC_Init( seRTC_InitTypeDef* InitStruct )
{
    seStatus fResult = seSTATUS_NG;

    ///< Check that OSC1 is enabled
    /// RELE - we should just enable it here
    if ( (seS1D13C00Read16( CLG_OSC ) & CLG_OSC1EN_bits) == CLG_OSC1EN_bits )
    {
        fResult = seSTATUS_OK;
    }

    if ( fResult == seSTATUS_OK )
    {
        seRTC_Stop();

        ///< Select operation mode;
        seRTC_Set12_24Mode( InitStruct->H12_24Format );

        ///< Select initial Date time as "01/01/00 Sunday 00:00:00"
        seS1D13C00Write16( RTC_YAR, 0 );

        seSetBits16( RTC_MON, RTC_MOH_bits, 0 << 12 );
        seSetBits16( RTC_MON, RTC_MOL_bits, 1 <<  8 );
        seSetBits16( RTC_MON, RTC_DH_bits, 0  <<  4 );
        seSetBits16( RTC_MON, RTC_DL_bits, 1  <<  0 );

        seSetBits16( RTC_HUR, RTC_HH_bits, 0 << 12 );
        seSetBits16( RTC_HUR, RTC_HL_bits, 0 <<  8 );
        seSetBits16( RTC_HUR, RTC_MIH_bits, 0 << 4 );
        seSetBits16( RTC_HUR, RTC_MIL_bits, 0 << 0 );

        seSetBits16( RTC_SEC, RTC_SH_bits, 0 << 12 );
        seSetBits16( RTC_SEC, RTC_SL_bits, 0 <<  8 );

        ///< Initialize interrupts.
        seRTC_DisableInt( seRTC_ALL_INT );
        seRTC_ClearIntFlag( seRTC_ALL_INT );
    }

    return fResult;
}


seStatus seRTC_Start( void )
{
    seStatus fStatus = seSTATUS_OK;

    ///< Execute software reset.
    seSetBits8( RTC_CTLL, RTC_RST_bits, RTC_RST_bits );
    while( seS1D13C00Read8(RTC_CTLL) & RTC_RST_bits );
    seSetBits8( RTC_CTLL, RTC_RUN_bits, 1 );

    return fStatus;
}


void seRTC_Stop( void )
{
    seSetBits8( RTC_CTLL, RTC_RUN_bits, 0 );
}


void seRTC_EnableInt( seRTC_Interrupt irq )
{
    seOrBits16( RTC_INTE, irq );
}


void seRTC_DisableInt( seRTC_Interrupt irq )
{
    seAndBits16( RTC_INTE, ~irq );
}


seInterruptStatus seRTC_GetIntFlag( seRTC_Interrupt irq )
{
    seInterruptStatus fResult = seINTERRUPT_NOT_OCCURRED;
    uint16_t flagged = seS1D13C00Read16( RTC_INTF );

    if ( irq )
    {
        flagged &= irq;
        if ( flagged == irq ) {
            fResult = seINTERRUPT_OCCURRED;
        }
    }

    return fResult;
}


void seRTC_ClearIntFlag( seRTC_Interrupt irq )
{
    seS1D13C00Write16( RTC_INTF, irq );
}


void seRTC_Enable1SecTimer( void )
{
    seRTC_ClearIntFlag( seRTC_1SECI );
    seSetBits16( RTC_INTE, RTC_T1SECIE_bits, RTC_T1SECIE_bits );
}


void seRTC_Disable1SecTimer( void )
{
    seSetBits16( RTC_INTE, RTC_T1SECIE_bits, 0 );
    seRTC_ClearIntFlag( seRTC_1SECI );
}


uint8_t seRTC_CalculateTrm( int32_t m, int32_t n )
{
    int32_t tmp = seClamp32( 2L * m * n / 78125L, -64, 63 );
    return (tmp & 0x7F);
}


seStatus seRTC_SetYearMonthDayWeek( uint8_t year, uint8_t month, uint8_t day, uint8_t week )
{
    seStatus fStatus = seSTATUS_NG;

    if ( !( year > 99 || month > 12 || month == 0 || day > 31 ) )
    {
        fStatus = unprotect();
        seSetBits16( RTC_YAR, RTC_YH_bits, (year / 10) << 4);
        seSetBits16( RTC_YAR, RTC_YL_bits, (year % 10) << 0 );
        seSetBits16( RTC_YAR, RTC_WK_bits, week << 8 );

        seSetBits16( RTC_MON, RTC_MOH_bits, (month / 10) << 12 );
        seSetBits16( RTC_MON, RTC_MOL_bits, (month % 10) <<  8 );
        seSetBits16( RTC_MON, RTC_DH_bits, (day / 10) << 4 );
        seSetBits16( RTC_MON, RTC_DL_bits, (day % 10) << 0 );

        protect();
    }

    return fStatus;
}


seStatus seRTC_GetYearMonthDayWeek( uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* week )
{
    seStatus fStatus = unprotect();
    uint16_t tmp, reg;

    ///< Get year.
    reg = seS1D13C00Read16( RTC_YAR );
    tmp = (((reg & RTC_YH_bits) >> 4) * 10) + (reg & RTC_YL_bits);
    *year = ( tmp <= 99 ) ? tmp : 0;

    ///< Get month
    reg = seS1D13C00Read16( RTC_MON );
    tmp = (((reg & RTC_MOH_bits) >> 12) * 10) + ((reg & RTC_MOL_bits) >> 8);
    *month= ( tmp <= 12 && tmp > 0 ) ? tmp : 1;

    ///< Get day
    reg = seS1D13C00Read16( RTC_MON );
    tmp = (((reg & RTC_DH_bits) >> 4) * 10) + (reg & RTC_DL_bits);
    *day = ( tmp > 0 &&  tmp <= 31 ) ? tmp :1 ;

    ///< Get of the week
    reg = seS1D13C00Read16( RTC_YAR );
    tmp = ((reg & RTC_WK_bits) >> 8);
    *week = tmp;

    protect();
    return fStatus;
}


seRTC_Hours12_24 seRTC_Get12_24Mode( void )
{
    unprotect();
    uint16_t value = seS1D13C00Read8( RTC_CTLL );
    protect();

    return (seRTC_Hours12_24)((value & RTC_24H_bits) >> 4);
}


seStatus seRTC_Set12_24Mode( seRTC_Hours12_24 mode )
{
    seStatus fStatus = unprotect();
    seSetBits8(RTC_CTLL, RTC_24H_bits, mode << 4 );
    protect();

    return fStatus;
}


seStatus seRTC_SetHourMinuteSecond( uint8_t h, uint8_t m, uint8_t s )
{
    seStatus fStatus = unprotect();
    uint8_t value;

    if ( fStatus == seSTATUS_OK )
    {
        value = seS1D13C00Read8( RTC_CTLL );
        if ( (value & RTC_24H_bits) && (!seRTC_VALID_24HOURS(h)) ) {
            fStatus = seSTATUS_NG;
        }

        if ( ((value & RTC_24H_bits) == 0) && (!seRTC_VALID_24HOURS(h)) ) {
            fStatus = seSTATUS_NG;
        }

        if ( !seRTC_VALID_MINUTES(m) ) {
            fStatus = seSTATUS_NG;
        }

        if ( !seRTC_VALID_MINUTES(s) ) {
            fStatus = seSTATUS_NG;
        }

        seSetBits16( RTC_HUR, RTC_HH_bits, (h / 10) << 12 );    ///< Set hour(tenths digit).
        seSetBits16( RTC_HUR, RTC_HL_bits, (h % 10) << 8 );     ///< Set hour(unit digit).
        seSetBits16( RTC_HUR, RTC_MIH_bits, (m / 10) << 4 );    ///< Set minute(tenths digit).
        seSetBits16( RTC_HUR, RTC_MIL_bits, (m % 10) );         ///< Set minute(unit digit).

        seSetBits16( RTC_SEC, RTC_SH_bits, (s / 10) << 12 );    ///< Set second(tenths digit).
        seSetBits16( RTC_SEC, RTC_SL_bits, (s % 10) << 8 );     ///< Set second(unit digit).
    }

    protect();
    return fStatus;
}


seStatus seRTC_GetHourMinuteSecond( uint8_t* h, uint8_t* m, uint8_t* s )
{
    seStatus fStatus = unprotect();
    uint16_t reg;

    reg = seS1D13C00Read16( RTC_HUR );
    *h = (((reg & RTC_HH_bits) >> 12) * 10) + ((reg & RTC_HL_bits) >> 8);

    *m = (((reg & RTC_MIH_bits) >> 4) * 10) + (reg & RTC_MIL_bits);

    reg = seS1D13C00Read16( RTC_SEC );
    *s = (((reg & RTC_SH_bits) >> 12) * 10) + ((reg & RTC_SL_bits) >> 8);

    protect();
    return fStatus;
}


seStatus seRTC_SetAM_PM( seRTC_AM_PM indicator )
{
    seStatus fStatus = unprotect();

    seSetBits16( RTC_HUR, RTC_AP_bits, indicator << 14 );

    protect();
    return fStatus;
}


seRTC_AM_PM seRTC_GetAM_PM( void )
{
  unprotect();
  uint16_t value = (seS1D13C00Read16( RTC_HUR ) & RTC_AP_bits) >> 14;
  protect();

  return (seRTC_AM_PM)value;
}


seStatus seRTC_SetAlarm( uint8_t h, uint8_t m, uint8_t s, seRTC_AM_PM am ) {

    seStatus fStatus = seSTATUS_NG;

    if ( seRTC_VALID_MINUTES(m) && seRTC_VALID_SECONDS(s) && seRTC_VALID_24HOURS(h) )
    {
        uint16_t mode = seRTC_Get12_24Mode();

        if ( ( mode == seRTC_CTL_RTC24H_12 && h <= 12 ) || ( mode == seRTC_CTL_RTC24H_24 && h <= 24 ))
        {
            seRTC_DisableInt( seRTC_ALARMI );

            seSetBits16( RTC_ALM1, RTC_SHA_bits, (s / 10) << 12 );
            seSetBits16( RTC_ALM1, RTC_SLA_bits, (s % 10) <<  8 );

            seSetBits16( RTC_ALM2, RTC_MIHA_bits, (m / 10) << 4 );
            seSetBits16( RTC_ALM2, RTC_MILA_bits, (m % 10) );

            seSetBits16( RTC_ALM2, RTC_PA_bits, am <<  14 );
            seSetBits16( RTC_ALM2, RTC_HHA_bits, (h / 10) << 12 );
            seSetBits16( RTC_ALM2, RTC_HLA_bits, (h % 10) <<  8 );

            seRTC_ClearIntFlag( seRTC_ALARMI );
            seRTC_EnableInt( seRTC_ALARMI );
            fStatus = seSTATUS_OK;
        }
    }

    return fStatus;
}


void seRTC_GetAlarm( uint8_t *h, uint8_t *m, uint8_t *s, seRTC_AM_PM *am )
{
    uint16_t reg;

    reg = seS1D13C00Read16( RTC_ALM1 );
    *s = (((reg & RTC_SHA_bits) >> 12) * 10) + ((reg & RTC_SLA_bits) >> 8 );

    reg = seS1D13C00Read16( RTC_ALM2 );
    *m = (((reg & RTC_MIHA_bits) >> 4) * 10) + (reg & RTC_MILA_bits);
    *h = (((reg & RTC_HHA_bits) >> 12) * 10) + ((reg & RTC_HLA_bits) >> 8);
}


seStatus seRTC_SetSecondsAlarm( uint32_t alarmsec )
{
    seStatus fStatus = seSTATUS_NG;

    uint8_t currhour, currminute, currsecond;
    uint32_t hours, minutes, seconds;
    uint16_t mode;
    seRTC_AM_PM ampm;

    if (alarmsec < 86400)  // check that it is less than 24 hours
    {
        seRTC_GetHourMinuteSecond( &currhour,&currminute, &currsecond );
        ampm = seRTC_GetAM_PM();
        mode = seRTC_Get12_24Mode();

        hours = (alarmsec / 3600);
        minutes = (alarmsec - (hours * 3600)) / 60;
        seconds = (alarmsec - (hours * 3600) - (minutes * 60));

        seconds = seconds + currsecond;
        if (seconds >= 60)
        {
            seconds -= 60;
            minutes++;
        }

        minutes = minutes + currminute;
        if (minutes >= 60)
        {
            minutes -= 60;
            hours++;
        }

        if (mode == seRTC_CTL_RTC24H_24)
        {
            hours = hours + currhour;
            if (hours >= 24)
            {
                hours -= 24;
            }
        }
        else
        {
            hours = hours + currhour;
            if (hours >= 24)
            {
                hours -= 12;
                if (ampm == seRTC_HUR_RTCP_AM)
                    ampm = seRTC_HUR_RTCP_PM;
                else
                    ampm = seRTC_HUR_RTCP_AM;
            }
        }

        fStatus = seRTC_SetAlarm( hours, minutes, seconds, ampm );
    }

    return(fStatus);
}


seStatus seRTC_Set30secCorrection( void )
{
    seStatus fStatus = seSTATUS_OK;


    if ( (seS1D13C00Read8(RTC_CTLL) & RTC_ADJ_bits) == 0 )
    {
        seSetBits8( RTC_CTLL, RTC_ADJ_bits, RTC_ADJ_bits );
        // Wait until this bit goes away to prevent an unintentional write of 1 by read/modify of CTL. And return the status
        while( seS1D13C00Read8(RTC_CTLL) & RTC_ADJ_bits );
    } else {
        fStatus = seSTATUS_NG;
    }

    return fStatus;
}


void seRTC_StartStopWatchCount( seRTC_Interrupt irqs )
{
    seRTC_ClearIntFlag( irqs );
    seRTC_EnableInt( irqs );
    seSetBits16( RTC_SWCTL, RTC_SWRUN_bits, RTC_SWRUN_bits );
}

void seRTC_StopStopWatchCount( seRTC_Interrupt irqs )
{
    seRTC_DisableInt( irqs );
    seRTC_ClearIntFlag( irqs );
    seSetBits16( RTC_SWCTL, RTC_SWRUN_bits, 0 );
}

void seRTC_ResetStopWatchCount( swCounter * StopWatchCounter )
{
    uint16_t IsStopWatchRunning = seS1D13C00Read16( RTC_SWCTL ) & RTC_SWRUN_bits;

    StopWatchCounter->swInt1HzCount = 0;

    if ( !IsStopWatchRunning )
    {
        seS1D13C00Write16( RTC_SWCTL, 0x11 );
        seSetBits16( RTC_SWCTL, RTC_SWRUN_bits, 0 );
  } else {
          seSetBits16( RTC_SWCTL, RTC_SWRST_bits, RTC_SWRST_bits );
    }
}


seStatus seRTC_ReadStopWatchCount( swCounter * StopWatchCounter )
{
    seStatus fStatus = seSTATUS_NG;
    uint16_t i = 0;

    while ( i++ < 10 )
    {
        uint16_t val = seS1D13C00Read16( RTC_SWCTL );
        if ( val == seS1D13C00Read16( RTC_SWCTL ) )
        {
            StopWatchCounter->swChar10HzDigit  = 0x000F & (val >> 12);
            StopWatchCounter->swChar100HzDigit = 0x000F & (val >>  8);
            fStatus = seSTATUS_OK;
            break;
        }
    }

    return fStatus;
}


const uint8_t month_code[] = {0, 6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

uint8_t seRTC_CalcWeekDay( uint8_t y, uint8_t m, uint8_t d )
{
    uint8_t yearcode;
    uint8_t daycode;
    uint8_t weekday;

    daycode = d % 7;
    yearcode = y / 4;
    yearcode = (yearcode * 5) % 7;

    if ((y % 4) == 0)
    {
        // Leap year
        if (m < 3)
        {
            weekday = daycode + (month_code[m] - 1) +  yearcode;
        }
        else
        {
            weekday = daycode + month_code[m] +  yearcode;
        }
        weekday = weekday % 7;
    } else {
        weekday = daycode + month_code[m] +  yearcode + (y % 4);
        weekday = weekday % 7;
    }

    return weekday;
}


static int32_t freqerrsum_mHz = 0;
static int16_t prev_freqerr_mHz = 0;
static uint32_t tsamp = 900;
static int32_t rtcTrim = 0;

seStatus seRTC_InitTheoreticalRegulation( uint32_t sampling_period, int16_t curr_freqerr_mHz )
{
    seStatus fStatus = seSTATUS_NG;

    prev_freqerr_mHz = curr_freqerr_mHz;
    freqerrsum_mHz = 0;
    tsamp = sampling_period;

    fStatus = seRTC_SetSecondsAlarm( tsamp );  // Set alarm

    return fStatus;
}


seStatus seRTC_TheoreticalRegulationTrim( int16_t curr_freqerr_mHz )
{
    seStatus fStatus = seSTATUS_NG;

    // get average frequency error between current and previous value
    if ((prev_freqerr_mHz + curr_freqerr_mHz) < 0)
      prev_freqerr_mHz = (prev_freqerr_mHz + curr_freqerr_mHz - 1) / 2;
    else
      prev_freqerr_mHz = (prev_freqerr_mHz + curr_freqerr_mHz + 1) / 2;

    // Add to accumulator/sum
    freqerrsum_mHz += (int32_t) prev_freqerr_mHz;

    // trim value = - [((tsamp * 256)/ 32768) * freqerrsum_mHz] / 1000;
    if (freqerrsum_mHz < 0)
      rtcTrim = ((freqerrsum_mHz * tsamp) - 64000) / 128000L;  // round off negative value
    else
      rtcTrim = ((freqerrsum_mHz * tsamp) + 64000) / 128000L;  // round off positive value
    rtcTrim = -rtcTrim;  // Opposite sign of time drift

    // Clip to max adjust values
    if ((rtcTrim < 0) && (rtcTrim < -64))
      rtcTrim = -64;
    else if ((rtcTrim >= 0) && (rtcTrim > 63))
      rtcTrim = 63;

    // Adjust frequency error sum
    if (rtcTrim < 0)
      freqerrsum_mHz += (((rtcTrim * 128000L) - (tsamp>>1)) / tsamp);  // freqerrsum_mHz = freqerrsum_mHz + (rtcTrim * 32768 * 1000) / (256 * tsamp)
    else
      freqerrsum_mHz += (((rtcTrim * 128000L) + (tsamp>>1)) / tsamp);  // freqerrsum_mHz = freqerrsum_mHz + (rtcTrim * 32768 * 1000) / (256 * tsamp)

    // Trigger trimming
    if (rtcTrim < 0)  // make value right for RTCTRM register
      rtcTrim = rtcTrim + 128;

    while ( seS1D13C00Read8( RTC_CTLH ) & RTC_TRMBSY_bits );
    seSetBits8( RTC_CTLH, RTC_TRM_bits, (uint8_t)((rtcTrim & 0x7F) << 8));

    // Set up for next sample
    prev_freqerr_mHz = curr_freqerr_mHz;

    fStatus = seRTC_SetSecondsAlarm( tsamp );

    return fStatus;
}


static seStatus unprotect( void )
{
    seStatus fStatus = seSTATUS_OK;

    ///< Check busy.
    while( seS1D13C00Read8( RTC_CTLL ) & RTC_BSY_bits );


    ///< Halt RTC
    seSetBits8( RTC_CTLL, RTC_HLD_bits, RTC_HLD_bits );

    // When the RTCTRMBSY bit = 1, the RTCHLD bit cannot be written to 1 (fixed at 0).
    // Check if it was successful stop.
    fStatus = ( seS1D13C00Read8( RTC_CTLL ) & RTC_HLD_bits ) ? seSTATUS_OK : seSTATUS_NG;

    return fStatus;
}


static void protect( void )
{
  seSetBits8( RTC_CTLL, RTC_HLD_bits, 0 );
}
