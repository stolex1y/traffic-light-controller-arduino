#ifndef LED_H
#define LED_H

#include <stdint.h>

typedef struct Led Led;
typedef enum LedState { LED_TURN_ON, LED_TURN_OFF } LedState;

void ledSetStateConstantly(Led *led, LedState state);
void ledSetStateWithDuration(Led *led, LedState state, uint16_t duration);
LedState ledGetState(const Led *led);
void ledSetPeriod(Led *led, uint16_t period, uint16_t duration);
Led* ledInit(LedState initState, uint8_t pin);
void ledUpdateState(Led *led);
uint8_t ledFinishedLightning(const Led *led);
uint32_t ledGetLightningTime(const Led *led);


#endif //LED_H
