//////////////////////////////////////////////////////////////////////////////
///
///  \file s1d13c00_memregs.h
///
/// \par	 Memory Map and Registers Definitions
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

#ifndef seMEMREGS_H_INCLUDED
#define seMEMREGS_H_INCLUDED


// ========== Memory map ====================================
#define EXTMEM_BASE                 0x00040000
#define RAM_BASE                    0x20000000

#define SYS_PROT_BASE               0x40000000
#define CLG_REGS_BASE               0x40000040
#define RTC_REGS_BASE               0x400000C0
#define PORT_REGS_BASE              0x40000200
#define T16_1_REGS_BASE             0x400003A0
#define SPI_REGS_BASE               0x400003A0
#define I2C_REGS_BASE               0x400003C0
#define T16_2_REGS_BASE             0x40000680
#define QSPI_REGS_BASE              0x40000690
#define SND_REGS_BASE               0x40000700
#define REMC_REGS_BASE              0x40000720
#define DMA_REGS_BASE               0x40001000
#define MDC_REGS_BASE               0x40003000
#define SYS_REGS_BASE               0x400030E0

// ========== SYS Registers =================================
#define SYS_SYSPROT            SYS_PROT_BASE + 0x00

// ========== CLG Registers =================================
#define CLG_OSC                 CLG_REGS_BASE + 0x02
#define     CLG_OSC1EN_bits         0x0002
#define CLG_OSC1                CLG_REGS_BASE + 0x06
#define     CLG_OSDRB_bits          0x4000
#define     CLG_OSDEN_bits          0x2000
#define     CLG_OSC1BUP_bits        0x1000
#define     CLG_OSC1SELCR_bits      0x0800
#define     CLG_CGI1_bits           0x0700
#define     CLG_INV1B_bits          0x00C0
#define     CLG_INV1N_bits          0x0030
#define     CLG_OSC1WT_bits         0x0003
#define CLG_INTF                CLG_REGS_BASE + 0x0C
#define     CLG_OSC1TRMBSY_bits     0x0080
#define     CLG_OSC1TEDIF_bits      0x0040
#define     CLG_OSC1STPIF_bits      0x0020
#define     CLG_OSC1STAIF_bits      0x0002
#define CLG_INTE                CLG_REGS_BASE + 0x0E
#define     CLG_OSC1TEDIE_bits      0x0040
#define     CLG_OSC1STPIE_bits      0x0020
#define     CLG_OSC1STAIE_bits      0x0002
#define CLG_FOUT                CLG_REGS_BASE + 0x10
#define     CLG_FOUTDIV_bits        0x007F
#define CLG_OSC1TRM             CLG_REGS_BASE + 0x14
#define     CLG_OSC1SAJ_bits        0x003F


// ========== RTC Registers =================================
#define RTC_CTLL                 RTC_REGS_BASE + 0x00
#define     RTC_BSY_bits             0x40
#define     RTC_HLD_bits             0x20
#define     RTC_24H_bits             0x10
#define     RTC_ADJ_bits             0x04
#define     RTC_RST_bits             0x02
#define     RTC_RUN_bits             0x01
#define RTC_CTLH                 RTC_REGS_BASE + 0x01
#define     RTC_TRMBSY_bits          0x80
#define     RTC_TRM_bits             0x7F
#define RTC_ALM1                RTC_REGS_BASE + 0x02
#define     RTC_SHA_bits            0x7000
#define     RTC_SLA_bits            0x0F00
#define RTC_ALM2                RTC_REGS_BASE + 0x04
#define     RTC_PA_bits             0x4000
#define     RTC_HHA_bits            0x3000
#define     RTC_HLA_bits            0x0F00
#define     RTC_MIHA_bits           0x00F0
#define     RTC_MILA_bits           0x000F
#define RTC_SWCTL               RTC_REGS_BASE + 0x06
#define     RTC_BCD10_bits          0xF000
#define     RTC_BCD100_bits         0x0F00
#define     RTC_SWRST_bits          0x0010
#define     RTC_SWRUN_bits          0x0001
#define RTC_SEC                 RTC_REGS_BASE + 0x08
#define     RTC_SH_bits             0x7000
#define     RTC_SL_bits             0x0F00
#define     RTC_1HZ_bits            0x0080
#define     RTC_2HZ_bits            0x0040
#define     RTC_4HZ_bits            0x0020
#define     RTC_8HZ_bits            0x0010
#define     RTC_16HZ_bits           0x0008
#define     RTC_32HZ_bits           0x0004
#define     RTC_64HZ_bits           0x0002
#define     RTC_128HZ_bits          0x0001
#define RTC_HUR                 RTC_REGS_BASE + 0x0A
#define     RTC_AP_bits             0x4000
#define     RTC_HH_bits             0x3000
#define     RTC_HL_bits             0x0F00
#define     RTC_MIH_bits            0x0070
#define     RTC_MIL_bits            0x000F
#define RTC_MON                 RTC_REGS_BASE + 0x0C
#define     RTC_MOH_bits            0x1000
#define     RTC_MOL_bits            0x0F00
#define     RTC_DH_bits             0x0030
#define     RTC_DL_bits             0x000F
#define RTC_YAR                 RTC_REGS_BASE + 0x0E
#define     RTC_WK_bits             0x0700
#define     RTC_YH_bits             0x00F0
#define     RTC_YL_bits             0x000F
#define RTC_INTF                RTC_REGS_BASE + 0x10
#define     RTC_TRMIF_bits          0x8000
#define     RTC_SW1IF_bits          0x4000
#define     RTC_SW10IF_bits         0x2000
#define     RTC_SW100IF_bits        0x1000
#define     RTC_ALARMIF_bits        0x0100
#define     RTC_T1DAYIF_bits        0x0080
#define     RTC_T1HURIF_bits        0x0040
#define     RTC_T1MINIF_bits        0x0020
#define     RTC_T1SECIF_bits        0x0010
#define     RTC_T1_2SECIF_bits      0x0008
#define     RTC_T1_4SECIF_bits      0x0004
#define     RTC_T1_8SECIF_bits      0x0002
#define     RTC_T1_32SECIF_bits     0x0001
#define RTC_INTE                RTC_REGS_BASE + 0x12
#define     RTC_TRMIE_bits       0x8000
#define     RTC_SW1IE_bits          0x4000
#define     RTC_SW10IE_bits         0x2000
#define     RTC_SW100IE_bits        0x1000
#define     RTC_ALARMIE_bits        0x0100
#define     RTC_T1DAYIE_bits        0x0080
#define     RTC_T1HURIE_bits        0x0040
#define     RTC_T1MINIE_bits        0x0020
#define     RTC_T1SECIE_bits        0x0010
#define     RTC_T1_2SECIE_bits      0x0008
#define     RTC_T1_4SECIE_bits      0x0004
#define     RTC_T1_8SECIE_bits      0x0002
#define     RTC_T1_32SECIE_bits     0x0001

