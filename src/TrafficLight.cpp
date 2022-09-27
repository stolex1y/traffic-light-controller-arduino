#include "TrafficLight.h"

static uint16_t durations[4] = {
        [RED] = 4000,
        [YELLOW] = 500,
        [GREEN] = 1000
};

static Led *leds[3];

static Color currentColor = RED;
static bool isFlashing = false;
static bool updated = false;

static void turnOnOne(Color color);

void trafficLightSetDuration(Color color, uint32_t duration) {
    durations[color] = duration;
    updated = true;
}

void trafficLightUpdate() {
    if (ledFinishedLightning(leds[currentColor])) {
        if (updated) {
            currentColor = RED;
            updated = false;
        }
        switch (currentColor) {
            case RED:
                currentColor = GREEN;
                ledSetState(leds[currentColor], LED_TURN_ON, durations[currentColor]);
                break;
            case GREEN:
                if (!isFlashing) {
                    isFlashing = true;
                    ledSetPeriod(leds[currentColor], durations[currentColor] >> 2, durations[currentColor]);
                } else {
                    isFlashing = false;
                    currentColor = YELLOW;
                    ledSetState(leds[currentColor], LED_TURN_ON, durations[currentColor]);
                }
                break;
            case YELLOW:
                currentColor = RED;
                ledSetState(leds[currentColor], LED_TURN_ON, durations[currentColor]);
                break;
        }
    }
    ledUpdateState(leds[RED]);
    ledUpdateState(leds[YELLOW]);
    ledUpdateState(leds[GREEN]);
}

void trafficLightInit(uint8_t red_pin, uint8_t yellow_pin, uint8_t green_pin) {
    leds[RED] = ledInit(LED_TURN_OFF, red_pin);
    leds[YELLOW] = ledInit(LED_TURN_OFF, yellow_pin);
    leds[GREEN] = ledInit(LED_TURN_OFF, green_pin);
}

void trafficLightStart() {
    currentColor = YELLOW;
    trafficLightUpdate();
}

static void turnOnOne(Color color) {
    ledSetState(leds[color], LED_TURN_ON, durations[color]);
    ledSetState(leds[(color + 1) % 3], LED_TURN_OFF);
    ledSetState(leds[(color + 2) % 3], LED_TURN_OFF);
}