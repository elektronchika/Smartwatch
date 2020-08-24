/**
  ******************************************************************************
  * @file    semdc_gfx.h
  * @author  Epson
  * @version V1.0  
  * @date    25-September-2017
  * @brief   This file contains the function prototypes for the MDC graphics library.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#ifndef SEMDC_GFX_H
#define SEMDC_GFX_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @defgroup seGraphicsLibrary seGraphicsLibrary
  * @{
  * @brief The S1D13C00 Graphics Library provides routines for drawing and displaying text on the memory LCD panel.
  */ 
   
/** @defgroup MDC_GFX_Constants
  * @{
  */ 
   
typedef enum {
   seMDC_GFX_CENTER_JUSTIFIED   = 0U,     ///< Center justification
   seMDC_GFX_LEFT_JUSTIFIED     = 1U,     ///< Left justification
   seMDC_GFX_RIGHT_JUSTIFIED    = 2U      ///< Right justification
} seMDC_GFX_STR_JUSTIFICATION;

typedef enum {
   seMDC_GFX_ROTSTRING          = 0U,     ///< Rotate whole string
   seMDC_GFX_ROTCHAR            = 1U      ///< Rotate individual characters
} seMDC_GFX_STR_ROTTYPE;



/**
  * @}
  */  

/** @defgroup MDC_GFX_Types
  * @{ 
  */

/** 
  * @brief  MDC font character structure.
  */

typedef struct {
   uint16_t width;                      ///< Character width
   uint32_t offsetloc;                  ///< Offset location of character from base of pixel data array
} seMDC_GFX_FontChar;


/** 
  * @brief  MDC font set structure.  Supports variable-width font characters.
  */

typedef struct {
   seMDC_BITMAPFMT bitmapfmt;           ///< Bitmap format:  0=1-bit, 1=2-bit
   uint16_t height;                     ///< Height of font characters
   uint32_t numchars;                   ///< Number of characters
   uint32_t unicode_base;               ///< Unicode start character
   seMDC_GFX_FontChar *charstbl;        ///< Pointer to table of descriptors for each font character
   uint8_t *pxdata;                     ///< Pointer to font set pixel data array
} seMDC_GFX_FontStruct;

/** 
  * @brief  MDC Structure for font stored in external serial flash (QSPI memory-mapped access)
  */
typedef struct {
  uint32_t rmadrh;                            ///< Value to write to QSPI RMADRH register
  seMDC_GFX_FontStruct *font;                 ///< Pointer to font structure
} seMDC_GFX_SerFlashFontStruct;


/** 
  * @brief  MDC string display parameters structure
  */
typedef struct {
   uint16_t destx;                      ///< Destination X coordinate
   uint16_t desty;                      ///< Destination Y coordinate
   seMDC_GFX_FontStruct *font1;         ///< Pointer to font1 structure
   uint8_t extloc1;                     ///< Font1 location:  0=internal (Host MCU), 1=external (memory-mapped serial flash)
   seMDC_GFX_FontStruct *font2;         ///< Pointer to font2 structure
   uint8_t extloc2;                     ///< Font2 location:  0=internal (Host MCU), 1=external (memory-mapped serial flash)
   uint16_t textcolor;                  ///< Color of text
   uint16_t xscale;                     ///< X scaling value, scale factor = xscale/256
   uint16_t yscale;                     ///< Y scaling value, scale factor = yscale/256
   seMDC_GFX_STR_JUSTIFICATION justify; ///< Justification setting
   seMDC_GFX_STR_ROTTYPE rotation;      ///< Rotation type, whole string or individual characters
   uint16_t rotval;                     ///< Rotation value, rotation is rotval*360/512 degrees counterclockwise
   uint32_t extbuffaddr;                ///< Address of external buffer (RAM) for copying
   seDMAC_CHANNEL dmachan;              ///< DMA channel used for copying data
} seMDC_GFX_PutStr_Params;


/** 
  * @brief  MDC analog clock minutes/seconds ticks drawing parameters structure
  */
