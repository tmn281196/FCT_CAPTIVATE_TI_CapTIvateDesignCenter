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
//*****************************************************************************
// CAPT_BSP.c
//
// This board support package supports the MSP430FR2676 device.
// 
// *** CLOCK CONFIGURATION ***
// MCLK is 8 MHz, sourced from the DCO
// SMCLK is 2 MHz, sourced from MCLK
// ACLK is 32kHz, sourced from REFO
// FLLREF is 32kHz, sourced from REFO
// NOTE: ACLK and FLLREF can be sourced from XT1
//
//
// \version 1.83.00.05
// Released on May 15, 2020
//
//*****************************************************************************

#include <msp430.h>
#include <stdint.h>

#include "CAPT_BSP.h"

//*****************************************************************************
//
//! This function is configures the MCU Digital IO and CS for operation.
//
//*****************************************************************************
void BSP_configureMCU(void)
{
//    uint8_t ui8LFOsc = DEFAULT_OSC_SELECTION;

    // P1.0: LED
    // P1.1: LED
    // P1.2: UCB0 I2C SDA
    // P1.3: UCB0 I2C SCL
    // P1.4: UCA0 UART TXD
    // P1.5: UCA0 UART RXD
    // P1.6: LED
    // P1.7: LED
    P1OUT  = (0);
    P1DIR  = (GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN6 | GPIO_PIN7);
    P1SEL0 = (GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
    P1SEL1 = (0);

    // P3.0: Speaker
    P3OUT  =  (0);
    P3DIR  =  GPIO_PIN0;
    P3SEL0 =  (0);
    P3SEL1 =  (0);

    //
    // Clear port lock
    //
    PM5CTL0 &= ~LOCKLPM5;

    //
    // Initialize Clock Signals
    //
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    __bis_SR_register(SCG0);                           // disable FLL
    CSCTL3 |= SELREF__REFOCLK;                         // Set REFO as FLL reference source
    CSCTL0 = 0;                                        // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                            // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;                               // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;                             // DCOCLKDIV = 16MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                           // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));         // FLL locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;         // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
    CSCTL5 |= DIVM_0 | DIVS_3;                         // MCLK = DCOCLK = 16MHZ,
                                                       // SMCLK = MCLK/4 = 2MHz
}

//*****************************************************************************
//
//! This function disables the watchdog timer during boot, ensuring a WDT
//! reset does not occur during boot before main() is entered.
//
//*****************************************************************************
int _system_pre_init(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    return 1;
}
