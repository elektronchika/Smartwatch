/**
  ******************************************************************************
  * @file    settings.h
  * @author  Epson
  * @version V1.0
  * @date    6-December-2017
  * @brief   Central configuration file for the S1D13C00 project files.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */


#ifndef SETTINGS_H_
#define SETTINGS_H_

///////////////////////////////////////////////////////////////////////////////
//  Interface between host CPU and S1D13C00
//  Select one, and only one, interface type
///////////////////////////////////////////////////////////////////////////////
// Indirect 8-bit Host Interface
// Pins 2 and 3 of jumper JP19 of S5U13C00P00C100 board should be connected
//#define HOST_INDIRECT8

// SPI/DSPI/QSPI Host Interface
// Pins 1 and 2 of jumper JP19 of S5U13C00P00C100 board should be connected
//#define HOST_SPI
//#define HOST_DSPI
#define HOST_QSPI
///////////////////////////////////////////////////////////////////////////////



#endif /* SETTINGS_H_ */