// ========== MDC Registers =================================
#define MDC_DISPCTL             MDC_REGS_BASE + 0x00
#define     MDC_RGBORD_bits         0x0400
#define     MDC_ADDRLSB_bits        0x0200
#define     MDC_AUTOCOM_bits        0x0100
#define     MDC_DISPINVERT_bits     0x0080
#define     MDC_UPDFUNC_bits        0x0040
#define     MDC_SPITYPE_bits        0x0020
#define     MDC_DISPSPI_bits        0x0010
#define     MDC_ROTSEL_bits         0x000C
#define     MDC_VCOMEN_bits         0x0002
#define MDC_DISPWIDTH           MDC_REGS_BASE + 0x02
#define MDC_DISPHEIGHT          MDC_REGS_BASE + 0x04
#define MDC_DISPVCOMDIV         MDC_REGS_BASE + 0x06
#define MDC_DISPCLKDIV          MDC_REGS_BASE + 0x08
#define     MDC_TIM0_bits           0xFF00
#define     MDC_CLKDIV_bits         0x00FF
#define MDC_DISPPRM21           MDC_REGS_BASE + 0x0A
#define     MDC_TIM2_bits           0xFF00
#define     MDC_TIM1_bits           0x00FF
#define MDC_DISPPRM43           MDC_REGS_BASE + 0x0C
#define     MDC_TIM4_bits           0xFF00
#define     MDC_TIM3_bits           0x00FF
#define MDC_DISPPRM65           MDC_REGS_BASE + 0x0E
#define     MDC_TIM6_bits           0xFF00
#define     MDC_TIM5_bits           0x00FF
#define MDC_DISPPRM87           MDC_REGS_BASE + 0x10
#define     MDC_TIM8_bits           0xFF00
#define     MDC_TIM7_bits           0x00FF
#define MDC_DISPSTARTY          MDC_REGS_BASE + 0x12
#define MDC_DISPENDY            MDC_REGS_BASE + 0x14
#define MDC_DISPSTRIDE          MDC_REGS_BASE + 0x16
#define MDC_DISPFRMBUFF0        MDC_REGS_BASE + 0x18
#define MDC_DISPFRMBUFF1        MDC_REGS_BASE + 0x1A
#define MDC_TRIGCTL             MDC_REGS_BASE + 0x1C
#define     MDC_UPDTRIG_bits        0x0002
#define     MDC_GFXTRIG_bits        0x0001
#define MDC_INTCTL              MDC_REGS_BASE + 0x1E
#define     MDC_VCNTIE_bits         0x0400
#define     MDC_UPDIE_bits          0x0200
#define     MDC_GFXIE_bits          0x0100
#define     MDC_VCNTIF_bits         0x0004
#define     MDC_UPDIF_bits          0x0002
#define     MDC_GFXIF_bits          0x0001
#define MDC_GFXCTL              MDC_REGS_BASE + 0x20
#define     MDC_CPYNEGY_bits        0x8000
#define     MDC_CPYNEGX_bits        0x4000
#define     MDC_SHEARNEGY_bits      0x2000
#define     MDC_SHEARNEGX_bits      0x1000
#define     MDC_FILLEN_bits         0x0800
#define     MDC_BITMAPFMT_bits      0x0400
#define     MDC_BITMAPEN_bits       0x0200
#define     MDC_ALPHAVAL_bits       0x00C0
#define     MDC_ALPHOVRRD_bits      0x0020
#define     MDC_GFXFUNC_bits        0x0007
#define MDC_GFXIXCENTER         MDC_REGS_BASE + 0x22
#define MDC_GFXIYCENTER         MDC_REGS_BASE + 0x24
#define MDC_GFXIWIDTH           MDC_REGS_BASE + 0x26
#define MDC_GFXIHEIGHT          MDC_REGS_BASE + 0x28
#define MDC_GFXOXCENTER         MDC_REGS_BASE + 0x2A
#define MDC_GFXOYCENTER         MDC_REGS_BASE + 0x2C
#define MDC_GFXOWIDTH           MDC_REGS_BASE + 0x2E
#define MDC_GFXOHEIGHT          MDC_REGS_BASE + 0x30
#define MDC_GFXXLSCALE          MDC_REGS_BASE + 0x32
#define MDC_GFXXRSCALE          MDC_REGS_BASE + 0x34
#define MDC_GFXYTSCALE          MDC_REGS_BASE + 0x36
#define MDC_GFXYBSCALE          MDC_REGS_BASE + 0x38
#define MDC_GFXSHEAR            MDC_REGS_BASE + 0x3A
#define MDC_GFXROTVAL           MDC_REGS_BASE + 0x3C
#define MDC_GFXCOLOR            MDC_REGS_BASE + 0x3E
#define MDC_GFXIBADDR0          MDC_REGS_BASE + 0x40
#define MDC_GFXIBADDR1          MDC_REGS_BASE + 0x42
#define MDC_GFXOBADDR0          MDC_REGS_BASE + 0x44
#define MDC_GFXOBADDR1          MDC_REGS_BASE + 0x46
#define MDC_GFXISTRIDE          MDC_REGS_BASE + 0x48
#define MDC_GFXOSTRIDE          MDC_REGS_BASE + 0x4A
#define MDC_GFXOWLEFT           MDC_REGS_BASE + 0x4C
#define MDC_GFXOWRIGHT          MDC_REGS_BASE + 0x4E
#define MDC_GFXOWTOP            MDC_REGS_BASE + 0x50
#define MDC_GFXOWBOT            MDC_REGS_BASE + 0x52
#define MDC_DISPPRM109          MDC_REGS_BASE + 0x54
#define MDC_DISPPRM1211         MDC_REGS_BASE + 0x56
#define MDC_DISPPRM1413         MDC_REGS_BASE + 0x58
#define MDC_DISPCTL2            MDC_REGS_BASE + 0x5A
#define     MDC_REV_bits            0xFF00
#define     MDC_CSPOL_bits          0x0010
#define     MDC_VSTFALL_bits        0x0008
#define     MDC_HSTFALL_bits        0x0004
#define     MDC_ENBPHASE_bits       0x0002
#define     MDC_FSTVCLK_bits        0x0001
#define MDC_VCNTCOMP            MDC_REGS_BASE + 0x5C
#define MDC_VCNT                MDC_REGS_BASE + 0x5E
#define MDC_SCRATCHA0           MDC_REGS_BASE + 0x60
#define MDC_SCRATCHA1           MDC_REGS_BASE + 0x62
#define MDC_EPBASEADDR0         MDC_REGS_BASE + 0x64
#define MDC_EPBASEADDR1         MDC_REGS_BASE + 0x66
#define MDC_SCRATCHB            MDC_REGS_BASE + 0x68
#define MDC_VCOMCLKCTL          MDC_REGS_BASE + 0x69
#define MDC_EPCTRL              MDC_REGS_BASE + 0x6A
#define     MDC_TRIGSEL_bits        0x0006
#define     MDC_EPEN_bits           0x0001
#define MDC_PRODCODE            MDC_REGS_BASE + 0x6C
#define MDC_REVCODE             MDC_REGS_BASE + 0x6E
#define MDC_BSTCLK              MDC_REGS_BASE + 0x80
#define     MDC_BSTCLKDIV_bits       0x0070
#define     MDC_CLKSRC_bits          0x0001
#define MDC_BSTPWR              MDC_REGS_BASE + 0x84
#define     MDC_VMDBUP_bits         0x0008
#define     MDC_BSTON_bits          0x0004
#define     MDC_REGECO_bits         0x0002
#define     MDC_REGON_bits          0x0001
#define MDC_BSTVMD              MDC_REGS_BASE + 0x88
#define     MDC_VMDHVOL_bits        0x7000
#define     MDC_VMDHON_bits         0x0100
#define     MDC_VMDLVOL_bits        0x0070
#define     MDC_VMDLON_bits         0x0001


