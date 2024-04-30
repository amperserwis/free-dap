/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_PIO_H
#define _HARDWARE_PIO_H

typedef uint32_t uint;

typedef struct pio_program {
//typedef struct {
    const uint16_t *instructions;
    uint8_t length;
    int8_t origin; // required instruction memory origin or -1
} pio_program;

typedef struct {
    uint32_t clkdiv;
    uint32_t execctrl;
    uint32_t shiftctrl;
    uint32_t pinctrl;
} pio_sm_config;

static inline pio_sm_config pio_get_default_sm_config(void) {
    pio_sm_config c = {0, 0, 0, 0};
    return c;
}

static inline void sm_config_set_wrap(pio_sm_config *c, uint wrap_target, uint wrap) {
    (void) c;           // silence unused parameter warning
    (void) wrap_target; // silence unused parameter warning
    (void) wrap;        // silence unused parameter warning
}

static inline void sm_config_set_sideset(pio_sm_config *c, uint bit_count, bool optional, bool pindirs) {
    (void) c;           // silence unused parameter warning
    (void) bit_count;   // silence unused parameter warning
    (void) optional;    // silence unused parameter warning
    (void) pindirs;     // silence unused parameter warning
}

#endif // _PIO_H_
