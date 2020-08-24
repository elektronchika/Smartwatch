//===========================================================================
//
// s1d13c00_hcl.c - Hardware Control Layer
//
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
//===========================================================================

#include "s1d13c00_hcl.h"
#include "s1d13C00_memregs.h"
#include "se_common.h"
#include "se_clg.h"
#include "se_port.h"

static hostmcu_config hostif_type;
static uint32_t spi_wrspeed;
static uint32_t spi_rdspeed;
static uint32_t system_freq;
static uint32_t rxdata;
static volatile uint8_t *indirect8base;


//---------------------------------------------------------------------------
// PRIVATE FUNCTION: SetSSI1ClkSpeed( uint32_t clkspeed )
//---------------------------------------------------------------------------
static void SetSSI1ClkSpeed( uint32_t clkspeed )
{
    /*
    uint32_t cpsrval;

    cpsrval = (clkspeed * (((SSI1_CR0_R>>8)&0xFF)+1));
    cpsrval = system_freq / cpsrval;
    SSI1_CPSR_R = cpsrval;
    */
}


//---------------------------------------------------------------------------
// PRIVATE FUNCTION: InitializeSSI1()
//
// SSI1 pin configurations:
//   PB4 = SSIFSS# (chip-select)
//   PB5 = SSICLK
//   PE4 = SSID0
//   PE5 = SSID1
//   PD4 = SSID2
//   PD5 = SSID3
//   PC7 = SPICFG[0]
//   PC6 = SPICFG[1]
//
//---------------------------------------------------------------------------
static void InitializeSSI1( hostmcu_config hostif )
{
    /*
    // Enable SSI1 and GPIO ports used for SPI/QSPI Host Interface
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);

    // DE# output = HIGH
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);

    // SPICFG[1:0]
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, (GPIO_PIN_6 | GPIO_PIN_7));
    if (hostif == HOSTMCU_SPI_DUAL_ALL)
        GPIOPinWrite(GPIO_PORTC_BASE, (GPIO_PIN_6 | GPIO_PIN_7), GPIO_PIN_6);                // SPICFG[1:0] = 10b
    else if (hostif == HOSTMCU_SPI_QUAD_ALL)
        GPIOPinWrite(GPIO_PORTC_BASE, (GPIO_PIN_6 | GPIO_PIN_7), (GPIO_PIN_6 | GPIO_PIN_7));  // SPICFG[1:0] = 11b
    else
        GPIOPinWrite(GPIO_PORTC_BASE, (GPIO_PIN_6 | GPIO_PIN_7), 0);                         // SPICFG[1:0] = 00b


    // SSI signals
    ROM_GPIOPinConfigure(GPIO_PB5_SSI1CLK);
    ROM_GPIOPinConfigure(GPIO_PE4_SSI1XDAT0);
    ROM_GPIOPinConfigure(GPIO_PE5_SSI1XDAT1);
    ROM_GPIOPinConfigure(GPIO_PD4_SSI1XDAT2);
    ROM_GPIOPinConfigure(GPIO_PD5_SSI1XDAT3);
    ROM_GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_5);
    ROM_GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    ROM_GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // Terminate floating data bus pins with pull-downs
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD0
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD1
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD2
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD3

    // Drive unused Host Interface pins
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);                                   // HIFD7
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);                                   // HIFD6
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);                                   // #HIFRD
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTP_BASE, GPIO_PIN_3);                                   // #HIFCS
    GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_3, GPIO_PIN_3);

    // SSI configuration
    ROM_SSIConfigSetExpClk(SSI1_BASE, system_freq, SSI_FRF_MOTO_MODE_0,
                                                        SSI_MODE_MASTER, spi_wrspeed, 8);
    ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
    ROM_SSIEnable(SSI1_BASE);
    */
}


//---------------------------------------------------------------------------
// PRIVATE FUNCTION: QSPI_TxBytes()
//---------------------------------------------------------------------------
static void QSPI_TxBytes( uint8_t data[], uint32_t size ) {

  /*
  int32_t i;

  for ( i = 0; i < size ; i++ )
  {
      ROM_SSIDataPut(SSI1_BASE, data[i]);
  }
  */

}

