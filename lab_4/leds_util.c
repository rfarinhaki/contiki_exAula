#include "leds_util.h"

void acende_led(LedsState_t state){
    switch(state){
        case None:
            leds_off(LEDS_RED);
            leds_off(LEDS_GREEN);
            break;
        case Red:
            leds_on(LEDS_RED);
            leds_off(LEDS_GREEN);
            break;
        case Green:
            leds_off(LEDS_RED);
            leds_on(LEDS_GREEN);
            break;
        case Both:
            leds_on(LEDS_RED);
            leds_on(LEDS_GREEN);
            break;
        default:
            leds_off(LEDS_RED);
            leds_off(LEDS_GREEN);
            break;
    }
}
