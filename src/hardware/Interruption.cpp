#include "hardware/Interruption.h"

#include <Arduino.h>
#include <../.pio/libdeps/nanoatmega328/PinChangeInterrupt/src/PinChangeInterrupt.h>

void pcintSetHandler(const uint8_t pin, const PcintHandler handler) {
    if (handler == NULL)
        return;
    pcintClearHandler(pin);
    attachPCINT(digitalPinToPCINT(pin), handler, CHANGE);
}

void pcintClearHandler(const uint8_t pin) {
    detachPCINT(digitalPinToPCINT(pin));
}

void pcintDisableHandler(const uint8_t pin) {
    disablePCINT(digitalPinToPCINT(pin));
}

void pcintEnableHandler(const uint8_t pin) {
    enablePCINT(digitalPinToPCINT(pin));
}

enum PIN_CHANGE getPcintTrigger(const uint8_t pin) {
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(pin));
    if (trigger == FALLING)
        return PIN_FALLING;
    else
        return PIN_RISING;
}