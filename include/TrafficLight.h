#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "hardware/Led.h"

typedef enum Color { RED = 0, YELLOW = 1, GREEN = 2 } Color;

void trafficLightUpdate();
void trafficLightSetShortRed(uint32_t duration);
void trafficLightSetDuration(Color color, uint32_t duration);
Color trafficLightGetColor();
uint8_t trafficLightIsBlinking();
void trafficLightStart();
void trafficLightStop();
void trafficLightInit(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin);
char * colorGetName(Color color);

#endif //TRAFFICLIGHT_H
