/**
  ******************************************************************************
  * @file    se_mdc.c
  * @author  Epson
  * @version V1.0
  * @date    23-October-2017
  * @brief   This file provides all the MDC hardware functions.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#include <stdio.h>

#include "s1d13c00_hcl.h"
#include "s1d13C00_memregs.h"
#include "se_common.h"
#include "se_mdc.h"



/**
  * Initialize Panel Interface for LPM011M133B (218x218, 6-bit color, 1.1" round)
  * Return value:  Status
  */
seStatus seMDC_InitPanel_LPM011M133B( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));
  
    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on
  
    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDL
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_4V5 << 12 | seMDC_VMDL_3V2 << 4 | 0x0001); // VMDH = 4.5V, VMDL = 4.5V
    seSysSleepMS(2);           // VMDL stabilization time = T1

    //================== T2 ======================================================================
    // Turn on VMDH
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_4V5 << 12 | seMDC_VMDL_3V2 << 4 | 0x0101); // VMDH = 4.5V, VMDL = 3.2V
    seSysSleepMS(2);           // VMDH stabilization time = T2

    //================== T3 ======================================================================
    seSysSleepMS(1);           // Panel internal circuit reset time = T3 (>twXRST)

    //================== T4 ======================================================================

    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 218);
    seS1D13C00Write16(MDC_DISPHEIGHT, 218);
    seS1D13C00Write16(MDC_DISPSTRIDE, 218);
    seS1D13C00Write16(MDC_DISPVCOMDIV, 136);                            // VCOM freq = 32768 / (4*(136+1)) = ~60Hz

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 2850000)) - 1;            // Divider value to get T = 0.35us (2.85MHz)
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0100 | clkdiv );               // VCKHST = (1 + 1) * T = 0.7us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0075 );                         // VSTVCK = 0x75 = (117 + 1) * T = 41.3us
                                                                        // HSTHCK = 0x00 = (0 + 1) * T = 0.35us

    seS1D13C00Write16( MDC_DISPPRM43, 0x1900 );                         // HCKDLY = (0 + 1) * T = 0.35us
                                                                        // t0 + t2 + (25) * 2T = 0.7 + 0.35 + 25*0.7 = 18.55us

    seS1D13C00Write16( MDC_DISPPRM65, 0x3636 );                         // ENBW = 54 HCK counts = 54 * 0.7us = 37.8us
                                                                        // XRSTVST = (54 + 1) * T = 19.25us;

    seS1D13C00Write16( MDC_DISPPRM87, 0x0000 );                         // HCK to HST/data = (0 + 1) * T = 0.35us
                                                                        // End of line to VCK = (0 + 1) * 2T = 0.7us

    seS1D13C00Write16( MDC_DISPPRM109, 0x0101 );                        // HCK count for start of pixel data
                                                                        // Extra HCK counts after end of line pixel data

    seS1D13C00Write16( MDC_DISPPRM1211, 0x0002 );                       // VCK count for start of pixel data
                                                                        // Extra VCK counts after last line of pixel data

    seS1D13C00Write16( MDC_DISPPRM1413, 0x0003 );                       // Fast VCK high/low width = 3 * T = 1.05us
  
    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS << 11 |
                                    seMDC_DISP_PIXNORMAL  <<  7 |
                                    seMDC_DISP_UPD        <<  6 |
                                    seMDC_DISP_PANEL6BIT  <<  4 |
                                    seMDC_DISP_ROT0       <<  2 |
                                    seMDC_VCOMEN_DISABLE  <<  1 );  // Initially VCOM should be disabled

    seS1D13C00Write16( MDC_DISPCTL2, seMDC_DISP_CSPOLNORM << 4 |
                                     seMDC_DISP_VSTFALL   << 3 |
                                     seMDC_DISP_HSTRISE   << 2 |
                                     seMDC_DISP_ENBLSB    << 1 |
                                     seMDC_DISP_VCKFAST   << 0 );

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 218);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 218);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 218);
    seMDC_DrawRectangle( 0, 0, 217, 217, 0x00C0, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 217);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS << 11 |
                                    seMDC_DISP_PIXNORMAL  <<  7 |
                                    seMDC_DISP_UPD        <<  6 |
                                    seMDC_DISP_PANEL6BIT  <<  4 |
                                    seMDC_DISP_ROT0       <<  2 |
                                    seMDC_VCOMEN_ENABLE   <<  1 );  // Enable VCOM


    seSysSleepMS(17);           // Wait about 1 VCOM cycle which is ~16.7ms (60Hz).

    return seSTATUS_OK;
}


/**
  * Initialize Panel Interface for LPM012M134B (240x240, 6-bit color, 1.2" round)
  * Return value:  seStatus
  */
