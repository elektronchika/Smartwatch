/**
  ******************************************************************************
  * @file    se_mdc.h
  * @author  Epson
  * @version V1.0
  * @date    23-October-2017
  * @brief   This file contains the functions prototypes for the MDC library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef S1D13C00_MDC_H
#define S1D13C00_MDC_H

#ifdef __cplusplus
 extern "C" {
#endif

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#endif


/** @addtogroup sePeriphLibrary
  * @{
  */

/** @defgroup MDC
  * @{
  * @brief The Memory Display Controller (MDC) provides hardware acceleration for interfacing to various memory display panels,
  * drawing lines/ellipses/rectangles, and copying images/bitmaps with rotation/scaling and horizontal/vertical shear.
  *
  */

/** @defgroup MDC_Constants
  * @{
  */

typedef enum {
     seMDC_IOSC             = 0,        ///< IOSC is a CLK source.
     seMDC_OSC1             = 1         ///< OSC1 is a CLK source.
 } seMDC_ClkSrc;

typedef enum {
    seMDC_VCOMEN_DISABLE    = 0U,       ///< VCOM disable
    seMDC_VCOMEN_ENABLE     = 1U        ///< VCOM enable
} seMDC_DISP_VCOMEN;

typedef enum {
    seMDC_DISP_ROT0         = 0U,       ///< Rotate display 0 degrees
    seMDC_DISP_ROT90        = 1U,       ///< Rotate display 90 degrees
    seMDC_DISP_ROT180       = 2U,       ///< Rotate display 180 degrees
    seMDC_DISP_ROT270       = 3U        ///< Rotate display 270 degrees
} seMDC_DISP_ROTSEL;

typedef enum {
    seMDC_DISP_PANEL6BIT    = 0U,       ///< 6-bit color panel interface
    seMDC_DISP_PANELSPI     = 1U        ///< SPI panel interface
} seMDC_DISP_NOTGSPANEL;

typedef enum {
    seMDC_DISP_SPI1BIT      = 0U,       ///< 1-bit black-and-white
    seMDC_DISP_SPI3BIT      = 1U        ///< 3-bit color
} seMDC_DISP_SPITYPE;

typedef enum {
    seMDC_DISP_UPD          = 0U,       ///< Frame update
    seMDC_DISP_CMD          = 1U        ///< Command write
} seMDC_DISP_UPDFUNC;

typedef enum {
    seMDC_DISP_PIXNORMAL    = 0U,       ///< pixel normal
    seMDC_DISP_PIXINVERT    = 1U        ///< pixel invert
} seMDC_DISP_INVERT;

typedef enum {
    seMDC_DISP_AUTOCOMOFF   = 0U,       ///< Automatic generation of COM bit for SPI panels is OFF
    seMDC_DISP_AUTOCOMON    = 1U        ///< Automatic generation of COM bit for SPI panels is ON
} seMDC_DISP_SPIAUTOCOM;

typedef enum {
    seMDC_DISP_SPIMSBFIRST  = 0U,       ///< Address MSB first
    seMDC_DISP_SPILSBFIRST  = 1U        ///< Address LSB first
} seMDC_DISP_SPIADDRLSB;

typedef enum {
    seMDC_DISP_SPIRGB       = 0U,       ///< R is sent first
    seMDC_DISP_SPIBGR       = 1U        ///< B is sent first
} seMDC_DISP_SPIRGBORD;

typedef enum {
    seMDC_DISP_PANELNOTGS   = 0U,       ///< Not grayscale panel interface
    seMDC_DISP_PANELGS      = 1U        ///< Grayscale panel interface
} seMDC_DISP_GSPANEL;

typedef enum {
    seMDC_DISP_VCKNORM      = 0,        ///< VCK pulses are normal
    seMDC_DISP_VCKFAST      = 1U        ///< VCK pulses are fast for the lines that are not updated
} seMDC_DISP_FASTVCK;

typedef enum {
    seMDC_DISP_ENBLSB       = 0,        ///< ENB pulses start on first LSB pixel data
    seMDC_DISP_ENBMSB       = 1U        ///< ENB pulses start on first MSB pixel data
} seMDC_DISP_ENBPHASE;

typedef enum {
    seMDC_DISP_HSTFALL      = 0,        ///< HST falling edge is after HCK fall
    seMDC_DISP_HSTRISE      = 1U        ///< HST falling edge is after HCK rise
} seMDC_DISP_HST;

