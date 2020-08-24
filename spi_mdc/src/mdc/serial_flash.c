/**
  ******************************************************************************
  * @file    serial_flash.c
  * @author  Epson
  * @version V1.0
  * @date    11-April-2018
  * @brief   This file provides routines for accessing the onboard serial flash
  *          on the S5U13C00P00Cx00 board.
  *          For read, Single, Dual I/O (address and data are dual),
  *               and Quad I/O (address and data are quad) modes supported.
  *               Other read modes are not supported.
  *          For write, only single mode is supported (Page Program).
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2018. All rights reserved.
  ******************************************************************************
  */


#include "s1d13c00_hcl.h"
#include "s1d13c00_memregs.h"
#include "se_common.h"
#include "se_qspi.h"
#include "serial_flash.h"


static seMDC_SERFLASH_DATAMODE flashmode = FLMODE_SINGLE;
static uint16_t xipword = 0;
static uint8_t manuf_id = 0;
static uint16_t device_id = 0;

#define PAGE_PROGRAM_SIZE 256

seStatus ReadFlashID( uint8_t * mfc_id, uint16_t * dev_id ) {
  
  seStatus fStatus = seSTATUS_NG;
  uint8_t data[4] = {0,0,0,0};
  uint8_t cmd = CMD_READ_ID_SINGLE_MODE;
    
  seQSPI_SetMode( seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK );
  seQSPI_SetIO( seQSPI_Output );
  seQSPI_ASSERT_MST_CS0();  
  if ( seSTATUS_OK == seQSPI_TxBytes( &cmd, 1 ) ) {
    seQSPI_SetIO( seQSPI_Input );
    fStatus = seQSPI_RxBytes( data, 3 );
  }
  seQSPI_NEGATE_MST_CS0();
 
  *mfc_id = data[0];
  *dev_id = (uint16_t)data[1]<<8 | data[2]<<0;

  return fStatus;
}


seStatus IdentifyFlash( uint8_t * mfc_id, uint16_t * dev_id ) {

  seStatus fStatus = seSTATUS_NG;
  uint8_t statusreg;

  if ( (fStatus = ReadFlashID(mfc_id, dev_id)) == seSTATUS_OK)
  {
      manuf_id = *mfc_id;
      device_id = *dev_id;

      if ((*mfc_id == S25FL127S_MFGID) && (*dev_id == S25FL127S_DEVID)) // Spansion S25FL127S
      {
          flashmode = FLMODE_SINGLE;
          xipword =  0x05FF;  // XIP entry and exit codes, bit-reversed
          seS1D13C00Write16(QSPI_MB, xipword);
          seS1D13C00Write16( QSPI_MMACFG2, 0x15A8);
                          // MMAEN = 0;
                          // ADRCYC = 0;
                          // ADRTMOD = 2;  // quad
                          // DATTMOD = 2;  // quad
                          // DUMTMOD = 2;  // quad
                          // DUMLN = 5;    // 6 clocks
                          // DUMDL = 1;    // 2 clocks

          // Enable Quad mode
          fStatus = WriteFlashCfgReg( 0x02, 0x00 );  // Latency code = 00b, QUAD = 1
      }
      else if ((*mfc_id == IS25LP128_MFGID) && (*dev_id == IS25LP128_DEVID))  // ISSI IS25LP128
      {
          flashmode = FLMODE_SINGLE;
          xipword =  0x05FF;  // XIP entry and exit codes, bit-reversed
          seS1D13C00Write16(QSPI_MB, xipword);
          seS1D13C00Write16( QSPI_MMACFG2, 0x15A8);
                          // MMAEN = 0;
                          // ADRCYC = 0;
                          // ADRTMOD = 2;  // quad
                          // DATTMOD = 2;  // quad
                          // DUMTMOD = 2;  // quad
                          // DUMLN = 5;    // 6 clocks
                          // DUMDL = 1;    // 2 clocks

          // Exit full QUAD mode if necessary, send 0xF5 in quad data mode
          statusreg = CMD_QIODI;
          seQSPI_SetMode(seQSPI_MODE_QUAD, seQSPI_02CLK, seQSPI_02CLK);
          seQSPI_SetIO( seQSPI_Output );
          seQSPI_ASSERT_MST_CS0();
          seQSPI_TxBytes( &statusreg, 1 );
          seQSPI_NEGATE_MST_CS0();

          seQSPI_SetMode(seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK);

          // Enable Quad mode
          if ((fStatus = ReadFlashStatusReg( &statusreg )) == seSTATUS_OK)
          {
              statusreg |= 0x40;  // QE = 1
              fStatus = WriteFlashStatusReg( statusreg );
          }
      }
      else if ((*mfc_id == MX25R6435F_MFGID) && (*dev_id == MX25R6435F_DEVID))  // Macronix MX25R6435F
      {
          flashmode = FLMODE_SINGLE;
          xipword =  0xA5FF;  // XIP entry and exit codes
          seS1D13C00Write16(QSPI_MB, xipword);
          seS1D13C00Write16( QSPI_MMACFG2, 0x15A8);
                          // MMAEN = 0;
                          // ADRCYC = 0;
                          // ADRTMOD = 2;  // quad
                          // DATTMOD = 2;  // quad
                          // DUMTMOD = 2;  // quad
                          // DUMLN = 5;    // 6 clocks
                          // DUMDL = 1;    // 2 clocks

          // Enable Quad mode
          if ((fStatus = ReadFlashStatusReg( &statusreg )) == seSTATUS_OK)
          {
              statusreg |= 0x40;  // QE = 1
              fStatus = WriteFlashStatusReg( statusreg );
          }
      }
  }

  return fStatus;

}


