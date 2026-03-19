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
#ifndef TCA95XX_H_
#define TCA95XX_H_
//#############################################################################
//
//! \file   tca95xx.h
//!
//! \brief  Functional driver for the TCA95xx family of I2C-bus IO expanders.
//!         This driver requires an I2C master driver to communicate with the
//!         TCA95xx devices.
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################

//*****************************************************************************
//! \addtogroup tca95xx
//! @{
//*****************************************************************************

//*****************************************************************************
// Includes
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief Null pointer definition
//!
#ifndef NULL
#define NULL (0)
#endif

#define TCA9535_VERIFY_WRITES

//! \brief Bitmask for no pins
//!
#define TCA9535_NONE    (0x0000)

//! \brief Bitmask for all pins
//!
#define TCA9535_ALL     (0xFFFF)


#define TCA9535_ADDR     (0x20)


//*****************************************************************************
// Function Prototypes
//*****************************************************************************

bool TCA9535_Init(uint8_t ui8Address);
bool TCA9535_SetAsOutputs(uint8_t ui8Address, uint16_t ui16Pins);
bool TCA9535_SetAsInputs(uint8_t ui8Address, uint16_t ui16Pins);
bool TCA9535_SetOutputPins(uint8_t ui8Address, uint16_t ui16Pins);
bool TCA9535_ClearOutputPins(uint8_t ui8Address, uint16_t ui16Pins);
bool TCA9535_SetOutput(uint8_t ui8Address, uint16_t ui16Pins);
bool TCA9535_ToggleOutputPins(uint8_t ui8Address, uint16_t ui16Pins);
uint16_t TCA9535_ReadInputPins(uint8_t ui8Address);
uint16_t TCA9535_ReadOutputPins(uint8_t ui8Address);

#endif /* TCA95XX_H_ */
//*****************************************************************************
//! Close the doxygen group
//! @}
//*****************************************************************************