typedef struct {
   uint16_t leftedge;                   ///< left edge coordinate of analog clock boundary
   uint16_t rightedge;                  ///< right edge coordinate of analog clock boundary
   uint16_t topedge;                    ///< top edge coordinate of analog clock boundary
   uint16_t botedge;                    ///< bottom edge coordinate of analog clock boundary
   uint8_t starttimeangle;              ///< starting time angle (0 to 59), clockwise from 12:00
   uint8_t endtimeangle;                ///< ending time angle (0 to 59), clockwise from 12:00
   uint16_t minorlength;                ///< minor ticks length (must > 0)
   uint16_t majorlength;                ///< major ticks (multiples of 5 minutes)length  (must be >0)
   uint16_t minorthickness;             ///< minor ticks line thickness
   uint16_t majorthickness;             ///< major ticks line thickness
   uint16_t minorlinecolor;             ///< minor ticks line color
   uint16_t majorlinecolor;             ///< major ticks line color
} seMDC_GFX_ClockTicksStruct;


/** 
  * @brief  MDC polygon outline drawing parameters structure
  */
typedef struct {
   int16_t xcoord;                      ///< X coordinate
   int16_t ycoord;                      ///< Y coordinate
} seMDC_GFX_Point;


/** 
  * @brief  MDC polygon outline drawing parameters structure
  */
typedef struct {
   uint16_t numpoints;                  ///< number of points in polygon
   uint16_t pencolor;                   ///< pen color
   uint16_t thickness;                  ///< line thickness
   int16_t centerx;                     ///< polygon center X coordinate
   int16_t centery;                     ///< polygon center Y coordinate
   uint16_t scaleval;                   ///< scaling value, scaling is (scaleval/256).
   uint16_t rotval;                     ///< rotation value, rotation is (rotval*360)/512 degrees counter clockwise
   uint16_t roundtips;                  ///< line segments have round tips or not
   seMDC_GFX_Point points[];            ///< array of points relative to center
} seMDC_GFX_PolygonStruct;


/**
  * @}
  */  


/** @defgroup MDC_GFX_Functions
  * @{
  */

/**
  * @brief  Calculate X/Y coordinate from center coordinate, radius, and time angle.
  * @param  centerx:  Center X coordinate
  * @param  centery:  Center Y coordinate
  * @param  radius:  Radius
  * @param  rotval:  Clock rotation value 0-59 (sec/min)
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_GFX_Rotval2XY ( int16_t centerx, int16_t centery, uint16_t radius, uint16_t rotval,
                               uint16_t *xval, uint16_t *yval );

/**
  * @brief  Draw a polar line (for analog clock hands/ticks).
  * @param  centerx:  Center X coordinate
  * @param  centery:  Center Y coordinate
  * @param  radius1:  Radius 1
  * @param  radius2:  Radius 2
  * @param  timeangle:  Time angle (0 to 59), clockwise from 12:00.
  * @param  linecolor:  Line color
  * @param  linethickness:  Line thickness
  * @param  xval: returns the X value for the center of rotation
  * @param  yval: returns the Y value for the center of rotation
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_GFX_ClockLine ( int16_t centerx, int16_t centery, uint16_t radius1, uint16_t radius2,
                               uint8_t timeangle, uint16_t linecolor, uint16_t linethickness );

/**
  * @brief  Draw analog clock circular minutes/seconds ticks.
  * @param  clockticks:  clock ticks structure of type @ref seMDC_GFX_ClockTicksStruct
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_GFX_DrawClockTicks ( seMDC_GFX_ClockTicksStruct *clockticks );
  
/**
  * @brief  Draw a line
  * @param  point1x: POINT1 X coordinate.
  * @param  point1y: POINT1 Y coordinate.
  * @param  point2x: POINT2 X coordinate.
  * @param  point2y: POINT2 Y coordinate.
  * @param  pencolor: pen color.
  * @param  thickness: line thickness.
  * @param  roundtips: specifies if tips of the line are round
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_GFX_DrawLine( uint16_t point1x, uint16_t point1y, uint16_t point2x, uint16_t point2y,
                             uint16_t pencolor, uint16_t thickness, uint8_t roundtips );

/**
  * @brief  Draw a horizontal dashed line
  * @param  point1x: POINT1 X coordinate.
  * @param  point1y: POINT1 Y coordinate.
  * @param  point2x: POINT2 X coordinate.
  * @param  pencolor: pen color.
  * @param  thickness: line thickness.
  * @param  dashlen: length of dash in pixels.
  * @param  blanklen: length of blank in pixels.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_GFX_DrawDHLine( uint16_t point1x, uint16_t point1y, uint16_t point2x,
                               uint16_t pencolor, uint16_t thickness,
                               uint16_t dashlen, uint16_t blanklen );

/**
  * @brief  Draw a vertical dashed line
  * @param  point1x: POINT1 X coordinate.
  * @param  point1y: POINT1 Y coordinate.
  * @param  point2y: POINT2 Y coordinate.
  * @param  pencolor: pen color.
  * @param  thickness: line thickness.
  * @param  dashlen: length of dash in pixels.
  * @param  blanklen: length of blank in pixels.
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_GFX_DrawDVLine( uint16_t point1x, uint16_t point1y, uint16_t point2y,
                               uint16_t pencolor, uint16_t thickness,
                               uint16_t dashlen, uint16_t blanklen );

/**
  * @brief  Draw an arc
  * @param  centerx: Center X coordinate.
  * @param  centery: Center Y coordinate.
  * @param  radius: Outer radius.
  * @param  thickness: Thickness.  Inner radius = radius - thickness
  * @param  startangle: Start angle.  0 to 511 corresponds to full circle
  * @param  endangle: End angle.  0 to 511 corresponds to full circle.  Y-axis is 0.  Clockwise.
  * @param  pencolor: Pen color.
  * @retval Status: can be a value of @ref seStatus
  */

