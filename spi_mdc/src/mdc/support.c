/**
  ******************************************************************************
  * @file    support.c
  * @author  Epson
  * @version V1.0
  * @date    12-Mar-2018
  * @brief   This file contains commont support routines to assist with 
  * interfacing the S1D13C00 to the TI TM4C1294XL evaluation board.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2018. All rights reserved.
  *****************************************************************************
  */

#include "s1d13c00_hcl.h"
#include "se_common.h"

#include <string.h>
#include <stdio.h>



uint32_t g_ui32SysClock;


void InitializeHost( void )
{

}

static void (*hifirqcallback)(void);
void PortM5Handler(void)
{
    /*
    if (GPIOIntStatus(GPIO_PORTM_BASE, GPIO_INT_PIN_5))
    {
        hifirqcallback();
        ROM_GPIOIntClear(GPIO_PORTM_BASE, GPIO_INT_PIN_5);
    }
    */
}

void ConfigureHostInterrupt( void (*callback)(void) )
{
    /*
    ROM_GPIOIntDisable( GPIO_PORTM_BASE, GPIO_PIN_5 );         // Disable interrupt for PM5 (in case it was enabled)
    ROM_GPIOIntClear(   GPIO_PORTM_BASE, GPIO_PIN_5 );         // Clear pending interrupts for PM5
    hifirqcallback = callback;
    GPIOIntRegister(GPIO_PORTM_BASE, &PortM5Handler);
    ROM_GPIOIntTypeSet( GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_FALLING_EDGE );  // Configure PM5 for falling edge trigger

#ifdef DEBUG_PRINT
    printf( "HIFIRQ (Host Interrupt port) configured...\n");
#endif
    */
}

void EnableHostInterrupt( void )
{
    //ROM_GPIOIntEnable( GPIO_PORTM_BASE, GPIO_PIN_5 );
}


void DisableHostInterrupt( void )
{
    //ROM_GPIOIntDisable( GPIO_PORTM_BASE, GPIO_PIN_5 );
}


void InitializeMDC( hostmcu_config hostif )
{
    // Initialize S1D13C00 controller interface
    seS1D13C00InitializeController(hostif, g_ui32SysClock, 6000000, 800000);

#ifdef DEBUG_PRINT
    printf("S1D13C00 Interface configured...\n");
#endif
}



static void (*button1callback)(void);
void PortJ1Handler(void)
{
    /*
    if (GPIOIntStatus(GPIO_PORTJ_BASE, GPIO_INT_PIN_0))
    {
        // Debounce
        seSysSleepMS(100);
        if (Button1Pressed())	
            button1callback();

        ROM_GPIOIntClear(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
    }
    */
}

void InitializeButton1( void (*callback)(void) )
{
    /*
    // Set up SW1 (port PJ0) push-button interrupt
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  // pull-up
    ROM_GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
    ROM_GPIOIntClear(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);

    button1callback = callback;
    GPIOIntRegister(GPIO_PORTJ_BASE, &PortJ1Handler);

    ROM_GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOJ);

#ifdef DEBUG_PRINT
    printf( "BUTTON1 configured...\n" );
#endif
    */
}


bool Button1Pressed( void )
{
    /*
    if (ROM_GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == GPIO_PIN_0)
       return false;
    else
       return true;
    */
}


//---- UART routines ---------------------------------------------------------------
#define UARTBUFF_SIZE   (1024+8)

static uint32_t uartrx_rdptr = 0;
static uint32_t uartrx_wrptr = 0;
static uint8_t uartrx_buff[UARTBUFF_SIZE];


//*****************************************************************************
//
// The UART RX interrupt handler.
//
//*****************************************************************************
static void UARTRXIntHandler( void )
{
    /*
    uint32_t ui32Status;
    uint32_t rxchar;

    // Get the interrupt status.
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);

    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART0_BASE, ui32Status);

    // Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        rxchar = ROM_UARTCharGetNonBlocking(UART0_BASE);
        if (uartrx_wrptr == (UARTBUFF_SIZE-1))
        {
            if (uartrx_rdptr != 0)
            {
                uartrx_buff[uartrx_wrptr] = (uint8_t) rxchar;
                uartrx_wrptr = 0;
            }
        }
        else
        {
            if ((uartrx_wrptr + 1) != uartrx_rdptr)
            {
                uartrx_buff[uartrx_wrptr] = (uint8_t) rxchar;
                uartrx_wrptr++;
            }
        }

    }
    */
}


//*****************************************************************************
// Initialize UART for 115,200 bps.  Provide RX character handler function.
// Only RX interrupt is enabled.  TX is not interrupt driven.
//*****************************************************************************
void InitializeUART( void )
{
    /*
    uartrx_rdptr = uartrx_wrptr = 0;

    // Set GPIO A0 and A1 as UART pins.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    UARTFIFOLevelSet( UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8 );
    UARTFIFOEnable( UART0_BASE );

    // Enable the UART interrupt.
    UARTIntRegister(UART0_BASE, &UARTRXIntHandler);
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

#ifdef DEBUG_PRINT
    printf( "UART configured...\n");
#endif
    */
}


//*****************************************************************************
// Send a string to the UART.  Not interrupt driven.
//*****************************************************************************
void UARTSend( char *pui8Buffer )
{
    /*
    uint32_t ui32Count;

    ui32Count = strlen((char *)pui8Buffer);

    // Loop while there are more characters to send.
    while(ui32Count--)
    {
        // Write the next character to the UART.
        ROM_UARTCharPut(UART0_BASE, *pui8Buffer++);
    }
    */
}

//*****************************************************************************
// Send a character to the UART.  Not interrupt driven.
//*****************************************************************************
void UARTWriteChar( uint8_t txchar )
{
    //ROM_UARTCharPut(UART0_BASE, txchar);
}

//*****************************************************************************
//
// Check character available from UART RX buffer.
//
//*****************************************************************************
bool UARTCharAvail( void )
{
    if (uartrx_rdptr == uartrx_wrptr)
        return false;
    else
        return true;
}


//*****************************************************************************
//
// Read character from UART RX buffer.
//
//*****************************************************************************
uint8_t UARTReadChar( void )
{
    uint8_t retval;

    if (uartrx_rdptr != uartrx_wrptr)
    {
        retval = uartrx_buff[uartrx_rdptr];
        if (++uartrx_rdptr == UARTBUFF_SIZE)
            uartrx_rdptr = 0;
        return retval;
    }
    else
        return 0;
}

//----------------------------------------------------------------------------------