seStatus seMDC_InitPanel_LPM012M134B( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));
  
    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on
  
    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDL
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_4V5 << 12 | seMDC_VMDL_3V2 << 4 | 0x0001); // VMDH = 4.5V, VMDL = 4.5V
          seSysSleepMS(2);           // VMDL stabilization time = T1

    //================== T2 ======================================================================
    // Turn on VMDH
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_4V5 << 12 | seMDC_VMDL_3V2 << 4 | 0x0101); // VMDH = 4.5V, VMDL = 3.2V
    seSysSleepMS(2);           // VMDH stabilization time = T2

    //================== T3 ======================================================================
    seSysSleepMS(1);           // Panel internal circuit reset time = T3 (>twXRST)

    //================== T4 ======================================================================

    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 240);
    seS1D13C00Write16(MDC_DISPHEIGHT, 240);
    seS1D13C00Write16(MDC_DISPSTRIDE, 240);
    seS1D13C00Write16(MDC_DISPVCOMDIV, 136);                            // VCOM freq = 32768 / (4*(136+1)) = ~60Hz

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 2850000)) - 1;            // Divider value to get T = 0.35us (2.85MHz)
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0100 | clkdiv );               // VCKHST = (1 + 1) * T = 0.7us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0075 );                         // VSTVCK = 0x75 = (117 + 1) * T = 41.3us
                                                                        // HSTHCK = 0x00 = (0 + 1) * T = 0.35us

    seS1D13C00Write16( MDC_DISPPRM43, 0x1D00 );                         // HCKDLY = (0 + 1) * T = 0.35us
                                                                        // t0 + t2 + (29) * 2T = 0.7 + 0.35 + 29*0.7 = 21.35us

    seS1D13C00Write16( MDC_DISPPRM65, 0x363C );                         // ENBW = 60 HCK counts = 54 * 0.7us = 42us
                                                                        // XRSTVST = (54 + 1) * T = 19.25us;

    seS1D13C00Write16( MDC_DISPPRM87, 0x0000 );                         // HCK to HST/data = (0 + 1) * T = 0.35us
                                                                        // End of line to VCK = (0 + 1) * 2T = 0.7us

    seS1D13C00Write16( MDC_DISPPRM109, 0x0101 );                        // HCK count for start of pixel data
                                                                        // Extra HCK counts after end of line pixel data

    seS1D13C00Write16( MDC_DISPPRM1211, 0x0002 );                       // VCK count for start of pixel data
                                                                        // Extra VCK counts after last line of pixel data

    seS1D13C00Write16( MDC_DISPPRM1413, 0x0003 );                       // Fast VCK high/low width = 3 * T = 1.05us
  
    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS << 11 |
                                    seMDC_DISP_PIXNORMAL  <<  7 |
                                    seMDC_DISP_UPD        <<  6 |
                                    seMDC_DISP_PANEL6BIT  <<  4 |
                                    seMDC_DISP_ROT0       <<  2 |
                                    seMDC_VCOMEN_DISABLE  <<  1 );  // Initially VCOM should be disabled


    seS1D13C00Write16( MDC_DISPCTL2, seMDC_DISP_CSPOLNORM << 4 |
                                     seMDC_DISP_VSTFALL   << 3 |
                                     seMDC_DISP_HSTRISE   << 2 |
                                     seMDC_DISP_ENBLSB    << 1 |
                                     seMDC_DISP_VCKFAST   << 0 );

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 240);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 240);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 240);
    seMDC_DrawRectangle( 0, 0, 239, 239, 0x00C0, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 239);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS << 11 |
                                    seMDC_DISP_PIXNORMAL  <<  7 |
                                    seMDC_DISP_UPD        <<  6 |
                                    seMDC_DISP_PANEL6BIT  <<  4 |
                                    seMDC_DISP_ROT0       <<  2 |
                                    seMDC_VCOMEN_ENABLE   <<  1 );  // Enable VCOM

    seSysSleepMS(17);           // Wait about 1 VCOM cycle which is ~16.7ms (60Hz).

    return seSTATUS_OK;
}


/**
  * Initialize Panel Interface for LS010B7DH01 (128x128, SPI, 1-bit BW, 1.0" round)
  * Return value:  seStatus
  */
seStatus seMDC_InitPanel_LS010B7DH01( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    seS1D13C00InitDispEn();   // Initialize DISP_EN control pin
 
    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));
  
    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on
  
    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDL and VMDH
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_5V0 << 12 | seMDC_VMDL_3V0 << 4 | 0x0101); // VMDH = 5.0V, VMDL = 3.0V
    seSysSleepMS(2);           // VMDL stabilization time = T1

    //================== T2 ======================================================================
    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 128);
    seS1D13C00Write16(MDC_DISPHEIGHT, 128);
    seS1D13C00Write16(MDC_DISPSTRIDE, 128);
    seS1D13C00Write16( MDC_DISPVCOMDIV, 16383 );                        // EXTCOM freq = 32768 / (4*(16383+1)) = ~0.5Hz typical

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 1500000)) - 1;            // SCLK is 1.5MHz
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0400 | clkdiv );               // tsSCS = (4 + 1) * T = 3.33us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0F01 );                         // thSCS = (1 + 1) * T = 1.33us
                                                                        // Data transfer period = (15 + 1) = 16 SCLK cycles

    seS1D13C00Write16( MDC_DISPPRM43, 0x0701 );                         // Mode bits value for display update
                                                                        // Number of Mode bits to send = (7+1) = 8

    seS1D13C00Write16( MDC_DISPPRM65, 0x0107 );                         // twSCSL = (1 + 1) * T = 1.33us;
                                                                        // Number of address bits to send = (7+1) = 8

    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPILSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI1BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_DISABLE    <<  1 );  // Initially VCOM should be disabled

    seS1D13C00Write16( MDC_DISPCTL2, seMDC_DISP_GSALPHAON << 5);

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 128);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 128);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 128);
    seMDC_DrawRectangle( 0, 0, 127, 127, 0x0002, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 127);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    //================== T3 ======================================================================
    seS1D13C00DispEnable();    // Enable display (DISP_EN = 1)
    seSysSleepMS(1);           // Wait at least 30us

    //================== T4 ======================================================================
    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPILSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI1BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_ENABLE     <<  1 );  // Enable VCOM

    seSysSleepMS(1);           // Wait at least 30us

    return seSTATUS_OK;
}

/**
  * Initialize Panel Interface for LS012B7DD06 (240x240, 6-bit color, 1.2" round)
  * Return value:  seStatus
  */