// ========== Asynchronous System Registers =================
#define SYS_CTRL               SYS_REGS_BASE + 0x00
#define     SYS_IOSCAJ_bits         0x3F00
#define     SYS_HCKINV_bits         0x0020
#define     SYS_INTPOL_bits         0x0010
#define     SYS_IOSCSTA_bits        0x0008
#define     SYS_IOSCFQ_bits         0x0006
#define     SYS_IOSCEN_bits         0x0001
#define SYS_TEST               SYS_REGS_BASE + 0x02
#define SYS_INTS               SYS_REGS_BASE + 0x04
#define     SYS_RTCINT_bits         0x0800
#define     SYS_SNDINT_bits        0x0400
#define     SYS_DMACINT_bits        0x0200
#define     SYS_REMCINT_bits        0x0100
#define     SYS_PORTINT_bits        0x0080
#define     SYS_CLGINT_bits         0x0040
#define     SYS_I2C0INT_bits        0x0020
#define     SYS_MDCINT_bits         0x0010
#define     SYS_T161INT_bits        0x0008
#define     SYS_SPI0INT_bits        0x0004
#define     SYS_T162INT_bits        0x0002
#define     SYS_QSPIINT_bits        0x0001


// ========== PORT Registers ================================
#define PORT_P0DAT              PORT_REGS_BASE + 0x00
#define     PORT_P0OUT7_bits        0x8000
#define     PORT_P0OUT6_bits        0x4000
#define     PORT_P0OUT5_bits        0x2000
#define     PORT_P0OUT4_bits        0x1000
#define     PORT_P0OUT3_bits        0x0800
#define     PORT_P0OUT2_bits        0x0400
#define     PORT_P0OUT1_bits        0x0200
#define     PORT_P0OUT0_bits        0x0100
#define     PORT_P0IN7_bits         0x0080
#define     PORT_P0IN6_bits         0x0040
#define     PORT_P0IN5_bits         0x0020
#define     PORT_P0IN4_bits         0x0010
#define     PORT_P0IN3_bits         0x0008
#define     PORT_P0IN2_bits         0x0004
#define     PORT_P0IN1_bits         0x0002
#define     PORT_P0IN0_bits         0x0001
#define PORT_P0IOEN             PORT_REGS_BASE + 0x02
#define     PORT_P0IEN7_bits        0x8000
#define     PORT_P0IEN6_bits        0x4000
#define     PORT_P0IEN5_bits        0x2000
#define     PORT_P0IEN4_bits        0x1000
#define     PORT_P0IEN3_bits        0x0800
#define     PORT_P0IEN2_bits        0x0400
#define     PORT_P0IEN1_bits        0x0200
#define     PORT_P0IEN0_bits        0x0100
#define     PORT_P0OEN7_bits        0x0080
#define     PORT_P0OEN6_bits        0x0040
#define     PORT_P0OEN5_bits        0x0020
#define     PORT_P0OEN4_bits        0x0010
#define     PORT_P0OEN3_bits        0x0008
#define     PORT_P0OEN2_bits        0x0004
#define     PORT_P0OEN1_bits        0x0002
#define     PORT_P0OEN0_bits        0x0001
#define PORT_P0RCTL             PORT_REGS_BASE + 0x04
#define     PORT_P0PDPU7_bits       0x8000
#define     PORT_P0PDPU6_bits       0x4000
#define     PORT_P0PDPU5_bits       0x2000
#define     PORT_P0PDPU4_bits       0x1000
#define     PORT_P0PDPU3_bits       0x0800
#define     PORT_P0PDPU2_bits       0x0400
#define     PORT_P0PDPU1_bits       0x0200
#define     PORT_P0PDPU0_bits       0x0100
#define     PORT_P0REN7_bits        0x0080
#define     PORT_P0REN6_bits        0x0040
#define     PORT_P0REN5_bits        0x0020
#define     PORT_P0REN4_bits        0x0010
#define     PORT_P0REN3_bits        0x0008
#define     PORT_P0REN2_bits        0x0004
#define     PORT_P0REN1_bits        0x0002
#define     PORT_P0REN0_bits        0x0001
#define PORT_P0INTF             PORT_REGS_BASE + 0x06
#define     PORT_P0IF7_bits         0x0080
#define     PORT_P0IF6_bits         0x0040
#define     PORT_P0IF5_bits         0x0020
#define     PORT_P0IF4_bits         0x0010
#define     PORT_P0IF3_bits         0x0008
#define     PORT_P0IF2_bits         0x0004
#define     PORT_P0IF1_bits         0x0002
#define     PORT_P0IF0_bits         0x0001
#define PORT_P0INTCTL           PORT_REGS_BASE + 0x08
#define     PORT_P0EDGE7            0x8000
#define     PORT_P0EDGE6            0x4000
#define     PORT_P0EDGE5            0x2000
#define     PORT_P0EDGE4            0x1000
#define     PORT_P0EDGE3            0x0800
#define     PORT_P0EDGE2            0x0400
#define     PORT_P0EDGE1            0x0200
#define     PORT_P0EDGE0            0x0100
#define     PORT_P0IE7              0x0080
#define     PORT_P0IE6              0x0040
#define     PORT_P0IE5              0x0020
#define     PORT_P0IE4              0x0010
#define     PORT_P0IE3              0x0008
#define     PORT_P0IE2              0x0004
#define     PORT_P0IE1              0x0002
#define     PORT_P0IE0              0x0001
#define PORT_P0CHATEN           PORT_REGS_BASE + 0x0A
#define     PORT_P0CHATEN7          0x0080
#define     PORT_P0CHATEN6          0x0040
#define     PORT_P0CHATEN5          0x0020
#define     PORT_P0CHATEN4          0x0010
#define     PORT_P0CHATEN3          0x0008
#define     PORT_P0CHATEN2          0x0004
#define     PORT_P0CHATEN1          0x0002
#define     PORT_P0CHATEN0          0x0001
#define PORT_P0MODSEL           PORT_REGS_BASE + 0x0C
#define     PORT_P0SEL7             0x0080
#define     PORT_P0SEL6             0x0040
#define     PORT_P0SEL5             0x0020
#define     PORT_P0SEL4             0x0010
#define     PORT_P0SEL3             0x0008
#define     PORT_P0SEL2             0x0004
#define     PORT_P0SEL1             0x0002
#define     PORT_P0SEL0             0x0001
#define PORT_P0FNCSEL           PORT_REGS_BASE + 0x0E
#define     PORT_P07MUX             0xC000
#define     PORT_P06MUX             0x0300
#define     PORT_P05MUX             0x0C00
#define     PORT_P04MUX             0x0300
#define     PORT_P03MUX             0x00C0
#define     PORT_P02MUX             0x0030
#define     PORT_P01MUX             0x000C
#define     PORT_P00MUX             0x0003
#define PORT_P1DAT              PORT_REGS_BASE + 0x10
#define     PORT_P1OUT1_bits        0x0200
#define     PORT_P1OUT0_bits        0x0100
#define     PORT_P1IN1_bits         0x0002
#define     PORT_P1IN0_bits         0x0001
#define PORT_P1IOEN             PORT_REGS_BASE + 0x12
#define     PORT_P1IEN1_bits        0x0200
#define     PORT_P1IEN0_bits        0x0100
#define     PORT_P1OEN1_bits        0x0002
#define     PORT_P1OEN0_bits        0x0001
#define PORT_P1RCTL             PORT_REGS_BASE + 0x14
#define     PORT_P1PDPU1_bits       0x0200
#define     PORT_P1PDPU0_bits       0x0100
#define     PORT_P1REN1_bits        0x0002
#define     PORT_P1REN0_bits        0x0001
#define PORT_P1INTF             PORT_REGS_BASE + 0x16
#define     PORT_P1IF1_bits         0x0002
#define     PORT_P1IF0_bits         0x0001
#define PORT_P1INTCTL           PORT_REGS_BASE + 0x18
#define     PORT_P0EDGE1            0x0200
#define     PORT_P0EDGE0            0x0100
#define     PORT_P0IE1              0x0002
#define     PORT_P0IE0              0x0001
#define PORT_P1CHATEN           PORT_REGS_BASE + 0x1A
#define     PORT_P1CHATEN1          0x0002
#define     PORT_P1CHATEN0          0x0001
#define PORT_P1MODSEL           PORT_REGS_BASE + 0x1C
#define     PORT_P1SEL1             0x0002
#define     PORT_P1SEL0             0x0001
#define PORT_P1FNCSEL           PORT_REGS_BASE + 0x1E
#define     PORT_P11MUX             0x000C
#define     PORT_P10MUX             0x0003
#define PORT_CLK                PORT_REGS_BASE + 0xE0
#define     PORT_CLKDIV_bits        0x00F0
#define     PORT_CLKSRC_bits        0x0001
#define PORT_INTFGRP            PORT_REGS_BASE + 0xE2
#define     PORT_P1INT_bits         0x0002
#define     PORT_P0INT_bits         0x0001


