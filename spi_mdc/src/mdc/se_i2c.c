/**
  ******************************************************************************
  * @file    se_i2c.c
  * @author  Epson
  * @version V1.0
  * @date    27-November-2017
  * @brief   This file provides the I2C hardware functions.
  ******************************************************************************
  * @attention
  *
  * Copyright(C) SEIKO EPSON CORPORATION 2017-2018. All rights reserved.
  ******************************************************************************
  */


#include "s1d13c00_hcl.h"
#include "s1d13C00_memregs.h"
#include "se_common.h"
#include "se_clg.h"
#include "se_port.h"
#include "se_i2c.h"

/**
  * Static functions declarations
  */
typedef enum {
  seI2C_STOP_CND      = 0x01,		///< I2C status : STOP condition.
  seI2C_START_CND     = 0x02,		///< I2C status : START condition.
  seI2C_TIME_OUT_CND  = 0x03		///< I2C status : START/STOP condition timeout.    
} seStartStopCondition;


inline seStartStopCondition waitI2cSlvStartStopCondition( void );
static seStatus setI2cMstStopCondition( void );
static seStatus setI2cMstStartCondition( void );

seI2C_ChannelDef I2C_CH0 = {
             {sePORT_P04, sePORT_ALT_0},  // SCL
             {sePORT_P05, sePORT_ALT_0}   // SDA
};


seStatus seI2C_Init( seI2C_InitTypeDef * I2C_InitStruct )
{
    seStatus fStatus = seSTATUS_NG;

    ///< Stop I2C.
    seI2C_Disable();

    ///< Configure port pins for I2C
    if ((fStatus = ConfigurePortsForI2C()) == seSTATUS_OK)
    {
        ///< Configuring the pins succeeded, so setup for Master or Slave mode
        if ( I2C_InitStruct->I2C_mode == seI2C_MODE_MASTER )
        {
            seSetBits16( I2C_CTL, I2C_MST_bits, I2C_MST_bits );   ///< Set master mode.
            seSetBits16( I2C_CLK, I2C_CLKSRC_bits, I2C_InitStruct->ClkSrc );
            seSetBits16( I2C_CLK, I2C_CLKDIV_bits, I2C_InitStruct->ClkDivider << 4 );
            seS1D13C00Write16( I2C_BR, I2C_InitStruct->BRT );
        }
        else
        {
            seSetBits16( I2C_CTL, I2C_MST_bits, 0 );   ///< Set slave mode.
            seSetBits16( I2C_MOD, I2C_OADR10_bits, I2C_InitStruct->AddrMode << 2 );
            seSetBits16( I2C_MOD, I2C_GCEN_bits, I2C_InitStruct->RespGenCalls << 1 );
            seS1D13C00Write16( I2C_OADR, I2C_InitStruct->SlaveAddr );
        }

        ///< Disable I2C interrupts.
        seS1D13C00Write16( I2C_INTE, 0 );
    }

    return fStatus;
}


void seI2C_InitStructForMaster( seI2C_InitTypeDef* I2C_InitStruct )
{
    I2C_InitStruct->I2C_mode    = seI2C_MODE_MASTER;
    I2C_InitStruct->ClkSrc      = seCLG_IOSC;
    I2C_InitStruct->ClkDivider  = seI2C_IOSC_CLKDIV_2;          // 20MHz / 2 = 10MHz

    ///< Set transfer rate( bps = CLK_I2C / (BRT + 3) x 2 ).
    ///<  8.00MHz / 20 = 400000 bps.
    ///< 10.00MHz / 25 = 400000 bps.
    I2C_InitStruct->BRT = 22;
}


void seI2C_InitStructForSlave( seI2C_InitTypeDef* I2C_InitStruct )
{
    I2C_InitStruct->I2C_mode      = seI2C_MODE_SLAVE;
    I2C_InitStruct->AddrMode      = seI2C_7BIT_SLV_ADDR;
    I2C_InitStruct->RespGenCalls  = seDISABLE;
    I2C_InitStruct->SlaveAddr     = 0x50;
}


seStatus seI2C_Reset( void )
{
    seStatus fStatus = seSTATUS_OK;

    seSetBits16( I2C_CTL, I2C_SFTRST_bits, I2C_SFTRST_bits );
    while( seS1D13C00Read16( I2C_CTL ) & I2C_SFTRST_bits );

    return fStatus;
}


