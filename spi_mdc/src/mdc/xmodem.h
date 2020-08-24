#ifndef XMODEM_H
#define XMODEM_H

#ifdef __cplusplus
 extern "C" {
#endif

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 1000
#define DLY_15S 1500
#define MAXRETRANS 25

#define XM_PACKETS_BUFFERING_ALLOCATION (8*1024)        // Area allocated for packets in RAM. Has to be at least 2*0x1000
#define XM_NUMBER_PACKETS_BUFFERING (8)                 // Area allocated for the flash page in RAM. Has to be

//#define VERIFICATION                                  // Flag defines debugging features. Shall be off for the demo.
#define TRANSMIT_XMODEM_1K                              //
                  //
int XM_Transmit( unsigned char *src, int srcsz );       //
int XM_Receive( unsigned char *dest, int destsz );      //

extern void flushinput( void );

#ifdef __cplusplus
}
#endif

 #endif // XMODEM_H
