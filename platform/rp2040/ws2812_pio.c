// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2021, Alex Taradov <alex@taradov.com>. All rights reserved.

//- Includes ------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "rp2040.h"
#include "ws2812.pio.h"

//- Definitions ---------------------------------------------------------------
#define F_WS2812            800000      // 800 kHz for ws2812
#define WS2812_LOOP_CYCLES  (ws2812_T1 + ws2812_T2 + ws2812_T3)

//- Variables -----------------------------------------------------------------

//- Implementations -----------------------------------------------------------

//-----------------------------------------------------------------------------
void pio_init(uint32_t f_cpu)
{
  //Life with David - BMA Chapter 06 - Demo 1 - PIO blink with PLL clock

  // Enable pio0
  RESETS_CLR->RESET = RESETS_RESET_pio0_Msk;
  while (0 == RESETS->RESET_DONE_b.pio0);

  // Upload pio program to instruction memory
  volatile uint32_t *instr_mem = &PIO0->INSTR_MEM0; // = start of the pio0 program memory
  for (uint32_t i = 0; i < ws2812_program.length; i++) {
      uint16_t instr = ws2812_program_instructions[i];
      *instr_mem++ = instr;
  }

  // Enable output for GPIO16 and connect to pio0
  IO_BANK0->GPIO16_CTRL = 0;
  IO_BANK0->GPIO16_CTRL_b.OEOVER = IO_BANK0_GPIO16_CTRL_OEOVER_ENABLE;
  IO_BANK0->GPIO16_CTRL_b.FUNCSEL = IO_BANK0_GPIO16_CTRL_FUNCSEL_pio0_16;
  
  // Setting gpio16 as sideset base and 1 pin
  PIO0->SM0_PINCTRL = 0;
  PIO0->SM0_PINCTRL_b.SIDESET_BASE = 16;
  PIO0->SM0_PINCTRL_b.SIDESET_COUNT = 1;
  
  // Set up clock divider
  PIO0->SM0_CLKDIV = 0;
  PIO0->SM0_CLKDIV_b.INT = f_cpu / (F_WS2812 * WS2812_LOOP_CYCLES);

  // Set up wrapping
  PIO0->SM0_EXECCTRL = 0;
  PIO0->SM0_EXECCTRL_b.WRAP_BOTTOM = 0;
  PIO0->SM0_EXECCTRL_b.WRAP_TOP = 3;

  // Set up autopull
  PIO0->SM0_SHIFTCTRL = 0;
  PIO0->SM0_SHIFTCTRL_b.OUT_SHIFTDIR = 0;   // shift out of output shift register to left
  PIO0->SM0_SHIFTCTRL_b.AUTOPULL = 1;       // pull automatically
  PIO0->SM0_SHIFTCTRL_b.PULL_THRESH = 24;   // number of bits shifted out of OSR before autopull
  
  // Cleanly start state machine
  PIO0->CTRL = 0;
  PIO0->CTRL_b.SM_RESTART = 1;          // restart SM0
  PIO0->CTRL_b.CLKDIV_RESTART = 1;      // restart the clock

  PIO0->SM0_INSTR_b.SM0_INSTR = 0;      // forced jump to start of PIO program

  PIO0->CTRL_b.SM_ENABLE = 1;           // enable SM0
}