seStatus seI2C_Enable( void )
{
  seStatus fStatus = seI2C_Reset();

  seSetBits16( I2C_CTL, I2C_MODEN_bits, I2C_MODEN_bits );

  return fStatus;
}


void seI2C_Disable( void )
{
    seSetBits16( I2C_CTL, I2C_MODEN_bits, 0 );
}


seStatus seI2C_MstSendData( uint16_t address, uint8_t data[], uint32_t size, uint32_t stop_pending )
{
    ///< Set Start condition.
    seStatus fStatus = setI2cMstStartCondition();
    uint16_t intF;

    if ( fStatus == seSTATUS_OK )
    {
        ///< Send slave address(+Write).
        if ( address > 0x7f )
        {
            address &= 0x3ff;         //< extract 10-bit address
            address |= 0x1e << 10;     //< add leading '11110’ to introduce the 10 bit addressing scheme
            seS1D13C00Write16( I2C_TXD, (uint8_t)((address>>8) << 1) + 0 );

            while( (seS1D13C00Read16( I2C_INTF ) & I2C_TBEIF_bits) == 0 );

            if ( fStatus == seSTATUS_OK ) {
                seS1D13C00Write16( I2C_TXD, (uint8_t)address );
            }
        }
        else
        {
            ///< Check result.
            ///< The result is not checked at the last data.
            seS1D13C00Write16( I2C_TXD, (uint8_t)(address << 1) + 0 );
        }

        while ( size--  && (fStatus==seSTATUS_OK) )
        {
            while( (seS1D13C00Read16( I2C_INTF ) & (I2C_TBEIF_bits | I2C_NACKIF_bits)) == 0 );

            if ( fStatus == seSTATUS_OK )
            {
                intF = seS1D13C00Read16( I2C_INTF );
                if ( intF & I2C_TBEIF_bits) {
                    seS1D13C00Write16( I2C_TXD, *data++ );
                }
                if ( intF & I2C_NACKIF_bits) 
                {
                    seI2C_ClearIntFlag( seI2C_NACKIF );
                    fStatus = seSTATUS_NG;
                    break;
                }
            }
        }

        ///< Set STOP condition.
        if ( ! stop_pending ) {
            if ( seSTATUS_NG == setI2cMstStopCondition() ) {
                fStatus = seSTATUS_NG;
            }
        }
    }

  return fStatus;
}


seStatus seI2C_MstReceiveData( uint16_t address, uint8_t data[], uint32_t size, uint32_t stop_pending )
{
    seStatus fStatus;
    uint16_t intF;

    if ( size == 1 ) {
        seSetBits16( I2C_CTL, I2C_TXNACK_bits, I2C_TXNACK_bits );
    }

    if ( address > 0x7F )
    {
        fStatus = seI2C_MstSendData( address, data, 0, seENABLE );

        if ( fStatus == seSTATUS_OK )
        {
            fStatus = setI2cMstStartCondition();

            if ( fStatus == seSTATUS_OK )
            {
                address &= 0x3ff;         //< extract 10-bit address
                address |= 0x1e << 10;     //< add leading '11110’ to introduce the 10 bit addressing scheme
                seS1D13C00Write8( I2C_TXD, (uint8_t)((address>>8) << 1) + 1);
            }
        }
    }
    else
    {
        fStatus = setI2cMstStartCondition();

        if ( fStatus == seSTATUS_OK )
        {
            ///< Send slave 7-bit address(7 to 1 bits) + Read(1)/Write(0)).
            seS1D13C00Write8( I2C_TXD, (uint8_t)((address << 1) + 1));
        }
    }

    while ( size--  && (fStatus == seSTATUS_OK) )
    {
        while( (seS1D13C00Read16( I2C_INTF ) & (I2C_RBFIF_bits | I2C_NACKIF_bits)) == 0 );

        if ( fStatus == seSTATUS_OK )
        {
            intF = seS1D13C00Read16( I2C_INTF );
            if ( intF & I2C_RBFIF_bits) 
            {
                ///< When the next-to-last data is received, send a NACK.
                if ( size == 1 ) {
                      seSetBits16( I2C_CTL, I2C_TXNACK_bits, I2C_TXNACK_bits );
                }
                ///< receive Data.
                *data++ = seS1D13C00Read8( I2C_RXD );
            }

            if ( intF & I2C_NACKIF_bits) 
            {
                seI2C_ClearIntFlag( seI2C_NACKIF );
                fStatus = seSTATUS_NG;
                break;
            }
        }
    }

    ///< Set STOP condition.
    if ( ! stop_pending )
    {
        if ( seSTATUS_NG == setI2cMstStopCondition( ) ) {
            fStatus = seSTATUS_NG;
        }
    }

    return fStatus;
}