seStatus seMDC_InitPanel_LS012B7DD06( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));
  
    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on
  
    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDL
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_5V0 << 12 | seMDC_VMDL_3V2 << 4 | 0x0001); // VMDH = 5.0V, VMDL = 4.5V
          seSysSleepMS(2);           // VMDL stabilization time = T1

    //================== T2 ======================================================================
    // Turn on VMDH
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_5V0 << 12 | seMDC_VMDL_3V2 << 4 | 0x0101); // VMDH = 5.0V, VMDL = 3.2V
    seSysSleepMS(2);           // VMDH stabilization time = T2

    //================== T3 ======================================================================
    seSysSleepMS(1);           // Panel internal circuit reset time = T3 (>twXRST)

    //================== T4 ======================================================================

    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 240);
    seS1D13C00Write16(MDC_DISPHEIGHT, 240);
    seS1D13C00Write16(MDC_DISPSTRIDE, 240);
    seS1D13C00Write16(MDC_DISPVCOMDIV, 272);                            // VCOM freq = 32768 / (4*(272+1)) = ~30Hz

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 5000000)) - 1;            // Divider value to get T = 0.2us (5MHz)
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0000 | clkdiv );               // VCKHST = (0 + 1) * T = 0.2us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0036 );                         // VSTVCK = 0x36 = (54 + 1) * T = 11us
                                                                        // HSTHCK = 0x00 = (0 + 1) * T = 0.2us

    seS1D13C00Write16( MDC_DISPPRM43, 0x7A00 );                         // HCKDLY = (0 + 1) * T = 0.2us
                                                                        // t0 + t2 + (122) * 2T = 0.7 + 0.2 + 122*0.4 = 49.2us

    seS1D13C00Write16( MDC_DISPPRM65, 0x3118 );                         // ENBW = 24 HCK counts = 24 * 0.4us = 9.6us
                                                                        // XRSTVST = (49 + 1) * T = 10us;

    seS1D13C00Write16( MDC_DISPPRM87, 0x2100 );                         // HCK to HST/data = (0 + 1) * T = 0.2us
                                                                        // End of line to VCK = (33 + 1) * 2T = 13.6us

    seS1D13C00Write16( MDC_DISPPRM109, 0x0202 );                        // HCK count for start of pixel data
                                                                        // Extra HCK counts after end of line pixel data

    seS1D13C00Write16( MDC_DISPPRM1211, 0x0202 );                       // VCK count for start of pixel data
                                                                        // Extra VCK counts after last line of pixel data

    seS1D13C00Write16( MDC_DISPPRM1413, 0x0003 );                       // Fast VCK high/low width = 3 * T = 0.6us
  
    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS << 11 |
                                    seMDC_DISP_PIXNORMAL  <<  7 |
                                    seMDC_DISP_UPD        <<  6 |
                                    seMDC_DISP_PANEL6BIT  <<  4 |
                                    seMDC_DISP_ROT0       <<  2 |
                                    seMDC_VCOMEN_DISABLE  <<  1 );  // Initially VCOM should be disabled


    seS1D13C00Write16( MDC_DISPCTL2, seMDC_DISP_CSPOLNORM << 4 |
                                     seMDC_DISP_VSTFALL   << 3 |
                                     seMDC_DISP_HSTFALL   << 2 |
                                     seMDC_DISP_ENBMSB    << 1 |
                                     seMDC_DISP_VCKNORM   << 0 );

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 240);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 240);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 240);
    seMDC_DrawRectangle( 0, 0, 239, 239, 0x00C0, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 239);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS << 11 |
                                    seMDC_DISP_PIXNORMAL  <<  7 |
                                    seMDC_DISP_UPD        <<  6 |
                                    seMDC_DISP_PANEL6BIT  <<  4 |
                                    seMDC_DISP_ROT0       <<  2 |
                                    seMDC_VCOMEN_ENABLE   <<  1 );  // Enable VCOM

    seSysSleepMS(17);           // Wait about 1 VCOM cycle which is ~16.7ms (60Hz).

    return seSTATUS_OK;
}


/**
  * Initialize Panel Interface for LSxxxxxxxxx (260x260, 6-bit color, 1.2" round)
  * Return value:  seStatus
  */
seStatus seMDC_InitPanel_6BIT260X260( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));
  
    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on
  
    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDL
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_5V0 << 12 | seMDC_VMDL_3V2 << 4 | 0x0001); // VMDH = 5.0V, VMDL = 4.5V
          seSysSleepMS(2);           // VMDL stabilization time = T1

    //================== T2 ======================================================================
    // Turn on VMDH
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_5V0 << 12 | seMDC_VMDL_3V2 << 4 | 0x0101); // VMDH = 5.0V, VMDL = 3.2V
    seSysSleepMS(2);           // VMDH stabilization time = T2

    //================== T3 ======================================================================
    seSysSleepMS(1);           // Panel internal circuit reset time = T3 (>twXRST)

    //================== T4 ======================================================================

    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 260);
    seS1D13C00Write16(MDC_DISPHEIGHT, 260);
    seS1D13C00Write16(MDC_DISPSTRIDE, 260);
    seS1D13C00Write16(MDC_DISPVCOMDIV, 272);                            // VCOM freq = 32768 / (4*(272+1)) = ~30Hz

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 5000000)) - 1;            // Divider value to get T = 0.2us (5MHz)
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0000 | clkdiv );               // VCKHST = (0 + 1) * T = 0.2us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0036 );                         // VSTVCK = 0x36 = (54 + 1) * T = 11us
                                                                        // HSTHCK = 0x00 = (0 + 1) * T = 0.2us

    seS1D13C00Write16( MDC_DISPPRM43, 0x7A00 );                         // HCKDLY = (0 + 1) * T = 0.2us
                                                                        // t0 + t2 + (122) * 2T = 0.7 + 0.2 + 122*0.4 = 49.2us

    seS1D13C00Write16( MDC_DISPPRM65, 0x3118 );                         // ENBW = 24 HCK counts = 24 * 0.4us = 9.6us
                                                                        // XRSTVST = (49 + 1) * T = 10us;

    seS1D13C00Write16( MDC_DISPPRM87, 0x0000 );                         // HCK to HST/data = (0 + 1) * T = 0.2us
                                                                        // End of line to VCK = (0) * 2T = 0.0us

    seS1D13C00Write16( MDC_DISPPRM109, 0x0301 );                        // HCK count for start of pixel data = 1
                                                                        // Extra HCK counts after end of line pixel data = 3

    seS1D13C00Write16( MDC_DISPPRM1211, 0x0502 );                       // VCK count for start of pixel data = 2
                                                                        // Extra VCK counts after last line of pixel data = 5

    seS1D13C00Write16( MDC_DISPPRM1413, 0x0003 );                       // Fast VCK high/low width = 3 * T = 0.6us
  
    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS << 11 |
                                    seMDC_DISP_PIXNORMAL  <<  7 |
                                    seMDC_DISP_UPD        <<  6 |
                                    seMDC_DISP_PANEL6BIT  <<  4 |
                                    seMDC_DISP_ROT0       <<  2 |
                                    seMDC_VCOMEN_DISABLE  <<  1 );  // Initially VCOM should be disabled

    seS1D13C00Write16( MDC_DISPCTL2, seMDC_DISP_CSPOLNORM << 4 |
                                     seMDC_DISP_VSTFALL   << 3 |
                                     seMDC_DISP_HSTFALL   << 2 |
                                     seMDC_DISP_ENBLSB    << 1 |
                                     seMDC_DISP_VCKNORM   << 0 );

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 260);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 260);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 260);
    seMDC_DrawRectangle( 0, 0, 259, 259, 0x00C0, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 259);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS << 11 |
                                    seMDC_DISP_PIXNORMAL  <<  7 |
                                    seMDC_DISP_UPD        <<  6 |
                                    seMDC_DISP_PANEL6BIT  <<  4 |
                                    seMDC_DISP_ROT0       <<  2 |
                                    seMDC_VCOMEN_ENABLE   <<  1 );  // Enable VCOM

    seSysSleepMS(17);           // Wait about 1 VCOM cycle which is ~16.7ms (60Hz).

    return seSTATUS_OK;
}


