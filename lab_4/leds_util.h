#ifndef LEDS_UTIL_H
#define LEDS_UTIL_H

#include "dev/leds.h"

typedef enum {
    Red,
    Green,
    None,
    Both
} LedsState_t;

void acende_led(LedsState_t state);

#endif