// ========== SND Registers ================================
#define SND_CLK                 SND_REGS_BASE + 0x00
#define     SND_CLKDIV_bits         0x0070
#define     SND_CLKSRC_bits         0x0001
#define SND_SEL                 SND_REGS_BASE + 0x02
#define     SND_STIM_bits           0x0F00
#define     SND_SINV_bits           0x0004
#define     SND_MODSEL_bits         0x0003
#define SND_CTL                 SND_REGS_BASE + 0x04
#define     SND_SSTP_bits           0x0100
#define     SND_MODEN_bits          0x0001
#define SND_DAT                 SND_REGS_BASE + 0x06
#define     SND_MDTI_bits           0x8000
#define     SND_MDRS_bits           0x4000
#define     SND_SLEN_bits           0x3F00
#define     SND_SFRQ_bits           0x00FF
#define SND_INTF                SND_REGS_BASE + 0x08
#define     SND_SBSY_bits           0x0100
#define     SND_EMIF_bits           0x0002
#define     SND_EDIF_bits           0x0001
#define SND_INTE                SND_REGS_BASE + 0x0A
#define     SND_EMIE_bits           0x0002
#define     SND_EDIF_bits           0x0001
#define SND_EMDMAEN             SND_REGS_BASE + 0x0C
#define     SND_EMDMAEN_bits        0x000F


