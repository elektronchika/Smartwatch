/*	
 * Copyright 2001-2010 Georges Menie (www.menie.org)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* this code needs standard functions memcpy() and memset()
   and input/output functions _inbyte() and _outbyte().

   the prototypes of the input/output functions are:
     int _inbyte(unsigned short timeout); // msec timeout
     void _outbyte(int c);

 */
/* Copyright(C) Modified by SEIKO EPSON CORPORATION 2018. All rights reserved.
  ******************************************************************************
  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "s1d13c00_hcl.h"
#include "se_common.h"
#include "xmodem.h"
#include "crc16.h"
#include "sf_bridge.h"



//////////////////////////////////////////////////
// Xmodem s1c13c00 adaptation
#ifdef VERBOSE
extern int gVerbose;
#endif
static int _inbyte( unsigned int timeout );
static void _outbyte( int txchar );
static int ProcessInChunk(  int packetno, int addr, unsigned char* p, int size );
static int ProcessOutChunk(  int packetno, int addr, unsigned char* p, int size );

static int _inbyte( unsigned int timeoutMS ){
    // manage timeout here
    int poll = timeoutMS/10;
    do {
        if (UARTCharAvail()){
            break;
        }
        seSysSleepMS(10);
    } while (--poll > 0 );

    int c = poll ? UARTReadChar():-1;
#ifdef VERBOSE
    if ( gVerbose & 1 ) {
        static int count = 0;
        if ( count++ %64 == 0 ) {
            printf( "\n");
        }
        else {
            printf("%02x", c==-1 ? 0x0f : c );
        }
    }
#endif
    return c;
}

static void _outbyte( int txchar ){
#ifdef VERBOSE
    if ( gVerbose & 2 ) {

        static int count = 0;
        if ( count++ %64 == 0 ) {
            printf( "\n");
        }
        printf( "[ %02x ]", txchar );
    }
#endif
    UARTWriteChar((char) txchar);
}

int ProcessInChunk( int packetno, int addr, unsigned char* p, int size ){
#ifdef VERIFICATION
    printf( "\npkt# %d addr 0x%x ptr 0x%x sz %d\n", packetno, addr, (unsigned int)p, size );
#endif
    int ret = SF_bridge_write( addr, size, p );
    return ( ret ) ? ACK : NAK;
}


int ProcessOutChunk( int packetno, int addr, unsigned char* p, int size ){
#ifdef VERIFICATION
    printf( "\npkt# %d addr 0x%x ptr 0x%x sz %d\n", packetno, addr, (unsigned int)p, size );
#endif
    int ret = SF_bridge_read( addr, size, p );
    return ( ret ) ? ACK : NAK;
}

static int check(int crc, const unsigned char *buf, int sz)
{
	if (crc) {
		unsigned short crc = crc16_ccitt(buf, sz);
		unsigned short tcrc = (buf[sz]<<8)+buf[sz+1];
		if (crc == tcrc)
			return 1;
	}
	else {
		int i;
		unsigned char cks = 0;
		for (i = 0; i < sz; ++i) {
			cks += buf[i];
		}
		if (cks == buf[sz])
		return 1;
	}

	return 0;
}

void flushinput( void )
{
	while (_inbyte(((DLY_1S)*3)>>1) >= 0)
		;
}

int XM_Receive( unsigned char *dest, int destsz )
{
	unsigned char xbuff[1030]; /* Allocated on the stack. 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	unsigned char *p;
	int bufsz, crc = 0;
	unsigned char trychar = 'C';
	unsigned char packetno = 1;
	int i, c, len = 0, total = 0;
	int retry, retrans = MAXRETRANS;
	int ret = ACK;

	for(;;) {
		for( retry = 0; retry < 16; ++retry) {
			if (trychar) _outbyte(trychar);
			if ((c = _inbyte((DLY_15S)<<1)) >= 0) {
				switch (c) {
				case SOH:
					bufsz = 128;
					goto start_recv;
				case STX:
					bufsz = 1024;
					goto start_recv;
				case EOT:
					flushinput();
					_outbyte(ACK);
#ifdef VERBOSE
					if ( gVerbose & 2 )
					    printf( "[[[ EOT ]]]" );
#endif
					return total; /* normal end */
				case CAN:
					if ((c = _inbyte(DLY_1S)) == CAN) {
						flushinput();
						_outbyte(ACK);
						return -1; /* canceled by remote */
					}
					break;
				default:
					break;
				}
			}
		}
		//if (trychar == 'C') { trychar = NAK; continue; }
		flushinput();
		_outbyte(CAN);
		_outbyte(CAN);
		_outbyte(CAN);
		return -2; /* sync error */

	start_recv:
		if (trychar == 'C') crc = 1;
		trychar = 0;
		p = xbuff;
		*p++ = c;
		for (i = 0;  i < (bufsz+(crc?1:0)+3); ++i) {
			if ((c = _inbyte(DLY_1S)) < 0) {
#ifdef VERBOSE
			    if ( gVerbose & 2 )
			        printf( "=%d\n", i );
#endif
			    goto reject;
			}
			//else
			*p++ = c;
		}

		if (xbuff[1] == (unsigned char)(~xbuff[2]) && 
			(xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
			check(crc, &xbuff[3], bufsz)) {
			if (xbuff[1] == packetno)	{
				register int count = destsz - len;
				if (count > bufsz) count = bufsz;
				if (count > 0) {
					memcpy ( &dest[len], &xbuff[3], count );
					ret = ProcessInChunk( packetno, total, (unsigned char *)&dest[len], bufsz );

					len += count;
					total += count;
					if ( len >= bufsz*XM_NUMBER_PACKETS_BUFFERING )
					    len = 0;
				}
				++packetno;
				retrans = MAXRETRANS+1;
			}
			if (--retrans <= 0) {
				flushinput();
				_outbyte(CAN);
				_outbyte(CAN);
				_outbyte(CAN);
				return -3; /* too many retry error */
			}
			_outbyte( ret );
			continue;
		}
	reject:
		flushinput();
		_outbyte(NAK);
	}
}


