/**
  ******************************************************************************
  * @file    support.h
  * @author  Epson
  * @version V1.0
  * @date    12-Mar-2018
  * @brief   This file contains TM4C1294NCPDT support interface definitions.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2018. All rights reserved.
  *****************************************************************************
  */

#ifndef SUPPORT_H_
#define SUPPORT_H_

#define HIFIRQ_ASSERTED     ((ROM_GPIOPinRead(GPIO_PORTM_BASE, GPIO_PIN_5) == GPIO_PIN_5) ? 1 : 0)
#define LED_ON()            (GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0))
#define LED_OFF()           (GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0))
#define LED_TOGGLE()        (GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_0) ^ GPIO_PIN_0))

#define seSysSleepMS(msval) nrf_delay_ms(msval)//ROM_SysCtlDelay((120000000/3000)*msval)
#define seSysSleep()        ROM_SysCtlSleep()

#define TM4C1294

void InitializeHost( void );

void ConfigureHostInterrupt( void (*callback)(void) );
void EnableHostInterrupt( void );
void DisableHostInterrupt( void );


void InitializeMDC( hostmcu_config hostif );

void InitializeButton1( void (*callback)(void) );
bool Button1Pressed( void );

void InitializeUART( void );
void UARTSend( char * pui8Buffer );
void UARTWriteChar( uint8_t txchar );
bool UARTCharAvail( void );
uint8_t UARTReadChar( void );

#endif /* SUPPORT_H_ */
