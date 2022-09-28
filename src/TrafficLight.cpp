#include "TrafficLight.h"

#include "hardware/ButtonHardware.h"

struct TrafficLightState {
    uint32_t duration;
    Color color;
    uint8_t flashing;
};
typedef struct TrafficLightState TrafficLightState;

static Led *lights[3];

static uint32_t durations[4] = {
        [RED] = 4000,
        [YELLOW] = 500,
        [GREEN] = 1000
};

static TrafficLightState currentState = (TrafficLightState) { };
static TrafficLightState nextState = (TrafficLightState) { };
static uint8_t isShortRed = false;
static uint32_t shortRedDuration = 1000;
static uint8_t enabled = false;

static void updateNextState();
static void updateCurrentState();
static void turnOffAllLights();

Color trafficLightGetColor() {
    return currentState.color;
}

uint8_t trafficLightIsFlashing() {
    return currentState.flashing;
}

void trafficLightSetShortRed(uint32_t duration) {
    if (nextState.color == RED)
        nextState.duration = duration;
    else if (currentState.color == RED)
        currentState.duration = duration;
    else {
        isShortRed = true;
        shortRedDuration = duration;
    }
    trafficLightUpdate();
}

void trafficLightSetDuration(Color color, uint32_t duration) {
    if (nextState.color == color)
        nextState.duration = duration;
    else if (currentState.color == color)
        currentState.duration = duration;
    durations[color] = duration;
    trafficLightUpdate();
}

void trafficLightUpdate() {
    if (!enabled)
        return;

    buttonUpdateState();

    if (ledFinishedLightning(lights[currentState.color]) ||
            ledGetLightningTime(lights[currentState.color]) >= currentState.duration) {
        ledSetState(lights[currentState.color], LED_TURN_OFF);
        updateCurrentState();
        updateNextState();
    }

    ledUpdateState(lights[RED]);
    ledUpdateState(lights[YELLOW]);
    ledUpdateState(lights[GREEN]);
}

void trafficLightInit(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin) {
    lights[RED] = ledInit(LED_TURN_OFF, redPin);
    lights[YELLOW] = ledInit(LED_TURN_OFF, yellowPin);
    lights[GREEN] = ledInit(LED_TURN_OFF, greenPin);
}

void trafficLightStart() {
    enabled = true;
    nextState.color = RED;
    nextState.flashing = false;
    nextState.duration = durations[RED];
    trafficLightUpdate();
}

void trafficLightStop() {
    enabled = false;
    turnOffAllLights();
}

static void turnOffAllLights() {
    ledSetState(lights[RED], LED_TURN_OFF);
    ledSetState(lights[GREEN], LED_TURN_OFF);
    ledSetState(lights[YELLOW], LED_TURN_OFF);
}

static void updateCurrentState() {
    currentState.color = nextState.color;
    currentState.flashing = nextState.flashing;
    if (currentState.color == RED && isShortRed) {
        currentState.duration = shortRedDuration;
        isShortRed = false;
    } else {
        currentState.duration = nextState.duration;
    }
    // light updated current state periodically or constantly
    if (currentState.flashing) {
        ledSetPeriod(lights[currentState.color], currentState.duration >> 2, currentState.duration);
    } else {
        ledSetState(lights[currentState.color], LED_TURN_ON, currentState.duration);
    }
}

static void updateNextState() {
    switch (currentState.color) {
        case RED:
            nextState.color = GREEN;
            nextState.flashing = false;
            break;
        case GREEN:
            if (currentState.flashing) {
                nextState.color = YELLOW;
                nextState.flashing = false;
            } else {
                nextState.color = GREEN;
                nextState.flashing = true;
            }
            break;
        case YELLOW:
            nextState.color = RED;
            nextState.flashing = false;
            break;
    }
    nextState.duration = durations[nextState.color];
}