seStatus seI2C_SlvSendData( uint8_t data[], uint32_t size )
{
    seStatus fStatus = seSTATUS_OK;
    uint16_t intF;

    if ( (seS1D13C00Read16( I2C_MOD ) & I2C_OADR10_bits) != 0 ) {
        fStatus = seI2C_SlvReceiveData( data, 0 );
    }

    if ( fStatus == seSTATUS_OK )
    {
        ///< Wait Start flag condition.
        if ( seI2C_START_CND == waitI2cSlvStartStopCondition() )
        {
            ///< Check transfer mode (1:Transmit/0:Receive).
            if ( (seS1D13C00Read16( I2C_INTF ) & I2C_TR_bits) != 0 )
            {
                // Transmit
                seI2C_ClearIntFlag( seI2C_STARTIF );
                while ( size-- )
                {
                    seS1D13C00Write8( I2C_TXD, *data++ );

                    ///< Wait for an interrupt request
                    while( (seS1D13C00Read16(I2C_INTF) & (I2C_TBEIF_bits | I2C_NACKIF_bits | I2C_STOPIF_bits)) == 0 );
                    if ( fStatus == seSTATUS_OK )
                    {
                        intF = seS1D13C00Read16( I2C_INTF );
                        if ( (intF & I2C_NACKIF_bits) != 0 )
                        {
                            seI2C_ClearIntFlag( seI2C_NACKIF );
                            fStatus = seSTATUS_NG;
                        }
                        if ( (intF & I2C_STOPIF_bits) != 0 ) {
                            fStatus = seSTATUS_NG;
                        }
                    }
                    if ( fStatus == seSTATUS_NG ) {
                        break;
                    }
                }

                if ( fStatus == seSTATUS_OK )
                {
                    while( (seS1D13C00Read16( I2C_INTF) & I2C_TBEIF_bits) == 0 );

                    if ( fStatus == seSTATUS_OK ) {
                        seS1D13C00Write8( I2C_TXD, 0 );
                    }
                }

                ///< Wait for the last byte transmitted
                while ( seINTERRUPT_NOT_OCCURRED == seI2C_GetIntFlag( seI2C_BYTEENDIF ) )
                {
                    ;
                }

                seI2C_ClearIntFlag( seI2C_BYTEENDIF );

                waitI2cSlvStartStopCondition();

            }
            else {
                fStatus = seSTATUS_NG;
            }
        }
        else {
            fStatus = seSTATUS_NG;
        }
    }

  return fStatus;
}


seStatus seI2C_SlvReceiveData( uint8_t data[], uint32_t size )
{
    seStatus fStatus = seSTATUS_NG;

    if ( size == 1 ) {
        seSetBits16( I2C_CTL, I2C_TXNACK_bits, I2C_TXNACK_bits );
    }

    if ( seI2C_START_CND == waitI2cSlvStartStopCondition() )
    {
        ///< Clear START condition interrupt.

        ///< Check transfer mode(1:Transmit/0:Receive).
        if ( (seS1D13C00Read16( I2C_INTF ) & I2C_TR_bits) == 0 )
        {
            fStatus = seSTATUS_OK;

            ///< Receive data.
            seI2C_ClearIntFlag( seI2C_STARTIF );

            while ( size-- )
            {
                while( (seS1D13C00Read16( I2C_INTF) & (I2C_RBFIF_bits | I2C_BYTEENDIF_bits)) == 0 );

                if ( fStatus == seSTATUS_NG ) {
                    break;
                    ///< Wait for a STOP condition interrupt.
                }
                seI2C_ClearIntFlag( seI2C_BYTEENDIF );
                if ( size == 1 ) {
                    seSetBits16( I2C_CTL, I2C_TXNACK_bits, I2C_TXNACK_bits );
                }
                if ( (seS1D13C00Read16( I2C_INTF ) & I2C_RBFIF_bits) != 0 ) {
                    ///< Receive NG.
                    *data++ = seS1D13C00Read8( I2C_RXD );
                }
            }
            waitI2cSlvStartStopCondition();
        }
    }

    return fStatus;
}