//---------------------------------------------------------------------------
// PRIVATE FUNCTION: QSPI_RxBytes()
//---------------------------------------------------------------------------
static void QSPI_RxBytes( uint8_t data[], uint32_t size ) {

  /*
  int32_t i;

  // Dummy byte read
  SSI1_DR_R = 0x11;  // Dummy write
  ROM_SSIDataGet(SSI1_BASE, &rxdata);

  for ( i = 0; i < size ; i++ )
  {
      SSI1_DR_R = 0x11;  // Dummy write
      ROM_SSIDataGet(SSI1_BASE, &rxdata);
      data[i] = (uint8_t) (rxdata & 0xFF);
  }
  */

}

//---------------------------------------------------------------------------
// PRIVATE FUNCTION: InitializeINDIRECT8()
//---------------------------------------------------------------------------
static void InitializeINDIRECT8(void)
{
    /*
    // Enable EPI0 and GPIO ports used for INDIRECT8 Host Interface
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_EPI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);

    // DE# output = HIGH
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);

    // EPI0 signals
    ROM_GPIOPinConfigure(GPIO_PK0_EPI0S0);      // D0
    ROM_GPIOPinConfigure(GPIO_PK1_EPI0S1);      // D1
    ROM_GPIOPinConfigure(GPIO_PK2_EPI0S2);      // D2
    ROM_GPIOPinConfigure(GPIO_PK3_EPI0S3);      // D3
    ROM_GPIOPinConfigure(GPIO_PC7_EPI0S4);      // D4
    ROM_GPIOPinConfigure(GPIO_PC6_EPI0S5);      // D5
    ROM_GPIOPinConfigure(GPIO_PC5_EPI0S6);      // D6
    ROM_GPIOPinConfigure(GPIO_PC4_EPI0S7);      // D7
    ROM_GPIOPinConfigure(GPIO_PB3_EPI0S28);     // RDn
    ROM_GPIOPinConfigure(GPIO_PP2_EPI0S29);     // WRn
    ROM_GPIOPinConfigure(GPIO_PP3_EPI0S30);     // CSn
    GPIOPinTypeEPI(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinTypeEPI(GPIO_PORTC_BASE, (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7));
    GPIOPinTypeEPI(GPIO_PORTK_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3));
    GPIOPinTypeEPI(GPIO_PORTP_BASE, (GPIO_PIN_2 | GPIO_PIN_3));

    // Terminate floating data bus pins with pull-downs
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD0
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD1
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD2
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD3
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD4
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD5
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD6
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);  // HIFD7

    // EPI configuration
    ROM_EPIModeSet(EPI0_BASE, EPI_MODE_HB8);
    ROM_EPIDividerSet(EPI0_BASE, 5);
    ROM_EPIConfigHB8Set(EPI0_BASE, (EPI_HB8_MODE_ADDEMUX | EPI_HB8_WRWAIT_3 | EPI_HB8_RDWAIT_3 | EPI_HB8_CSCFG_CS), 0);
    ROM_EPIAddressMapSet(EPI0_BASE, (EPI_ADDR_PER_SIZE_256B | EPI_ADDR_PER_BASE_A));

    indirect8base = (uint8_t *)0xA0000000;  // Base address of EPI bus
    */
}

#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define SPI_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

#define TEST_STRING "Nordic"
static uint8_t       m_tx_buf[] = {0};// = TEST_STRING;           /**< TX buffer. */
static uint8_t       m_rx_buf[] = {0};//[sizeof(TEST_STRING) + 1];    /**< RX buffer. */
static uint8_t m_length = 0;//sizeof(m_tx_buf);        /**< Transfer length. */

/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
    spi_xfer_done = true;
    NRF_LOG_INFO("Transfer completed.");
    if (m_rx_buf[0] != 0)
    {
        NRF_LOG_INFO(" Received:");
        NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf));
    }
}