seStatus seMDC_GFX_DrawArc ( uint16_t centerx, uint16_t centery, uint16_t radius, uint16_t thickness,
                             uint16_t startangle, uint16_t endangle , uint16_t pencolor) ;

/**
  * @brief  Draw a dashed arc
  * @param  centerx: Center X coordinate.
  * @param  centery: Center Y coordinate.
  * @param  radius: Outer radius.
  * @param  thickness: Thickness.  Inner radius = radius - thickness
  * @param  startangle: Start angle.  0 to 511 corresponds to full circle
  * @param  endangle: End angle.  0 to 511 corresponds to full circle.  Y-axis is 0.  Clockwise.
  * @param  pencolor: Pen color.
  * @param  dashang: Dash angle segments in angle units (0 to 511)
  * @param  blankang: Blank angle segments in angle units (0 to 511)
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_GFX_DrawDArc ( uint16_t centerx, uint16_t centery, uint16_t radius, uint16_t thickness,
                             uint16_t startangle0, uint16_t endangle0, uint16_t pencolor, uint16_t dashang, uint16_t blankang);

/**
  * @brief  Draw a polygon outline.
  * @param  polygon:  polygon parameters structure of type @ref seMDC_GFX_PolygonStruct
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_GFX_DrawPolygon ( seMDC_GFX_PolygonStruct *polygon );
  
/**
  * @brief  UTF-8 to Unicode converter.  String pointer is incremented according to UTF-8 code.
  * @param  textstrp:  pointer to pointer to a UTF-8 text string
  * @retval uint32_t:  32-bit Unicode value or 0xFFFFFFFF if invalid UTF-8 code
  */
uint32_t UTF8toUnicode ( char **textstrp );

/**
  * @brief  Calculate the number of UTF-8 characters in a string.
  * @param  textstr:  pointer to text string
  * @retval uint16_t: Number of UTF-8 characters in the string.
  */
uint16_t utf8strlen ( char *textstr );

/**
  * @brief  Display a string using a bitmap font set.  Starting point in Destination Window is in the center of character.
  * @param  putstr_params:  String display parameters structure @ref seMDC_GFX_PutStr_Params
  * @param  textstr:  pointer to text string
  * @retval Status: can be a value of @ref seStatus
  */
seStatus seMDC_GFX_PutString ( seMDC_GFX_PutStr_Params *putstr_params, char *textstr );

/**
  * @}
  */   // MDC_GFX_Functions

/**
  * @}
  */   // seGraphicsLibrary


#ifdef __cplusplus
}
#endif
#endif	// SEMDC_GFX_H