seStatus SetFlashMode( seMDC_SERFLASH_DATAMODE mode ) {
  flashmode = mode;
  
  return seSTATUS_OK;
}

seMDC_SERFLASH_DATAMODE GetFlashMode( void ) {
  return (flashmode);
}

seStatus EnableFlashWrite( void ) {
  
  seStatus fStatus = seSTATUS_NG;
  uint8_t cmd = CMD_WRITE_ENABLE;
  
  seQSPI_SetMode( seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK );
  seQSPI_SetIO( seQSPI_Output );
  seQSPI_ASSERT_MST_CS0(); 
  if ( seSTATUS_OK == seQSPI_TxBytes( &cmd, 1 ) ) {
    fStatus = seSTATUS_OK;      
  }
  seQSPI_NEGATE_MST_CS0();   

  fStatus = WaitFlashBusy();

  return fStatus;
}

seStatus EraseFlash( void ) {
  
  seStatus fStatus = seSTATUS_NG;
  uint8_t cmd = CMD_CHIP_ERASE;
  
  if ( seSTATUS_OK == EnableFlashWrite() ) {
    seQSPI_SetIO( seQSPI_Output );
    seQSPI_ASSERT_MST_CS0(); 
    if ( seSTATUS_OK == seQSPI_TxBytes( &cmd, 1 ) ) {
      fStatus = seSTATUS_OK;        
    } 
    seQSPI_NEGATE_MST_CS0();   
  } 
    
  if ( fStatus == seSTATUS_OK ) {
    fStatus = WaitFlashBusy();
  }
  
  return fStatus;
}

seStatus EraseFlashSector( uint32_t addr ) {
  
  seStatus fStatus = seSTATUS_NG;
  
  if ( seSTATUS_OK == EnableFlashWrite() ) {
    
    uint8_t write[4];
  
    seQSPI_SetIO( seQSPI_Output );
    write[0] = CMD_BLOCK_ERASE;
    write[1] = (uint8_t)(addr >> 16);
    write[2] = (uint8_t)(addr >>  8);
    write[3] = (uint8_t)(addr >>  0);  
  
    seQSPI_ASSERT_MST_CS0(); 
    if ( seSTATUS_OK == seQSPI_TxBytes( write, 4 ) ) {
      fStatus = seSTATUS_OK;        
    } 
    seQSPI_NEGATE_MST_CS0();   
  } 
    
  if ( fStatus == seSTATUS_OK ) {
     fStatus = WaitFlashBusy();
  }
  
  return fStatus;
}