void initSPI(void)
{
    
    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin    = SPI_SS_PIN;
    spi_config.miso_pin  = SPI_MISO_PIN;
    spi_config.mosi_pin  = SPI_MOSI_PIN;
    spi_config.sck_pin   = SPI_SCK_PIN;
    spi_config.frequency = NRF_DRV_SPI_FREQ_500K;
    spi_config.mode      = NRF_DRV_SPI_MODE_0;
    spi_config.bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;

    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));

}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00InitializeController()
//---------------------------------------------------------------------------
void seS1D13C00InitializeController( hostmcu_config hostif, uint32_t sysfreq, uint32_t spi_writespeed, uint32_t spi_readspeed )
{
    hostif_type = hostif;
    spi_wrspeed = spi_writespeed;
    spi_rdspeed = spi_readspeed;
    system_freq = sysfreq;

    switch(hostif)
    {
        case(HOSTMCU_SPI_MONOADDR_MONODATA):
            initSPI();
        case(HOSTMCU_SPI_MONOADDR_DUALDATA):
        case(HOSTMCU_SPI_DUALADDR_DUALDATA):
        case(HOSTMCU_SPI_MONOADDR_QUADDATA):
        case(HOSTMCU_SPI_QUADADDR_QUADDATA):
        case(HOSTMCU_SPI_DUAL_ALL):
        case(HOSTMCU_SPI_QUAD_ALL):
            InitializeSSI1(hostif);
            break;

        case(HOSTMCU_INDIRECT_8BIT):
            InitializeINDIRECT8();
            break;

        default:
            return;
    }

   seS1D13C00SoftReset();
}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00SoftReset()
//---------------------------------------------------------------------------
void seS1D13C00SoftReset( void )
{
    seS1D13C00Write16(SYS_CTRL, 0x8000);  // Soft reset

    // >20us delay before doing anything else
    seSysSleepMS(1);
}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00InitDispEn()
//   Initialize Display Enable control output for SPI panels.
//---------------------------------------------------------------------------
void seS1D13C00InitDispEn( void )
{
    sePORT_InitAsOutput(sePORT_P07);
    sePORT_SetOutput(sePORT_P07, sePORT_DATA_HIGH);
}




//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00DispEnable()
//   Set Display Enable HIGH for SPI panels.
//---------------------------------------------------------------------------
void seS1D13C00DispEnable( void )
{
    sePORT_SetOutput(sePORT_P07, sePORT_DATA_HIGH);
}




//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00DispDisable()
//   Set Display Enable LOW for SPI panels.
//---------------------------------------------------------------------------
void seS1D13C00DispDisable( void )
{
    sePORT_SetOutput(sePORT_P07, sePORT_DATA_LOW);
}