typedef enum {
    seMDC_DISP_VSTFALL      = 0,        ///< VST falling edge is after VCK fall
    seMDC_DISP_VSTRISE      = 1U        ///< VST falling edge is after VCK rise
} seMDC_DISP_VST;

typedef enum {
    seMDC_DISP_CSPOLNORM    = 0,        ///< Normal chip select polarity
    seMDC_DISP_CSPOLINV     = 1U        ///< Inverted chip select polarity
} seMDC_DISP_CSPOL;

typedef enum {
    seMDC_DISP_GSALPHAOFF   = 0,        ///< Grayscale alpha disabled
    seMDC_DISP_GSALPHAON    = 1U        ///< Grayscale alpha enabled
} seMDC_DISP_GSALPHA;



typedef enum {
    seMDC_VMDH_4V3          = 0U,       ///< 4.4V
    seMDC_VMDH_4V4          = 1U,       ///< 4.5V
    seMDC_VMDH_4V5          = 2U,       ///< 4.5V
    seMDC_VMDH_4V6          = 3U,       ///< 4.6V
    seMDC_VMDH_4V7          = 4U,       ///< 4.7V
    seMDC_VMDH_4V8          = 5U,       ///< 4.8V
    seMDC_VMDH_4V9          = 6U,       ///< 4.9V
    seMDC_VMDH_5V0          = 7U        ///< 5.0V
} seMDC_VMDHVOL;

typedef enum {
    seMDC_VMDL_2V3          = 0U,       ///< 2.3V
    seMDC_VMDL_2V7          = 1U,       ///< 2.7V
    seMDC_VMDL_3V0          = 2U,       ///< 3.0V
    seMDC_VMDL_3V1          = 3U,       ///< 3.1V
    seMDC_VMDL_3V2          = 4U,       ///< 3.2V
    seMDC_VMDL_3V3          = 5U,       ///< 3.3V
    seMDC_VMDL_3V4          = 6U,       ///< 3.4V
    seMDC_VMDL_3V6          = 7U        ///< 3.6V
} seMDC_VMDLVOL;

typedef enum {
   seMDC_GS_1BPP                = 0U,     ///< 1 bit per pixel
   seMDC_GS_2BPP                = 1U,     ///< 2 bits per pixel
   seMDC_GS_4BPP                = 2U,     ///< 4 bits per pixel
   seMDC_GS_8BPP                = 3U      ///< 8 bits per pixel
} seMDC_GS_BPP;

typedef enum
{
    seMDC_1BIT_BITMAP            = 0U,     ///< 1-bit bitmap
    seMDC_2BBIT_BITMAP           = 1U,     ///< 2-bit bitmap
    seMDC_1BPP_GRSCL             = 2U,     ///< 1 bit per pixel
    seMDC_2BPP_GRSCL             = 3U,     ///< 2 bits per pixel
    seMDC_4BPP_GRSCL             = 4U,     ///< 4 bits per pixel
    seMDC_8BPP_GRSCL             = 5U,     ///< 8 bits per pixel
    seMDC_SPI1BIT_BW             = 6U,     ///< 1-bit black-and-white
    seMDC_SPI3BIT_COLOR          = 7U,     ///< 3-bit color
    seMDC_6BIT_COLOR             = 8U      ///< 6-bit color
} seMDC_IMGTYPE;

typedef enum {
   seMDC_BITMAP_1BIT            = 0U,     ///< 1-bit format
   seMDC_BITMAP_2BIT            = 1U      ///< 2-bit format
} seMDC_BITMAPFMT;

typedef enum {
   seMDC_FUNC_COPYROTSCALE      = 0U,     ///< Image/bitmap copy with rotation and scaling
   seMDC_FUNC_COPYHVSHEAR       = 1U,     ///< Image/bitmap copy with horizontal and vertical shear
   seMDC_FUNC_RECTDRAW          = 2U,     ///< Rectangle draw
   seMDC_FUNC_LINEDRAW          = 3U,     ///< Line draw
   seMDC_FUNC_ELLIPDRAW         = 4U,     ///< Ellipse draw
   seMDC_FUNC_RESERVED1         = 5U,     ///< Reserved
   seMDC_FUNC_DISPUPDATE        = 6U,     ///< Panel display update
   seMDC_FUNC_RESERVED2         = 7U      ///< Reserved
} seMDC_GFXFUNC;