seStatus ProgramFlash( uint32_t flash_addr, uint8_t data[], uint32_t nBytes ) {
  
  seStatus fStatus = seSTATUS_NG;
  
  while ( nBytes >=1 &&  (fStatus = EnableFlashWrite())== seSTATUS_OK ) {
    uint8_t write[4];
    uint32_t nFlashCount;
    
    nFlashCount = ((nBytes > PAGE_PROGRAM_SIZE) ? PAGE_PROGRAM_SIZE : nBytes);

    if (flashmode == FLMODE_QUAD)
    {
        if ((manuf_id == MX25R6435F_MFGID) && (device_id == MX25R6435F_DEVID))  // Macronix MX25R6435F
            write[0] = CMD_QUAD_PROG_IO;
        else
            write[0] = CMD_QUAD_PROG;
    }
    else
    {
        write[0] = CMD_PAGE_PROG;
    }

    write[1] = (uint8_t)(flash_addr >> 16);
    write[2] = (uint8_t)(flash_addr >>  8);
    write[3] = (uint8_t)(flash_addr >>  0);
    
    seQSPI_SetIO( seQSPI_Output );
    seQSPI_ASSERT_MST_CS0();
    
    if ((flashmode == FLMODE_QUAD) && (manuf_id == MX25R6435F_MFGID) && (device_id == MX25R6435F_DEVID))  // Macronix MX25R6435F QUAD mode
    {	    
        //  Send command single mode
        fStatus = seQSPI_TxBytes( write, 1 );
        seQSPI_SetMode(seQSPI_MODE_QUAD, seQSPI_02CLK, seQSPI_02CLK);

        //  Send address in quad mode
        fStatus = seQSPI_TxBytes( write+1, 3 );
    }
    else
    {	    
        //  Send command and address in single mode
        fStatus = seQSPI_TxBytes( write, 4 );

        // Switch to quad data if necessary	
        if (flashmode == FLMODE_QUAD)
            seQSPI_SetMode(seQSPI_MODE_QUAD, seQSPI_02CLK, seQSPI_02CLK);
    }

    // Send data
    if ( fStatus == seSTATUS_OK ) {
        fStatus = seQSPI_TxBytes( data, nFlashCount );
    }        
    seQSPI_NEGATE_MST_CS0();  
    
    if ( fStatus == seSTATUS_OK ) {
      fStatus = WaitFlashBusy();
      nBytes -= nFlashCount;
      data += nFlashCount;
      flash_addr += nFlashCount;
    } else  
      break;
  } 
  
  return fStatus;
}


seStatus ReadFlash( uint32_t flash_addr, uint8_t data[], uint32_t nBytes )  {
  
  seStatus fStatus;
  uint8_t wr_data[4];

  if (flashmode == FLMODE_QUAD)
      wr_data[0] = CMD_READ_SINGLE_IO;
  else if (flashmode == FLMODE_DUAL)
      wr_data[0] = CMD_READ_DUAL_IO;
  else
      wr_data[0] = CMD_READ_QUAD_IO;

  wr_data[1] = (uint8_t)(flash_addr >> 16);
  wr_data[2] = (uint8_t)(flash_addr >>  8);
  wr_data[3] = (uint8_t)(flash_addr >>  0);  

  seQSPI_SetMode(seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK);

  seQSPI_SetIO( seQSPI_Output );
  seQSPI_ASSERT_MST_CS0();    
  
  //  Send command
  fStatus = seQSPI_TxBytes( wr_data, 1 );

  if (flashmode == FLMODE_QUAD)
      seQSPI_SetMode(seQSPI_MODE_QUAD, seQSPI_02CLK, seQSPI_02CLK);
  else if (flashmode == FLMODE_DUAL)
      seQSPI_SetMode(seQSPI_MODE_DUAL, seQSPI_04CLK, seQSPI_04CLK);

  //  Send address
  fStatus = seQSPI_TxBytes( &(wr_data[1]), 3 );

  //  Read data
  if ( fStatus == seSTATUS_OK ) {    
      seQSPI_SetIO( seQSPI_Input );

      //  skip dummy cycles
      switch (flashmode) {
          case FLMODE_SINGLE:
            fStatus = seQSPI_RxBytes( data, 1);
            break;
	  case FLMODE_DUAL:
            fStatus = seQSPI_RxBytes( data, 1 );
            break;
          case FLMODE_QUAD:
            fStatus = seQSPI_RxBytes( data, 3 );
            break;
          default:
            fStatus = seSTATUS_NG;
            break;
       }

       if ( fStatus == seSTATUS_OK ) {
         fStatus = seQSPI_RxBytes( data, nBytes );
       }
  }
  
  seQSPI_NEGATE_MST_CS0();

  return fStatus;  
}