void seI2C_EnableInt( seI2C_Interrupt irq )
{
    seOrBits16( I2C_INTE, irq );
}

void seI2C_DisableInt( seI2C_Interrupt irq )
{
    seAndBits16( I2C_INTE, ~irq );
}


seInterruptStatus seI2C_GetIntFlag( seI2C_IntFlag flag )
{
    seInterruptStatus fStatus = seINTERRUPT_NOT_OCCURRED;
    uint16_t flagged = seS1D13C00Read16( I2C_INTF );

    if ( flag )
    {
        flagged &= flag;
        if ( flagged == flag ) {
            fStatus = seINTERRUPT_OCCURRED;
        }
    }

    return fStatus;
}


void seI2C_ClearIntFlag( seI2C_IntFlag flag )
{
    seS1D13C00Write16( I2C_INTF, flag );
}


/**
  * Static functions
  */


/**
  * @brief  Issue a START condition.
  * @retval Status
  */
static seStatus setI2cMstStartCondition( void )
{
    seStatus fStatus = seSTATUS_OK;

    ///< Clear START condition interrupt.
    seI2C_ClearIntFlag( seI2C_STARTIF );

    ///< Issue a START condition.
    seSetBits16( I2C_CTL, I2C_TXSTART_bits, I2C_TXSTART_bits );

    ///< Wait START condition.
    while( (seS1D13C00Read16( I2C_INTF ) & I2C_STARTIF_bits) == 0 );

    ///< Clear START condition interrupt.
    seI2C_ClearIntFlag( seI2C_STARTIF );

    return fStatus;
}


/**
  * @brief  Issue a STOP condition.
  * @retval Status
  */
static seStatus setI2cMstStopCondition( void )
{
    seStatus fStatus = seSTATUS_OK;

    ///< Clear STOP condition interrupt.
    seI2C_ClearIntFlag( seI2C_STOPIF );

    ///< Issue a STOP condition.
    seSetBits16( I2C_CTL, I2C_TXSTOP_bits, I2C_TXSTOP_bits );

    ///< Wait Stop flag condition.
    while( (seS1D13C00Read16( I2C_INTF ) & I2C_STOPIF_bits) == 0 );

    ///< Clear STOP condition interrupt.
    seI2C_ClearIntFlag( seI2C_STOPIF );

    return fStatus;
}


/**
  * @brief  wait I2C START/STOP condition.
  * @retval seI2C_status
  */
inline seStartStopCondition waitI2cSlvStartStopCondition( void )
{
    seStatus fStatus = seSTATUS_OK;
    uint16_t intF;

    seStartStopCondition ss_condition = seI2C_TIME_OUT_CND;

    ///< Wait for a START/STOP condition interrupt.
    while( (seS1D13C00Read16( I2C_INTF) & (I2C_STOPIF_bits | I2C_STARTIF_bits)) == 0 );

    if ( fStatus == seSTATUS_OK )
    {
        intF = seS1D13C00Read16( I2C_INTF );

        ///<  Check START condition interrupt.
        if ( (intF & I2C_STARTIF_bits) != 0 )
        {
            ///< Clear START condition interrupt.
            ss_condition = seI2C_START_CND;
        }
        ///< Check STOP condition interrupt.
        else if ( (intF & I2C_STOPIF_bits) != 0 )
        {
            ///< Clear STOP condition interrupt.
            seI2C_ClearIntFlag( seI2C_STOPIF );
            ///< Time-out START/STOP condition interrupt.
            ss_condition = seI2C_STOP_CND;
        } else { }
    }

    return ss_condition;
}


seStatus ConfigurePortsForI2C( void )
{
    seStatus fStatus = seSTATUS_NG;

    // Program port alternate function for SCL
    fStatus = sePORT_InitAsAltFunction( I2C_CH0.SCL.portID, I2C_CH0.SCL.AltFunc );

    // Program port alternate function for SDA
    if (fStatus == seSTATUS_OK)
        fStatus = sePORT_InitAsAltFunction( I2C_CH0.SDA.portID, I2C_CH0.SDA.AltFunc );

  return fStatus;
}