/**
  * Initialize Panel Interface for LS013B4DN04 (96x96, SPI, 1-bit BW, 1.3" square)
  * Return value:  Status
  */
seStatus seMDC_InitPanel_LS013B4DN04 ( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    seS1D13C00InitDispEn();   // Initialize DISP_EN control pin

    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));
  
    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on
  
    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDL
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDL_3V0 << 4 | 0x0001 ); // VMDL = 3.0V
    seSysSleepMS(2);           // VMDL stabilization time = T1

    //================== T2 ======================================================================
    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 96);
    seS1D13C00Write16(MDC_DISPHEIGHT, 96);
    seS1D13C00Write16(MDC_DISPSTRIDE, 96);
    seS1D13C00Write16( MDC_DISPVCOMDIV, 16383 );                        // EXTCOM freq = 32768 / (4*(16383+1)) = ~0.5Hz typical

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 1500000)) - 1;            // SCLK is 1.5MHz
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0800 | clkdiv );               // tsSCS = (8 + 1) * T = 6us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0F02 );                         // thSCS = (2 + 1) * T = 2us
                                                                        // Data transfer period = (15 + 1) = 16 SCLK cycles

    seS1D13C00Write16( MDC_DISPPRM43, 0x0701 );                         // Mode bits value for display update
                                                                        // Number of Mode bits to send = (7+1) = 8

    seS1D13C00Write16( MDC_DISPPRM65, 0x0207 );                         // twSCSL = (2 + 1) * T = 2us;
                                                                        // Number of address bits to send = (7+1) = 8

    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPILSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI1BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_DISABLE    <<  1 );  // Initially VCOM should be disabled

    seS1D13C00Write16( MDC_DISPCTL2, seMDC_DISP_GSALPHAON << 5);

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 96);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 96);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 96);
    seMDC_DrawRectangle( 0, 0, 95, 95, 0x0002, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 95);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    //================== T3 ======================================================================
    seS1D13C00DispEnable();    // Enable display (DISP_EN = 1)
    seSysSleepMS(1);           // Wait at least 30us

    //================== T4 ======================================================================
    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPILSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI1BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_ENABLE     << 1 );  // Enable VCOM

    seSysSleepMS(1);           // Wait at least 30us

    return seSTATUS_OK;
}


/**
  * Initialize Panel Interface for LS013B7DH06 (128x128, SPI, 3-bit color, 1.33" square)
  * Return value:  Status
  */
seStatus seMDC_InitPanel_LS013B7DH06 ( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    seS1D13C00InitDispEn();   // Initialize DISP_EN control pin

    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));

    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on

    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDH
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDH_5V0 << 12 | seMDC_VMDL_3V0 << 4 | 0x0101); // VMDH = 5.0V, VMDL = 3.0V
    seSysSleepMS(2);           // VMDH stabilization time = T1

    //================== T2 ======================================================================
    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 128);
    seS1D13C00Write16(MDC_DISPHEIGHT, 128);
    seS1D13C00Write16(MDC_DISPSTRIDE, 128);
    seS1D13C00Write16( MDC_DISPVCOMDIV, 16383 );                        // EXTCOM freq = 32768 / (4*(16383+1)) = ~0.5Hz typical

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 1500000)) - 1;            // SCLK is 1.5MHz
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0400 | clkdiv );               // tsSCS = (4 + 1) * T = 3.33us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0F01 );                         // thSCS = (1 + 1) * T = 1.33us
                                                                        // Data transfer period = (15 + 1) = 16 SCLK cycles

    seS1D13C00Write16( MDC_DISPPRM43, 0x0701 );                         // Mode bits value for display update
                                                                        // Number of Mode bits to send = (7+1) = 8

    seS1D13C00Write16( MDC_DISPPRM65, 0x0107 );                         // Number of address bits to send = (7+1) = 8
                                                                        // twSCSL = (1 + 1) * T = 1.33us;

    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPIRGB       << 10 |
                                    seMDC_DISP_SPILSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI3BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_DISABLE    <<  1 );  // Initially VCOM should be disabled

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 128);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 128);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 128);
    seMDC_DrawRectangle( 0, 0, 127, 127, 0x0008, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 127);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    //================== T3 ======================================================================
    seS1D13C00DispEnable();    // Enable display (DISP_EN = 1)
    seSysSleepMS(1);           // Wait at least 30us

    //================== T4 ======================================================================
    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPIRGB       << 10 |
                                    seMDC_DISP_SPILSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI3BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_ENABLE     <<  1 );  // Enable VCOM

    seSysSleepMS(1);           // Wait at least 30us

    return seSTATUS_OK;
}


/**
  * Initialize Panel Interface for LS012B7DH02 (240x240, SPI, 1-bit BW, 1.2" round)
  * Return value:  seStatus
  */
