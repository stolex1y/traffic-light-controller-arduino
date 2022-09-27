#include "hardware/LedHardware.h"

#include <Arduino.h>

struct Led {
    LedState state;
    uint32_t startTime;
    int32_t duration;
    int32_t period;
    uint8_t pin;
};

static void ledHardwareSet(Led *led);
static void ledToggleState(Led *led);

struct Led* ledInit(LedState initState, uint8_t pin) {
    pinMode(pin, OUTPUT);
    Led *led = (Led *) malloc(sizeof(Led));
    *led = (Led) { .state = initState, .startTime = 0, .duration = 0, .period = 0, .pin = pin };
    ledSetState(led, initState);
    return led;
}

void ledUpdateState(Led *led) {
    if (led->duration == 0 && led->period == 0) {
        return;
    }
    uint32_t time = millis();
    uint16_t delta = time - led->startTime;
    if (led->period > 0 && delta >= led->period) {
        led->duration -= led->period;
        if (led->duration <= 0) {
            ledSetState(led, LED_TURN_OFF);
        } else {
            ledToggleState(led);
            led->startTime = millis();
        }
    } else if (time - led->startTime >= led->duration) {
        ledToggleState(led);
        led->duration = 0;
    }
}

void ledSetState(Led *led, LedState state) {
    led->state = state;
    led->duration = 0;
    led->period = 0;
    ledHardwareSet(led);
}

void ledSetPeriod(Led *led, uint16_t period, uint16_t duration) {
    led->state = LED_TURN_ON;
    led->period = period;
    led->duration = duration;
    led->startTime = millis();
    ledHardwareSet(led);
}

void ledSetState(Led *led, LedState state, uint16_t duration) {
    led->state = state;
    led->period = 0;
    led->duration = duration;
    led->startTime = millis();
    ledHardwareSet(led);
}

LedState ledGetState(Led *led) {
    return led->state;
}

bool ledFinishedLightning(Led *led) {
    return led->duration == 0 && led->period == 0;
}

static void ledToggleState(Led *led) {
    led->state = led->state == LED_TURN_ON ? LED_TURN_OFF : LED_TURN_ON;
    ledHardwareSet(led);
}

static void ledHardwareSet(Led *led) {
    digitalWrite(led->pin, led->state == LED_TURN_ON ? HIGH : LOW);
}