// ========== REMC registers ================================
#define REMC_CLK               REMC_REGS_BASE + 0x00
#define     REMC_CLKDIV_bits        0x00F0
#define     REMC_CLKSRC_bits        0x0001
#define REMC_DBCTL             REMC_REGS_BASE + 0x02
#define     REMC_PRESET_bits        0x0200
#define     REMC_PRUN_bits          0x0100
#define     REMC_REMOINV_bits       0x0010
#define     REMC_BUFEN_bits         0x0008
#define     REMC_TRMD_bits          0x0004
#define     REMC_REMCRST_bits       0x0002
#define     REMC_MODEN_bits         0x0001
#define REMC_DBCNT             REMC_REGS_BASE + 0x04
#define     REMC_DBCNT_bits         0xFF0FF
#define REMC_APLEN             REMC_REGS_BASE + 0x06
#define     REMC_APLEN_bits         0xFFFF
#define REMC_DBLEN             REMC_REGS_BASE + 0x08
#define     REMC_DBLEN_bits         0xFFFF
#define REMC_INTF              REMC_REGS_BASE + 0x0A
#define     REMC_DBCNTRUN_bits      0x0400
#define     REMC_DBLENBSY_bits      0x0200
#define     REMC_FPLENBSY_bits      0x0100
#define     REMC_DBIF_bits          0x0002
#define     REMC_FPIC_bits          0x0001
#define REMC_INTE              REMC_REGS_BASE + 0x0C
#define     REMC_DBIE_bits          0x0002
#define     REMC_FPIE_bits          0x0001
#define REMC_CARR              REMC_REGS_BASE + 0x10
#define     REMC_CRDTY_bits         0xFF00
#define     REMC_CRPER_bits         0x00FF
#define REMC_CCTL              REMC_REGS_BASE + 0x12
#define     REMC_OUTINVN_bits       0x0100
#define     REMC_CARREN_bits        0x0001


