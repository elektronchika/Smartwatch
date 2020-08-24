//////////////////////////////////////////////////////////////////////////////
///
///  \file s1d13c00_hcl.h
///
/// \par HCL (Hardware Control Layer)
///	The HCL (Hardware Control Layer) abstracts low level hardware functions
///	such as reading and writing to registers and memory using functions such
///	as seWriteReg(), and seReadReg().  
///	\par
/// There are also common bit wise manipulation functions such as seClearBitsReg()
/// to make coding easier to read and faster to write.
///	\par
///	Burst read and write is also supported for both memory and registers 
///	These functions provide speed improvements when using indirect parallel 
///	or serial host interfaces as the address is only sent once at the 
/// beginning instead of each cycle.
///	\par
///	All API register and memory accesses should be abstracted through this 
///	interface.
///
//  ©SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
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

#ifndef seHCL_H_INCLUDED
#define seHCL_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"


//=========================== BOARD-SPECIFIC DEFINITIONS =================================
//*****************************************************************************
//
// Defines the Host Interface configurations
//
//*****************************************************************************
typedef enum {
  HOSTMCU_SPI_MONOADDR_MONODATA = 0,       ///< SPI single Command, single address, single data
  HOSTMCU_SPI_MONOADDR_DUALDATA = 1,       ///< SPI single Command, single address, dual data
  HOSTMCU_SPI_DUALADDR_DUALDATA = 2,       ///< SPI single Command, dual address, dual data
  HOSTMCU_SPI_MONOADDR_QUADDATA = 3,       ///< SPI single Command, single address, quad data
  HOSTMCU_SPI_QUADADDR_QUADDATA = 4,       ///< SPI single Command, quad address, quad data
  HOSTMCU_SPI_DUAL_ALL          = 5,       ///< SPI dual command/address/data
  HOSTMCU_SPI_QUAD_ALL          = 6,       ///< SPI quad command/address/data
  HOSTMCU_INDIRECT_8BIT         = 7        ///< Parallel INDIRECT 8-bit
} hostmcu_config;


#include "support.h"


//=================== S1D13C00 I/O Routines ==============================================

//*****************************************************************************
//
// S1D13C00 Host Interface command definitions
//
//*****************************************************************************

#define CMD_PAGEPROG                    0x02
#define CMD_DUALINFASTPROG              0xA2
#define CMD_EXTDUALINFASTPROG           0xD2
#define CMD_QUADINFASTPROG              0x32
#define CMD_EXTQUADINFASTPROG           0x12

#define CMD_FASTREAD                    0x0B
#define CMD_DUALOUTFASTREAD             0x3B
#define CMD_DUALIOFASTREAD              0xBB
#define CMD_QUADOUTFASTREAD             0x6B
#define CMD_QUADIOFASTREAD              0xEB



#ifdef __cplusplus
extern "C" {
#endif

void seS1D13C00InitializeController( hostmcu_config hostif, uint32_t sysfreq, uint32_t spi_writespeed, uint32_t spi_readspeed );
void seS1D13C00SoftReset( void );
void seS1D13C00InitDispEn( void );
void seS1D13C00DispEnable( void );
void seS1D13C00DispDisable( void );
void seS1D13C00Write( uint32_t addr, uint8_t data[], uint32_t nBytes );
void seS1D13C00Write8( uint32_t addr, uint8_t data );
void seS1D13C00Write16( uint32_t addr, uint16_t data );
void seS1D13C00Write32( uint32_t addr, uint32_t data );
void seS1D13C00Read( uint32_t addr, uint8_t data[], uint32_t nBytes );
uint8_t seS1D13C00Read8( uint32_t addr );
uint16_t seS1D13C00Read16( uint32_t addr );
uint32_t seS1D13C00Read32( uint32_t addr );


#ifdef __cplusplus
}
#endif


#endif // seHCL_H_INCLUDED
