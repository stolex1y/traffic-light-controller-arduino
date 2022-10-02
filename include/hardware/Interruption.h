#ifndef INTERRUPTION_H
#define INTERRUPTION_H

#include <stdint.h>

typedef void (*PcintHandler)();

typedef enum PIN_CHANGE { PIN_RISING, PIN_FALLING } PIN_CHANGE;

void pcintSetHandler(uint8_t pin, PcintHandler handler);
void pcintClearHandler(uint8_t pin);
void pcintEnableHandler(uint8_t pin);
void pcintDisableHandler(uint8_t pin);
enum PIN_CHANGE getPcintTrigger(uint8_t pin);

#endif //INTERRUPTION_H
