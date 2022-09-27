#ifndef EMBEDDED_ARDUINO_TRAFFICLIGHT_H
#define EMBEDDED_ARDUINO_TRAFFICLIGHT_H

#include "hardware/LedHardware.h"

enum Color { RED = 0, YELLOW = 1, GREEN = 2 };

void trafficLightUpdate();
void trafficLightSetDuration(Color color, uint32_t duration);
void trafficLightStart();
void trafficLightInit(uint8_t red_pin, uint8_t yellow_pin, uint8_t green_pin);

#endif //EMBEDDED_ARDUINO_TRAFFICLIGHT_H