#define FILL_ENABLE            1          ///< Enable fill for drawing and image copy
#define FILL_DISABLE           0          ///< Disable fill for drawing and image copy


/**
  * @}
  */ // MDC_Constants


/** @defgroup MDC_Types
  * @{
  */

/**
  * @brief  MDC Image Structure
  */
typedef struct {
    uint16_t width;                       ///< Image Width
    uint16_t height;                      ///< Image Height
    uint16_t stride;                      ///< Image Stride (typically same as width)
    seMDC_IMGTYPE imgtype;                ///< Image type (seMDC_IMGTYPE)
    uint8_t *pxdata;                      ///< Pixel data
} seMDC_ImgStruct;


/** 
  * @brief  MDC Structure for image stored in external serial flash (QSPI memory-mapped access)
  */
typedef struct {
  uint16_t rmadrh;                        ///< Value to write to QSPI RMADRH register
  seMDC_ImgStruct *img;                   ///< Pointer to image structure
} seMDC_SerFlashImgStruct;

/**
  * @brief  MDC Destination Window Parameters
  */
typedef struct {
    union {
        uint32_t obaseaddr;             ///< Destination Window Base Address
        struct {
            uint16_t obaseaddr0;        ///< Destination Window Base Address 0
            uint16_t obaseaddr1;        ///< Destination Window Base Address 1
        } obaseaddr_b;                  ///< Bit field access
    };

    uint16_t owidth;                    ///< Destination Window Width
    uint16_t oheight;                   ///< Destination Window Height
    uint16_t ostride;                   ///< Destination Window Stride
} seMDC_DestWindowParams;

/**
  * @brief  MDC Image/Bitmap Copy with Rotation and Scaling Control bits
  */

typedef union {
  uint16_t  ctrlword;                   ///< Control word

  struct {
    uint16_t  gfxfunc    :  3;          ///< These bits specify the graphics accelerator function to execute
    uint16_t  RES4_3     :  2;          ///< Reserved bits
    uint16_t  alphaovrrd :  1;          ///< This bit enables/disables alpha value override.
    uint16_t  alphaovrval:  2;          /**< When alpha value override is enabled (alphaovrrd=1), this specifies
                                                   the override alpha value to use. */
    uint16_t  RES8       :  1;          ///< Reserved bit
    uint16_t  bitmapen   :  1;          ///< This bit specifies whether the source is an image or bitmap
    uint16_t  bitmapfmt  :  1;          ///< This bit selects 1-bit or 2-bit bitmap format (when bitmapen = 1)
    uint16_t  fillen     :  1;          ///< This bit enables/disables fill option
    uint16_t  RES13_12   :  2;          ///< Reserved bits
    uint16_t  cpynegx    :  1;          /**< This bit selects whether or not the X coordinate value of the input pixel
                                                should be negated (make negative) to create the effect of
                                                "flip around the Y axis". */
    uint16_t  cpynegy    :  1;          /**< This bit selects whether or not the Y coordinate value of the input pixel
                                                should be negated (make negative) to create the effect of
                                                "flip around the X axis". */
  } ctrlword_b;                         ///< Bit field access
} seMDC_ImgCopyRotScaleCtrl;


/**
  * @brief  MDC Image/Bitmap Copy with Horizontal and Vertical Shear Control Parameters
  */

typedef struct {
  union {
    uint16_t shearctrl;                 ///< Shear control
    struct {
      uint8_t xshear;                   ///< Horizontal shear value
      uint8_t yshear;                   ///< Vertical shear value
    } shearctrl_b;                      ///< Bit field access
  };

  union {
    uint16_t  ctrlword;                 ///< Control word

    struct {
      uint16_t  gfxfunc    :  3;        ///< These bits specify the graphics accelerator function to execute
      uint16_t  RES4_3     :  2;        ///< Reserved bits
      uint16_t  alphaovrrd :  1;        ///< This bit enables/disables alpha value override.
      uint16_t  alphaovrval:  2;        /**< When alpha value override is enabled (alphaovrrd=1), this specifies
                                                the override alpha value to use. */
      uint16_t  RES8       :  1;        ///< Reserved bit
      uint16_t  bitmapen   :  1;        ///< This bit specifies whether the source is an image or bitmap
      uint16_t  bitmapfmt  :  1;        ///< This bit selects 1-bit or 2-bit bitmap format (when bitmapen = 1)
      uint16_t  fillen     :  1;        ///< This bit enables/disables fill option
      uint16_t  shearnegx  :  1;        ///< This bit selects whether or not the horizontal shear value is negated.
      uint16_t  shearnegy  :  1;        ///< This bit selects whether or not the vertical shear value is negated.
      uint16_t  cpynegx    :  1;        /**< This bit selects whether or not the X coordinate value of the input pixel
                                                should be negated (make negative) to create the effect of
                                                "flip around the Y axis". */
      uint16_t  cpynegy    :  1;        /**< This bit selects whether or not the Y coordinate value of the input pixel
                                                should be negated (make negative) to create the effect of
                                                "flip around the X axis". */
    } ctrlword_b;                       ///< Bit field access
  };

} seMDC_ImgCopyHVShearCtrl;


