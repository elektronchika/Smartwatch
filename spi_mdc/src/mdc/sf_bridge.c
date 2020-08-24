/**
  ******************************************************************************
  * @file    sf_bridge.c
  * @author  Epson
  * @version V1.1
  * @date    22-April-2018
  * @brief   This file provides serial flash programming interface.
  * Data provided from host by Xmodem protocol.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2018. All rights reserved.
  ******************************************************************************
  */
#include <stdio.h>
#include <string.h>

#include "s1d13c00_hcl.h"
#include "s1d13c00_memregs.h"
#include "se_common.h"
#include "se_clg.h"
#include "se_port.h"
#include "se_t16.h"
#include "se_qspi.h"
#include "serial_flash.h"
#include "sf_bridge.h"
#include "xmodem.h"
#include "crc16.h"
#include "sf_bridge.h"

static char buf[XM_PACKETS_BUFFERING_ALLOCATION];
int serial_flash_data_size;
#ifdef VERIFICATION
int gVerbose;
char testread[1024];
#endif


static seT16_InitTypeDef T16_InitStruct;
static seQSPI_InitTypeDef QSPI_InitStruct;

int SF_bridge_init( void ){

  seStatus fStatus = seSTATUS_OK;

  memset( &T16_InitStruct, 0, sizeof( T16_InitStruct ) );
  memset( &QSPI_InitStruct, 0, sizeof( QSPI_InitStruct ) );


  // Init T16
  seT16_InitStruct(&T16_InitStruct);
  T16_InitStruct.Period = 0x80;
  seT16_Init( T16_1_REGS_BASE, &T16_InitStruct );

  // Init Spi as Master
  seQSPI_InitStructForMaster(&QSPI_InitStruct);
  QSPI_InitStruct.CHDL = seQSPI_08CLK;
  QSPI_InitStruct.CHLN = seQSPI_08CLK;
  seQSPI_Init( &QSPI_InitStruct );

  uint32_t busspeed = 10000000;
  seQSPI_SetBusSpeed( busspeed );

  //  Start QSPI in a single mode
  seQSPI_Start();

  SetFlashMode( FLMODE_SINGLE );

  fStatus = WaitFlashBusy();

  return ( fStatus == seSTATUS_OK ) ;
}

int SF_bridge_id( uint8_t *mfc_id, uint16_t *dev_id ){

  *mfc_id = 0;
  *dev_id = 0;
 
  seStatus fStatus = IdentifyFlash( mfc_id, dev_id );
  serial_flash_data_size = 600*1024;
  return ( fStatus == seSTATUS_OK ) ;
}

int SF_bridge_sector_erase( uint32_t EraseStartAddress ) {

  seStatus fStatus = seSTATUS_OK;
  fStatus = EraseFlashSector( EraseStartAddress );
  return ( fStatus == seSTATUS_OK );
}

int SF_bridge_read( uint32_t adr, uint32_t sz, uint8_t *buf ){

  adr = adr & 0x0FFFFFFF;
  seStatus fStatus = ReadFlash( adr, buf, sz );
  return ( fStatus == seSTATUS_OK );
}

int SF_bridge_write( uint32_t adr, uint32_t sz, uint8_t *buf  ) {

  adr = adr & 0x0FFFFFFF;
  seStatus fStatus = ProgramFlash( adr, buf, sz );
  return ( fStatus == seSTATUS_OK );
}

int SF_bridge_mass_erase( void ) {

  seStatus fStatus = seSTATUS_OK;
  fStatus = EraseFlash();
  return ( fStatus == seSTATUS_OK );
}

int SF_bridge_total_erase( void ){

    seStatus fStatus = seSTATUS_OK;
    int sectnum;

    for ( sectnum = 0; sectnum < 1; sectnum++ )
    {
        // Erase Flash Sector
        if ( fStatus == seSTATUS_OK ) {
          // Erase Flash Sector
          fStatus = EraseFlashSector( sectnum*4096 );
        }

        fStatus = WaitFlashBusy();
    }

    return ( fStatus == seSTATUS_OK );
}


