/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//#############################################################################
//
//! \file   tca95xx.c
//
//  Group:          MSP
//  Target Device:  MSP430 and MSP432
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: TIDM-CAPTOUCHEMCREF Version 1.00.00.00
// Release Date: 14 March 2016 
//#############################################################################

//*****************************************************************************
//! \addtogroup tca95xx
//! @{
//*****************************************************************************

//*****************************************************************************
// Includes
//*****************************************************************************

#include <eusci_I2C_driver.h>
#include <TCA9535/TCA9535.h>

//*****************************************************************************
// Definitions
//*****************************************************************************

#define TCA9535_IN          (0x00)
#define TCA9535_OUT         (0x02)
#define TCA9535_POLARITY    (0x04)
#define TCA9535_CONFIG      (0x06)

uint16_t g_ui16TCA9535_TxBuff, g_ui16TCA9535_RxBuff;

//*****************************************************************************
// Function Implementations
//*****************************************************************************
bool
TCA9535_Init(uint8_t ui8Address)
{
    //
    // Init the UCB0 I2C Peripheral
    //
    UCB0_MasterI2C_init();

    //
    // Init polarity to 1=1, 0=0 (Default Configuration)
    //
    g_ui16TCA9535_TxBuff = TCA9535_NONE;
    UCB0_I2C_write(ui8Address, TCA9535_POLARITY, (uint8_t*)&g_ui16TCA9535_TxBuff, 2);
#ifdef TCA9535_VERIFY_WRITES
    UCB0_I2C_read(ui8Address, TCA9535_POLARITY, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    if (g_ui16TCA9535_RxBuff != g_ui16TCA9535_TxBuff)
    {
        return false;
    }
#endif

    //
    // Init output registers to low (cleared) (Default Configuration)
    //
    g_ui16TCA9535_TxBuff = TCA9535_NONE;
    UCB0_I2C_write(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_TxBuff, 2);
#ifdef TCA9535_VERIFY_WRITES
    UCB0_I2C_read(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    if (g_ui16TCA9535_RxBuff != g_ui16TCA9535_TxBuff)
    {
        return false;
    }
#endif

    //
    // Set All IO's as Hi-Z Inputs (Default Configuration)
    //
    g_ui16TCA9535_TxBuff = TCA9535_ALL;
    UCB0_I2C_write(ui8Address, TCA9535_CONFIG, (uint8_t*)&g_ui16TCA9535_TxBuff, 2);
#ifdef TCA9535_VERIFY_WRITES
    UCB0_I2C_read(ui8Address, TCA9535_CONFIG, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    if (g_ui16TCA9535_RxBuff != g_ui16TCA9535_TxBuff)
    {
        return false;
    }
#endif

    return true;
}

bool
TCA9535_SetAsOutputs(uint8_t ui8Address, uint16_t ui16Pins)
{
    //
    // Init IO Config
    //
    UCB0_I2C_read(ui8Address, TCA9535_CONFIG, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    g_ui16TCA9535_TxBuff = g_ui16TCA9535_RxBuff & ~(ui16Pins);
    UCB0_I2C_write(ui8Address, TCA9535_CONFIG, (uint8_t*)&g_ui16TCA9535_TxBuff, 2);

#ifdef TCA9535_VERIFY_WRITES
    //
    // If write verification is enabled, read back write and verify it
    //
    UCB0_I2C_read(ui8Address, TCA9535_CONFIG, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    if (g_ui16TCA9535_RxBuff != g_ui16TCA9535_TxBuff)
    {
        return false;
    }
#endif

    return true;
}

bool
TCA9535_SetAsInputs(uint8_t ui8Address, uint16_t ui16Pins)
{
    //
    // Init IO Config
    //
    UCB0_I2C_read(ui8Address, TCA9535_CONFIG, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    g_ui16TCA9535_TxBuff = g_ui16TCA9535_RxBuff | ui16Pins;
    UCB0_I2C_write(ui8Address, TCA9535_CONFIG, (uint8_t*)&g_ui16TCA9535_TxBuff, 2);

#ifdef TCA9535_VERIFY_WRITES
    //
    // If write verification is enabled, read back write and verify it
    //
    UCB0_I2C_read(ui8Address, TCA9535_CONFIG, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    if (g_ui16TCA9535_RxBuff != g_ui16TCA9535_TxBuff)
    {
        return false;
    }
#endif

    return true;
}

bool
TCA9535_SetOutputPins(uint8_t ui8Address, uint16_t ui16Pins)
{
    //
    // Init IO Config
    //
    UCB0_I2C_read(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    g_ui16TCA9535_TxBuff = g_ui16TCA9535_RxBuff | ui16Pins;
    UCB0_I2C_write(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_TxBuff, 2);

#ifdef TCA9535_VERIFY_WRITES
    //
    // If write verification is enabled, read back write and verify it
    //
    UCB0_I2C_read(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    if (g_ui16TCA9535_RxBuff != g_ui16TCA9535_TxBuff)
    {
        return false;
    }
#endif

    return true;
}

bool
TCA9535_SetOutput(uint8_t ui8Address, uint16_t ui16Pins)
{
    //
    // Init IO Config
    //
    g_ui16TCA9535_TxBuff = ui16Pins;
    UCB0_I2C_write(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_TxBuff, 2);
    return true;
}

bool
TCA9535_ClearOutputPins(uint8_t ui8Address, uint16_t ui16Pins)
{
    //
    // Init IO Config
    //
    UCB0_I2C_read(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    g_ui16TCA9535_TxBuff = g_ui16TCA9535_RxBuff & ~ui16Pins;
    UCB0_I2C_write(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_TxBuff, 2);

#ifdef TCA9535_VERIFY_WRITES
    //
    // If write verification is enabled, read back write and verify it
    //
    UCB0_I2C_read(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    if (g_ui16TCA9535_RxBuff != g_ui16TCA9535_TxBuff)
    {
        return false;
    }
#endif

    return true;
}

bool
TCA9535_ToggleOutputPins(uint8_t ui8Address, uint16_t ui16Pins)
{
    //
    // Read back current pin output state, modify, and write back
    //
    UCB0_I2C_read(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    g_ui16TCA9535_TxBuff = g_ui16TCA9535_RxBuff ^ ui16Pins;
    UCB0_I2C_write(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_TxBuff, 2);

#ifdef TCA9535_VERIFY_WRITES
    //
    // If write verification is enabled, read back write and verify it
    //
    UCB0_I2C_read(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);
    if (g_ui16TCA9535_RxBuff != g_ui16TCA9535_TxBuff)
    {
        return false;
    }
#endif

    return true;
}

uint16_t
TCA9535_ReadInputPins(uint8_t ui8Address)
{
    //
    // Read back current pin output state, modify, and write back
    //
    UCB0_I2C_read(ui8Address, TCA9535_IN, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);

    return g_ui16TCA9535_RxBuff;
}

uint16_t
TCA9535_ReadOutputPins(uint8_t ui8Address)
{
    //
    // Read back current pin output state, modify, and write back
    //
    UCB0_I2C_read(ui8Address, TCA9535_OUT, (uint8_t*)&g_ui16TCA9535_RxBuff, 2);

    return g_ui16TCA9535_RxBuff;
}

//*****************************************************************************
//! Close the doxygen group
//! @}
//*****************************************************************************
