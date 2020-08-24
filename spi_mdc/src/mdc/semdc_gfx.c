/**
  ******************************************************************************
  * @file    semdc_gfx.c
  * @author  Epson
  * @version V1.0
  * @date    25-September-2017
  * @brief   This file provides the MDC graphics library functions.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */

#include <string.h>
#include <stdlib.h>

#include "s1d13c00_hcl.h"
#include "s1d13c00_memregs.h"
#include "se_common.h"
#include "se_mdc.h"
#include "se_dmac.h"
#include "semdc_gfx.h"


//=============== global functions ===========================================================

const static uint32_t sinlut[129] = {
0,
804,
1608,
2412,
3216,
4019,
4821,
5623,
6424,
7224,
8022,
8820,
9616,
10411,
11204,
11996,
12785,
13573,
14359,
15143,
15924,
16703,
17479,
18253,
19024,
19792,
20557,
21320,
22078,
22834,
23586,
24335,
25080,
25821,
26558,
27291,
28020,
28745,
29466,
30182,
30893,
31600,
32303,
33000,
33692,
34380,
35062,
35738,
36410,
37076,
37736,
38391,
39040,
39683,
40320,
40951,
41576,
42194,
42806,
43412,
44011,
44604,
45190,
45769,
46341,
46906,
47464,
48015,
48559,
49095,
49624,
50146,
50660,
51166,
51665,
52156,
52639,
53114,
53581,
54040,
54491,
54934,
55368,
55794,
56212,
56621,
57022,
57414,
57798,
58172,
58538,
58896,
59244,
59583,
59914,
60235,
60547,
60851,
61145,
61429,
61705,
61971,
62228,
62476,
62714,
62943,
63162,
63372,
63572,
63763,
63944,
64115,
64277,
64429,
64571,
64704,
64827,
64940,
65043,
65137,
65220,
65294,
65358,
65413,
65457,
65492,
65516,
65531,
65536
};


/**
  * Calculate X/Y coordinate from center coordinate, radius, and rotation value.
  * Parameters:
  *         centerx: Center X coordinate
  *         centery: Center Y coordinate
  *         radius:  Radius
  *         rotval:  Clock rotation value 0-59 (sec/min)
  *         xval:    return the X value for the center of rotation
  *         yval:    return the Y value for the center of rotation
  *
  * Return value:  Status
  */
seStatus seMDC_GFX_Rotval2XY ( int16_t centerx, int16_t centery, uint16_t radius, uint16_t rotval,
                               uint16_t *xval, uint16_t *yval ) {

  long x, y;
  unsigned long dx, dy;
  long gfxOWidth = seS1D13C00Read16( MDC_GFXOWIDTH );
  long gfxOHeight = seS1D13C00Read16( MDC_GFXOHEIGHT );

  if (rotval <= 15)
      rotval = ((15-rotval)*512)/60;
  else
      rotval = ((75-rotval)*512)/60;

  if (rotval < 128)
  {
     dx = ((((uint32_t) radius) * sinlut[128-rotval]) + 32768) >> 16;
     dy = ((((uint32_t) radius) * sinlut[rotval]) + 32768) >> 16;
     x = ((int32_t) centerx) + ((uint16_t) dx);
     y = ((int32_t) centery) - ((uint16_t) dy);
     *xval = (uint16_t) x;
     *yval = (uint16_t) y;
     if ((x < 0) || (y < 0) || (x >= gfxOWidth) || (y >= gfxOHeight))
       return seSTATUS_NG;
  }
  else if (rotval < 256)
  {
     dx = ((((int32_t) radius) * sinlut[rotval-128]) + 32768) >> 16;
     dy = ((((int32_t) radius) * sinlut[256-rotval]) + 32768) >> 16;
     x = ((int32_t) centerx) - ((uint16_t) dx);
     y = ((int32_t) centery) - ((uint16_t) dy);
     *xval = (uint16_t) x;
     *yval = (uint16_t) y;
     if ((x < 0) || (y < 0) || (x >= gfxOWidth) || (y >= gfxOHeight))
       return seSTATUS_NG;
  }
  else if (rotval < 384)
  {
     dx = ((((uint32_t) radius) * sinlut[384-rotval]) + 32768) >> 16;
     dy = ((((uint32_t) radius) * sinlut[rotval-256]) + 32768) >> 16;
     x = ((int32_t) centerx) - ((uint16_t) dx);
     y = ((int32_t) centery) + ((uint16_t) dy);
     *xval = (uint16_t) x;
     *yval = (uint16_t) y;
     if ((x < 0) || (y < 0) || (x >= gfxOWidth) || (y >= gfxOHeight))
       return seSTATUS_NG;
  }
  else if (rotval < 512)
  {
     dx = ((((uint32_t) radius) * sinlut[rotval-384]) + 32768) >> 16;
     dy = ((((uint32_t) radius) * sinlut[512-rotval]) + 32768) >> 16;
     x = ((int32_t) centerx) + ((uint16_t) dx);
     y = ((int32_t) centery) + ((uint16_t) dy);
     *xval = (uint16_t) x;
     *yval = (uint16_t) y;
     if ((x < 0) || (y < 0) || (x >= gfxOWidth) || (y >= gfxOHeight))
       return seSTATUS_NG;
  }
  else
  {
     return seSTATUS_NG;
  }

  return seSTATUS_OK;
};



/**
  * Draw a polar line (for analog clock hands/ticks).
  * Parameters:
  *         centerx:  Center X coordinate
  *         centery:  Center Y coordinate
  *         radius1:  Radius 1
  *         radius2:  Radius 2
  *         timeangle:  Time angle (0 to 59), clockwise from 12:00.
  *         linecolor:  Line color
  *         linethickness:  Line thickness
  * Return value:  Status
  */
seStatus seMDC_GFX_ClockLine ( int16_t centerx, int16_t centery, uint16_t radius1, uint16_t radius2,
                               uint8_t timeangle, uint16_t linecolor, uint16_t linethickness )
{
    uint16_t x1, y1, x2, y2;
    seStatus fResult;

    fResult = seMDC_GFX_Rotval2XY( centerx, centery, radius1, ((uint16_t) timeangle), &x1, &y1 );
    if (fResult == seSTATUS_OK)
        fResult = seMDC_GFX_Rotval2XY( centerx, centery, radius2, ((uint16_t) timeangle)%60, &x2, &y2 );

    if (fResult == seSTATUS_OK)
        fResult = seMDC_DrawLine( x1, y1, x2, y2, linecolor, linethickness );

    seMDC_WaitGfxDone();
    return fResult;
}


/**
  * Draw analog clock circular minutes/seconds ticks.
  * Parameters:
  *         clockticks:   pointer to clock ticks structure
  * Return value:  Status
  */
