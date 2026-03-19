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
#ifndef DEMO_H_
#define DEMO_H_

#include <stdbool.h>
#include "TCA9535.h"

#define LED_POUT                                          (P1OUT)
#define LED_PDIR                                          (P1DIR)

#define LED1_PIN                                          (BIT1)
#define RUN_LED_ON                                        (LED_POUT |= LED1_PIN)
#define RUN_LED_OFF                                       (LED_POUT &= ~LED1_PIN)
#define RUN_LED_TOGGLE                                    (LED_POUT ^= LED1_PIN)

#define LED2_PIN                                          (BIT0)
#define DETECT_LED_ON                                     (LED_POUT |= LED2_PIN)
#define DETECT_LED_OFF                                    (LED_POUT &= ~LED2_PIN)
#define DETECT_LED_TOGGLE                                 (LED_POUT ^= LED2_PIN)

#define LED3_PIN                                          (BIT6)
#define FAULT_LED_ON                                      (LED_POUT |= LED3_PIN)
#define FAULT_LED_OFF                                     (LED_POUT &= ~LED3_PIN)
#define FAULT_LED_TOGGLE                                  (LED_POUT ^= LED3_PIN)

#define LED4_PIN                                          (BIT7)
#define NOISE_LED_ON                                      (LED_POUT |= LED4_PIN)
#define NOISE_LED_OFF                                     (LED_POUT &= ~LED4_PIN)
#define NOISE_LED_TOGGLE                                  (LED_POUT ^= LED4_PIN)

#define LED_ALL_PIN                                       (LED1_PIN + LED2_PIN + LED3_PIN + LED4_PIN)
#define LED_ALL_ON                                        (LED_POUT |= LED_ALL_PIN)
#define LED_ALL_OFF                                       (LED_POUT &= ~LED_ALL_PIN)

void Demo_Init();
void Demo_Ready();
void Speaker_Beep(uint16_t ui16Duration, uint8_t ui8Times, uint8_t ui8Tone);

#endif /* DEMO_H_ */