#define SPI_MOD               SPI_REGS_BASE + 0x10
#define     SPI_CHLN_bits          0x0F00
#define     SPI_PUEN_bits          0x0020
#define     SPI_NOCLKDIV_bits      0x0010
#define     SPI_LSBFST_bits        0x0008
#define     SPI_CPHA_bits          0x0004
#define     SPI_CPOL_bits          0x0002
#define     SPI_MST_bits           0x0001
#define SPI_CTL               SPI_REGS_BASE + 0x12
#define     SPI_SFTRST_bits        0x0002
#define     SPI_MODEN_bits         0x0001
#define SPI_TXD               SPI_REGS_BASE + 0x14
#define SPI_RXD               SPI_REGS_BASE + 0x16
#define SPI_INTF              SPI_REGS_BASE + 0x18
#define     SPI_BSY_bits           0x0080
#define     SPI_OEIF_bits          0x0008
#define     SPI_TENDIF_bits        0x0004
#define     SPI_RBFIF_bits         0x0002
#define     SPI_TBEIF_bits         0x0001
#define SPI_INTE              SPI_REGS_BASE + 0x1A
#define     SPI_OEIE_bits          0x0008
#define     SPI_TENDIE_bits        0x0004
#define     SPI_RBIFE_bits         0x0002
#define     SPI_TBEIE_bits         0x0001
#define SPI_TBEDMAEN          SPI_REGS_BASE + 0x1C
#define     SPI_TBEDMA3EN_bits     0x0008
#define     SPI_TBEDMA2EN_bits     0x0004
#define     SPI_TBEDMA1EN_bits     0x0002
#define     SPI_TBEDMA0EN_bits     0x0001
#define SPI_RBFDMAEN          SPI_REGS_BASE + 0x1E
#define     SPI_RBDFDMA3EN_bits    0x0008
#define     SPI_RBDFDMA2EN_bits    0x0004
#define     SPI_RBDFDMA1EN_bits    0x0002
#define     SPI_RBDFDMA0EN_bits    0x0001


