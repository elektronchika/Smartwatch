/**
  ******************************************************************************
  * @file    se_rtc.h
  * @author  Epson
  * @version V1.0
  * @date    20-September-2017
  * @brief   This file contains function prototypes for the RTC library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SE_RTC_H
#define SE_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

   
/** @addtogroup sePeriphLibrary
  * @{
  */

/**
  * @defgroup RTC
  * @{
  * @brief RTC is a real-time clock with a perpetual calendar function.
  */

/** @defgroup RTC_Constants
  * @{
  */

typedef enum {
  seRTC_CTL_RTC24H_12   = 0,           ///< Hour counter - 12H mode.
  seRTC_CTL_RTC24H_24   = 1,           ///< Hour counter - 24H mode.
} seRTC_Hours12_24;

typedef enum {
  seRTC_HUR_RTCP_AM   = 0,             ///< Mode - A.M.
  seRTC_HUR_RTCP_PM   = 1,             ///< Mode - P.M.
} seRTC_AM_PM;

typedef enum {
  seRTC_YAR_RTCWK_SUN 	= 0,            ///< Day of the week - Sunday.
  seRTC_YAR_RTCWK_MON 	= 1,            ///< Day of the week - Monday.
  seRTC_YAR_RTCWK_TUE 	= 2,            ///< Day of the week - Tuesday.
  seRTC_YAR_RTCWK_WED 	= 3,            ///< Day of the week - Wednesday.
  seRTC_YAR_RTCWK_THU 	= 4,            ///< Day of the week - Thursday.
  seRTC_YAR_RTCWK_FRI 	= 5,            ///< Day of the week - Friday.
  seRTC_YAR_RTCWK_SAT 	= 6,            ///< Day of the week - Saturday.
} seRTC_DayOfTheWeek;

typedef enum {
  seRTC_1_32SECI = 0x0001U,             ///< 1/32 second interrupt
  seRTC_1_8SECI  = 0x0002U,             ///< 1/8 second interrupt
  seRTC_1_4SECI  = 0x0004U,             ///< 1/4 second interrupt
  seRTC_1_2SECI  = 0x0008U,             ///< 1/2 second interrupt
  seRTC_1SECI    = 0x0010U,             ///< 1 second interrupt
  seRTC_1MINI    = 0x0020U,             ///< 1 minute interrupt
  seRTC_1HURI    = 0x0040U,             ///< 1 hour interrupt
  seRTC_1DAYI    = 0x0080U,             ///< 1 day interrupt
  seRTC_ALARMI   = 0x0100U,             ///< ALARM interrupt
  seRTC_SW100I   = 0x1000U,             ///< 100 Hz interrupt
  seRTC_SW10I    = 0x2000U,             ///< 10 Hz interrupt
  seRTC_SW1I     = 0x4000U,             ///< 1 Hz interrupt
  seRTC_RTCTRMI  = 0x8000U,             ///< Theoretical regulation interrupt
  seRTC_ALL_INT  = 0xF1FFU,             ///< All
} seRTC_Interrupt;

#define seRTC_INTS(a) ((seRTC_Interrupt)((a))) ///< Combination of any of the seRTC_Interrupt enumerations

#define seRTC_VALID_12HOURS( h )    ((h) <= 12  && (h) > 0 )     ///< Checks for valid 12-hour value
#define seRTC_VALID_24HOURS( h )    ((h) <  24 )                 ///< Checks for valid 24-hour value
#define seRTC_VALID_MINUTES( m )    ((m) <  60 )                 ///< Checks for valid minutes value
#define seRTC_VALID_SECONDS( s )    ((s) <  60 )                 ///< Checks for valid seconds value

/**
  * @}
  */

/** @defgroup RTC_Types
  * @{
  */

 /**
  * @brief  RTC Stopwatch counter structure definition
  */
typedef struct {
  uint16_t swInt1HzCount;       ///< Counts of 1Hz interrupts(0-65535).
  uint8_t swChar10HzDigit;      ///< 10Hz-digit stopwatch count(0-9).
  uint8_t swChar100HzDigit;     ///< 100Hz-digit stopwatch count(0-9).
}swCounter;

/**
  * @brief  RTC Init structure definition
  */
typedef struct{
  seState           ClkSupldInDebugMode;    ///< Specifies if System clock supplied in CPU Sleep Mode.
  seRTC_Hours12_24 H12_24Format;            ///< Specifies the RTC 12/24 Hour Format.
} seRTC_InitTypeDef;


/**
  * @}
  */

/** @defgroup RTC_Functions
  * @{
  */

