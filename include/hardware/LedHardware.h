#ifndef EMBEDDED_ARDUINO_LEDHARDWARE_H
#define EMBEDDED_ARDUINO_LEDHARDWARE_H

#include <stdint.h>

typedef struct Led Led;
typedef enum LedState { LED_TURN_ON, LED_TURN_OFF } LedState;

void ledSetState(Led *led, LedState state);
void ledSetState(Led *led, LedState state, uint16_t duration);
LedState ledGetState(Led *led);
void ledSetPeriod(Led *led, uint16_t period, uint16_t duration);
Led* ledInit(LedState initState, uint8_t pin);
void ledUpdateState(Led *led);
bool ledFinishedLightning(Led *led);


#endif //EMBEDDED_ARDUINO_LEDHARDWARE_H