seStatus WaitFlashBusy( void ) {
  
  seStatus fStatus = seSTATUS_NG;
  uint8_t cmd = CMD_READ_STATUS_REG;
     
  seQSPI_SetMode( seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK );
  seQSPI_SetIO( seQSPI_Output );
  seQSPI_ASSERT_MST_CS0();      
  if ( seSTATUS_OK == seQSPI_TxBytes( &cmd, 1 ) ) {
    uint8_t busy;
    seQSPI_SetIO( seQSPI_Input );
    do {
      if ( seSTATUS_OK == seQSPI_RxBytes( &busy, 1 ) ) {
        busy &= FLASH_WRITE_IN_PROGRESS; 
        fStatus = seSTATUS_OK;
      } else {
        fStatus = seSTATUS_NG;
        break;
      }
    } while( busy );    
  }  
  seQSPI_NEGATE_MST_CS0(); 
  
  return fStatus;
}


seStatus ReadFlashStatusReg( uint8_t * val ) {

  seStatus fStatus = seSTATUS_NG;
  uint8_t cmd = CMD_READ_STATUS_REG;

  seQSPI_SetMode( seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK );
  seQSPI_SetIO( seQSPI_Output );
  seQSPI_ASSERT_MST_CS0();
  if ( seSTATUS_OK == seQSPI_TxBytes( &cmd, 1 ) ) {
    seQSPI_SetIO( seQSPI_Input );
    if ( seSTATUS_OK == seQSPI_RxBytes( val, 1 ) ) {
      fStatus = seSTATUS_OK;
    }
  }
  seQSPI_NEGATE_MST_CS0();

  return fStatus;
}


seStatus WriteFlashStatusReg( uint8_t val ) {
  
  seStatus fStatus = EnableFlashWrite();
  
  if ( fStatus == seSTATUS_OK ) {
    fStatus = WaitFlashBusy();
  }  
  
  if ( fStatus == seSTATUS_OK ) {
    uint8_t data[2];
    
    data[0] = CMD_WRITE_STATUS_REG;
    data[1] = val;
    
    seQSPI_SetIO( seQSPI_Output );
    seQSPI_ASSERT_MST_CS0();      
    if ( seSTATUS_OK == seQSPI_TxBytes( data, 2 ) ) {
      fStatus = seSTATUS_OK;   
    }
    seQSPI_NEGATE_MST_CS0(); 
  }
  
  fStatus = WaitFlashBusy();

  return fStatus;
}


seStatus ReadFlashCfgReg( uint8_t * cfg1, uint8_t * cfg2 ) {

  seStatus fStatus = seSTATUS_NG;
  uint8_t cmd, cfgdata[2];

  if ((manuf_id == S25FL127S_MFGID) && (device_id == S25FL127S_DEVID)) // Spansion S25FL127S
      cmd = CMD_READ_CONFIG_REG_S25FL127S;
  else if ((manuf_id == MX25R6435F_MFGID) && (device_id == MX25R6435F_DEVID))  // Macronix MX25R6435F
      cmd = CMD_READ_CONFIG_REG_MX25R6435F;
  else
  {
      *cfg1 = 0;
      *cfg2 = 0;
      return(seSTATUS_OK);
  }

  seQSPI_SetMode( seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK );
  seQSPI_SetIO( seQSPI_Output );
  seQSPI_ASSERT_MST_CS0();
  if ( seSTATUS_OK == seQSPI_TxBytes( &cmd, 1 ) ) {
    seQSPI_SetIO( seQSPI_Input );
    if ( seSTATUS_OK == seQSPI_RxBytes( cfgdata, 2 ) ) {
      fStatus = seSTATUS_OK;
    }
  }
  seQSPI_NEGATE_MST_CS0();

  *cfg1 = cfgdata[0];
  *cfg2 = cfgdata[1];

  return fStatus;
}