/**
  * @brief  Initializes the RTC peripheral according to the specified
  *   parameters in the seRTC_InitStruct.
  * @param  InitStruct: pointer to a seRTC_InitTypeDef structure that
  *   contains the configuration information for the specified RTC peripheral.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_Init( seRTC_InitTypeDef* InitStruct );

/**
  * @brief  Starts RTC channel by start supplying operating clock.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_Start( void );

/**
  * @brief  Stops RTC channel by stop supplying operating clock.
  * @retval None
  */
void seRTC_Stop( void );

/**
  * @brief Enables RTC channel interrupt.
  * @param irq: Selected interrupt flag @ref seRTC_Interrupt
  * @retval None
  */
void seRTC_EnableInt( seRTC_Interrupt irq );

/**
  * @brief  Disables RTC channel interrupt.
  * @param irq: Selected interrupt flag @ref seRTC_Interrupt
  * @retval None
  */
void seRTC_DisableInt( seRTC_Interrupt irq );

/**
  * @brief  Returns RTC interrupt flag.
  * @param irq: Selected interrupt flag @ref seRTC_Interrupt
  * @retval InterruptStatus
  */
seInterruptStatus seRTC_GetIntFlag( seRTC_Interrupt irq );

/**
  * @brief  Clears RTC channel interrupt.
  * @param irq: Selected interrupt flag @ref seRTC_Interrupt
  * @retval None
  */
void seRTC_ClearIntFlag( seRTC_Interrupt irq );

/**
  * @brief  Start 1 sec timer.
  * The 1-second interrupt of RTC is enabled. User shall enable interrupt globally and
  * provide an interrupt service routine.
  * @retval None
**/
void seRTC_Enable1SecTimer( void );

/**
  * @brief  Stops 1 sec timer.
  * The 1-second interrupt of RTC is disabled. User shall
  * provide appropriate actions in the interrupt service routine.
  * @retval None
**/
void seRTC_Disable1SecTimer( void );

/**
  * @brief Calculate theoretical regulation bits.
  * + Measure the frequency tolerance of fOSC1.
  * + Determine the theoretical regulation execution cycle time.
  * - The correction value for theoretical regulation can be specified within the
  * range from -64 to +63 and it should be written to the CTL register's RTCTRM[6:0] bits
  * as a twos-complement number.
  * @param  m: This parameter is the frequency tolerance in ppm time ten ( to preserve accuracy in integer math. ).
  * @param  n: This param is execution cycle time in seconds.
  * @retval This function returns TRM bits.
**/
uint8_t seRTC_CalculateTrm( int32_t m, int32_t n );


/**
  * @brief Writes 12/24 mode to hardware
  * @param mode: 12/24 mode
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_Set12_24Mode( seRTC_Hours12_24 mode );

/**
  * @brief Reads 12/24 mode from hardware
  * @retval 12/24 Mode: a value of @seRTC_Hours12_24
  */
seRTC_Hours12_24 seRTC_Get12_24Mode( void );


/**
  * @brief Setting a year, month, day, day of the week in hardware.
  * When a value out of the effective range is set to the year, day of the week,
  * the counter will be cleared to 0 at the next
  * count-up timing. When a such value is set to the month, day,
  * the counter will be set to 1 at the next count-up timing.
  * @param  year: Integer year value (from 0 to 99).
  * @param  month: Integer month value (from 1 to 12).
  * @param  day: Integer day value (from 1 to 31).
  * @param  week: Integer day of the week value of type @ref seRTC_DayOfTheWeek.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_SetYearMonthDayWeek( uint8_t year, uint8_t month, uint8_t day, uint8_t week );

/**
  * @brief Getting a year, month, day, day of the week from hardware.
  * @param  year: Integer year value (from 0 to 99).
  * @param  month: Integer month value (from 1 to 12).
  * @param  day: Integer day value (from 1 to 31).
  * @param  week: Integer day of the week value of type @ref seRTC_DayOfTheWeek.
   * @retval Status: can be a value of @ref seStatus
 */
seStatus seRTC_GetYearMonthDayWeek( uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* week );


/**
  * @brief Setting an hour, minute, second in hardware.
  * When a value out of the effective range is set to
  * hour (in 24H mode) counter, the counter will be cleared to 0 at the next
  * count-up timing. When a such value is set to hour (in 12H mode) counter,
  * the counter will be set to 1 at the next count-up timing.
  * @param  h: Integer hour value.
  * @param  m: Integer minute value.
  * @param  s: Integer second value.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_SetHourMinuteSecond( uint8_t h, uint8_t m, uint8_t s );

/**
  * @brief Getting an hour, minute, second from hardware.
  * @param  h: Pointer to integer hour value.
  * @param  m: Pointer to integer minute value.
  * @param  s: Pointer to integer second value.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_GetHourMinuteSecond( uint8_t* h, uint8_t* m, uint8_t* s );


/**
  * @brief Set AM/PM indicator.
  * @param indicator: AM/PM indicator. @ref seRTC_AM_PM_indicator
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_SetAM_PM( seRTC_AM_PM indicator );

/**
  * @brief Get AM/PM indicator.
  * @retval indicator: @ref seRTC_AM_PM.
  */
