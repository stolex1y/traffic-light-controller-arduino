#ifndef EMBEDDED_ARDUINO_BUTTONHARDWARE_H
#define EMBEDDED_ARDUINO_BUTTONHARDWARE_H

#include <stdint.h>

#define SHORT_CLICK 50
#define LONG_CLICK 2000

typedef void (*buttonActionListener)();

void buttonSetOnClick(buttonActionListener actionListener);
void buttonSetOnLongClick(buttonActionListener actionListener);
void buttonInit(uint8_t pin);
void buttonEnable();
void buttonDisable();
uint8_t buttonIsEnabled();
void buttonUpdateState();

#endif //EMBEDDED_ARDUINO_BUTTONHARDWARE_H