//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00Write()
//---------------------------------------------------------------------------
void seS1D13C00Write( uint32_t addr, uint8_t data[], uint32_t nBytes )
{

    uint32_t k, i;
    uint8_t write[5];

    if (nBytes == 0)
       return;

    // change the spi speed later
    //if (hostif_type != HOSTMCU_INDIRECT_8BIT)
    //   SetSSI1ClkSpeed(spi_wrspeed);
    
    write[1] = (uint8_t)(addr >> 24);
    write[2] = (uint8_t)(addr >> 16);
    write[3] = (uint8_t)(addr >>  8);
    write[4] = (uint8_t)(addr >>  0);

    // case HOSTMCU_SPI_MONOADDR_MONODATA
    write[0] = CMD_PAGEPROG;

    // beginning of sending the data over spi
    m_length = nBytes + 5;

    // Reset rx buffer and transfer done flag
    memset(m_rx_buf, 0, m_length);
    spi_xfer_done = false;

    //memset(m_tx_buf, (int)write, 5);
    //for(i = 0; i < 5; i++)
    //{
    //    m_tx_buf[i] = write[i];
    //}
    m_tx_buf[0] = write[0];
    m_tx_buf[1] = write[1];
    m_tx_buf[2] = write[2];
    m_tx_buf[3] = write[3];
    m_tx_buf[4] = write[4];
    //memset(m_tx_buf+5, (int)data, nBytes);
    for(i = 0; i < nBytes; i++)
    {
        m_tx_buf[i + 5] = data[i];
    }

    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, m_tx_buf, m_length, m_rx_buf, m_length));
    //nrf_drv_spi_transfer(&spi, m_tx_buf, m_length, m_rx_buf, m_length);

    while (!spi_xfer_done)
    {
        __WFE();
    }

    /*
    uint32_t k;
    uint8_t write[5];

    if (nBytes == 0)
       return;

    if (hostif_type != HOSTMCU_INDIRECT_8BIT)
       SetSSI1ClkSpeed(spi_wrspeed);

    write[1] = (uint8_t)(addr >> 24);
    write[2] = (uint8_t)(addr >> 16);
    write[3] = (uint8_t)(addr >>  8);
    write[4] = (uint8_t)(addr >>  0);

    // Host access to the S1D13C00 must be completely contained.
    // If another device were to interrupt and modify any of the
    // interface hardware in use then future communications could
    // be affected, thus the interrupt master disable/enable.
    // NOTE: there may be other control mechanisms to ensure singular
    //       access (eg. a flag or semaphore) whose implementation is
    //       dependent on the environment.
    ROM_IntMasterDisable();

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);  // Assert DE#

    if (hostif_type != HOSTMCU_INDIRECT_8BIT)
    {
       switch(hostif_type)
       {
          case HOSTMCU_SPI_MONOADDR_MONODATA:
             write[0] = CMD_PAGEPROG;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             QSPI_TxBytes( data, nBytes );
             break;

          case HOSTMCU_SPI_MONOADDR_DUALDATA:
             write[0] = CMD_DUALINFASTPROG;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_BI_WRITE);
             QSPI_TxBytes( data, nBytes );
             break;

          case HOSTMCU_SPI_DUALADDR_DUALDATA:
             write[0] = CMD_EXTDUALINFASTPROG;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 1 );
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_BI_WRITE);
             QSPI_TxBytes( &(write[1]), 4 );
             QSPI_TxBytes( data, nBytes );
             break;

          case HOSTMCU_SPI_MONOADDR_QUADDATA:
             write[0] = CMD_QUADINFASTPROG;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_QUAD_WRITE);
             QSPI_TxBytes( data, nBytes );
             break;

          case HOSTMCU_SPI_QUADADDR_QUADDATA:
             write[0] = CMD_EXTQUADINFASTPROG;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 1 );
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_QUAD_WRITE);
             QSPI_TxBytes( &(write[1]), 4 );
             QSPI_TxBytes( data, nBytes );
             break;

          case HOSTMCU_SPI_DUAL_ALL:
             write[0] = CMD_EXTDUALINFASTPROG;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_BI_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             QSPI_TxBytes( data, nBytes );
             break;

          case HOSTMCU_SPI_QUAD_ALL:
             write[0] = CMD_EXTQUADINFASTPROG;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_QUAD_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             QSPI_TxBytes( data, nBytes );
             break;

       }

    }
    else
    {
        write[0] = CMD_PAGEPROG;
        for (k = 0; k < 5; k++)
        {
            *indirect8base = write[k];
            while((EPI0_STAT_R & EPI_STAT_WBUSY) != 0);
        }
        for (k = 0; k < nBytes; k++)
        {
            *indirect8base = data[k];
            while((EPI0_STAT_R & EPI_STAT_WBUSY) != 0);
        }
    }

    if (hostif_type != HOSTMCU_INDIRECT_8BIT)
       while(ROM_SSIBusy(SSI1_BASE));

    for (k = 0; k < 4; k++)
       GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);  // Assert DE#

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);  // De-assert DE#

    ROM_IntMasterEnable();
    */
}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00Write8()
//---------------------------------------------------------------------------
void seS1D13C00Write8( uint32_t addr, uint8_t data )
{
    seS1D13C00Write(addr, (uint8_t *)&data, 1);
}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00Write16()
//---------------------------------------------------------------------------
void seS1D13C00Write16( uint32_t addr, uint16_t data )
{
    seS1D13C00Write(addr, (uint8_t *)&data, 2);
}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00Write32()
//---------------------------------------------------------------------------
void seS1D13C00Write32( uint32_t addr, uint32_t data )
{
    seS1D13C00Write(addr, (uint8_t*)&data, 4);
}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00Read()
//---------------------------------------------------------------------------
void seS1D13C00Read( uint32_t addr, uint8_t data[], uint32_t nBytes )
{

    uint8_t write[5];
    uint32_t k, i;

    // change the spi speed later
    //if (hostif_type != HOSTMCU_INDIRECT_8BIT)
    //   SetSSI1ClkSpeed(spi_rdspeed);

    write[1] = (uint8_t)(addr >> 24);
    write[2] = (uint8_t)(addr >> 16);
    write[3] = (uint8_t)(addr >>  8);
    write[4] = (uint8_t)(addr >>  0);

    // case HOSTMCU_SPI_MONOADDR_MONODATA:
    write[0] = CMD_FASTREAD;

    // beginning of receiving the data over spi
    m_length = nBytes + 5;

    // Reset rx buffer and transfer done flag
    memset(m_rx_buf, 0, m_length);
    spi_xfer_done = false;

    //memset(m_tx_buf, (int)write, 5);
    m_tx_buf[0] = write[0];
    m_tx_buf[1] = write[1];
    m_tx_buf[2] = write[2];
    m_tx_buf[3] = write[3];
    m_tx_buf[4] = write[4];

    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, m_tx_buf, m_length, m_rx_buf, m_length));
    //nrf_drv_spi_transfer(&spi, m_tx_buf, m_length, m_rx_buf, m_length);

    while (!spi_xfer_done)
    {
        __WFE();
    }

    for(i = 0; i < nBytes; i++)
    {
        data[i] = m_rx_buf[i + 5];
    }

    /*
    uint8_t write[5];
    uint32_t k;

    if (hostif_type != HOSTMCU_INDIRECT_8BIT)
       SetSSI1ClkSpeed(spi_rdspeed);

    write[1] = (uint8_t)(addr >> 24);
    write[2] = (uint8_t)(addr >> 16);
    write[3] = (uint8_t)(addr >>  8);
    write[4] = (uint8_t)(addr >>  0);

    // Host access to the S1D13C00 must be completely contained.
    // If another device were to interrupt and modify any of the
    // interface hardware in use then future communications could
    // be affected, thus the interrupt master disable/enable.
    // NOTE: there may be other control mechanisms to ensure singular
    //       access (eg. a flag or semaphore) whose implementation is
    //       dependent on the environment.
    ROM_IntMasterDisable();

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);  // Assert DE#

    if (hostif_type != HOSTMCU_INDIRECT_8BIT)
    {
       switch(hostif_type)
       {
          case HOSTMCU_SPI_MONOADDR_MONODATA:
             write[0] = CMD_FASTREAD;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             while(ROM_SSIBusy(SSI1_BASE));  // Wait for cmd and address to finish
             while(ROM_SSIDataGetNonBlocking(SSI1_BASE, &rxdata));  // Clear out RX bytes
             QSPI_RxBytes( data, nBytes );    // Includes dummy byte already

             break;

          case HOSTMCU_SPI_MONOADDR_DUALDATA:
             write[0] = CMD_DUALOUTFASTREAD;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             while(ROM_SSIBusy(SSI1_BASE));  // Wait for cmd and address to finish
             while(ROM_SSIDataGetNonBlocking(SSI1_BASE, &rxdata));  // Clear out RX bytes
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_BI_READ);
             QSPI_RxBytes( data, nBytes );    // Includes dummy byte already
             break;

          case HOSTMCU_SPI_DUALADDR_DUALDATA:
             write[0] = CMD_DUALIOFASTREAD;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 1 );
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_BI_WRITE);
             QSPI_TxBytes( &(write[1]), 4 );
             while(ROM_SSIBusy(SSI1_BASE));  // Wait for cmd and address to finish
             while(ROM_SSIDataGetNonBlocking(SSI1_BASE, &rxdata));  // Clear out RX bytes
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_BI_READ);
             QSPI_RxBytes( data, nBytes );    // Includes dummy byte already
             break;

          case HOSTMCU_SPI_MONOADDR_QUADDATA:
             write[0] = CMD_QUADOUTFASTREAD;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             while(ROM_SSIBusy(SSI1_BASE));  // Wait for cmd and address to finish
             while(ROM_SSIDataGetNonBlocking(SSI1_BASE, &rxdata));  // Clear out RX bytes
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_QUAD_READ);
             QSPI_RxBytes( data, nBytes );    // Includes dummy byte already
             break;

          case HOSTMCU_SPI_QUADADDR_QUADDATA:
             write[0] = CMD_QUADIOFASTREAD;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_READ_WRITE);
             QSPI_TxBytes( &(write[0]), 1 );
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_QUAD_WRITE);
             QSPI_TxBytes( &(write[1]), 4 );
             while(ROM_SSIBusy(SSI1_BASE));  // Wait for cmd and address to finish
             while(ROM_SSIDataGetNonBlocking(SSI1_BASE, &rxdata));  // Clear out RX bytes
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_QUAD_READ);
             QSPI_RxBytes( data, nBytes );    // Includes dummy byte already
             break;

          case HOSTMCU_SPI_DUAL_ALL:
             write[0] = CMD_DUALIOFASTREAD;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_BI_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             while(ROM_SSIBusy(SSI1_BASE));  // Wait for cmd and address to finish
             while(ROM_SSIDataGetNonBlocking(SSI1_BASE, &rxdata));  // Clear out RX bytes
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_BI_READ);
             QSPI_RxBytes( data, nBytes );    // Includes dummy byte already
             break;

          case HOSTMCU_SPI_QUAD_ALL:
             write[0] = CMD_QUADIOFASTREAD;
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_QUAD_WRITE);
             QSPI_TxBytes( &(write[0]), 5 );
             while(ROM_SSIBusy(SSI1_BASE));  // Wait for cmd and address to finish
             while(ROM_SSIDataGetNonBlocking(SSI1_BASE, &rxdata));  // Clear out RX bytes
             ROM_SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_QUAD_READ);
             QSPI_RxBytes( data, nBytes );    // Includes dummy byte already
             break;

       }
    }
    else
    {
        write[0] = CMD_FASTREAD;
        for (k = 0; k < 5; k++)
        {
            *indirect8base = write[k];
            while((EPI0_STAT_R & EPI_STAT_WBUSY) != 0);
        }

        data[0] = *indirect8base;  // Dummy byte read
        for (k = 0; k < nBytes; k++)
        {
            data[k] = *indirect8base;
            while((EPI0_STAT_R & EPI_STAT_NBRBUSY) != 0);
        }
    }

    if (hostif_type != HOSTMCU_INDIRECT_8BIT)
       while(ROM_SSIBusy(SSI1_BASE));

    for (k = 0; k < 4; k++)
       GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);  // Assert DE#

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);  // De-assert DE#

    ROM_IntMasterEnable();
    */
}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00Read8()
//---------------------------------------------------------------------------
uint8_t seS1D13C00Read8( uint32_t addr )
{
    uint8_t data;
    seS1D13C00Read(addr, (uint8_t *)&data, 1);
    return(data);
}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00Read16()
//---------------------------------------------------------------------------
uint16_t seS1D13C00Read16( uint32_t addr )
{
    uint16_t data;
    seS1D13C00Read(addr, (uint8_t *)&data, 2);
    return(data);
}


//---------------------------------------------------------------------------
// PUBLIC FUNCTION: seS1D13C00Read32()
//---------------------------------------------------------------------------
uint32_t seS1D13C00Read32( uint32_t addr )
{
    uint32_t data;
    seS1D13C00Read(addr, (uint8_t *)&data, 4);
    return(data);
}