seRTC_AM_PM seRTC_GetAM_PM( void );

/**
  * @brief  Sets an alarm clock and enables alarm interrupt,
  * user provides an interrupt service routine and enables interrupt globally.
  * @param  Integer hour value from 0 to 24 @ref seRTC_VALID_24HOURS.
  * @param  Integer minute value from 0 to 60 @ref seRTC_MINUTES.
  * @param  Integer second value from 0 to 60 @ref seRTC_VALID_SECONDS.
  * @param  AM/FM settings @ref seRTC_AM_PM.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_SetAlarm( uint8_t h, uint8_t m, uint8_t s, seRTC_AM_PM am );

/**
  * @brief  Gets an alarm clock and enables alarm interrupt,
  * user provides an interrupt service routine and enables interrupt globally.
  * @param  h: pointer to Integer hour value from 0 to 24 @ref seRTC_VALID_24HOURS.
  * @param  m: pointer to Integer minute value from 0 to 60 @ref seRTC_MINUTES.
  * @param  s: pointer to Integer second value from 0 to 60 @ref seRTC_VALID_SECONDS.
  * @param  am: pointer to AM/FM settings @ref seRTC_AM_PM.
  * @retval None
  */
void seRTC_GetAlarm( uint8_t *h, uint8_t *m, uint8_t *s, seRTC_AM_PM *am );

/**
  * @brief  Sets an alarm in number of seconds from current time.
  * seRTC_SetAlarm() is called.
  * User provides an interrupt service routine and enables interrupt globally.
  * @param  alarmsec:  alarm seconds from current time.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_SetSecondsAlarm( uint32_t alarmsec );


  /**
  * @brief Set 30-second correction.
  * This function is provided to set the time-of-day clock by the time signal. Writing 1 to the CTL RTCDJ
  * bit adds 1 to the minute counter if the second counter represents 30 to 59 seconds, or clears the second counter
  * with the minute counter left unchanged if the second counter represents 0 to 29 seconds.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_Set30secCorrection( void );

/**
  * @brief Calculate weekday from year, month, day
  * @param y: Integer year value.
  * @param m: Integer month value.
  * @param d: Integer day of the month value.
  * @retval weekday: @ref seRTC_DayOfTheWeek.
  */
uint8_t seRTC_CalcWeekDay( uint8_t y, uint8_t m, uint8_t d );

/**
  * @brief Start Stopwatch counter
  * @param irqs: 1Hz, 10Hz and 100Hz Interrupt, see @ref seRTC_Interrupt
  * @retval None
  */
void seRTC_StartStopWatchCount( seRTC_Interrupt irqs );

/**
  * @brief Stop Stopwatch counter
  * @param irqs: 1Hz, 10Hz and 100Hz Interrupt, see @ref seRTC_Interrupt
  * @retval None
  */
void seRTC_StopStopWatchCount(  seRTC_Interrupt irqs );

/**
  * @brief Reset Stopwatch counter and set members of StopWatchCounter to 0
  * @param StopWatchCounter: 1Hz, 10Hz and 100Hz Interrupt, see @ref swCounter
  * @retval None
  */
void seRTC_ResetStopWatchCount( swCounter *StopWatchCounter );

/**
  * @brief Read the 1/10-second and 1/100-second digits of  the stopwatch counter.
  * Note, that seconds and minutes should be assigned outside of this function.
  * @param StopWatchCounter: pointer to StopWatchCounter value of the stopwatch counter
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_ReadStopWatchCount( swCounter *StopWatchCounter );

/**
  * @brief  Initialize Theoretical Regulation variables and set RTC alarm for next adjustment.
  * seRTC_SetAlarm() is called.
  * User provides an interrupt service routine and enables interrupt globally.
  * @param  sampling_period:  sampling period for Theoretical Regulation adjustments.
  * @param  curr_freqerr_mHz:  current OSC1 frequency error in milliHertz.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_InitTheoreticalRegulation( uint32_t sampling_period, int16_t curr_freqerr_mHz );

/**
  * @brief  Theoretical Regulation adjustment calculation/trimming and set RTC alarm for next adjustment.
  * seRTC_SetAlarm() is called.
  * User provides an interrupt service routine and enables interrupt globally.
  * @param  curr_freqerr_mHz:  current OSC1 frequency error in milliHertz.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seRTC_TheoreticalRegulationTrim( int16_t curr_freqerr_mHz );

/**
  * @}
  */ 

/**
  * @}
  */ 
/**
  * @}
  */ 
#ifdef __cplusplus
}
#endif

#endif /*SE_RTC_H */