/**
  * @}
  */ // MDC_Types


/** @defgroup MDC_Functions
  * @{
  */

/**
  * @brief  Initialize Panel Interface for LPM011M133B (218x218, 6-bit color, 1.1" round)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_LPM011M133B ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface for LPM012M134B (240x240, 6-bit color, 1.2" round)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_LPM012M134B ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface for LS010B7DH01 (128x128, SPI, 1-bit BW, 1.0" round)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_LS010B7DH01 ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface for LS012B7DD06 (240x240, 6-bit color, 1.2" round)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_LS012B7DD06 ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface for LSxxxxxxxxx (260x260, 6-bit color, 1.2" round)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_6BIT260X260 ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface for LS013B4DN04 (96x96, SPI, 1-bit BW, 1.3" square)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_LS013B4DN04 ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface for LS013B7DH06 (128x128, SPI, 3-bit color, 1.33" square)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_LS013B7DH06 ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface for LS012B7DH02 (240x240, SPI, 1-bit BW, 1.2" square)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_LS012B7DH02 ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface for LPM013M126C (176x176, SPI, 3-bit color, 1.28" square)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_LPM013M126C ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface for LPM027M128A (400x240, SPI, 3-bit color, 2.7" rectangular)
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel_LPM027M128A ( uint32_t sysfreqhz, uint32_t framebuffaddr );

/**
  * @brief  Initialize Panel Interface
  * @param  sysfreqhz:  system clock frequency in hertz
  * @param  framebuffaddr:  frame buffer base address
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_InitPanel( uint32_t sysfreqhz, uint32_t framebuffaddr );


/**
  * @brief  Power-on sequencer for 6-bit panel.
  * @retval None
  */
void seMDC_PwrOnSeq6Bit ( void );