int XM_Transmit( unsigned char *src, int srcsz )
{
    unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
    int bufsz, crc = -1;
    unsigned char packetno = 1;
    int i, c, len = 0, total = 0;
    int retry;

    for(;;) {
        for( retry = 0; retry < 16; ++retry) {
            if ((c = _inbyte((DLY_1S)<<1)) >= 0) {
                switch (c) {
                case 'C':
                    crc = 1;
                    goto start_trans;
                case NAK:
                    crc = 0;
                    goto start_trans;
                case CAN:
                    if ((c = _inbyte(DLY_1S)) == CAN) {
                        _outbyte(ACK);
                        flushinput();
                        return -1; /* canceled by remote */
                    }
                    break;
                default:
                    break;
                }
            }
        }
        _outbyte(CAN);
        _outbyte(CAN);
        _outbyte(CAN);
        flushinput();
        return -2; /* no sync */

        for(;;) {
        start_trans:
#ifdef TRANSMIT_XMODEM_1K
            xbuff[0] = STX; bufsz = 1024;
#else
            xbuff[0] = SOH; bufsz = 128;
#endif
            xbuff[1] = packetno;
            xbuff[2] = ~packetno;
            c = srcsz - total;
            if (c > bufsz) c = bufsz;
            if (c > 0) {
                memset (&xbuff[3], 0, bufsz);
                ProcessOutChunk( packetno, total, (unsigned char *)&src[len], bufsz );
                memcpy (&xbuff[3], &src[len], c);

                if (c < bufsz) xbuff[3+c] = CTRLZ;
                if (crc) {
                    unsigned short ccrc = crc16_ccitt(&xbuff[3], bufsz);
                    xbuff[bufsz+3] = (ccrc>>8) & 0xFF;
                    xbuff[bufsz+4] = ccrc & 0xFF;
                }
                else {
                    unsigned char ccks = 0;
                    for (i = 3; i < bufsz+3; ++i) {
                        ccks += xbuff[i];
                    }
                    xbuff[bufsz+3] = ccks;
                }
                for (retry = 0; retry < MAXRETRANS; ++retry) {
                    for (i = 0; i < bufsz+4+(crc?1:0); ++i) {
                        _outbyte(xbuff[i]);
                    }
                    if ((c = _inbyte(DLY_1S)) >= 0 ) {
                        switch (c) {
                        case ACK:
                            ++packetno;

                            len += bufsz;
                            total += bufsz;
                            if ( len >= bufsz*XM_NUMBER_PACKETS_BUFFERING )
                                   len = 0;


                            //len += count;
                           // total += count;
                            //if ( len >= bufsz*XM_NUMBER_PACKETS_BUFFERING )
                            //    len = 0;



                            goto start_trans;
                        case CAN:
                            if ((c = _inbyte(DLY_1S)) == CAN) {
                                _outbyte(ACK);
                                flushinput();
                                return -1; /* canceled by remote */
                            }
                            break;
                        case NAK:
                        default:
                            break;
                        }
                    }
                }
                _outbyte(CAN);
                _outbyte(CAN);
                _outbyte(CAN);
                flushinput();
                return -4; /* xmit error */
            }
            else {
                for (retry = 0; retry < 10; ++retry) {
                    _outbyte(EOT);
                    if ((c = _inbyte((DLY_1S)<<1)) == ACK) break;
                }
                flushinput();
                return (c == ACK)?total:-5;
            }
        }
    }
}
