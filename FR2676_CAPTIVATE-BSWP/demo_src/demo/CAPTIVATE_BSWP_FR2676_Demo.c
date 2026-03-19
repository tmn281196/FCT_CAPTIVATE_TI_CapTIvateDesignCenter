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
//! \file   CAPTIVATE_BSWP_FR2676_Demo.c
//
//  Group:          MSP
//  Target Devices: MSP430FR2676
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.83.00.05
// Release Date: May 15, 2020
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include <msp430.h>
#include "captivate.h"
#include "CAPT_BSP.h"
#include "CAPTIVATE_BSWP_FR2676_Demo.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//*****************************************************************************
// Global Variables
//*****************************************************************************
volatile bool Demo_keypadSensor = false;
volatile bool Demo_proximitySensor = false;
volatile bool Demo_sliderSensor = false;
volatile bool Demo_wheelSensor = false;

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief The event handler for the keypad sensor.
//! \param pSensor is a pointer to the calling sensor.
void Demo_keypadSensorHandler(tSensor* pSensor);

//! \brief The event handler for the proximity sensor.
//! \param pSensor is a pointer to the calling sensor.
void Demo_proximitySensorHandler(tSensor* pSensor);

//! \brief The event handler for the slider sensor.
//! \param pSensor is a pointer to the calling sensor.
void Demo_sliderSensorHandler(tSensor* pSensor);

//! \brief The event handler for the wheel sensor.
//! \param pSensor is a pointer to the calling sensor.
void Demo_wheelSensorHandler(tSensor* pSensor);


void Demo_init(void)
{
    MAP_CAPT_registerCallback(
                &keypadSensor,
                &Demo_keypadSensorHandler
            );
    MAP_CAPT_registerCallback(
                &proximitySensor,
                &Demo_proximitySensorHandler
            );
    MAP_CAPT_registerCallback(
                &sliderSensor,
                &Demo_sliderSensorHandler
            );
    MAP_CAPT_registerCallback(
                &wheelSensor,
                &Demo_wheelSensorHandler
            );
}

void Demo_keypadSensorHandler(tSensor* pSensor)
{
    if (pSensor->bSensorTouch == true)
    {
        Demo_keypadSensor = true;
    }
    else
    {
        Demo_keypadSensor = false;
    }
}

void Demo_proximitySensorHandler(tSensor* pSensor)
{
    if (pSensor->bSensorProx == true)
    {
        Demo_proximitySensor = true;
    }
    else
    {
        Demo_proximitySensor = false;
    }
}

void Demo_sliderSensorHandler(tSensor* pSensor)
{
    if (pSensor->bSensorTouch == true)
    {
        Demo_sliderSensor = true;
    }
    else
    {
        Demo_sliderSensor = false;
    }
}

void Demo_wheelSensorHandler(tSensor* pSensor)
{
    if (pSensor->bSensorTouch == true)
    {
        Demo_wheelSensor = true;
    }
    else
    {
        Demo_wheelSensor = false;
    }
}

void Demo_checkForValidTouchandProximity()
{
    if( (Demo_keypadSensor || Demo_sliderSensor || Demo_wheelSensor) == true)
    {
        LED3_ON;
    }
    else
    {
        LED3_OFF;
    }

    if( Demo_proximitySensor  == false)
    {
        LED2_OFF;
    }
    else
    {
        LED2_ON;
    }
}


