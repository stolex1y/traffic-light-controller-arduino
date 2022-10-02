#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

#define SHORT_CLICK 50
#define LONG_CLICK 2000

typedef void (*ButtonActionListener)();

void buttonSetOnClick(ButtonActionListener actionListener);
void buttonSetOnLongClick(ButtonActionListener actionListener);
void buttonInit(uint8_t pin);
void buttonEnable();
void buttonDisable();
uint8_t buttonIsEnabled();
void buttonUpdateState();

#endif //BUTTON_H
