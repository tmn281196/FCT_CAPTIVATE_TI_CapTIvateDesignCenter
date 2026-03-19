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
#include <Demo/Demo.h>
#include <TCA9535/TCA9535.h>
#include "CAPT_UserConfig.h"

uint16_t ButtonStateRetention = 0x0000;

uint8_t Beep_Timers = 0;
uint16_t SpeakerTone[8] =
{
 3824,
 3407,
 3035,
 2865,
 2551,
 2273,
 2024,
 1912
};

/*
 *  ui16Duration define how long a beep last, the unit is milliseconds
 *  ui8Times define how many beeps user want
 *  ui8Tone define the speaker tone 0 ~ 7 stands for CDEFGAB
 */
void Speaker_Beep(uint16_t ui16Duration, uint8_t ui8Times, uint8_t ui8Tone)
{
    if(Beep_Timers == 0)
    {
        TA2CCR0 = SpeakerTone[ui8Tone];
        TA2CCR2 = SpeakerTone[ui8Tone] >> 1;
        Beep_Timers = ui8Times * 2 - 1;
        TA0CCR0 = (32768 >> 10) * ui16Duration;
        P3SEL0 |= BIT0;
        TA0CTL |= TASSEL__ACLK | MC__UP;              // SMCLK, up mode
    }
}

void ButtonGroupEventHandler(tSensor *sensor)
{
    static const uint16_t buttonToLEDMap[16] =
    {
     BIT0, BIT4, BIT8, BITC,
     BIT1, BIT5, BIT9, BITD,
     BIT2, BIT6, BITA, BITE,
     BIT3, BIT7, BITB, BITF
    };

    if ((sensor->bSensorTouch == true) && (sensor->bSensorPrevTouch == false))
    {
        //
        // Handle a new touch press event
        //
        DETECT_LED_ON;

        if(sensor == &BTN_SELF)
        {
            ButtonStateRetention ^= buttonToLEDMap[CAPT_getDominantButton(sensor)];
        }
        else
        {
            ButtonStateRetention ^= buttonToLEDMap[CAPT_getDominantButton(sensor) + 8];
        }

        TCA9535_SetOutput(TCA9535_ADDR, ButtonStateRetention);

        Speaker_Beep(25, 1, 7);
    }

    else if ((sensor->bSensorTouch == false) && (sensor->bSensorPrevTouch == true))
    {
        //
        // Handle a release event
        //
        DETECT_LED_OFF;

    }

    //
    // If the sensor is experiencing noise, flash the noise LED.
    //
    if (sensor->bSensorNoiseState == true)
    {
        NOISE_LED_ON;
    }
    else
    {
        NOISE_LED_OFF;
    }
}

void Demo_Init()
{
    // Configure Timer for speaker beep
    TA0CCTL0 |= CCIE;                               // TACCR0 interrupt enabled

    // Configure PWM output for speaker tone
    TA2CCTL2 = OUTMOD_7;                            // CCR2 reset/set
    TA2CTL = TASSEL__SMCLK | MC__UP | TACLR;

    Speaker_Beep(1000, 1, 2);    // 1s, 1 time, tone 2

    LED_ALL_ON;

    TCA9535_Init(TCA9535_ADDR);
    TCA9535_SetAsOutputs(TCA9535_ADDR, TCA9535_ALL);
    TCA9535_SetOutput(TCA9535_ADDR, TCA9535_ALL);

    MAP_CAPT_registerCallback(&BTN_MUTUAL, &ButtonGroupEventHandler);
    MAP_CAPT_registerCallback(&BTN_SELF, &ButtonGroupEventHandler);
}

void Demo_Ready()
{
    LED_ALL_OFF;

    TCA9535_SetOutput(TCA9535_ADDR, TCA9535_NONE);

    Speaker_Beep(50, 2, 0);    // 50ms, 2 times, tone 0
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{

    if(Beep_Timers > 0)
    {
        Beep_Timers--;
        P3SEL0 ^= BIT0;
    }
    else
    {
        TA0CTL = 0;         // Stop beep timer
    }
}