seStatus seMDC_InitPanel_LS012B7DH02( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    seS1D13C00InitDispEn();   // Initialize DISP_EN control pin

    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));
  
    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on
  
    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDL
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDL_3V0 << 4 | 0x0001 ); // VMDL = 3.0V
    seSysSleepMS(2);           // VMDL stabilization time = T1

    //================== T2 ======================================================================
    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 240);
    seS1D13C00Write16(MDC_DISPHEIGHT, 240);
    seS1D13C00Write16(MDC_DISPSTRIDE, 240);
    seS1D13C00Write16( MDC_DISPVCOMDIV, 286 );                          // EXTCOM freq = 32768 / (4*(286+1)) = ~28.5Hz typical

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 1500000)) - 1;            // SCLK is 1.5MHz
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0800 | clkdiv );               // tsSCS = (8 + 1) * T = 6us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0F01 );                         // tsSCS = (4 + 1) * T = 3.33us
                                                                        // Data transfer period = (15 + 1) = 16 SCLK cycles

    seS1D13C00Write16( MDC_DISPPRM43, 0x0701 );                         // Mode bits value for display update
                                                                        // Number of Mode bits to send = (7+1) = 8

    seS1D13C00Write16( MDC_DISPPRM65, 0x0107 );                         // Number of address bits to send = (7+1) = 8
                                                                        // twSCSL = (1 + 1) * T = 1.33us;

    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPILSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI1BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_DISABLE    <<  1 );  // Initially VCOM should be disabled

    seS1D13C00Write16( MDC_DISPCTL2, seMDC_DISP_GSALPHAON << 5);

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 240);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 240);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 240);
    seMDC_DrawRectangle( 0, 0, 239, 239, 0x0002, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 239);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    //================== T3 ======================================================================
    seS1D13C00DispEnable();    // Enable display (DISP_EN = 1)
    seSysSleepMS(1);           // Wait at least 30us

    //================== T4 ======================================================================
    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPILSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI1BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_ENABLE     <<  1 );  // Enable VCOM

    seSysSleepMS(1);           // Wait at least 30us

    return seSTATUS_OK;
}

/**
  * Initialize Panel Interface for LPM013M126C (176x176, SPI, 3-bit color, 1.28" square)
  * Return value:  Status
  */
seStatus seMDC_InitPanel_LPM013M126C( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    seS1D13C00InitDispEn();   // Initialize DISP_EN control pin

    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));
  
    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on
  
    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDL
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDL_3V0 << 4 | 0x0001 ); // VMDL = 3.0V
    seSysSleepMS(2);           // VMDL stabilization time = T1

    //================== T2 ======================================================================
    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 176);
    seS1D13C00Write16(MDC_DISPHEIGHT, 176);
    seS1D13C00Write16(MDC_DISPSTRIDE, 176);
    seS1D13C00Write16( MDC_DISPVCOMDIV, 8191 );                         // EXTCOM freq = 32768 / (4*(8191+1)) = ~1Hz typical

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 1500000)) - 1;            // SCLK is 1.5MHz
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0800 | clkdiv );               // tsSCS = (8 + 1) * T = 6us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0F02 );                         // thSCS = (2 + 1) * T = 2us
                                                                        // Data transfer period = (15 + 1) = 16 SCLK cycles

    seS1D13C00Write16( MDC_DISPPRM43, 0x0501 );                         // Mode bits value for display update
                                                                        // Number of Mode bits to send = (5+1) = 6

    seS1D13C00Write16( MDC_DISPPRM65, 0x0809 );                         // Number of address bits to send = (9+1) = 10
                                                                        // twSCSL = (8 + 1) * T = 6us;

    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPIRGB       << 10 |
                                    seMDC_DISP_SPIMSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI3BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_DISABLE    <<  1 );  // Initially VCOM should be disabled

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 176);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 176);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 176);
    seMDC_DrawRectangle( 0, 0, 175, 175, 0x0008, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 175);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    //================== T3 ======================================================================
    seS1D13C00DispEnable();    // Enable display (DISP_EN = 1)
    seSysSleepMS(1);           // Wait at least 30us

    //================== T4 ======================================================================
    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPIRGB       << 10 |
                                    seMDC_DISP_SPIMSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI3BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_ENABLE     <<  1 );  // Enable VCOM

    seSysSleepMS(1);           // Wait at least 30us

    return seSTATUS_OK;
}


/**
  * Initialize Panel Interface for LPM027M128A (400x240, SPI, 3-bit color, 2.7" rectangular)
  * Return value:  Status
  */
seStatus seMDC_InitPanel_LPM027M128A ( uint32_t sysfreqhz, uint32_t framebuffaddr )
{
    seS1D13C00InitDispEn();   // Initialize DISP_EN control pin

    // Sequence power-on of voltage supplies
    seS1D13C00Write16(MDC_BSTCLK, ((5 << 4) | (seMDC_OSC1 << 0)));
  
    seS1D13C00Write16( MDC_BSTPWR, 0x0007 ); // VMDBUP = normal, BSTON = on, REGECO = on, REGON = on
  
    seSysSleepMS(2);           // Wait stabilization time

    //================== T1 ======================================================================
    // Turn on VMDL
    seS1D13C00Write16( MDC_BSTVMD, seMDC_VMDL_3V0 << 4 | 0x0001 ); // VMDL = 3.0V
    seSysSleepMS(2);           // VMDL stabilization time = T1

    //================== T2 ======================================================================
    // Initialize clocks and panel interface
    seS1D13C00Write16(MDC_DISPWIDTH, 400);
    seS1D13C00Write16(MDC_DISPHEIGHT, 240);
    seS1D13C00Write16(MDC_DISPSTRIDE, 400);
    seS1D13C00Write16( MDC_DISPVCOMDIV, 8191 );                         // EXTCOM freq = 32768 / (4*(8191+1)) = ~1Hz typical

    uint16_t clkdiv = ((uint16_t)(sysfreqhz / 1500000)) - 1;            // SCLK is 1.5MHz
    seS1D13C00Write16( MDC_DISPCLKDIV, 0x0800 | clkdiv );               // tsSCS = (8 + 1) * T = 6us

    seS1D13C00Write16( MDC_DISPPRM21, 0x0F02 );                         // thSCS = (2 + 1) * T = 2us
                                                                        // Data transfer period = (15 + 1) = 16 SCLK cycles

    seS1D13C00Write16( MDC_DISPPRM43, 0x0501 );                         // Mode bits value for display update
                                                                        // Number of Mode bits to send = (5+1) = 6

    seS1D13C00Write16( MDC_DISPPRM65, 0x0809 );                         // Number of address bits to send = (9+1) = 10
                                                                        // twSCSL = (8 + 1) * T = 6us;

    seS1D13C00Write16( MDC_DISPFRMBUFF0, (uint16_t) (framebuffaddr & 0xFFFF));
    seS1D13C00Write16( MDC_DISPFRMBUFF1, (uint16_t)((framebuffaddr >> 16) & 0xFFFF));

    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPIRGB       << 10 |
                                    seMDC_DISP_SPIMSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI3BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_DISABLE    <<  1 );  // Initially VCOM should be disabled

    // Send black screen to panel
    seS1D13C00Write32(MDC_GFXOBADDR0, framebuffaddr);
    seS1D13C00Write16(MDC_GFXOWIDTH, 400);
    seS1D13C00Write16(MDC_GFXOHEIGHT, 240);
    seS1D13C00Write16(MDC_GFXOSTRIDE, 400);
    seMDC_DrawRectangle( 0, 0, 399, 239, 0x0008, 0, 0, 1);  // filled rectangle with black pixel
    seMDC_WaitGfxDone();

    seS1D13C00Write16(MDC_DISPSTARTY, 0);
    seS1D13C00Write16(MDC_DISPENDY, 239);

    // Clear interrupt flag
    seS1D13C00Write8(MDC_INTCTL, 0x02);  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8(MDC_TRIGCTL, 0x02);  // UPDTRIG = 1
    seMDC_WaitUpdDone();

    //================== T3 ======================================================================
    seS1D13C00DispEnable();    // Enable display (DISP_EN = 1)
    seSysSleepMS(1);           // Wait at least 30us

    //================== T4 ======================================================================
    seS1D13C00Write16( MDC_DISPCTL, seMDC_DISP_PANELNOTGS   << 11 |
                                    seMDC_DISP_SPIRGB       << 10 |
                                    seMDC_DISP_SPIMSBFIRST  <<  9 |
                                    seMDC_DISP_AUTOCOMOFF   <<  8 |
                                    seMDC_DISP_PIXNORMAL    <<  7 |
                                    seMDC_DISP_UPD          <<  6 |
                                    seMDC_DISP_SPI3BIT      <<  5 |
                                    seMDC_DISP_PANELSPI     <<  4 |
                                    seMDC_DISP_ROT0         <<  2 |
                                    seMDC_VCOMEN_ENABLE     <<  1 );  // Enable VCOM

    seSysSleepMS(1);           // Wait at least 30us

    return seSTATUS_OK;
}


