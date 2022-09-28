#include "hardware/InterraptionHardware.h"

#include <Arduino.h>
#include <../.pio/libdeps/nanoatmega328/PinChangeInterrupt/src/PinChangeInterrupt.h>

void pcintSetHandler(uint8_t pin, pcintHandler handler) {
    if (handler == NULL)
        return;
    pcintClearHandler(pin);
    attachPCINT(digitalPinToPCINT(pin), handler, CHANGE);
}

void pcintClearHandler(uint8_t pin) {
    detachPCINT(digitalPinToPCINT(pin));
}

void pcintDisableHandler(uint8_t pin) {
    disablePCINT(digitalPinToPCINT(pin));
}

void pcintEnableHandler(uint8_t pin) {
    enablePCINT(digitalPinToPCINT(pin));
}

enum PIN_CHANGE getPcintTrigger(uint8_t pin) {
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(pin));
    if (trigger == FALLING)
        return PIN_FALLING;
    else
        return PIN_RISING;
}