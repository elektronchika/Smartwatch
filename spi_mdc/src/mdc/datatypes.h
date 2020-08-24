//////////////////////////////////////////////////////////////////////////////
///
/// \file datatypes.h
///
/// \par  COMMON DATA TYPES
///
///	As much as possible, programs should use natural data types (eg. int) to
///	allow compiler optimizations to kick in.  When specific data widths are
///	required, then the following types should be used.  Note that on some
///	platforms an int is ONLY 16-BITS and that all int expressions will not
/// be promoted to longs. Therefore, int must be used carefully.
///
//  ©SEIKO EPSON CORPORATION 2012 - 2018. All rights reserved.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
//
//////////////////////////////////////////////////////////////////////////////

//	Please do not declare HCL-specific datatypes here, put them in s1D13C00_hcl.h

#include <stdint.h>
#include <stdbool.h>

#ifndef seDATATYPE_H_INCLUDED
#define seDATATYPE_H_INCLUDED

typedef unsigned char       UInt8;			///<  8-Bit Unsigned Integer
typedef unsigned short      UInt16;			///< 16-Bit Unsigned Integer
typedef unsigned int        UInt32;			///< 32-Bit Unsigned Integer
typedef unsigned int        UInt;

typedef signed char         Int8;			///<  8-Bit Signed Integer
typedef signed short        Int16;			///< 16-Bit Signed Integer
typedef signed int          Int32;			///< 32-Bit Signed Integer
typedef signed int			Int;

// These types are defined in stdint.h. Configure as required
#ifndef _STDINT_H_
typedef unsigned char       uint8_t;        ///<  8-Bit Unsigned Integer
typedef unsigned short      uint16_t;       ///< 16-Bit Unsigned Integer
#ifndef _UINT32_T_DECLARED
typedef unsigned int        uint32_t;       ///< 32-Bit Unsigned Integer
#define _UINT32_T_DECLARED
#endif
typedef unsigned int        uint_t;

typedef signed char         int8_t;         ///<  8-Bit Signed Integer
typedef signed short        int16_t;        ///< 16-Bit Signed Integer
#ifndef _INT32_T_DECLARED
typedef signed int          int32_t;        ///< 32-Bit Signed Integer
#define _INT32_T_DECLARED
#endif
typedef signed int          int_t;
#endif

// If using C++, use bool (true/false).
typedef int					Boolean;		///< Boolean uses int as standard C doesn't have bool.		

typedef void*	            Handle;

#ifndef __IO
#define __IO volatile
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL				0
#else
#define NULL				((void*)0)
#endif
#endif

#ifndef UINT32_MAX
#define UINT32_MAX          0xFFFFFFFF
#endif

#ifndef RGB32
typedef struct tagRGBQ
{
    UInt8 rgbBlue;
	UInt8 rgbGreen;
	UInt8 rgbRed;
	UInt8 rgbReserved;
} RGBQ;

typedef union tagRGB32
{
	RGBQ   rgbq;
	UInt32 rgb32;
} RGB32;
#endif

#endif // seDATATYPE_H_INCLUDED