/**
  * Update the panel with image from frame/display buffer
  * Parameters:
  *     startline: Starting line number (0 is first line).
  *     endline: Endng line number
  * For full screen update, startline=0 and endline=(<display height>-1).
  * Return value:  Status
  */
seStatus seMDC_PanelUpdate( uint16_t startline, uint16_t endline )
{
    seSetBits16( MDC_BSTPWR, MDC_VMDBUP_bits | MDC_REGECO_bits, MDC_VMDBUP_bits );  // Faster response for voltage booster, exit ECO mode
    seSysSleepMS(1);           // Wait stabilization time

    seS1D13C00Write16(MDC_DISPSTARTY, startline );
    seS1D13C00Write16(MDC_DISPENDY, endline );

    // Clear interrupt flag
    seS1D13C00Write8( MDC_INTCTL, 0x02 );  // UPDINT flag

    // Trigger display update
    seS1D13C00Write8( MDC_TRIGCTL, 0x02 );  // UPDTRIG = 1

    seSetBits16( MDC_BSTPWR, MDC_VMDBUP_bits | MDC_REGECO_bits, MDC_REGECO_bits ); // Slower response for voltage booster, enter ECO mode

    return seSTATUS_OK;
}


/**
  * Change VCOM divider value
  * Parameters:
  *     vcomval: new divider value
  * Return value:  Status
  */
seStatus seMDC_VCOMChangeDivider( uint16_t vcomval )
{
    seS1D13C00Write8(MDC_VCOMCLKCTL, 0x01);  // Disable CLK32K for VCOM counter
    seS1D13C00Write8(MDC_VCOMCLKCTL, 0x03);  // Hold VCOM counter in reset
    seS1D13C00Write16(MDC_DISPVCOMDIV, vcomval);  // New VCOM divider value
    seS1D13C00Write8(MDC_VCOMCLKCTL, 0x01);  // Release VCOM counter from reset
    seS1D13C00Write8(MDC_VCOMCLKCTL, 0x00);  // Reenable CLK32K for VCOM counter
    return seSTATUS_OK;
}


/**
  * Set Destination Window
  * Parameters:
  *     destwinparams_ptr: Destination Window parameters descriptor. Must be aligned.
  * Return value:  Status
  */
seStatus seMDC_SetDestWindow( seMDC_DestWindowParams * destwinparams_ptr )
{
    seS1D13C00Write16( MDC_GFXOBADDR0, destwinparams_ptr->obaseaddr_b.obaseaddr0 );
    seS1D13C00Write16( MDC_GFXOBADDR1, destwinparams_ptr->obaseaddr_b.obaseaddr1 );
    seS1D13C00Write16( MDC_GFXOWIDTH,  destwinparams_ptr->owidth );
    seS1D13C00Write16( MDC_GFXOHEIGHT, destwinparams_ptr->oheight );
    seS1D13C00Write16( MDC_GFXOSTRIDE, destwinparams_ptr->ostride );
    return seSTATUS_OK;
}


/**
  * Draw a line
  * Parameters:
  *         point1x: POINT1 X coordinate.
  *         point1y: POINT1 Y coordinate.
  *         point2x: POINT2 X coordinate.
  *         point2y: POINT2 Y coordinate.
  *         pencolor: pen color.
  *         thickness: line thickness.
  * Return value:  Status
  *
  * NOTE:  This routine triggers the graphics engine only and does not check for completion.
  *        It is the calling function's responsibility to check for completion before triggering
  *        another graphics function.
  */
seStatus seMDC_DrawLine( uint16_t point1x, uint16_t point1y, uint16_t point2x, uint16_t point2y,
                         uint16_t pencolor, uint16_t thickness )
{

    // Setup the line draw parameters
    seS1D13C00Write16( MDC_GFXIXCENTER, (point1x >= 0x8000) ? 0 : point1x );
    seS1D13C00Write16( MDC_GFXIYCENTER, (point1y >= 0x8000) ? 0 : point1y );
    seS1D13C00Write16( MDC_GFXOXCENTER, (point2x >= 0x8000) ? 0 : point2x );
    seS1D13C00Write16( MDC_GFXOYCENTER, (point2y >= 0x8000) ? 0 : point2y );
    seS1D13C00Write16( MDC_GFXCOLOR, pencolor );
    seS1D13C00Write16( MDC_GFXIWIDTH, thickness );

    // Clear GFX interrupt flag
    seS1D13C00Write8( MDC_INTCTL, 0x01 );  // clear GFXINT flag

    // Trigger update
    seSetBits16( MDC_GFXCTL, MDC_GFXFUNC_bits, seMDC_FUNC_LINEDRAW );
    seS1D13C00Write8( MDC_TRIGCTL, 0x01 );  // GFXTRIG = 1

    return seSTATUS_OK;
}


