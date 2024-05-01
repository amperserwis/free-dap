// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2021, Alex Taradov <alex@taradov.com>. All rights reserved.

#ifndef _WS2812_PIO_H_
#define _WS2812_PIO_H_

#ifdef DAP_STATUS_USE_WS2812
//- Definitions ---------------------------------------------------------------

// color pattern ---------> 0xGGRRBB00
#define WS2812_COLOR_SET    0x02060000
#define WS2812_COLOR_CLR    0x00020200

void pio_init(uint32_t f_cpu);

//- Variables -----------------------------------------------------------------
static bool DAP_STATUS_is_set = false;

//- Implementations -----------------------------------------------------------

//-----------------------------------------------------------------------------
static inline void HAL_GPIO_DAP_STATUS_out(void)
{
  HAL_GPIO_DAP_STATUS2_out();
  (void)HAL_GPIO_DAP_STATUS_out;
}

//-----------------------------------------------------------------------------
static inline void HAL_GPIO_DAP_STATUS_set(void)
{
  HAL_GPIO_DAP_STATUS2_set();
  PIO0->TXF0 = WS2812_COLOR_SET;
  DAP_STATUS_is_set = true;
  (void)HAL_GPIO_DAP_STATUS_set;
}

//-----------------------------------------------------------------------------
static inline void HAL_GPIO_DAP_STATUS_clr(void)
{
  HAL_GPIO_DAP_STATUS2_clr();
  PIO0->TXF0 = WS2812_COLOR_CLR;
  DAP_STATUS_is_set = false;
  (void)HAL_GPIO_DAP_STATUS_clr;
}

//-----------------------------------------------------------------------------
static inline void HAL_GPIO_DAP_STATUS_toggle(void)
{
  HAL_GPIO_DAP_STATUS2_toggle();
  if (DAP_STATUS_is_set) {
    HAL_GPIO_DAP_STATUS_clr();
  } else {
    HAL_GPIO_DAP_STATUS_set();
  }
  (void)HAL_GPIO_DAP_STATUS_toggle;	
}
#endif // DAP_STATUS_USE_WS2812

#endif // _WS2812_PIO_H_
