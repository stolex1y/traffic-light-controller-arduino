#ifndef EMBEDDED_ARDUINO_TRAFFICLIGHT_H
#define EMBEDDED_ARDUINO_TRAFFICLIGHT_H

#include "hardware/LedHardware.h"

enum Color { RED = 0, YELLOW = 1, GREEN = 2 };

void trafficLightUpdate();
void trafficLightSetShortRed(uint32_t duration);
void trafficLightSetDuration(Color color, uint32_t duration);
Color trafficLightGetColor();
uint8_t trafficLightIsFlashing();
void trafficLightStart();
void trafficLightStop();
void trafficLightInit(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin);

#endif //EMBEDDED_ARDUINO_TRAFFICLIGHT_H