// ========== I2C registers =================================
#define I2C_CLK                I2C_REGS_BASE + 0x00
#define     I2C_CLKDIV_bits         0x0030
#define     I2C_CLKSRC_bits         0x0001
#define I2C_MOD                I2C_REGS_BASE + 0x02
#define     I2C_OADR10_bits        0x0004
#define     I2C_GCEN_bits           0x0002
#define I2C_BR                 I2C_REGS_BASE + 0x04
#define     I2C_BRT_bits            0x007F
#define I2C_OADR               I2C_REGS_BASE + 0x08
#define     I2C_OADR_bits          0x03FF
#define I2C_CTL                I2C_REGS_BASE + 0x0A
#define     I2C_MST_bits            0x0020
#define     I2C_TXNACK_bits         0x0010
#define     I2C_TXSTOP_bits         0x0008
#define     I2C_TXSTART_bits        0x0004
#define     I2C_SFTRST_bits         0x0002
#define     I2C_MODEN_bits          0x0001
#define I2C_TXD                I2C_REGS_BASE + 0x0C
#define     I2C_TXD_bits            0x00FF
#define I2C_RXD                I2C_REGS_BASE + 0x0E
#define     I2C_RXD_bits            0x00FF
#define I2C_INTF               I2C_REGS_BASE + 0x10
#define     I2C_SDALOW_bits         0x1000
#define     I2C_SCALOW_bits         0x0800
#define     I2C_BSY_bits            0x0400
#define     I2C_TR_bits             0x0200
#define     I2C_BYTEENDIF_bits      0x0080
#define     I2C_GCIF_bits           0x0040
#define     I2C_NACKIF_bits         0x0020
#define     I2C_STOPIF_bits         0x0010
#define     I2C_STARTIF_bits        0x0008
#define     I2C_ERRIF_bits          0x0004
#define     I2C_RBFIF_bits          0x0002
#define     I2C_TBEIF_bits          0x0001
#define I2C_INTE               I2C_REGS_BASE + 0x12
#define     I2C_BYTENDIE_bits       0x0080
#define     I2C_GCIE_bits           0x0040
#define     I2C_NACKIE_bits         0x0020
#define     I2C_STOPIE_bits         0x0010
#define     I2C_STARTIE_bits        0x0008
#define     I2C_ERRIE_bits          0x0004
#define     I2C_RBFIE_bits          0x0002
#define     I2C_TBEIE_bits          0x0001
#define I2C_TBEDMAEN           I2C_REGS_BASE + 0x14
#define     I2C_TBEDMAEN_bits       0x000F
#define I2C_RBFDMAEN           I2C_REGS_BASE + 0x16
#define     I2C_RBFDMAEN_bits       0x000F

#define T16_CLK_OFFSET          0x00
#define     T16_CLKDIV_bits         0x00F0
#define     T16_CLKSRC_bits         0x0001
#define T16_MOD_OFFSET          0x02
#define     T16_TRMD_bits           0x0001
#define T16_CTL_OFFSET          0x04
#define     T16_PRUN_bits           0x0100
#define     T16_PRESET_bits         0x0002
#define     T16_MODEN_bits          0x0001
#define T16_TR_OFFSET           0x06
#define     T16_TR_bits             0xFFFF
#define T16_TC_OFFSET           0x08
#define     T16_TC_bits             0xFFFF
#define T16_INTF_OFFSET         0x0A
#define     T16_UFIF_bits          0x0001
#define T16_INTE_OFFSET         0x0C
#define     T16_UFIE_bits           0x0001


#define QSPI_MOD                QSPI_REGS_BASE + 0x00
#define     QSPI_CHDL               0xF000
#define     QSPI_CHLN               0x0F00
#define     QSPI_TMOD               0x00C0
#define     QSPI_PUEN               0x0020
#define     QSPI_NOCLKDIV           0x0010
#define     QSPI_LSBFST             0x0008
#define     QSPI_CPHA               0x0004
#define     QSPI_CPOL               0x0002
#define     QSPI_MST                0x0001
#define QSPI_CTL               QSPI_REGS_BASE + 0x02
#define     QSPI_DIR                0x0008
#define     QSPI_MSTSSO             0x0004
#define     QSPI_SFTRST             0x0002
#define     QSPI_MODEN              0x0001
#define QSPI_TXD               QSPI_REGS_BASE + 0x04
#define QSPI_RXD               QSPI_REGS_BASE + 0x06
#define QSPI_INTF              QSPI_REGS_BASE + 0x08
#define     QSPI_INTF_BSY           0x0080
#define     QSPI_INTF_MMABSY        0x0040
#define     QSPI_INTF_OEIF          0x0008
#define     QSPI_INTF_TENDIF        0x0004
#define     QSPI_INTF_RBFIF         0x0002
#define     QSPI_INTF_TBEIF         0x0001
#define QSPI_INTE              QSPI_REGS_BASE + 0x0A
#define     QSPI_INTE_OEIE          0x0008
#define     QSPI_INTE_TENDIE        0x0004
#define     QSPI_INTE_RBFIE         0x0002
#define     QSPI_INTE_TBEIE         0x0001
#define QSPI_TBEDMAEN          QSPI_REGS_BASE + 0x0C
#define     QSPI_TBEDMAEN3          0x0008
#define     QSPI_TBEDMAEN2          0x0004
#define     QSPI_TBEDMAEN1          0x0002
#define     QSPI_TBEDMAEN0          0x0001
#define QSPI_RBFDMAEN          QSPI_REGS_BASE + 0x0E
#define     QSPI_RBFDMAEN3          0x0008
#define     QSPI_RBFDMAEN2          0x0004
#define     QSPI_RBFDMAEN1          0x0002
#define     QSPI_RBFDMAEN0          0x0001
#define QSPI_FRLDMAEN          QSPI_REGS_BASE + 0x10
#define     QSPI_FRLDMAEN3          0x0008
#define     QSPI_FRLDMAEN2          0x0004
#define     QSPI_FRLDMAEN1          0x0002
#define     QSPI_FRLDMAEN0          0x0001
#define QSPI_MMACFG1           QSPI_REGS_BASE + 0x12
#define     QSPI_TCSH               0x000F
#define QSPI_RMADRH            QSPI_REGS_BASE + 0x14
#define     QSPI_RMADR              0xFFF0
#define QSPI_MMACFG2           QSPI_REGS_BASE + 0x16
#define     QSPI_DUMDL              0xF000
#define     QSPI_DUMLN              0x0F00
#define     QSPI_DATTMOD            0x00C0
#define     QSPI_DUMTMOD            0x0030
#define     QSPI_ADRTMOD            0x000C
#define     QSPI_ADRCYC             0x0002
#define     QSPI_MMAEN              0x0001
#define QSPI_MB                QSPI_REGS_BASE + 0x18
#define     QSPI_XIPACT             0xFF00
#define     QSPI_XIPEXT             0x00FF


