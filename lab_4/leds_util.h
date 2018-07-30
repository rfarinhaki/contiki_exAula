#include "dev/leds.h"

typedef enum {
    None,
    Red,
    Green,
    Both
} LedsState_t;

static void acende_leds(LedsState_t state);