seStatus WriteFlashCfgReg( uint8_t cfg1, uint8_t cfg2 ) {

  if ((manuf_id == IS25LP128_MFGID) && (device_id == IS25LP128_DEVID))  // ISSI IS25LP128, not supported
      return(seSTATUS_NG);

  uint32_t numbytes;
  seStatus fStatus = seSTATUS_NG;
  uint8_t statusreg;

  if ((fStatus = ReadFlashStatusReg( &statusreg )) == seSTATUS_OK) {
    fStatus = EnableFlashWrite();
  }

  if ( fStatus == seSTATUS_OK ) {
    fStatus = WaitFlashBusy();
  }

  if ((manuf_id == S25FL127S_MFGID) && (device_id == S25FL127S_DEVID)) // Spansion S25FL127S
    numbytes = 3;	  
  else if ((manuf_id == MX25R6435F_MFGID) && (device_id == MX25R6435F_DEVID))  // Macronix MX25R6435F
    numbytes = 4;	  
  else
    numbytes = 3;  

  if ( fStatus == seSTATUS_OK ) {
    uint8_t data[4];

    data[0] = CMD_WRITE_STATUS_REG;
    data[1] = statusreg;
    data[2] = cfg1;
    data[3] = cfg2;

    seQSPI_SetIO( seQSPI_Output );
    seQSPI_ASSERT_MST_CS0();
    if ( seSTATUS_OK == seQSPI_TxBytes( data, numbytes ) ) {
      fStatus = seSTATUS_OK;
    }
    seQSPI_NEGATE_MST_CS0();
  }

  fStatus = WaitFlashBusy();

  return fStatus;
}


seStatus FlashEnterDeepPowerDown( void ) {
  seStatus fStatus = seSTATUS_NG;

  if (((manuf_id == IS25LP128_MFGID) && (device_id == IS25LP128_DEVID))  || // ISSI IS25LP128
      ((manuf_id == MX25R6435F_MFGID) && (device_id == MX25R6435F_DEVID)))  // Macronix MX25R6435F
  {
      uint8_t data[1];
    
      data[0] = CMD_ENTER_DEEP_PWRDOWN;
    
      seQSPI_SetMode( seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK );
      seQSPI_SetIO( seQSPI_Output );
      seQSPI_ASSERT_MST_CS0();      
      if ( seSTATUS_OK == seQSPI_TxBytes( data, 1 ) ) {
        fStatus = seSTATUS_OK;   
      }
      seQSPI_NEGATE_MST_CS0(); 
  }
  else
     return(seSTATUS_NG);  // Feature not supported

  return fStatus;
}


seStatus FlashExitDeepPowerDown( void ) {
  seStatus fStatus = seSTATUS_NG;

  if ((manuf_id == IS25LP128_MFGID) && (device_id == IS25LP128_DEVID))  // ISSI IS25LP128
  {
      uint8_t data[1];
    
      data[0] = CMD_ISSI_EXIT_DEEP_PWRDOWN;
    
      seQSPI_SetMode( seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK );
      seQSPI_SetIO( seQSPI_Output );
      seQSPI_ASSERT_MST_CS0();      
      if ( seSTATUS_OK == seQSPI_TxBytes( data, 1 ) ) {
        fStatus = seSTATUS_OK;   
      }
      seQSPI_NEGATE_MST_CS0(); 
  
  }
  else if ((manuf_id == MX25R6435F_MFGID) && (device_id == MX25R6435F_DEVID))  // Macronix MX25R6435F
  {
      seQSPI_ASSERT_MST_CS0();    // Pulse CS low
      seQSPI_NEGATE_MST_CS0(); 
  }
  else
     return(seSTATUS_NG);  // Feature not supported

  return fStatus;
}



