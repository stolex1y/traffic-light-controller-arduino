#include "hardware/Led.h"

#include <Arduino.h>

struct Led {
    LedState state;
    uint32_t startTime;
    uint32_t duration;
    uint32_t period;
    uint8_t pin;
};

static void ledHardwareSet(Led *led);
static void ledToggleState(Led *led);

struct Led* ledInit(const LedState initState, const uint8_t pin) {
    pinMode(pin, OUTPUT);
    Led * const led = (Led *) malloc(sizeof(Led));
    *led = (Led) { .state = initState, .startTime = 0, .duration = 0, .period = 0, .pin = pin };
    ledSetStateConstantly(led, initState);
    return led;
}

void ledUpdateState(Led * const led) {
    if (led->duration == 0 && led->period == 0) {
        return;
    }
    const uint32_t time = millis();
    const uint16_t delta = time - led->startTime;
    if (led->period > 0 && delta >= led->period) {
        led->duration -= led->period;
        if (led->duration <= 0) {
            ledSetStateConstantly(led, LED_TURN_OFF);
        } else {
            ledToggleState(led);
            led->startTime = millis();
        }
    } else if (time - led->startTime >= led->duration) {
        ledToggleState(led);
        led->duration = 0;
    }
}

uint32_t ledGetLightningTime(const Led * const led) {
    if (led->state != LED_TURN_ON)
        return 0;
    return millis() - led->startTime;
}

void ledSetStateConstantly(Led *led, LedState state) {
    led->state = state;
    led->duration = 0;
    led->period = 0;
    ledHardwareSet(led);
}

void ledSetPeriod(Led * const led, const uint16_t period, const uint16_t duration) {
    led->state = LED_TURN_ON;
    led->period = period;
    led->duration = duration;
    led->startTime = millis();
    ledHardwareSet(led);
}

void ledSetStateWithDuration(Led * const led, const LedState state, const uint16_t duration) {
    led->state = state;
    led->period = 0;
    led->duration = duration;
    led->startTime = millis();
    ledHardwareSet(led);
}

LedState ledGetState(const Led * const led) {
    return led->state;
}

uint8_t ledFinishedLightning(const Led * const led) {
    return led->duration == 0 && led->period == 0;
}

static void ledToggleState(Led * const led) {
    led->state = led->state == LED_TURN_ON ? LED_TURN_OFF : LED_TURN_ON;
    ledHardwareSet(led);
}

static void ledHardwareSet(Led * const led) {
    digitalWrite(led->pin, led->state == LED_TURN_ON ? HIGH : LOW);
}
