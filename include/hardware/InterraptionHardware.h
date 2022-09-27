#ifndef EMBEDDED_ARDUINO_INTERRAPTIONHARDWARE_H
#define EMBEDDED_ARDUINO_INTERRAPTIONHARDWARE_H

#include <stdint.h>

typedef void (*pcintHandler)(void);

enum PIN_CHANGE { PIN_RISING, PIN_FALLING };

void pcintSetHandler(uint8_t pin, pcintHandler handler);
void pcintClearHandler(uint8_t pin);
void pcintEnableHandler(uint8_t pin);
void pcintDisableHandler(uint8_t pin);
enum PIN_CHANGE getPcintTrigger(uint8_t pin);

#endif //EMBEDDED_ARDUINO_INTERRAPTIONHARDWARE_H