// ========== DMAC registers ================================
#define DMAC_STAT               DMA_REGS_BASE + 0x00
#define     DMAC_CHNLS_bits         0x001F0000
#define     DMAC_STATE_bits         0x000000F0
#define     DMAC_MSTENSTATE_bits    0x00000001
#define DMAC_CFG                DMA_REGS_BASE + 0x04
#define     DMAC_MSTEN_bits         0x00000001
#define DMAC_CPTR               DMA_REGS_BASE + 0x08
#define     DMAC_CPTR_bits          0xFFFFFFFF
#define DMAC_ACPTR              DMA_REGS_BASE + 0x0C
#define     DMAC_ACPTR_bits         0xFFFFFFFF
#define DMAC_SWREQ              DMA_REGS_BASE + 0x14
#define     DMAC_SWREQ_bits         0x0000000F
#define DMAC_RMSET              DMA_REGS_BASE + 0x20
#define     DMAC_RMSET_bits         0x0000000F
#define DMAC_RMCLR              DMA_REGS_BASE + 0x24
#define     DMAC_RMCLR_bits         0x0000000F
#define DMAC_ENSET              DMA_REGS_BASE + 0x28
#define     DMAC_ENSET_bits         0x0000000F
#define DMAC_ENCLR              DMA_REGS_BASE + 0x2C
#define     DMAC_ENCLR_bits         0x0000000F
#define DMAC_PASET              DMA_REGS_BASE + 0x30
#define     DMAC_PASET_bits         0x0000000F
#define DMAC_PACLR              DMA_REGS_BASE + 0x34
#define     DMAC_PACLR_bits         0x0000000F
#define DMAC_PRSET              DMA_REGS_BASE + 0x38
#define     DMAC__PRSET_bits        0x0000000F
#define DMAC_PRCLR              DMA_REGS_BASE + 0x3C
#define     DMAC_PRCLR_bits         0x0000000F
#define DMAC_ERRIF              DMA_REGS_BASE + 0x4C
#define     DMAC_ERRIF_bits         0x00000001
#define DMAC_ENDIF              DMA_REGS_BASE + 0x1000
#define     DMAC_ENDIF_bits         0x0000000F
#define DMAC_ENDIESET           DMA_REGS_BASE + 0x1008
#define     DMAC_ENDIESET_bits      0x0000000F
#define DMAC_ENDIECLR           DMA_REGS_BASE + 0x100C
#define     DMAC_ENDIECLR_bits      0x0000000F
#define DMAC_ERRIESET           DMA_REGS_BASE + 0x1010
#define     DMAC_ERRIESET_bits      0x00000001
#define DMAC_ERRIECLR           DMA_REGS_BASE + 0x1014
#define     DMAC_ERRIECLR_bits      0x00000001


/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

#define C00_SYS          		SYS_REGS_BASE
#define C00_CLG                 CLG_REGS_BASE
#define C00_DMAC                DMAC_REGS_BASE
#define C00_RTC                 RTC_REGS_BASE
#define C00_T16_1               T16_1_REGS_BASE
#define C00_T16_2               T16_2_REGS_BASE
#define C00_PORT                PORT_REGS_BASE
#define C00_SPI                 SPI_REGS_BASE
#define C00_I2C                 I2C_REGS_BASE
#define C00_SND                 SND_REGS_BASE
#define C00_REMC                REMC_REGS_BASE
#define C00_USB                 USB_REGS_BASE
#define C00_QSPI                QSPI_REGS_BASE
#define C00_MDC                 MDC_REGS_BASE

#endif // seMEMREGS_H_INCLUDED