int SF_XmFlashDownload( int verbose )
{
    int st;
#ifdef VERIFICATION
    gVerbose = verbose;
#endif
    //SERIAL FLASH
    SF_bridge_init();
    uint8_t mfc_id; uint16_t dev_id;
    SF_bridge_id( &mfc_id, &dev_id );

#ifdef VERIFICATION
    printf ("\nIdentified flash [%02x %04x]\n", mfc_id, dev_id );
#else
    char outstr[32];
    UARTSend( "\nIdentified flash " );
    sprintf( outstr, "[%02x %04x]", mfc_id, dev_id );
    UARTSend( outstr );
    UARTSend("\r\n");;
#endif

#ifdef VERIFICATION
    SF_bridge_sector_erase( 0 );

    char *test =
            "abcdefghijklmnopqrstuvwxyz_abcdefghijklmnopqrstuvwxyz_abcdefghijklmnopqrstuvwxyz_abcdefghijklmnopqrstuvwxyz";
    memset( testread, 0xaa, 1024 );
    SF_bridge_write( 128, 128, (uint8_t*)test );
    SF_bridge_read( 128, 128, (uint8_t*)testread );
    if ( strncmp( testread, test, sizeof( *test )) == 0 )
        printf( "Serial flash OK.\n" );
#endif

#ifdef VERIFICATION
    printf("Erasing whole flash. Can take long time...\n");
#else
    UARTSend( "Erasing whole flash. Can take long time...\r\n" );
#endif

    SF_bridge_mass_erase();

#ifdef VERIFICATION
    printf ( "Send data using the xmodem protocol from your terminal emulator now...\n");
#else
    UARTSend( "Send data using the xmodem protocol from your terminal emulator now...\r\n" );
#endif

    flushinput();

    st = XM_Receive( (unsigned char *)buf, XM_PACKETS_BUFFERING_ALLOCATION );

    flushinput();
    if (st < 0) {

#ifdef VERIFICATION
        printf ("\nXmodem receive error: status: %d\n", st);
#else
        UARTSend( "\nXmodem receive error: status: " );
        sprintf( outstr, "%d", st );
        UARTSend( outstr );
        UARTSend("\r\n");;
#endif
    }
    else  {
#ifdef VERIFICATION
        printf ("\nXmodem successfully received %d bytes.\nFlash programmed.", st);
#else
        UARTSend("\r\nXmodem successfully received " );
        sprintf( outstr, "%d", st );
        UARTSend( outstr );
        UARTSend(" bytes.\r\n");
        UARTSend("\r\nFlash programmed.\n\n" );
#endif
    }

#ifdef VERIFICATION
    int i = 0;
    for ( i = 0; i < XM_NUMBER_PACKETS_BUFFERING; i++ ) {
        memset( testread, (i%2) ? 0xaa : 0x55, 1024 );
        SF_bridge_read( i*1024, 1024, (uint8_t*)testread );
        if ( strncmp( testread, buf+i*1024, 1024 ) == 0 )
            printf( "Sector %d of Serial flash OK.\n", i );
    }
#endif

    seSysSleepMS( 3000 );
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////
int SF_XmFlashUpload( int verbose )
{
    int st;
#ifdef VERIFICATION
    gVerbose = verbose;
#endif
    //SERIAL FLASH
    SF_bridge_init();
    uint8_t mfc_id; uint16_t dev_id;
    SF_bridge_id( &mfc_id, &dev_id );

#ifdef VERIFICATION
    printf ("\nIdentified flash [%02x %04x]\n", mfc_id, dev_id );
    int i=0;
    for ( i = 0; i < XM_PACKETS_BUFFERING_ALLOCATION; i++ ){
        buf[i] = i;
    }
#else
    char outstr[32];
    UARTSend( "\nIdentified flash " );
    sprintf( outstr, "[%02x %04x]", mfc_id, dev_id );
    UARTSend( outstr );
    UARTSend("\r\n");;
#endif
    if ((serial_flash_data_size = getDataSize())>0){

#ifdef VERIFICATION
    printf ( "Send data using the xmodem protocol to your terminal emulator now...\n");
#else
    UARTSend( "Send data using the xmodem protocol to your terminal emulator now...\r\n" );
#endif

        st = XM_Transmit( (unsigned char *)buf, serial_flash_data_size );

    if (st < 0) {

#ifdef VERIFICATION
        printf ("\nXmodem transmit error: status: %d\n", st);
#else
        UARTSend( "\nXmodem transmit error: status: " );
        sprintf( outstr, "%d", st );
        UARTSend( outstr );
        UARTSend("\r\n");;
#endif
    }
    else  {
#ifdef VERIFICATION
        printf ("\nXmodem successfully transmited %d bytes.\nFlash programmed.", st);
#else
        UARTSend("\r\nXmodem successfully transmited " );
        sprintf( outstr, "%d", st );
        UARTSend( outstr );
        UARTSend(" bytes.\r\n");
        UARTSend("\r\nFlash data saved to host.\n\n" );
#endif
    }
    }
    else {
        UARTSend("\r\nXmodem Upload has not been started.\r\n\r\n" );
    }

    return 0;
}
int  getDataSize(void){
    char str[32];

    UARTSend("Enter data size in decimal, eight digits maximum.");
    UARTSend("\r\nQ     - quit command");
    UARTSend("\r\nENTER - done entering data size\r\n");
    UARTSend("\r\nEnter data size now : ");
    unsigned int digit = 0;
    unsigned int number = 0;
    int n=8+1;
    do {
        if (UARTCharAvail())
        {
            digit = UARTReadChar();
            if ( digit == 'q' ) return 0;
            if ( digit == 13 || digit == 10 ){
                sprintf( str, "\r\nEntered %d [0x%x]\r\n", number, number );
                UARTSend( str );
                UARTSend("\r\n");
                return number;
            }
            if ( digit < '0' || digit > '9' ){
                UARTWriteChar( 7 );
                continue;
            }
            UARTWriteChar( digit );
            number *= 10;
            number += ( digit-'0' );
            n--;
        }
    } while (n > 0);
    return 0;
}