seStatus seMDC_GFX_DrawClockTicks ( seMDC_GFX_ClockTicksStruct *clockticks )
{
    uint8_t odd, timeangle, startangle, endangle;
    uint16_t center, xc, yc, xc1, yc1, radius1, radius2, linecolor, linethickness;

    // Valid parameters checking:
    //   rightedge > leftedge
    //   botedge > topedge
    //   (rightedge-leftedge) == (botedge-topedge)
    //   minorlength > 0
    //   majorlength > 0
    if ((clockticks->rightedge <= clockticks->leftedge) ||
        (clockticks->botedge <= clockticks->topedge) ||
       ((center = (clockticks->rightedge - clockticks->leftedge)) !=
        (clockticks->botedge - clockticks->topedge)) ||
        (clockticks->minorlength == 0) ||
        (clockticks->majorlength == 0))
            return(seSTATUS_NG);

    // Determine X and Y is odd or even
    if (center & 1)
    {
        odd = 0;
        center = (center >> 1);
        radius2 = center + 1;
    }
    else
    {
        odd = 1;
        center = (center >> 1);
        radius2 = center;
    }

    xc = clockticks->leftedge + center;  // X coordinate of clock center
    yc = clockticks->topedge + center;   // Y coordinate of clock center

    // Make sure time angles are within 0 to 59
    startangle = clockticks->starttimeangle % 60;
    endangle = clockticks->endtimeangle % 60;

    // Swap time angles if needed
    if (endangle < startangle)
    {
        timeangle = startangle;
        startangle = endangle;
        endangle = timeangle;
    }

    // Loop through each tick
    for (timeangle = startangle; timeangle <= endangle; timeangle++)
    {
        if ((timeangle % 5) == 0)  // major tick
        {
            linecolor = clockticks->majorlinecolor;
            linethickness = clockticks->majorthickness;
            radius1 = center - clockticks->majorlength - 1;
        }
        else                       // minor tick
        {
            linecolor = clockticks->minorlinecolor;
            linethickness = clockticks->minorthickness;
            radius1 = center - clockticks->minorlength - 1;
        }

        xc1 = xc;
        yc1 = yc;

        // 0 to 14
        if (timeangle < 15)
        {
            xc1 = xc;
            yc1 = yc;
            if (odd == 0)
                yc1++;
        }

        // 15 to 29
        else if (timeangle < 30)
        {
            xc1 = xc;
            yc1 = yc;
        }

        // 30 to 44
        else if (timeangle < 45)
        {
            xc1 = xc;
            yc1 = yc;
            if (odd == 0)
                xc1++;
        }

        // 45 to 59
        else
        {
            xc1 = xc;
            yc1 = yc;
            if (odd == 0)
            {
                xc1++;
                yc1++;
            }
        }

        seMDC_GFX_ClockLine( xc1, yc1, radius1, radius2, timeangle, linecolor, linethickness);
    }

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
  *         roundtips: specifies if tips of the line are round
  * Return value:  Status
  */
seStatus seMDC_GFX_DrawLine( uint16_t point1x, uint16_t point1y, uint16_t point2x, uint16_t point2y,
                             uint16_t pencolor, uint16_t thickness, uint8_t roundtips )
{
    seStatus fResult;

    fResult = seMDC_DrawLine(point1x, point1y, point2x, point2y, pencolor, thickness);
    seMDC_WaitGfxDone();
    if ((fResult == seSTATUS_OK) && roundtips)
    {
        fResult = seMDC_DrawEllipse(point1x, point1y, thickness, thickness, pencolor, 0, 0, FILL_ENABLE);
        seMDC_WaitGfxDone();
        if (fResult == seSTATUS_OK) {
            fResult = seMDC_DrawEllipse(point2x, point2y, thickness, thickness, pencolor, 0, 0, FILL_ENABLE);
            seMDC_WaitGfxDone();
        }
    }

  return fResult;
}


/**
  * Draw a horizontal dashed line
  * Parameters:
  *         point1x: POINT1 X coordinate.
  *         point1y: POINT1 Y coordinate.
  *         point2x: POINT2 X coordinate.
  *         pencolor: pen color.
  *         thickness: line thickness.
  *         dashlen: length of dash in pixels.
  *         blanklen: length of blank in pixels.
  * Return value:  Status
  */
seStatus seMDC_GFX_DrawDHLine( uint16_t point1x, uint16_t point1y, uint16_t point2x,
                               uint16_t pencolor, uint16_t thickness,
                               uint16_t dashlen, uint16_t blanklen )
{
  seStatus fResult;
  uint32_t start, end;

  if (point1x > point2x)
  {
    start = point2x;
    point2x = point1x;
  }
  else
    start = point1x;

  while (start < point2x)
  {

    end = (start+dashlen-1);
    if (end > point2x)
      end = point2x;

    fResult = seMDC_DrawLine(start, point1y, end, point1y, pencolor, thickness);
    seMDC_WaitGfxDone();

    if (fResult != seSTATUS_OK)
      break;

    start = start + dashlen + blanklen;
  }


  return(fResult);
}


/**
  * Draw a vertical dashed line
  * Parameters:
  *         point1x: POINT1 X coordinate.
  *         point1y: POINT1 Y coordinate.
  *         point2y: POINT2 Y coordinate.
  *         pencolor: pen color.
  *         thickness: line thickness.
  *         dashlen: length of dash in pixels.
  *         blanklen: length of blank in pixels.
  * Return value:  Status
  */
seStatus seMDC_GFX_DrawDVLine( uint16_t point1x, uint16_t point1y, uint16_t point2y,
                               uint16_t pencolor, uint16_t thickness,
                               uint16_t dashlen, uint16_t blanklen )
{
  seStatus fResult;
  uint32_t start, end;

  if (point1y > point2y)
  {
    start = point2y;
    point2y = point1y;
  }
  else
    start = point1y;

  while (start < point2y)
  {

    end = (start+dashlen-1);
    if (end > point2y)
      end = point2y;

    fResult = seMDC_DrawLine(point1x, start, point1x, end, pencolor, thickness);
    seMDC_WaitGfxDone();

    if (fResult != seSTATUS_OK)
      break;

    start = start + dashlen + blanklen;
  }


  return(fResult);
}


/**
  * Draw an arc
  * Parameters:
  *         centerx: Center X coordinate.
  *         centery: Center Y coordinate.
  *         radius: Outer radius.
  *         thickness: Thickness.  Inner radius = radius - thickness
  *         startangle: Start angle.  0 to 511 corresponds to full circle
  *         endangle: End angle.  0 to 511 corresponds to full circle
  *                    (Angle is referenced to positive Y-axis as 0, clockwise direction.
  *                     128 is positive X-axis, 256 is negative Y-axis, 384 is negative X-axis.)
  *         pencolor: Pen color.
  * Return value:  Status
  */

static uint32_t ocq1axs;         // Outer circle Q1 A X start
static uint32_t ocq1ays;         // Outer circle Q1 A Y start
static uint32_t ocq1axe;         // Outer circle Q1 A X end
static uint32_t icq1axs;         // Inner circle Q1 A X start
static uint32_t icq1axe;         // Inner circle Q1 A X end
static uint32_t icq1aye;         // Inner circle Q1 A Y end
static uint32_t ocq1bxs;         // Outer circle Q1 B X start
static uint32_t ocq1bys;         // Outer circle Q1 B Y start
static uint32_t ocq1bye;         // Outer circle Q1 B Y end
static uint32_t icq1bys;         // Inner circle Q1 B Y start
static uint32_t icq1bxe;         // Inner circle Q1 B X end
static uint32_t icq1bye;         // Inner circle Q1 B Y end
static uint32_t ocq2axs;         // Outer circle Q2 A X start
static uint32_t ocq2ays;         // Outer circle Q2 A Y start
static uint32_t ocq2axe;         // Outer circle Q2 A X end
static uint32_t icq2axs;         // Inner circle Q2 A X start
static uint32_t icq2axe;         // Inner circle Q2 A X end
static uint32_t icq2aye;         // Inner circle Q2 A Y end
static uint32_t ocq2bxs;         // Outer circle Q2 B X start
static uint32_t ocq2bys;         // Outer circle Q2 B Y start
static uint32_t ocq2bye;         // Outer circle Q2 B Y end
static uint32_t icq2bys;         // Inner circle Q2 B Y start
static uint32_t icq2bxe;         // Inner circle Q2 B X end
static uint32_t icq2bye;         // Inner circle Q2 B Y end
static uint32_t ocq3axs;         // Outer circle Q3 A X start
static uint32_t ocq3ays;         // Outer circle Q3 A Y start
static uint32_t ocq3axe;         // Outer circle Q3 A X end
static uint32_t icq3axs;         // Inner circle Q3 A X start
static uint32_t icq3axe;         // Inner circle Q3 A X end
static uint32_t icq3aye;         // Inner circle Q3 A Y end
static uint32_t ocq3bxs;         // Outer circle Q3 B X start
static uint32_t ocq3bys;         // Outer circle Q3 B Y start
static uint32_t ocq3bye;         // Outer circle Q3 B Y end
static uint32_t icq3bys;         // Inner circle Q3 B Y start
static uint32_t icq3bxe;         // Inner circle Q3 B X end
static uint32_t icq3bye;         // Inner circle Q3 B Y end
static uint32_t ocq4axs;         // Outer circle Q4 A X start
static uint32_t ocq4ays;         // Outer circle Q4 A Y start
static uint32_t ocq4axe;         // Outer circle Q4 A X end
static uint32_t icq4axs;         // Inner circle Q4 A X start
static uint32_t icq4axe;         // Inner circle Q4 A X end
static uint32_t icq4aye;         // Inner circle Q4 A Y end
static uint32_t ocq4bxs;         // Outer circle Q4 B X start
static uint32_t ocq4bys;         // Outer circle Q4 B Y start
static uint32_t ocq4bye;         // Outer circle Q4 B Y end
static uint32_t icq4bys;         // Inner circle Q4 B Y start
static uint32_t icq4bxe;         // Inner circle Q4 B X end
static uint32_t icq4bye;         // Inner circle Q4 B Y end

static uint32_t ocxs2;
static uint32_t ocys2;
static uint32_t ics2;
static uint32_t ice2;

static int32_t oxsq;
static int32_t oysq;
static int32_t oxsqtimes4;
static int32_t oysqtimes4;
static int32_t osigma;
static int32_t ixsq;
static int32_t iysq;
static int32_t ixsqtimes4;
static int32_t iysqtimes4;
static int32_t isigma;

#ifdef TM4C1294
#pragma FUNCTION_OPTIONS(seMDC_GFX_DrawArc, "--opt_level=off")
#endif

seStatus seMDC_GFX_DrawArc ( uint16_t centerx, uint16_t centery, uint16_t radius, uint16_t thickness,
                             uint16_t startangle0, uint16_t endangle0, uint16_t pencolor)
{
    uint32_t iradius;                            // inner radius
    uint32_t startangle, endangle;
    uint32_t q1draw, q2draw, q3draw, q4draw;      // Quadrant draw enable:
                                                  //    Q1 is top right, Q2 is top left,
                                                  //    Q3 is bottom left, Q4 is bottom right
    uint8_t icalc, ocalc;
    int32_t ox, oy;
    int32_t ix, iy;
    uint16_t penthick;
    uint8_t split, firstseg;

    startangle = (uint32_t) startangle0;
    endangle = (uint32_t) endangle0;

    startangle = startangle % 512;  // Make it 0 to 511
    endangle = endangle % 512;      // Make it 0 to 511

    iradius = radius - thickness;  // inner X radius
    iradius = radius - thickness;  // inner Y radius

    oxsq = radius * radius;
    oysq = radius * radius;
    oxsqtimes4 = 4 * oxsq;
    oysqtimes4 = 4 * oysq;
    ixsq = iradius * iradius;
    iysq = iradius * iradius;
    ixsqtimes4 = 4 * ixsq;
    iysqtimes4 = 4 * iysq;

    // If angles are equal, do nothing and return OK
    if (startangle == endangle)
        return(seSTATUS_OK);

    // Evaluate 8 regions of circle
    q1draw = (startangle < 128) |
             ((startangle < 256) & (startangle >= 128) & (startangle > endangle) & (endangle > 0)) |
             ((startangle < 384) & (startangle >= 256) & (startangle > endangle) & (endangle > 0)) |
             ((startangle < 512) & (startangle >= 384) & (startangle > endangle) & (endangle > 0));
    q2draw = ((startangle < 511) & (startangle >= 384)) |
             ((startangle < 256) & (startangle >= 128) & ((startangle > endangle) | ((startangle <= endangle) & (endangle > 384)))) |
             ((startangle < 384) & (startangle >= 256) & ((startangle > endangle) | ((startangle <= endangle) & (endangle > 384)))) |
             ((startangle < 128) & ((startangle > endangle) | ((startangle <= endangle) & (endangle > 384))));
    q3draw = ((startangle < 384) & (startangle >= 256)) |
             ((startangle < 128) & ((startangle > endangle) | ((startangle <= endangle) & (endangle > 256)))) |
             ((startangle < 256) & (startangle >= 128) & ((startangle > endangle) | ((startangle <= endangle) & (endangle > 256)))) |
             ((startangle < 512) & (startangle >= 384) & (startangle > endangle) & (endangle > 256));
    q4draw = ((startangle < 256) & (startangle >= 128)) |
             ((startangle < 128) & ((startangle > endangle) | ((startangle <= endangle) & (endangle > 128)))) |
             ((startangle < 384) & (startangle >= 256) & (startangle > endangle) & (endangle > 128)) |
             ((startangle < 512) & (startangle >= 384) & (startangle > endangle) & (endangle > 128));

    if (((startangle < 128) & (endangle <= 128) & (startangle <= endangle)) |
        ((startangle < 128) & (endangle > 128) & (endangle <= 256)) |
        ((startangle < 128) & (endangle > 256) & (endangle <= 384)) |
        ((startangle < 128) & (endangle > 384) & (endangle <= 512)))
    {
        ocq1axs= ((((uint32_t) radius) * sinlut[startangle]) + 32768) >> 16;
        ocq1ays= ((((uint32_t) radius) * sinlut[128-startangle]) + 32768) >> 16;
        icq1axs= ((((uint32_t) iradius) * sinlut[startangle]) + 32768) >> 16;
    }
    else
    {
        ocq1axs = 0;
        ocq1ays = 0;
        icq1axs = 0;
    }

    if (((startangle < 128) & (startangle > endangle) & (endangle != 0)) |
        ((startangle >= 128) & (startangle < 256) & (endangle <= 128) & (endangle != 0)) |
        ((startangle >= 256) & (startangle < 384) & (endangle <= 128) & (endangle != 0)) |
        ((startangle >= 384) & (startangle < 512) & (endangle <= 128) & (endangle != 0)) |
        ((startangle < 128) & (endangle <= 128) & (startangle <= endangle)))
    {
        ocq1axe= ((((uint32_t) radius) * sinlut[endangle]) + 32768) >> 16;
        icq1axe= ((((uint32_t) iradius) * sinlut[endangle]) + 32768) >> 16;
    }
    else
    {
        ocq1axe = radius;
        icq1axe = iradius;
    }

    if (((startangle < 128) & (endangle <= 128) & (startangle <= endangle)) |
        ((startangle >= 128) & (startangle < 256) & (endangle <= 128) & (endangle != 0)) |
        ((startangle >= 256) & (startangle < 384) & (endangle <= 128) & (endangle != 0)) |
        ((startangle >= 384) & (startangle < 512) & (endangle <= 128) & (endangle != 0)))
    {
        ocq1bxs = ((((uint32_t) radius) * sinlut[endangle]) + 32768) >> 16;
        ocq1bys = ((((uint32_t) radius) * sinlut[128-endangle]) + 32768) >> 16;
        icq1bys= ((((uint32_t) iradius) * sinlut[128-endangle]) + 32768) >> 16;
    }
    else
    {
        ocq1bxs = 0;
        ocq1bys = 0;
        icq1bys = 0;
    }

    if (((startangle < 128) & (endangle < 128) & (startangle > endangle)) |
        ((startangle < 128) & (endangle > 128) & (endangle <= 256)) |
        ((startangle < 128) & (endangle > 256) & (endangle <= 384)) |
        ((startangle < 128) & (endangle > 384) & (endangle <= 512)) |
        ((startangle < 128) & (endangle <= 128) & (startangle <= endangle)))
    {
        ocq1bye = ((((uint32_t) radius) * sinlut[128-startangle]) + 32768) >> 16;
        icq1bye = ((((uint32_t) iradius) * sinlut[128-startangle]) + 32768) >> 16;
    }
    else
    {
        ocq1bye = radius;
        icq1bye = iradius;
    }

    if (((startangle < 128) & (endangle > 384)) |
        ((startangle >= 128) & (startangle < 256) & (endangle > 384)) |
        ((startangle >= 256) & (startangle < 384) & (endangle > 384)) |
        ((startangle >= 384) & (startangle < 512) & (startangle <= endangle)))
    {
        ocq2axs = ((((uint32_t) radius) * sinlut[512-endangle]) + 32768) >> 16;
        ocq2ays = ((((uint32_t) radius) * sinlut[endangle-384]) + 32768) >> 16;
        icq2axs = ((((uint32_t) iradius) * sinlut[512-endangle]) + 32768) >> 16;
    }
    else
    {
        ocq2axs = 0;
        ocq2ays = 0;
        icq2axs = 0;
    }

    if (((startangle >= 384) & (startangle < 512) & (endangle <= 384)) |
        ((startangle >= 384) & (startangle < 512)))
    {
        ocq2axe = ((((uint32_t) radius) * sinlut[512-startangle]) + 32768) >> 16;
        icq2axe = ((((uint32_t) iradius) * sinlut[512-startangle]) + 32768) >> 16;
    }
    else
    {
        ocq2axe = radius;
        icq2axe = iradius;
    }

    if (((startangle >= 384) & (startangle < 512) & (endangle <= 384)) |
        ((startangle >= 384) & (startangle < 512) & (startangle <= endangle)))
    {
        ocq2bxs = ((((uint32_t) radius) * sinlut[512-startangle]) + 32768) >> 16;
        ocq2bys = ((((uint32_t) radius) * sinlut[startangle-384]) + 32768) >> 16;
        icq2bys = ((((uint32_t) iradius) * sinlut[startangle-384]) + 32768) >> 16;
    }
    else
    {
        ocq2bxs = 0;
        ocq2bys = 0;
        icq2bys = 0;
    }

    if (((startangle < 128) & (endangle > 384)) |
        ((startangle >= 128) & (startangle < 256) & (endangle > 384)) |
        ((startangle >= 256) & (startangle < 384) & (endangle > 384)) |
        ((startangle >= 384) & (startangle < 512) & (endangle > 384)))
    {
        ocq2bye = ((((uint32_t) radius) * sinlut[endangle-384]) + 32768) >> 16;
        icq2bye = ((((uint32_t) iradius) * sinlut[endangle-384]) + 32768) >> 16;
    }
    else
    {
        ocq2bye = radius;
        icq2bye = iradius;
    }

    if (((startangle >= 256) & (startangle < 384) & (endangle <= 256)) |
        ((startangle >= 256) & (startangle < 384) & (endangle <= 384) & (startangle <= endangle)) |
        ((startangle >= 256) & (startangle < 384) & (endangle > 384)))
    {
        ocq3axs = ((((uint32_t) radius) * sinlut[startangle-256]) + 32768) >> 16;
        ocq3ays = ((((uint32_t) radius) * sinlut[384-startangle]) + 32768) >> 16;
        icq3axs = ((((uint32_t) iradius) * sinlut[startangle-256]) + 32768) >> 16;
    }
    else
    {
        ocq3axs = 0;
        ocq3ays = 0;
        icq3axs = 0;
    }


    if (((startangle < 128) & (endangle > 256) & (endangle <= 384)) |
        ((startangle < 256) & (startangle >= 128) & (endangle > 256) & (endangle <= 384)) |
        ((startangle < 384) & (startangle >= 256) & (endangle > 256) & (endangle <= 384)) |
        ((startangle < 512) & (startangle >= 384) & (endangle > 256) & (endangle <= 384)))
    {
        ocq3axe = ((((uint32_t) radius) * sinlut[endangle-256]) + 32768) >> 16;
        icq3axe = ((((uint32_t) iradius) * sinlut[endangle-256]) + 32768) >> 16;
    }
    else
    {
        ocq3axe = radius;
        icq3axe = iradius;
    }

    if (((startangle < 128) & (endangle <= 384) & (endangle > 256)) |
        ((startangle < 256) & (startangle >= 128) & (endangle <= 384) & (endangle > 256)) |
        ((startangle < 384) & (startangle >= 256) & (startangle <= endangle) & (endangle <= 384)) |
        ((startangle < 512) & (startangle >= 384) & (endangle <= 384) & (endangle > 256)))
    {
        ocq3bxs = ((((uint32_t) radius) * sinlut[endangle-256]) + 32768) >> 16;
        ocq3bys = ((((uint32_t) radius) * sinlut[384-endangle]) + 32768) >> 16;
        icq3bys = ((((uint32_t) iradius) * sinlut[384-endangle]) + 32768) >> 16;
    }
    else
    {
        ocq3bxs = 0;
        ocq3bys = 0;
        icq3bys = 0;
    }

    if ((startangle < 384) & (startangle >= 256))
    {
        ocq3bye = ((((uint32_t) radius) * sinlut[384-startangle]) + 32768) >> 16;
        icq3bye = ((((uint32_t) iradius) * sinlut[384-startangle]) + 32768) >> 16;
    }
    else
    {
        ocq3bye = radius;
        icq3bye = iradius;
    }


    if (((startangle < 128) & (endangle > 128) & (endangle <= 256)) |
        ((startangle >= 128) & (startangle < 256) & (startangle <= endangle) & (endangle <= 256)) |
        ((startangle >= 256) & (startangle < 384) & (endangle > 128) & (endangle <= 256)) |
        ((startangle >= 384) & (startangle < 512) & (endangle > 128) & (endangle <= 256)))
    {
        ocq4axs = ((((uint32_t) radius) * sinlut[256-endangle]) + 32768) >> 16;
        ocq4ays = ((((uint32_t) radius) * sinlut[endangle-128]) + 32768) >> 16;
        icq4axs = ((((uint32_t) iradius) * sinlut[256-endangle]) + 32768) >> 16;
    }
    else
    {
        ocq4axs = 0;
        ocq4ays = 0;
        icq4axs = 0;
    }

    if (((startangle >= 128) & (startangle < 256) & (endangle <= 128)) |
        ((startangle >= 128) & (startangle < 256)))
    {
        ocq4axe = ((((uint32_t) radius) * sinlut[256-startangle]) + 32768) >> 16;
        icq4axe = ((((uint32_t) iradius) * sinlut[256-startangle]) + 32768) >> 16;
    }
    else
    {
        ocq4axe = radius;
        icq4axe = iradius;
    }

    if (((startangle < 256) & (startangle >= 128) & (endangle <= 128)) |
        ((startangle < 256) & (startangle >= 128) & (startangle <= endangle) & (endangle <= 256)) |
        ((startangle < 256) & (startangle >= 128) & (endangle > 256) & (endangle <= 512)))
    {
        ocq4bxs = ((((uint32_t) radius) * sinlut[256-startangle]) + 32768) >> 16;
        ocq4bys = ((((uint32_t) radius) * sinlut[startangle-128]) + 32768) >> 16;
        icq4bys = ((((uint32_t) iradius) * sinlut[startangle-128]) + 32768) >> 16;
    }
    else
    {
        ocq4bxs = 0;
        ocq4bys = 0;
        icq4bys = 0;
    }

    if ((endangle > 128) & (endangle <= 256))
    {
        ocq4bye = ((((uint32_t) radius) * sinlut[endangle-128]) + 32768) >> 16;
        icq4bye = ((((uint32_t) iradius) * sinlut[endangle-128]) + 32768) >> 16;
    }
    else
    {
        ocq4bye = radius;
        icq4bye = iradius;
    }


    split = 0;
    ice2 = ((((uint32_t) iradius) * sinlut[64]) + 32768) >> 16;
    if ((startangle <= 128) & (startangle > endangle) & (startangle <= 64))
    {
        split = 1;  // Q1 A
        firstseg = 1;
        ocxs2 = ((((uint32_t) radius) * sinlut[startangle]) + 32768) >> 16;
        ocys2 = ((((uint32_t) radius) * sinlut[128-startangle]) + 32768) >> 16;
        ics2 = ((((uint32_t) iradius) * sinlut[startangle]) + 32768) >> 16;
    }
    else if ((startangle <= 128) & (startangle > endangle) & (endangle > 64))
    {
        split = 2;  // Q1 B
        firstseg = 1;
        ocxs2 = ((((uint32_t) radius) * sinlut[endangle]) + 32768) >> 16;
        ocys2 = ((((uint32_t) radius) * sinlut[128-endangle]) + 32768) >> 16;
        ics2 = ((((uint32_t) iradius) * sinlut[128-endangle]) + 32768) >> 16;
    }
    else if ((startangle > 128) & (startangle <= 256) & (startangle > endangle) & (endangle <= 192))
    {
        split = 4;  // Q4 B
        firstseg = 1;
        ocxs2 = ((((uint32_t) radius) * sinlut[256-startangle]) + 32768) >> 16;
        ocys2 = ((((uint32_t) radius) * sinlut[startangle-128]) + 32768) >> 16;
        ics2 = ((((uint32_t) iradius) * sinlut[startangle-128]) + 32768) >> 16;
    }
    else if ((startangle > 128) & (startangle <= 256) & (startangle > endangle) & (endangle > 192))
    {
        split = 3;  // Q4 A
        firstseg = 1;
        ocxs2 = ((((uint32_t) radius) * sinlut[256-endangle]) + 32768) >> 16;
        ocys2 = ((((uint32_t) radius) * sinlut[endangle-128]) + 32768) >> 16;
        ics2 = ((((uint32_t) iradius) * sinlut[256-endangle]) + 32768) >> 16;
    }
    else if ((startangle > 256) & (startangle <= 384) & (startangle > endangle) & (endangle <= 320))
    {
        split = 5;  // Q3 A
        firstseg = 1;
        ocxs2 = ((((uint32_t) radius) * sinlut[startangle-256]) + 32768) >> 16;
        ocys2 = ((((uint32_t) radius) * sinlut[384-startangle]) + 32768) >> 16;
        ics2 = ((((uint32_t) iradius) * sinlut[startangle-256]) + 32768) >> 16;
    }
    else if ((startangle > 256) & (startangle <= 384) & (startangle > endangle) & (endangle > 320))
    {
        split = 6;  // Q3 B
        firstseg = 1;
        ocxs2 = ((((uint32_t) radius) * sinlut[endangle-256]) + 32768) >> 16;
        ocys2 = ((((uint32_t) radius) * sinlut[384-endangle]) + 32768) >> 16;
        ics2 = ((((uint32_t) iradius) * sinlut[384-endangle]) + 32768) >> 16;
    }
    else if ((startangle > 384) & (startangle <= 512) & (startangle > endangle) & (endangle <= 448))
    {
        split = 8;  // Q2 B
        firstseg = 1;
        ocxs2 = ((((uint32_t) radius) * sinlut[512-startangle]) + 32768) >> 16;
        ocys2 = ((((uint32_t) radius) * sinlut[startangle-384]) + 32768) >> 16;
        ics2 = ((((uint32_t) iradius) * sinlut[startangle-384]) + 32768) >> 16;
    }
    else if ((startangle > 384) & (startangle <= 512) & (startangle > endangle) & (endangle > 448))
    {
        split = 7;  // Q2 A
        firstseg = 1;
        ocxs2 = ((((uint32_t) radius) * sinlut[512-endangle]) + 32768) >> 16;
        ocys2 = ((((uint32_t) radius) * sinlut[endangle-384]) + 32768) >> 16;
        ics2 = ((((uint32_t) iradius) * sinlut[512-endangle]) + 32768) >> 16;
    }


    // ---------- Drawing loops ------------------------------------------

    /* A half */
    ox = 0;
    oy = radius;
    osigma = 2*oysq + oxsq * (1 - 2 * radius);

    ix = 0;
    iy = iradius;
    isigma = 2*iysq + ixsq * (1 - 2 * iradius);

    icalc = 1;
    ocalc = 1;

    penthick = 0;
    while (1)
    {
        if (q1draw)
        {
            if (ix == icq1axe)
                icq1aye = iy;

            if (split == 1)
               if (firstseg)
               {
                    if ((ix >= icq1axs) && (ox <= ocq1axe))
                    {
                        seMDC_DrawLine( centerx+((ox < ocq1axs) ? ocq1axs : ox), centery-((ox < ocq1axs) ? ocq1ays : oy),
                                        centerx+((ix > icq1axe) ? icq1axe : ix), centery-((ix > icq1axe) ? icq1aye : iy), pencolor, penthick );
                        seMDC_WaitGfxDone();
                        seMDC_DrawLine( centerx+((ox < ocq1axs) ? ocq1axs : ox), centery-((ox < ocq1axs) ? (ocq1ays-1) : (oy-1)),
                                        centerx+((ix > icq1axe) ? (icq1axe+1) : (ix+1)), centery-((ix > icq1axe) ? icq1aye : iy), pencolor, penthick );
                        seMDC_WaitGfxDone();
                    }
                    if (ix == icq1axe)
                    {
                        icalc = 0;
                    }

                    if (ox == ocq1axe)
                    {
                       firstseg = 0;
                       icalc = 1;
                       if (ox != ix)
                           ocalc = 0;
                       ox = ix;
                    }
               }
               else
               {
                   if (ox == ocq1axe)
                       ocalc = 1;

                   if (ix >= ics2)
                   {
                       seMDC_DrawLine( centerx+((ox < ocxs2) ? ocxs2 : ox), centery-((ox < ocxs2) ? ocys2 : oy),
                                       centerx+((ix > ice2) ? ice2 : ix), centery-((ix > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx+((ox < ocxs2) ? ocxs2 : ox), centery-((ox < ocxs2) ? (ocys2-1) : (oy-1)),
                                       centerx+((ix > ice2) ? (ice2+1) : (ix+1)), centery-((ix > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
               }
            else
            {
                if ((ix >= icq1axs) && (ox <= ocq1axe))
                {
                    seMDC_DrawLine( centerx+((ox < ocq1axs) ? ocq1axs : ox), centery-((ox < ocq1axs) ? ocq1ays : oy),
                                    centerx+((ix > icq1axe) ? icq1axe : ix), centery-((ix > icq1axe) ? icq1aye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                    seMDC_DrawLine( centerx+((ox < ocq1axs) ? ocq1axs : ox), centery-((ox < ocq1axs) ? (ocq1ays-1) : (oy-1)),
                                    centerx+((ix > icq1axe) ? (icq1axe+1) : (ix+1)), centery-((ix > icq1axe) ? icq1aye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                }
            }
        }

        if (q2draw)
        {
            if (ix == icq2axe)
                icq2aye = iy;

            if (split == 7)
               if (firstseg)
               {
                   if ((ix >= icq2axs) && (ox <= ocq2axe))
                   {
                       seMDC_DrawLine( centerx-((ox < ocq2axs) ? ocq2axs : ox), centery-((ox < ocq2axs) ? ocq2ays : oy),
                                       centerx-((ix > icq2axe) ? icq2axe : ix), centery-((ix > icq2axe) ? icq2aye : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx-((ox < ocq2axs) ? ocq2axs : ox), centery-((ox < ocq2axs) ? (ocq2ays-1) : (oy-1)),
                                       centerx-((ix > icq2axe) ? (icq2axe+1) : (ix+1)), centery-((ix > icq2axe) ? icq2aye : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
                   if (ix == icq2axe)
                   {
                       icalc = 0;
                   }

                   if (ox == ocq2axe)
                   {
                      firstseg = 0;
                      icalc = 1;
                      if (ox != ix)
                          ocalc = 0;
                      ox = ix;
                   }
               }
               else
               {
                   if (ox == ocq2axe)
                       ocalc = 1;

                   if (ix >= ics2)
                   {
                       seMDC_DrawLine( centerx-((ox < ocxs2) ? ocxs2 : ox), centery-((ox < ocxs2) ? ocys2 : oy),
                                       centerx-((ix > ice2) ? ice2 : ix), centery-((ix > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx-((ox < ocxs2) ? ocxs2 : ox), centery-((ox < ocxs2) ? (ocys2-1) : (oy-1)),
                                       centerx-((ix > ice2) ? (ice2+1) : (ix+1)), centery-((ix > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
               }
            else
            {
                if ((ix >= icq2axs) && (ox <= ocq2axe))
                {
                    seMDC_DrawLine( centerx-((ox < ocq2axs) ? ocq2axs : ox), centery-((ox < ocq2axs) ? ocq2ays : oy),
                                    centerx-((ix > icq2axe) ? icq2axe : ix), centery-((ix > icq2axe) ? icq2aye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                    seMDC_DrawLine( centerx-((ox < ocq2axs) ? ocq2axs : ox), centery-((ox < ocq2axs) ? (ocq2ays-1) : (oy-1)),
                                    centerx-((ix > icq2axe) ? (icq2axe+1) : (ix+1)), centery-((ix > icq2axe) ? icq2aye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                }
            }
        }

        if (q3draw)
        {
            if (ix == icq3axe)
                icq3aye = iy;

            if (split == 5)
               if (firstseg)
               {
                   if ((ix >= icq3axs) && (ox <= ocq3axe))
                   {
                       seMDC_DrawLine( centerx-((ox < ocq3axs) ? ocq3axs : ox), centery+((ox < ocq3axs) ? ocq3ays : oy),
                                       centerx-((ix > icq3axe) ? icq3axe : ix), centery+((ix > icq3axe) ? icq3aye : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx-((ox < ocq3axs) ? ocq3axs : ox), centery+((ox < ocq3axs) ? (ocq3ays-1) : (oy-1)),
                                       centerx-((ix > icq3axe) ? (icq3axe+1) : (ix+1)), centery+((ix > icq3axe) ? icq3aye : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
                   if (ix == icq3axe)
                   {
                       icalc = 0;
                   }

                   if (ox == ocq3axe)
                   {
                      firstseg = 0;
                      icalc = 1;
                      if (ox != ix)
                          ocalc = 0;
                      ox = ix;
                   }
               }
               else
               {
                   if (ox == ocq3axe)
                       ocalc = 1;

                   if (ix >= ics2)
                   {
                       seMDC_DrawLine( centerx-((ox < ocxs2) ? ocxs2 : ox), centery+((ox < ocxs2) ? ocys2 : oy),
                                       centerx-((ix > ice2) ? ice2 : ix), centery+((ix > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx-((ox < ocxs2) ? ocxs2 : ox), centery+((ox < ocxs2) ? (ocys2-1) : (oy-1)),
                                       centerx-((ix > ice2) ? (ice2+1) : (ix+1)), centery+((ix > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
               }
            else
            {
                if ((ix >= icq3axs) && (ox <= ocq3axe))
                {
                    seMDC_DrawLine( centerx-((ox < ocq3axs) ? ocq3axs : ox), centery+((ox < ocq3axs) ? ocq3ays : oy),
                                    centerx-((ix > icq3axe) ? icq3axe : ix), centery+((ix > icq3axe) ? icq3aye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                    seMDC_DrawLine( centerx-((ox < ocq3axs) ? ocq3axs : ox), centery+((ox < ocq3axs) ? (ocq3ays-1) : (oy-1)),
                                    centerx-((ix > icq3axe) ? (icq3axe+1) : (ix+1)), centery+((ix > icq3axe) ? icq3aye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                }
            }
        }

        if (q4draw)
        {
            if (ix == icq4axe)
                icq4aye = iy;

            if (split == 3)
               if (firstseg)
               {
                   if ((ix >= icq4axs) && (ox <= ocq4axe))
                   {
                       seMDC_DrawLine( centerx+((ox < ocq4axs) ? ocq4axs : ox), centery+((ox < ocq4axs) ? ocq4ays: oy),
                                       centerx+((ix > icq4axe) ? icq4axe : ix), centery+((ix > icq4axe) ? icq4aye : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx+((ox < ocq4axs) ? ocq4axs : ox), centery+((ox < ocq4axs) ? (ocq4ays-1): (oy-1)),
                                       centerx+((ix > icq4axe) ? (icq4axe+1) : (ix+1)), centery+((ix > icq4axe) ? icq4aye : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
                   if (ix == icq4axe)
                   {
                       icalc = 0;
                   }

                   if (ox == ocq4axe)
                   {
                      firstseg = 0;
                      icalc = 1;
                      if (ox != ix)
                          ocalc = 0;
                      ox = ix;
                   }
               }
               else
               {
                   if (ox == ocq4axe)
                       ocalc = 1;

                   if (ix >= ics2)
                   {
                       seMDC_DrawLine( centerx+((ox < ocxs2) ? ocxs2 : ox), centery+((ox < ocxs2) ? ocys2 : oy),
                                       centerx+((ix > ice2) ? ice2 : ix), centery+((ix > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx+((ox < ocxs2) ? ocxs2 : ox), centery+((ox < ocxs2) ? (ocys2-1) : (oy-1)),
                                       centerx+((ix > ice2) ? (ice2+1) : (ix+1)), centery+((ix > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
               }
            else
            {
                if ((ix >= icq4axs) && (ox <= ocq4axe))
                {
                    seMDC_DrawLine( centerx+((ox < ocq4axs) ? ocq4axs : ox), centery+((ox < ocq4axs) ? ocq4ays: oy),
                                    centerx+((ix > icq4axe) ? icq4axe : ix), centery+((ix > icq4axe) ? icq4aye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                    seMDC_DrawLine( centerx+((ox < ocq4axs) ? ocq4axs : ox), centery+((ox < ocq4axs) ? (ocq4ays-1): (oy-1)),
                                    centerx+((ix > icq4axe) ? (icq4axe+1) : (ix+1)), centery+((ix > icq4axe) ? icq4aye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                }
            }
        }

        if (ocalc)
        {
            if (osigma >= 0)
            {
                osigma += oxsqtimes4 * (1 - oy);
                oy--;
            }
            osigma += oysq * ((4 * ox) + 6);
        }

        if (icalc)
        {
            if (isigma >= 0)
            {
                isigma += ixsqtimes4 * (1 - iy);
                iy--;
            }
            isigma += iysq * ((4 * ix) + 6);
        }

        ox++;

        if ((oysq*ox) > (oxsq*oy))
            break;

        if (icalc)
            ix++;

        if (((iysq*ix) > (ixsq*iy)) && icalc)
        {
            icalc = 0;
            ix--;
        }
    }

    /* B half */
    ox = radius;
    oy = 0;
    osigma = 2*oxsq + oysq * (1 - 2 * radius);

    ix = iradius;
    iy = 0;
    isigma = 2*ixsq + iysq * (1 - 2 * iradius);

    icalc = 1;
    ocalc = 1;

    penthick = 0;
    while (1)
    {
        if (q1draw)
        {
            if (iy == icq1bye)
                icq1bxe = ix;

            if (split == 2)
               if (firstseg)
               {
                    if ((iy >= icq1bys) && (oy <= ocq1bye))
                    {
                        seMDC_DrawLine( centerx+((oy < ocq1bys) ? ocq1bxs : ox), centery-((oy < ocq1bys) ? ocq1bys : oy),
                                        centerx+((iy > icq1bye) ? icq1bxe : ix), centery-((iy > icq1bye) ? icq1bye : iy), pencolor, penthick );
                        seMDC_WaitGfxDone();
                        seMDC_DrawLine( centerx+((oy < ocq1bys) ? (ocq1bxs-1) : (ox-1)), centery-((oy < ocq1bys) ? ocq1bys : oy),
                                        centerx+((iy > icq1bye) ? icq1bxe : ix), centery-((iy > icq1bye) ? (icq1bye+1) : (iy+1)), pencolor, penthick );
                        seMDC_WaitGfxDone();
                    }
                    if (iy == icq1bye)
                    {
                        icalc = 0;
                    }

                    if (oy == ocq1bye)
                    {
                       firstseg = 0;
                       icalc = 1;
                       if (oy != iy)
                           ocalc = 0;
                       oy = iy;
                    }
               }
               else
               {
                   if (oy == ocq1bye)
                       ocalc = 1;

                   if (iy >= ics2)
                   {
                       seMDC_DrawLine( centerx+((oy < ocys2) ? ocxs2 : ox), centery-((oy < ocys2) ? ocys2 : oy),
                                       centerx+((iy > ice2) ? ice2 : ix), centery-((iy > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx+((oy < ocys2) ? (ocxs2-1) : (ox-1)), centery-((oy < ocys2) ? ocys2 : oy),
                                       centerx+((iy > ice2) ? ice2 : ix), centery-((iy > ice2) ? (ice2+1) : (iy+1)), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
               }
            else
            {
                if ((iy >= icq1bys) && (oy <= ocq1bye))
                {
                    seMDC_DrawLine( centerx+((oy < ocq1bys) ? ocq1bxs : ox), centery-((oy < ocq1bys) ? ocq1bys : oy),
                                    centerx+((iy > icq1bye) ? icq1bxe : ix), centery-((iy > icq1bye) ? icq1bye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                    seMDC_DrawLine( centerx+((oy < ocq1bys) ? (ocq1bxs-1) : (ox-1)), centery-((oy < ocq1bys) ? ocq1bys : oy),
                                    centerx+((iy > icq1bye) ? icq1bxe : ix), centery-((iy > icq1bye) ? (icq1bye+1) : (iy+1)), pencolor, penthick );
                    seMDC_WaitGfxDone();
                }
            }
        }

        if (q2draw)
        {
            if (iy == icq2bye)
                icq2bxe = ix;

            if (split == 8)
               if (firstseg)
               {
                   if ((iy >= icq2bys) && (oy <= ocq2bye))
                   {
                       seMDC_DrawLine( centerx-((oy < ocq2bys) ? ocq2bxs : ox), centery-((oy < ocq2bys) ? ocq2bys : oy),
                                       centerx-((iy > icq2bye) ? icq2bxe : ix), centery-((iy > icq2bye) ? icq2bye : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx-((oy < ocq2bys) ? (ocq2bxs-1) : (ox-1)), centery-((oy < ocq2bys) ? ocq2bys : oy),
                                       centerx-((iy > icq2bye) ? icq2bxe : ix), centery-((iy > icq2bye) ? (icq2bye+1) : (iy+1)), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
                   if (iy == icq2bye)
                   {
                       icalc = 0;
                   }

                   if (oy == ocq2bye)
                   {
                      firstseg = 0;
                      icalc = 1;
                      if (oy != iy)
                          ocalc = 0;
                      oy = iy;
                   }
               }
               else
               {
                   if (oy == ocq2bye)
                       ocalc = 1;

                   if (iy >= ics2)
                   {
                       seMDC_DrawLine( centerx-((oy < ocys2) ? ocxs2 : ox), centery-((oy < ocys2) ? ocys2 : oy),
                                       centerx-((iy > ice2) ? ice2 : ix), centery-((iy > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx-((oy < ocys2) ? (ocxs2-1) : (ox-1)), centery-((oy < ocys2) ? ocys2 : oy),
                                       centerx-((iy > ice2) ? ice2 : ix), centery-((iy > ice2) ? (ice2+1) : (iy+1)), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
               }
            else
            {
                if ((iy >= icq2bys) && (oy <= ocq2bye))
                {
                    seMDC_DrawLine( centerx-((oy < ocq2bys) ? ocq2bxs : ox), centery-((oy < ocq2bys) ? ocq2bys : oy),
                                    centerx-((iy > icq2bye) ? icq2bxe : ix), centery-((iy > icq2bye) ? icq2bye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                    seMDC_DrawLine( centerx-((oy < ocq2bys) ? (ocq2bxs-1) : (ox-1)), centery-((oy < ocq2bys) ? ocq2bys : oy),
                                    centerx-((iy > icq2bye) ? icq2bxe : ix), centery-((iy > icq2bye) ? (icq2bye+1) : (iy+1)), pencolor, penthick );
                    seMDC_WaitGfxDone();
                }
            }
        }

        if (q3draw)
        {
            if (iy == icq3bye)
                icq3bxe = ix;

            if (split == 6)
               if (firstseg)
               {
                   if ((iy >= icq3bys) && (oy <= ocq3bye))
                   {
                       seMDC_DrawLine( centerx-((oy < ocq3bys) ? ocq3bxs : ox), centery+((oy < ocq3bys) ? ocq3bys : oy),
                                       centerx-((iy > icq3bye) ? icq3bxe : ix), centery+((iy > icq3bye) ? icq3bye : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx-((oy < ocq3bys) ? (ocq3bxs-1) : (ox-1)), centery+((oy < ocq3bys) ? ocq3bys : oy),
                                       centerx-((iy > icq3bye) ? icq3bxe : ix), centery+((iy > icq3bye) ? (icq3bye+1) : (iy+1)), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
                   if (iy == icq3bye)
                   {
                       icalc = 0;
                   }

                   if (oy == ocq3bye)
                   {
                      firstseg = 0;
                      icalc = 1;
                      if (oy != iy)
                          ocalc = 0;
                      oy = iy;
                   }
               }
               else
               {
                   if (oy == ocq3bye)
                       ocalc = 1;

                   if (iy >= ics2)
                   {
                       seMDC_DrawLine( centerx-((oy < ocys2) ? ocxs2 : ox), centery+((oy < ocys2) ? ocys2 : oy),
                                       centerx-((iy > ice2) ? ice2 : ix), centery+((iy > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx-((oy < ocys2) ? (ocxs2-1) : (ox-1)), centery+((oy < ocys2) ? ocys2 : oy),
                                       centerx-((iy > ice2) ? ice2 : ix), centery+((iy > ice2) ? (ice2+1) : (iy+1)), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
               }
            else
            {
                if ((iy >= icq3bys) && (oy <= ocq3bye))
                {
                    seMDC_DrawLine( centerx-((oy < ocq3bys) ? ocq3bxs : ox), centery+((oy < ocq3bys) ? ocq3bys : oy),
                                    centerx-((iy > icq3bye) ? icq3bxe : ix), centery+((iy > icq3bye) ? icq3bye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                    seMDC_DrawLine( centerx-((oy < ocq3bys) ? (ocq3bxs-1) : (ox-1)), centery+((oy < ocq3bys) ? ocq3bys : oy),
                                    centerx-((iy > icq3bye) ? icq3bxe : ix), centery+((iy > icq3bye) ? (icq3bye+1) : (iy+1)), pencolor, penthick );
                    seMDC_WaitGfxDone();
                }
            }
        }

        if (q4draw)
        {
            if (iy == icq4bye)
                icq4bxe = ix;

            if (split == 4)
               if (firstseg)
               {
                   if ((iy >= icq4bys) && (oy <= ocq4bye))
                   {
                       seMDC_DrawLine( centerx+((oy < ocq4bys) ? ocq4bxs : ox), centery+((oy < ocq4bys) ? ocq4bys : oy),
                                       centerx+((iy > icq4bye) ? icq4bxe : ix), centery+((iy > icq4bye) ? icq4bye : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx+((oy < ocq4bys) ? (ocq4bxs-1) : (ox-1)), centery+((oy < ocq4bys) ? ocq4bys : oy),
                                       centerx+((iy > icq4bye) ? icq4bxe : ix), centery+((iy > icq4bye) ? (icq4bye+1) : (iy+1)), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
                   if (iy == icq4bye)
                   {
                       icalc = 0;
                   }

                   if (oy == ocq4bye)
                   {
                      firstseg = 0;
                      icalc = 1;
                      if (oy != iy)
                          ocalc = 0;
                      oy = iy;
                   }
               }
               else
               {
                   if (oy == ocq4bye)
                       ocalc = 1;

                   if (iy >= ics2)
                   {
                       seMDC_DrawLine( centerx+((oy < ocys2) ? ocxs2 : ox), centery+((oy < ocys2) ? ocys2 : oy),
                                       centerx+((iy > ice2) ? ice2 : ix), centery+((iy > ice2) ? ice2 : iy), pencolor, penthick );
                       seMDC_WaitGfxDone();
                       seMDC_DrawLine( centerx+((oy < ocys2) ? (ocxs2-1) : (ox-1)), centery+((oy < ocys2) ? ocys2 : oy),
                                       centerx+((iy > ice2) ? ice2 : ix), centery+((iy > ice2) ? (ice2+1) : (iy+1)), pencolor, penthick );
                       seMDC_WaitGfxDone();
                   }
               }
            else
            {
                if ((iy >= icq4bys) && (oy <= ocq4bye))
                {
                    seMDC_DrawLine( centerx+((oy < ocq4bys) ? ocq4bxs : ox), centery+((oy < ocq4bys) ? ocq4bys : oy),
                                    centerx+((iy > icq4bye) ? icq4bxe : ix), centery+((iy > icq4bye) ? icq4bye : iy), pencolor, penthick );
                    seMDC_WaitGfxDone();
                    seMDC_DrawLine( centerx+((oy < ocq4bys) ? (ocq4bxs-1) : (ox-1)), centery+((oy < ocq4bys) ? ocq4bys : oy),
                                    centerx+((iy > icq4bye) ? icq4bxe : ix), centery+((iy > icq4bye) ? (icq4bye+1) : (iy+1)), pencolor, penthick );
                    seMDC_WaitGfxDone();
                }
            }
        }

        if (ocalc)
        {
            if (osigma >= 0)
            {
                osigma += oysqtimes4 * (1 - ox);
                ox--;
            }
            osigma += oxsq * ((4 * oy) + 6);
        }

        if (icalc)
        {
            if (isigma >= 0)
            {
                isigma += iysqtimes4 * (1 - ix);
                ix--;
            }
            isigma += ixsq * ((4 * iy) + 6);
        }

        oy++;
        if ((oxsq*oy) > (oysq*ox))
            break;

        if (icalc)
            iy++;

        if (((ixsq*iy) > (iysq*ix)) && icalc)
        {
            icalc = 0;
            iy--;
        }
    }

    return(seSTATUS_OK);
}



/**
  * Draw a dashed arc
  * Parameters:
  *         centerx: Center X coordinate.
  *         centery: Center Y coordinate.
  *         radius: Outer radius.
  *         thickness: Thickness.  Inner radius = radius - thickness
  *         startangle: Start angle.  0 to 511 corresponds to full circle
  *         endangle: End angle.  0 to 511 corresponds to full circle
  *                    (Angle is referenced to positive Y-axis as 0, clockwise direction.
  *                     128 is positive X-axis, 256 is negative Y-axis, 384 is negative X-axis.)
  *         pencolor: Pen color.
  *         dashang: Dash angle segments in angle units (0 to 511)
  *         blankang: Blank angle segments in angle units (0 to 511)
  * Return value:  Status
  */
seStatus seMDC_GFX_DrawDArc ( uint16_t centerx, uint16_t centery, uint16_t radius, uint16_t thickness,
                              uint16_t startangle, uint16_t endangle, uint16_t pencolor, uint16_t dashang, uint16_t blankang)
{
    seStatus fResult = seSTATUS_OK;
    uint32_t start, end, endangle1;

    if ((startangle > 511) || (endangle > 511))
        return(seSTATUS_NG);

    if (startangle > endangle)
        endangle1 = endangle + 512;
    else
        endangle1 = endangle;

    start = (uint32_t) startangle;
    while (start < endangle1)
    {
        end = start + dashang - 1;
        if (end > endangle1)
            end = endangle1;

        fResult = seMDC_GFX_DrawArc(centerx, centery, radius, thickness, start%512, end%512, pencolor);
        if (fResult != seSTATUS_OK)
            break;

        start = start + dashang + blankang;
    }

    return(fResult);
}



/**
  * Draw a polygon outline.
  * Parameters:
  *         polygon:   pointer to polygon parameters structure
  * Return value:  Status
  */
seStatus seMDC_GFX_DrawPolygon ( seMDC_GFX_PolygonStruct *polygon )
{
    seStatus fResult = seSTATUS_OK;
    uint16_t k;
    int32_t xcoord1, ycoord1, xcoord2, ycoord2, xcoordtmp, ycoordtmp;
    int32_t sinval, cosval;
    uint16_t rotval;

    for (k = 0; k < polygon->numpoints; k++)
    {
        // Calculate scaled coordinates
        xcoord1 = polygon->points[k].xcoord;
        xcoord1 = (xcoord1*polygon->scaleval);
        if (xcoord1 < 0)
            xcoord1 -= 128;
        else
            xcoord1 += 128;
        xcoord1 = (xcoord1 >> 8);

        ycoord1 = polygon->points[k].ycoord;
        ycoord1 = (ycoord1*polygon->scaleval);
        if (ycoord1 < 0)
            ycoord1 -= 128;
        else
            ycoord1 += 128;
        ycoord1 = (ycoord1 >> 8);

        if (k < (polygon->numpoints-1))
        {
            xcoord2 = polygon->points[k+1].xcoord;
            ycoord2 = polygon->points[k+1].ycoord;
        }
        else
        {
            xcoord2 = polygon->points[0].xcoord;
            ycoord2 = polygon->points[0].ycoord;
        }

        xcoord2 = (xcoord2*polygon->scaleval);
        if (xcoord2 < 0)
            xcoord2 -= 128;
        else
            xcoord2 += 128;
        xcoord2 = (xcoord2 >> 8);

        ycoord2 = (ycoord2*polygon->scaleval);
        if (ycoord2 < 0)
            ycoord2 -= 128;
        else
            ycoord2 += 128;
        ycoord2 = (ycoord2 >> 8);

        // Calculate rotated coordinates
        rotval = polygon->rotval;
        if (rotval < 128)
        {
            cosval = sinlut[128-rotval];
            sinval = sinlut[rotval];
        }
        else if (polygon->rotval < 256)
        {
            cosval = -sinlut[rotval-128];
            sinval = sinlut[256-rotval];
        }
        else if (polygon->rotval < 384)
        {
            cosval = -sinlut[384-rotval];
            sinval = -sinlut[rotval-256];
        }
        else
        {
            cosval = sinlut[rotval-384];
            sinval = -sinlut[512-rotval];
        }

        xcoordtmp = (xcoord1 * cosval) - (ycoord1 * sinval);
        ycoordtmp = (xcoord1 * sinval) + (ycoord1 * cosval);
        if (xcoordtmp < 1)
            xcoordtmp -= 32768;
        else
            xcoordtmp += 32768;
        xcoord1 = xcoordtmp >> 16;

        if (ycoordtmp < 1)
            ycoordtmp -= 32768;
        else
            ycoordtmp += 32768;
        ycoord1 = ycoordtmp >> 16;

        xcoordtmp = (xcoord2 * cosval) - (ycoord2 * sinval);
        ycoordtmp = (xcoord2 * sinval) + (ycoord2 * cosval);
        if (xcoordtmp < 1)
            xcoordtmp -= 32768;
        else
            xcoordtmp += 32768;
        xcoord2 = xcoordtmp >> 16;

        if (ycoordtmp < 1)
            ycoordtmp -= 32768;
        else
            ycoordtmp += 32768;
        ycoord2 = ycoordtmp >> 16;

        // Center adjustment
        xcoord1 = polygon->centerx + xcoord1;
        ycoord1 = polygon->centery - ycoord1;
        xcoord2 = polygon->centerx + xcoord2;
        ycoord2 = polygon->centery - ycoord2;

        if (fResult == seSTATUS_OK) {
            fResult = seMDC_GFX_DrawLine(xcoord1, ycoord1, xcoord2, ycoord2,
                                   polygon->pencolor, polygon->thickness, 1);
            seMDC_WaitGfxDone();
        }
        else
            break;
  }

  return fResult;
}



/**
  * UTF-8 to Unicode converter.
  * Parameters:
  *         textstrp:  point to pointer to first byte of UTF-8 character
  *                    (*textstrp is incremented according to UTF-8 code)
  * Return value:  32-bit Unicode value or 0xFFFFFFFF if invalid UTF-8 code
  */
uint32_t UTF8toUnicode ( char **textstrp )
{
	uint32_t unicodeval;
	if (*(*textstrp) & 0x80)  // Check >= 01111111
        if ((*(*textstrp) & 0xE0) == 0xC0)  // Check 2-byte 110xxxxx
        {
            unicodeval = *(*textstrp) & 0x1F;
            unicodeval = (unicodeval << 6);
            *textstrp += 1;
            if ((*(*textstrp) & 0xC0) == 0x80)  // Check second byte 10xxxxxx
            {
                unicodeval |= (*(*textstrp) & 0x3F);
                *textstrp += 1;
            }
            else
            {
                unicodeval = 0xFFFFFFFF;
                *textstrp += 1;
            }
        }
        else
            if ((*(*textstrp) & 0xF0) == 0xE0)  // Check 3-byte 1110xxxx
            {
                unicodeval = (*(*textstrp) & 0x0F);
                unicodeval = (unicodeval << 6);
                *textstrp += 1;
                if ((*(*textstrp) & 0xC0) == 0x80)  // Check second byte 10xxxxxx
                {
                    unicodeval |= (*(*textstrp) & 0x3F);
                    unicodeval = (unicodeval << 6);
                    *textstrp += 1;
                    if ((*(*textstrp) & 0xC0) == 0x80)  // Check third byte 10xxxxxx
                    {
                        unicodeval |= (*(*textstrp) & 0x3F);
                        *textstrp += 1;
                    }
                    else
                    {
                        unicodeval = 0xFFFFFFFF;
                        *textstrp += 1;
                    }
                }
                else
                {
                    unicodeval = 0xFFFFFFFF;
                    *textstrp += 2;
                }
            }
            else
                if ((*(*textstrp) & 0xF8) == 0xF0)  // Check 4-byte 11110xxx
                {
                    unicodeval = (*(*textstrp) & 0x07);
                    unicodeval = (unicodeval << 6);
                    *textstrp += 1;
                    if ((*(*textstrp) & 0xC0) == 0x80)  // Check second byte 10xxxxxx
                    {
                        unicodeval |= (*(*textstrp) & 0x3F);
                        unicodeval = (unicodeval << 6);
                        *textstrp += 1;
                        if ((*(*textstrp) & 0xC0) == 0x80)  // Check third byte 10xxxxxx
                        {
                            unicodeval |= (*(*textstrp) & 0x3F);
                            unicodeval = (unicodeval << 6);
                            *textstrp += 1;
                            if ((*(*textstrp) & 0xC0) == 0x80)  // Check fourth byte 10xxxxxx
                            {
                                unicodeval |= (*(*textstrp) & 0x3F);
                                unicodeval = (unicodeval << 6);
                                *textstrp += 1;
                            }
                            else
                            {
                                unicodeval = 0xFFFFFFFF;
                                *textstrp += 1;
                            }
                        }
                        else
                        {
                            unicodeval = 0xFFFFFFFF;
                            *textstrp += 2;
                        }
                    }
                    else
                    {
                        unicodeval = 0xFFFFFFFF;
                        *textstrp += 3;
                    }
                }
                else
                {
                    unicodeval = 0xFFFFFFFF;
                    *textstrp += 1;
                }
    else
    {
        unicodeval = *(*textstrp) & 0x7F;
        *textstrp += 1;
    }

    return(unicodeval);
}




/**
  * Calculate the number of UTF-8 characters in a string.
  * Parameters:
  *         textstr:  pointer to text string
  * Return value:  Number of UTF-8 characters in the string.
  */
uint16_t utf8strlen ( char *textstr )
{
    uint16_t charcnt;

    charcnt = 0;
    while(*textstr != 0)
	{
        UTF8toUnicode(&textstr);
        charcnt++;
	}

    return(charcnt);
}




/**
  * Display a string using a bitmap font set.  Starting point in Destination Window is in the center of character.
  * Parameters:
  *         putstr_params:  Parameters structure for string display
  *         textstr:  pointer to text string
  * Return value:  Status
  */
seStatus seMDC_GFX_PutString ( seMDC_GFX_PutStr_Params *putstr_params, char *textstr )
{
    uint32_t unicode_base1, unicode_base2, unicode_val;
    uint32_t numfontchars1, numfontchars2;
    seMDC_GFX_STR_JUSTIFICATION justify;
    seMDC_GFX_FontChar fontchar;
    uint8_t *pxdata, extloc;
    uint16_t stringlen;
    int32_t ixcenter;
    int32_t oxcenter, originx, originy;
    uint16_t fontwidth, fontheight, pencolor, rotval, xscale,yscale;
    seMDC_ImgCopyRotScaleCtrl cpyctrl;
    uint32_t fontoffset;
    uint32_t k;
    seStatus fResult;
    seMDC_BITMAPFMT bitmapfmt;
    char *textstr1;

    long gfxOWidth   = seS1D13C00Read16( MDC_GFXOWIDTH );
    long gfxOHeight  = seS1D13C00Read16( MDC_GFXOHEIGHT );

    unicode_base1 = putstr_params->font1->unicode_base;
    numfontchars1 = putstr_params->font1->numchars;
    unicode_base2 = putstr_params->font2->unicode_base;
    numfontchars2 = putstr_params->font2->numchars;
    justify = putstr_params->justify;
    stringlen = utf8strlen(textstr);
    rotval = putstr_params->rotval;
    originx = putstr_params->destx;
    originy = putstr_params->desty;
    xscale = putstr_params->xscale;
    yscale = putstr_params->yscale;

    // Go through each character
    ixcenter = 0;
    oxcenter = originx;
    k = 0;
    textstr1 = textstr;
    while (*textstr1 != 0)
    {
    	unicode_val = UTF8toUnicode(&textstr1);

        // Check if character is not in font set character range
        if ((unicode_val >= unicode_base1) && (unicode_val < (unicode_base1 + numfontchars1)))
        {
            if (putstr_params->extloc1)
            {
                seDMAC_MemCpy8((uint32_t) &(putstr_params->font1->charstbl[unicode_val - unicode_base1]), putstr_params->extbuffaddr, sizeof(seMDC_GFX_FontChar), putstr_params->dmachan);
                seS1D13C00Read(putstr_params->extbuffaddr, (uint8_t *) &(fontchar), sizeof(seMDC_GFX_FontChar));
            }
            else
            {
                fontchar.width = putstr_params->font1->charstbl[unicode_val - unicode_base1].width;
                fontchar.offsetloc = putstr_params->font1->charstbl[unicode_val - unicode_base1].offsetloc;
            }
        }
        else if ((unicode_val >= unicode_base2) && (unicode_val < (unicode_base2 + numfontchars2)))
        {
            if (putstr_params->extloc2)
            {
                seDMAC_MemCpy8((uint32_t) &(putstr_params->font2->charstbl[unicode_val - unicode_base2]), putstr_params->extbuffaddr, sizeof(seMDC_GFX_FontChar), putstr_params->dmachan);
                seS1D13C00Read(putstr_params->extbuffaddr, (uint8_t *) &(fontchar), sizeof(seMDC_GFX_FontChar));
            }
            else
            {
                fontchar.width = putstr_params->font2->charstbl[unicode_val - unicode_base2].width;
                fontchar.offsetloc = putstr_params->font2->charstbl[unicode_val - unicode_base2].offsetloc;
            }
        }
        else
            return seSTATUS_NG;

        // Rotate whole string, use input X coordinate
        if ((putstr_params->rotation == seMDC_GFX_ROTSTRING) && (rotval != 0))
        {
            // Calculate starting ixcenter value according to justification
            if (justify == seMDC_GFX_CENTER_JUSTIFIED)      // Center-justified
            {
                ixcenter += fontchar.width;
                if (*textstr1 == 0)
                    ixcenter = ixcenter >> 1;
            }
            else if (justify == seMDC_GFX_LEFT_JUSTIFIED)   // Left-justified
            {
                if (k == 0)
                    ixcenter += fontchar.width>>1;
            }
            else                                            // Right-justified
            {
                if (k < (stringlen-1))
                    ixcenter += fontchar.width;
                else
                    ixcenter += fontchar.width>>1;
            }
        }
        else    // Rotate each character or rotation is 0, use output X coordinate
        {
            // Calculate starting oxcenter value according to justification
            if (justify == seMDC_GFX_CENTER_JUSTIFIED)    // Center-justified
            {
                if (k == 0)
                    oxcenter = fontchar.width>>1;
                ixcenter += fontchar.width;
                if (*textstr1 == 0)
                {
                    ixcenter = ixcenter >> 1;
                    ixcenter = ixcenter - oxcenter;
                }
            }
            else if (justify == seMDC_GFX_RIGHT_JUSTIFIED)  // Right-justified
            {
                if ((k < (stringlen-1)) && (k != 0))
                    ixcenter = fontchar.width;
                else
                {
                    if (k == 0)
                    {
                        oxcenter = fontchar.width;
                    }
                    else
                    {
                        ixcenter += ((oxcenter + fontchar.width)+1)>>1;
                    }
                }
            }
        }
        k++;
    }

    oxcenter = ((((int32_t) ixcenter)*xscale)+128)>>8;
    oxcenter = originx - oxcenter;

    pencolor = putstr_params->textcolor;

    cpyctrl.ctrlword = 0;
    cpyctrl.ctrlword_b.bitmapen = 1;

     // Rotate whole string
     if ((putstr_params->rotation == seMDC_GFX_ROTSTRING) && (rotval != 0))
     {
         while(*textstr != 0)
         {
        	 unicode_val = UTF8toUnicode(&textstr);

             if ((unicode_val >= unicode_base1) && (unicode_val < (unicode_base1 + numfontchars1)))
             {
                 if (putstr_params->extloc1)
                 {
                	 extloc = 1;
                     seDMAC_MemCpy8((uint32_t) &(putstr_params->font1->charstbl[unicode_val - unicode_base1]), putstr_params->extbuffaddr, sizeof(seMDC_GFX_FontChar), putstr_params->dmachan);
                     seS1D13C00Read(putstr_params->extbuffaddr, (uint8_t *) &(fontchar), sizeof(seMDC_GFX_FontChar));
                 }
                 else
                 {
                	 extloc = 0;
                     fontchar.width = putstr_params->font1->charstbl[unicode_val - unicode_base1].width;
                     fontchar.offsetloc = putstr_params->font1->charstbl[unicode_val - unicode_base1].offsetloc;
                 }

                 pxdata = putstr_params->font1->pxdata;
                 fontheight = putstr_params->font1->height;
                 bitmapfmt = putstr_params->font1->bitmapfmt;
                 cpyctrl.ctrlword_b.bitmapfmt = bitmapfmt;
             }
             else if ((unicode_val >= unicode_base2) && (unicode_val < (unicode_base2 + numfontchars2)))
             {
                 if (putstr_params->extloc2)
                 {
                	 extloc = 1;
                     seDMAC_MemCpy8((uint32_t) &(putstr_params->font2->charstbl[unicode_val - unicode_base2]), putstr_params->extbuffaddr, sizeof(seMDC_GFX_FontChar), putstr_params->dmachan);
                     seS1D13C00Read(putstr_params->extbuffaddr, (uint8_t *) &(fontchar), sizeof(seMDC_GFX_FontChar));
                 }
                 else
                 {
                	 extloc = 0;
                     fontchar.width = putstr_params->font2->charstbl[unicode_val - unicode_base2].width;
                     fontchar.offsetloc = putstr_params->font2->charstbl[unicode_val - unicode_base2].offsetloc;
                 }

                 pxdata = putstr_params->font2->pxdata;
                 fontheight = putstr_params->font2->height;
                 bitmapfmt = putstr_params->font2->bitmapfmt;
                 cpyctrl.ctrlword_b.bitmapfmt = bitmapfmt;
             }


             fontwidth = fontchar.width;
             k = ((fontwidth>>(3-bitmapfmt))+1) * fontheight;

             if (extloc)
             {
                 fontoffset = ((uint32_t) pxdata) + fontchar.offsetloc;
             }
             else
             {
                 fontoffset = putstr_params->extbuffaddr;
                 seS1D13C00Write(fontoffset, pxdata + fontchar.offsetloc, k);
             }

             fResult = seMDC_ImgCpyRotScale(originx, originy, fontoffset, fontwidth, fontwidth, fontheight, ixcenter, fontheight>>1,
                                            pencolor, rotval, xscale, xscale, yscale, yscale, &cpyctrl);
             seMDC_WaitGfxDone();
             if (fResult != seSTATUS_OK)
                 return(fResult);

             ixcenter -= fontwidth;
         }
     }
     else   // Rotate individual characters
     {
    	 textstr1 = textstr;
    	 unicode_val = UTF8toUnicode(&textstr);

         if ((unicode_val >= unicode_base1) && (unicode_val < (unicode_base1 + numfontchars1)))
         {
             if ((putstr_params->extloc1) && (*textstr != 0))
             {
            	 extloc = 1;
                 seDMAC_MemCpy8((uint32_t) &(putstr_params->font1->charstbl[unicode_val - unicode_base1]), putstr_params->extbuffaddr, sizeof(seMDC_GFX_FontChar), putstr_params->dmachan);
                 seS1D13C00Read(putstr_params->extbuffaddr, (uint8_t *) &(fontchar), sizeof(seMDC_GFX_FontChar));
             }
             else
             {
            	 extloc = 0;
                 fontchar.width = putstr_params->font1->charstbl[unicode_val - unicode_base1].width;
                 fontchar.offsetloc = putstr_params->font1->charstbl[unicode_val - unicode_base1].offsetloc;
             }

             pxdata = putstr_params->font1->pxdata;
             fontheight = putstr_params->font1->height;
             bitmapfmt = putstr_params->font1->bitmapfmt;
             cpyctrl.ctrlword_b.bitmapfmt = bitmapfmt;
         }
         else if ((unicode_val >= unicode_base2) && (unicode_val < (unicode_base2 + numfontchars2)))
         {
             if ((putstr_params->extloc2) && (*textstr != 0))
             {
            	 extloc = 1;
                 seDMAC_MemCpy8((uint32_t) &(putstr_params->font2->charstbl[unicode_val - unicode_base2]), putstr_params->extbuffaddr, sizeof(seMDC_GFX_FontChar), putstr_params->dmachan);
                 seS1D13C00Read(putstr_params->extbuffaddr, (uint8_t *) &(fontchar), sizeof(seMDC_GFX_FontChar));
             }
             else
             {
            	 extloc = 0;
                 fontchar.width = putstr_params->font2->charstbl[unicode_val - unicode_base2].width;
                 fontchar.offsetloc = putstr_params->font2->charstbl[unicode_val - unicode_base2].offsetloc;
             }

             pxdata = putstr_params->font2->pxdata;
             fontheight = putstr_params->font2->height;
             bitmapfmt = putstr_params->font2->bitmapfmt;
             cpyctrl.ctrlword_b.bitmapfmt = bitmapfmt;
         }

         while(*textstr1 != 0)
         {
             fontwidth = fontchar.width;
             k = ((fontwidth>>(3-bitmapfmt))+1) * fontheight;

             if (extloc)
             {
                 fontoffset = ((uint32_t) pxdata) + fontchar.offsetloc;
             }
             else
             {
                 fontoffset = putstr_params->extbuffaddr;
                 seS1D13C00Write(fontoffset, pxdata + fontchar.offsetloc, k);
             }

             if ((oxcenter >= 0) && (oxcenter < gfxOWidth) && (originy >= 0) && (originy < gfxOHeight)) {
                 fResult = seMDC_ImgCpyRotScale(oxcenter, originy, fontoffset, fontwidth, fontwidth, fontheight, fontwidth>>1, fontheight>>1,
                                                pencolor, rotval, xscale, xscale, yscale, yscale, &cpyctrl);
                 seMDC_WaitGfxDone();
             }
             else
                 fResult = seSTATUS_OK;

             if (fResult != seSTATUS_OK)
                 return(fResult);

             textstr1 = textstr;
             unicode_val = UTF8toUnicode(&textstr);

             if ((unicode_val >= unicode_base1) && (unicode_val < (unicode_base1 + numfontchars1)))
             {
                 if ((putstr_params->extloc1) && (*textstr1 != 0))
                 {
                	 extloc = 1;
                     seDMAC_MemCpy8((uint32_t) &(putstr_params->font1->charstbl[unicode_val - unicode_base1]), putstr_params->extbuffaddr, sizeof(seMDC_GFX_FontChar), putstr_params->dmachan);
                     seS1D13C00Read(putstr_params->extbuffaddr, (uint8_t *) &(fontchar), sizeof(seMDC_GFX_FontChar));
                 }
                 else
                 {
                	 extloc = 0;
                	 if (*textstr1 != 0)
                	 {
                         fontchar.width = putstr_params->font1->charstbl[unicode_val - unicode_base1].width;
                         fontchar.offsetloc = putstr_params->font1->charstbl[unicode_val - unicode_base1].offsetloc;
                	 }
                 }

                 pxdata = putstr_params->font1->pxdata;
                 fontheight = putstr_params->font1->height;
                 bitmapfmt = putstr_params->font1->bitmapfmt;
                 cpyctrl.ctrlword_b.bitmapfmt = bitmapfmt;
             }
             else if ((unicode_val >= unicode_base2) && (unicode_val < (unicode_base2 + numfontchars2)))
             {
                 if ((putstr_params->extloc2) && (*textstr1 != 0))
                 {
                	 extloc = 1;
                     seDMAC_MemCpy8((uint32_t) &(putstr_params->font2->charstbl[unicode_val - unicode_base2]), putstr_params->extbuffaddr, sizeof(seMDC_GFX_FontChar), putstr_params->dmachan);
                     seS1D13C00Read(putstr_params->extbuffaddr, (uint8_t *) &(fontchar), sizeof(seMDC_GFX_FontChar));
                 }
                 else
                 {
                	 extloc = 0;
                	 if (*textstr1 != 0)
                	 {
                         fontchar.width = putstr_params->font2->charstbl[unicode_val - unicode_base2].width;
                         fontchar.offsetloc = putstr_params->font2->charstbl[unicode_val - unicode_base2].offsetloc;
                	 }
                 }

                 pxdata = putstr_params->font2->pxdata;
                 fontheight = putstr_params->font2->height;
                 bitmapfmt = putstr_params->font2->bitmapfmt;
                 cpyctrl.ctrlword_b.bitmapfmt = bitmapfmt;
             }

             ixcenter -= ((fontwidth + fontchar.width)+1)>>1;
             oxcenter = ((((int32_t) ixcenter)*xscale)+128)>>8;
             oxcenter = originx - oxcenter;
         }
     }

    return fResult;
}