/**
  * Draw a rectangle
  * Parameters:
  *         tlcornerx: Top left corner X coordinate.
  *         tlcornery: Top left corner Y coordinate.
  *         brcornerx: Bottom right corner X coordinate.
  *         brcornery: Bottom right corner Y coordinate.
  *         pencolor: pen color.
  *         vlinethick: vertical line thickness.
  *         hlinethick: horizontal line thickness.
  *         fillenable: Fill Enable.
  * Return value:  Status
  *
  * NOTE:  This routine triggers the graphics engine only and does not check for completion.
  *        It is the calling function's responsibility to check for completion before triggering
  *        another graphics function.
  */
seStatus seMDC_DrawRectangle( uint16_t tlcornerx, uint16_t tlcornery, uint16_t brcornerx, uint16_t brcornery,
                              uint16_t pencolor, uint16_t vlinethick, uint16_t hlinethick, uint8_t fillenable)
{
    tlcornerx = ( tlcornerx >= 0x8000 ) ? 0 : tlcornerx;
    tlcornery = ( tlcornery >= 0x8000 ) ? 0 : tlcornery;
    brcornerx = ( brcornerx >= 0x8000 ) ? 0 : brcornerx;
    brcornery = ( brcornery >= 0x8000 ) ? 0 : brcornery;

    if ( (brcornery < tlcornery) || (brcornerx < tlcornerx) ) {
        return seSTATUS_NG;
    }

    // Setup the rectangle parameters
    seS1D13C00Write16( MDC_GFXIXCENTER, tlcornerx );
    seS1D13C00Write16( MDC_GFXIYCENTER, tlcornery );
    seS1D13C00Write16( MDC_GFXOXCENTER, brcornerx );
    seS1D13C00Write16( MDC_GFXOYCENTER, brcornery );
    seS1D13C00Write16( MDC_GFXCOLOR, pencolor );
    seS1D13C00Write16( MDC_GFXIWIDTH, vlinethick );
    seS1D13C00Write16( MDC_GFXIHEIGHT, hlinethick );

    fillenable = (fillenable == 0) ? 0 : 1;
    seSetBits16( MDC_GFXCTL, MDC_FILLEN_bits, fillenable << 11 );

    // clear interrupt flag
    seS1D13C00Write8( MDC_INTCTL, 0x01 );

    seSetBits16( MDC_GFXCTL, MDC_GFXFUNC_bits, seMDC_FUNC_RECTDRAW );
    seS1D13C00Write8( MDC_TRIGCTL, 0x01 );

    return seSTATUS_OK;
}


/**
  * Draw an ellipse.
  * Parameters:
  *         centerx: Center X coordinate.
  *         centery: Center Y coordinate.
  *         radiusx: X-axis radius
  *         radiusy: Y-axis radius.
  *         pencolor: pen color.
  *         xcrossthick: X-axis crossing thickness.
  *         ycrossthick: Y-axis crossing thickness.
  *         fillenable: Fill Enable.
  * Return value:  Status
  *
  * NOTE:  This routine triggers the graphics engine only and does not check for completion.
  *        It is the calling function's responsibility to check for completion before triggering
  *        another graphics function.
  */
seStatus seMDC_DrawEllipse( uint16_t centerx, uint16_t centery, uint16_t radiusx, uint16_t radiusy,
                            uint16_t pencolor, uint16_t xcrossthick, uint16_t ycrossthick, uint8_t fillenable )
{

    // Setup ellipse parameters
    seS1D13C00Write16( MDC_GFXIXCENTER, centerx );
    seS1D13C00Write16( MDC_GFXIYCENTER, centery );
    seS1D13C00Write16( MDC_GFXOXCENTER, radiusx );
    seS1D13C00Write16( MDC_GFXOYCENTER, radiusy );
    seS1D13C00Write16( MDC_GFXCOLOR, pencolor );
    seS1D13C00Write16( MDC_GFXIWIDTH, xcrossthick );
    seS1D13C00Write16( MDC_GFXIHEIGHT, ycrossthick );

    fillenable = (fillenable == 0) ? 0 : 1;
    seSetBits16( MDC_GFXCTL, MDC_FILLEN_bits, fillenable << 11 );

    // Clear interrupt flag
    seS1D13C00Write8( MDC_INTCTL, 0x01 );

    seSetBits16( MDC_GFXCTL, MDC_GFXFUNC_bits, seMDC_FUNC_ELLIPDRAW );
    seS1D13C00Write8( MDC_TRIGCTL, 0x01 );  // GFXTRIG = 1

    return seSTATUS_OK;
}


/**
  * Image/bitmap copy with rotation and scaling
  * Parameters:
  *         ocenterx:  Destination Window center X coordinate.
  *         ocentery:  Destination Window center Y coordinate.
  *         ibaseaddr:  Source Window base address.
  *         istride:  Source Window stride.
  *         iwidth:  Source Window width.
  *         iheight:  Source Window height.
  *         icenterx:  Source Window center of transformation X coordinate.
  *         icentery:  Source Window center of transformation Y coordinate.
  *         fillcolor:  Fill color (if FILL is enabled).
  *         rotval:  Rotation value
  *         xlscale:  Left half of source image scaling value
  *         xrscale:  Right half of source image scaling value
  *         ytscale:  Top half of source image scaling value
  *         ybscale:  Bottom half of source image scaling value
  *         ctrl_ptr:  Pointer to control word.
  * Return value:  Status
  *
  * NOTE:  This routine triggers the graphics engine only and does not check for completion.
  *        It is the calling function's responsibility to check for completion before triggering
  *        another graphics function.
  */