/**
  * @brief  Update the panel with image from frame/display buffer
  * @param  startline: Starting line number (0 is first line).
  * @param  endline: Endng line number
  * @note   For full screen update, startline=0 and endline=(<display height>-1).
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_PanelUpdate( uint16_t startline, uint16_t endline );

/**
  * @brief  Change VCOM divider value
  * @param  vcomval: new divider value
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_VCOMChangeDivider( uint16_t vcomval );

/**
  * @brief  Set Destination Window
  * @param  destwinparams_ptr: Destination Window parameters descriptor. Must be aligned.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_SetDestWindow( seMDC_DestWindowParams * destwinparams_ptr );

/**
  * @brief  Trigger graphics engine line draw.  Does not check for completion (caller is responsible).
  * @param  point1x: POINT1 X coordinate.
  * @param  point1y: POINT1 Y coordinate.
  * @param  point2x: POINT2 X coordinate.
  * @param  point2y: POINT2 Y coordinate.
  * @param  pencolor: pen color.
  * @param  thickness: line thickness.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_DrawLine( uint16_t point1x, uint16_t point1y, uint16_t point2x, uint16_t point2y,
                         uint16_t pencolor, uint16_t thickness );

/**
  * @brief  Trigger graphics engine rectangle draw.  Does not check for completion (caller is responsible).
  * @param  tlcornerx: Top left corner X coordinate.
  * @param  tlcornery: Top left corner Y coordinate.
  * @param  brcornerx: Bottom right corner X coordinate.
  * @param  brcornery: Bottom right corner Y coordinate.
  * @param  pencolor: pen color.
  * @param  vlinethick: vertical line thickness.
  * @param  hlinethick: horizontal line thickness.
  * @param  fillenable: Fill Enable.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_DrawRectangle( uint16_t tlcornerx, uint16_t tlcornery, uint16_t brcornerx, uint16_t brcornery,
                              uint16_t pencolor, uint16_t vlinethick, uint16_t hlinethick, uint8_t fillenable);

/**
  * @brief  Trigger graphics engine ellipse draw.  Does not check for completion (caller is responsible).
  * @param  centerx: Center X coordinate.
  * @param  centery: Center Y coordinate.
  * @param  radiusx: X-axis radius
  * @param  radiusy: Y-axis radius.
  * @param  pencolor: pen color.
  * @param  xcrossthick: X-axis crossing thickness.
  * @param  ycrossthick: Y-axis crossing thickness.
  * @param  fillenable: Fill Enable.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_DrawEllipse( uint16_t centerx, uint16_t centery, uint16_t radiusx, uint16_t radiusy,
                            uint16_t pencolor, uint16_t xcrossthick, uint16_t ycrossthick, uint8_t fillenable);

/**
  * @brief  Trigger graphics engine image/bitmap copy with rotation and scaling.  Does not check for completion (caller is responsible).
  * @param  ocenterx:  Destination Window center X coordinate.
  * @param  ocentery:  Destination Window center Y coordinate.
  * @param  ibaseaddr:  Source Window base address.
  * @param  istride:  Source Window stride.
  * @param  iwidth:  Source Window width.
  * @param  iheight:  Source Window height.
  * @param  icenterx:  Source Window center of transformation X coordinate.
  * @param  icentery:  Source Window center of transformation Y coordinate.
  * @param  fillcolor:  Fill color (if FILL is enabled).
  * @param  rotval:  Rotation value
  * @param  xlscale:  Left half of source image scaling value
  * @param  xrscale:  Right half of source image scaling value
  * @param  ytscale:  Top half of source image scaling value
  * @param  ybscale:  Bottom half of source image scaling value
  * @param  ctrl_ptr:  Pointer to control word.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_ImgCpyRotScale( uint16_t ocenterx, uint16_t ocentery,
                               uint32_t ibaseaddr, uint16_t istride, uint16_t iwidth, uint16_t iheight,
                               uint16_t icenterx, uint16_t icentery,
                               uint16_t fillcolor, uint16_t rotval,
                               uint16_t xlscale, uint16_t xrscale, uint16_t ytscale, uint16_t ybscale,
                               seMDC_ImgCopyRotScaleCtrl * ctrl_ptr);

/**
  * @brief  Trigger graphics engine image/bitmap copy with horizontal and vertical shear.  Does not check for completion (caller is responsible).
  * @param  ocenterx:  Destination Window center X coordinate.
  * @param  ocentery:  Destination Window center Y coordinate.
  * @param  ibaseaddr:  Source Window base address.
  * @param  istride:  Source Window stride.
  * @param  iwidth:  Source Window width.
  * @param  iheight:  Source Window height.
  * @param  icenterx:  Source Window center of transformation X coordinate.
  * @param  icentery:  Source Window center of transformation Y coordinate.
  * @param  fillcolor:  Fill color (if FILL is enabled).
  * @param  ctrl_ptr:  Pointer to control parameters
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_ImgCpyHVShear( uint16_t ocenterx, uint16_t ocentery,
                              uint32_t ibaseaddr, uint16_t istride, uint16_t iwidth, uint16_t iheight,
                              uint16_t icenterx, uint16_t icentery,
                              uint16_t fillcolor,
                              seMDC_ImgCopyHVShearCtrl * ctrl_ptr);

/**
  * @brief  Wait for graphics engine to complete by checking MDC interrupt status.
  * @retval None
  */
void seMDC_WaitGfxDone( void );

/**
  * @brief  Wait for display updater to complete by checking MDC interrupt status.
  * @retval None
  */
void seMDC_WaitUpdDone( void );

/**
  * @brief  Select MDC clock source
  * @param  clock:  Clock source. Can be any of @ref seMDC_ClkSrc.
  * @retval Status: None.
  */
void seMDC_SelectClkSrc( seMDC_ClkSrc clock );

/**
  * @}
  */ // MDC_Functions

/**
  * @}
  */  // MDC

/**
  * @}
  */ // sePeriphLibrary

#if defined(__CC_ARM)
  #pragma pop
#endif

#ifdef __cplusplus
}
#endif
#endif	// S1D13C00_MDC_H