seStatus FlashCancelXIP( void ) {

  seStatus fStatus = seSTATUS_NG;
  uint8_t dummydata[2];

  dummydata[0] = 0;
  dummydata[1] = 0;
  seQSPI_SetMode( seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK );
  seQSPI_SetIO( seQSPI_Output );
  seQSPI_ASSERT_MST_CS0();
  if ( seSTATUS_OK == seQSPI_TxBytes( dummydata, 2 ) ) {
    seQSPI_SetIO( seQSPI_Input );
    if ( seSTATUS_OK == seQSPI_RxBytes( dummydata, 2 ) ) {
      fStatus = seSTATUS_OK;
    }
  }
  seQSPI_NEGATE_MST_CS0();

  return fStatus;
}


seStatus ReadFlashDma( uint32_t flash_addr, uint16_t data[], uint32_t nWords )  {
  
  seStatus fStatus;
  uint8_t wr_data[4];
  
  if (flashmode == FLMODE_SINGLE)
      wr_data[0] = CMD_READ_SINGLE_IO;
  else if (flashmode == FLMODE_DUAL)
      wr_data[0] = CMD_READ_DUAL_IO;
  else
      wr_data[0] = CMD_READ_QUAD_IO;

  wr_data[1] = (uint8_t)(flash_addr >> 16);
  wr_data[2] = (uint8_t)(flash_addr >>  8);
  wr_data[3] = (uint8_t)(flash_addr >>  0);  

  seQSPI_SetMode(seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK);

  seQSPI_SetIO( seQSPI_Output );
  seQSPI_ASSERT_MST_CS0();    
  
  //  Send command
  fStatus = seQSPI_TxBytes( wr_data, 1 );

  if (flashmode == FLMODE_QUAD)
      seQSPI_SetMode(seQSPI_MODE_QUAD, seQSPI_02CLK, seQSPI_02CLK);
  else if (flashmode == FLMODE_DUAL)
      seQSPI_SetMode(seQSPI_MODE_DUAL, seQSPI_04CLK, seQSPI_04CLK);

  //  Send address
  fStatus = seQSPI_TxBytes( &(wr_data[1]), 3 );

  //  Read data
  if ( fStatus == seSTATUS_OK ) {    
      seQSPI_SetIO( seQSPI_Input );

      //  skip dummy cycles
      switch (flashmode) {
            case FLMODE_SINGLE:
              fStatus = seQSPI_RxBytes( (uint8_t *) data, 1);
              break;
            case FLMODE_DUAL:
              fStatus = seQSPI_RxBytes( (uint8_t *) data, 1 );
              break;
            case FLMODE_QUAD:
              fStatus = seQSPI_RxBytes( (uint8_t *) data, 3 );
              break;
            default:
              fStatus = seSTATUS_NG;
              break;
         }

        if ( fStatus == seSTATUS_OK ) {
          fStatus = seQSPI_DmaRxHWords( data, nWords, 0x20000000U );
        }
  }
  
  seQSPI_NEGATE_MST_CS0();
    
  return fStatus;  
}


seStatus ProgramFlashDma( uint32_t flash_addr, uint16_t data[], uint32_t nWords ) {

    seStatus fStatus = EnableFlashWrite();
  
    if ( fStatus == seSTATUS_OK ) {
      uint8_t write[4];
    
    write[0] = CMD_PAGE_PROG;
    write[1] = (uint8_t)(flash_addr >> 16);
    write[2] = (uint8_t)(flash_addr >>  8);
    write[3] = (uint8_t)(flash_addr >>  0);
    
    seQSPI_ASSERT_MST_CS0();  
    fStatus = seQSPI_TxBytes( write, 4 );
    
    if ( fStatus == seSTATUS_OK ) {
      fStatus = seQSPI_DmaTxHWords( data, nWords, 0x20010000U );
    }     
    
    seQSPI_NEGATE_MST_CS0();  
    
    seQSPI_SetMode(seQSPI_MODE_SINGLE, seQSPI_08CLK, seQSPI_08CLK);

    if ( fStatus == seSTATUS_OK ) {
      fStatus = WaitFlashBusy();
    }
  } 
  
  return fStatus;
}