seStatus seMDC_ImgCpyRotScale( uint16_t ocenterx, uint16_t ocentery,
                               uint32_t ibaseaddr, uint16_t istride, uint16_t iwidth, uint16_t iheight,
                               uint16_t icenterx, uint16_t icentery,
                               uint16_t fillcolor, uint16_t rotval,
                               uint16_t xlscale, uint16_t xrscale, uint16_t ytscale, uint16_t ybscale,
                               seMDC_ImgCopyRotScaleCtrl * ctrl_ptr)
{

    // Setup image copy parameters
    seS1D13C00Write16( MDC_GFXOXCENTER, ocenterx );
    seS1D13C00Write16( MDC_GFXOYCENTER, ocentery );

    seS1D13C00Write16( MDC_GFXIBADDR0, (uint16_t) (ibaseaddr & 0xFFFF) );
    seS1D13C00Write16( MDC_GFXIBADDR1, (uint16_t) ((ibaseaddr>>16) & 0xFFFF) );
    seS1D13C00Write16( MDC_GFXISTRIDE, istride );
    seS1D13C00Write16( MDC_GFXIWIDTH, iwidth );
    seS1D13C00Write16( MDC_GFXIHEIGHT, iheight );
    seS1D13C00Write16( MDC_GFXIXCENTER, icenterx );
    seS1D13C00Write16( MDC_GFXIYCENTER, icentery );

    seS1D13C00Write16( MDC_GFXCOLOR, fillcolor );
    seS1D13C00Write16( MDC_GFXROTVAL, rotval );

    seS1D13C00Write16( MDC_GFXXLSCALE, xlscale );
    seS1D13C00Write16( MDC_GFXXRSCALE, xrscale );
    seS1D13C00Write16( MDC_GFXYTSCALE, ytscale );
    seS1D13C00Write16( MDC_GFXYBSCALE, ybscale );

    // Clear interrupt flag
    seS1D13C00Write8( MDC_INTCTL, 0x01 );

    ctrl_ptr->ctrlword &= 0xFFF8;
    ctrl_ptr->ctrlword |= seMDC_FUNC_COPYROTSCALE;
    seS1D13C00Write16( MDC_GFXCTL, ctrl_ptr->ctrlword );

    seS1D13C00Write8( MDC_TRIGCTL, 0x01 );

    return seSTATUS_OK;
}


/**
  * Image/bitmap copy with horizontal and vertical shear
  * Parameters:
  *         ocenterx:  Destination Window center X coordinate.
  *         ocentery:  Destination Window center Y coordinate.
  *         ibaseaddr:  Source Window base address.
  *         istride:  Source Window stride.
  *         iwidth:  Source Window width.
  *         iheight:  Source Window height.
  *         icenterx:  Source Window center of transformation X coordinate.
  *         icentery:  Source Window center of transformation Y coordinate.
  *         fillcolor:  Fill color (if FILL is enabled).
  *         ctrl_ptr:  Pointer to control parameters.
  * Return value:  Status
  *
  * NOTE:  This routine triggers the graphics engine only and does not check for completion.
  *        It is the calling function's responsibility to check for completion before triggering
  *        another graphics function.
  */
seStatus seMDC_ImgCpyHVShear( uint16_t ocenterx, uint16_t ocentery,
                              uint32_t ibaseaddr, uint16_t istride, uint16_t iwidth, uint16_t iheight,
                              uint16_t icenterx, uint16_t icentery,
                              uint16_t fillcolor,
                              seMDC_ImgCopyHVShearCtrl * ctrl_ptr)
{

    seS1D13C00Write16( MDC_GFXOXCENTER, ocenterx );
    seS1D13C00Write16( MDC_GFXOYCENTER, ocentery );

    seS1D13C00Write16( MDC_GFXIBADDR0, (uint16_t) (ibaseaddr & 0xFFFF) );
    seS1D13C00Write16( MDC_GFXIBADDR1, (uint16_t) ((ibaseaddr>>16) & 0xFFFF) );
    seS1D13C00Write16( MDC_GFXISTRIDE, istride );
    seS1D13C00Write16( MDC_GFXIWIDTH, iwidth );
    seS1D13C00Write16( MDC_GFXIHEIGHT, iheight );
    seS1D13C00Write16( MDC_GFXIXCENTER, icenterx );
    seS1D13C00Write16( MDC_GFXIYCENTER, icentery );

    seS1D13C00Write16( MDC_GFXCOLOR, fillcolor );

    seS1D13C00Write16( MDC_GFXSHEAR, ctrl_ptr->shearctrl );

    // Clear interrupt flag
    seS1D13C00Write8( MDC_INTCTL, 0x01 );  // GFXINT flag

    // Trigger display update
    ctrl_ptr->ctrlword &= 0xFFF8;
    ctrl_ptr->ctrlword |= seMDC_FUNC_COPYHVSHEAR;
    seS1D13C00Write16( MDC_GFXCTL, ctrl_ptr->ctrlword );

    seS1D13C00Write8( MDC_TRIGCTL, 0x01 );  // GFXTRIG = 1

    return seSTATUS_OK;
}


void seMDC_WaitGfxDone( void )
{
    seS1D13C00Write8(MDC_INTCTL+1, 0x01);  // Enable interrupt
    while((seS1D13C00Read16(SYS_INTS) & SYS_MDCINT_bits) == 0);   // Poll wait until interrupt occurs
    seS1D13C00Write8(MDC_INTCTL, 0x01);    // Clear interrupt
}



void seMDC_WaitUpdDone( void )
{
    seS1D13C00Write8(MDC_INTCTL+1, 0x02);  // Enable interrupt
    while((seS1D13C00Read16(SYS_INTS) & SYS_MDCINT_bits) == 0);   // Poll wait until interrupt occurs
    seS1D13C00Write8(MDC_INTCTL, 0x02);    // Clear interrupt
}



void seMDC_SelectClkSrc( seMDC_ClkSrc clock )
{
    switch (clock)
    {
    case seMDC_IOSC:
        seSetBits16( MDC_BSTCLK, MDC_CLKSRC_bits, seMDC_IOSC );
        break;
    case seMDC_OSC1:
        seSetBits16( MDC_BSTCLK, MDC_CLKSRC_bits, seMDC_OSC1 );
        break;
    default:
        break;
    }
}


