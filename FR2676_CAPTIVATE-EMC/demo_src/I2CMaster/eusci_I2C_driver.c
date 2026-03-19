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
/*
 *	eusci.c
 *
 *  Texas Instruments Inc.
 *	Oct 2013
 *	Version 1.0.0
 *  Built with CCS Version 5.4.0.00048
 *  Built with IAR Version 5.51.6
 *
 */
/***************************************************
* NOTES:

****************************************************/

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "eusci_I2C_driver.h"

// NEED TO TRACK USCI STATUS AND PROVIDE DATA POINTER FOR USE IN ISR
volatile eUSCI_status ui8Status;
uint8_t* pData;
uint8_t ui8DummyRead;

/* ***************************************************************************
 * UCB0_MasterI2C_init
 * INITIALIZE I2C IN MASTER MODE
 * SEE EUSCI_DEF.H FOR SETUP DEFINITIONS
 *
*/
void UCB0_MasterI2C_init (void)
{
  	// RESET USCI MODULE
  	UCB0CTLW0 = UCSWRST;
  	
  	// I2C SINGLE MASTER, SYNC MODE, 7 BIT ADDRS, CLOCK SELECT (DEFINED IN EUSCI_DEF.H)
  	UCB0CTLW0 = UCMST + UCMODE_3 + UCSYNC + UCSSEL__SMCLK;
  	
  	// SET BAUDRATE
  	UCB0BRW = USCI_B0_BAUD;
  	
  	// ENABLE USCI MODULE
  	UCB0CTLW0 &= ~UCSWRST;
}

/*
 * MASTER I2C WRITE
 * RETURNS: FALSE IF NO ERRORS
 * RETURNS: TRUE IF SLAVE NACK'D
 */
bool UCB0_I2C_write(uint8_t ui8Addr, uint8_t ui8Reg, uint8_t *Data, uint8_t ui8ByteCount)
{
	uint8_t ui8index;

	UCB0CTLW0 |= UCSWRST;

  	UCB0CTLW1 |= UCASTP_1;

  	UCB0TBCNT = ui8ByteCount + 1;

	UCB0I2CSA = ui8Addr;

  	UCB0CTLW0 &= ~UCSWRST;

  	UCB0CTLW0 |= (UCTR + UCTXSTT);

  	// WAIT FOR THE FIRST TXBUF EMPTY INTERRUPT
  	while(!(UCB0IFG & UCTXIFG));

  	// LOAD TXBUF WITH THE TARGET REGISTER
  	UCB0TXBUF = ui8Reg;

  	for(ui8index = 0; ui8index < ui8ByteCount; ui8index++)
  	{
  		while(!(UCB0IFG & UCTXIFG0));
  			UCB0TXBUF = Data[ui8index];
  	}
	while(!(UCB0IFG & UCTXIFG));
	while (!(UCB0IFG & UCBCNTIFG));
	UCB0CTL1 |= UCTXSTP;
	while (!(UCB0IFG & UCSTPIFG));     // Ensure stop condition got sent
	UCB0CTL1  |= UCSWRST;

	return(true);
}

/*
 * MASTER I2C READ
 * RETURNS: FALSE IF NO ERRORS
 * RETURNS: TRUE IF SLAVE NACK'D
 */
bool UCB0_I2C_read(uint8_t ui8Addr, uint8_t ui8Reg, uint8_t *Data, uint8_t ui8ByteCount)
{
	uint8_t ui8index;

	UCB0CTLW0 |= UCSWRST;

  	// SETUP THE NUMBER OF BYTES TO RECEIVE (CAN ONLY BE DONE HERE WHEN EUSCI IN RESET)
  	UCB0TBCNT = 0x0002;

	// GENERATE AUTOMATIC STOP BIT WHEN UCB0TBCNT = TARGET
  	UCB0CTLW1 |= UCASTP_1;

	// LOAD THE DEVICE SLAVE ADDRESS
	UCB0I2CSA = ui8Addr;

  	// RESUME
  	UCB0CTLW0 &= ~UCSWRST;

  	UCB0CTLW0 |= (UCTR + UCTXSTT);

  	// WAIT FOR THE FIRST TXBUF EMPTY INTERRUPT BEFORE LOADING TARGET REGISTER
  	while(!(UCB0IFG & UCTXIFG));

  	// LOAD TXBUF WITH THE TARGET REGISTER
  	UCB0TXBUF = ui8Reg;

  	while(!(UCB0IFG & UCTXIFG));

	//TURN OFF TRANSMIT (ENABLE RECEIVE)
   	UCB0CTL1 &= ~UCTR;

   	// GENERATE (RE-)START BIT
  	UCB0CTL1 |= UCTXSTT;

  	while(!(UCB0IFG & UCRXIFG0));

	for(ui8index = 0; ui8index < ui8ByteCount - 1; ui8index++)
	{
		while(!(UCB0IFG & UCRXIFG0));
		Data[ui8index] = UCB0RXBUF;
		if(ui8index == ui8ByteCount - 1)
			UCB0CTL1 |= UCTXSTP; 	//send stop after next RX
	}

	UCB0CTLW0 |= UCTXSTP; 			//send stop after next RX
	while(!(UCB0IFG & UCRXIFG));
	Data[ui8index] = UCB0RXBUF;
	while (!(UCB0IFG & UCSTPIFG)); 	// Ensure stop condition got sent
	UCB0CTL1  |= UCSWRST;
	return(true);
}
