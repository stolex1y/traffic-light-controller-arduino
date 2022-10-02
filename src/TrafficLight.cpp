#include "TrafficLight.h"

#include "hardware/Button.h"

struct TrafficLightState {
    uint32_t duration;
    Color color;
    uint8_t blinking;
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
static uint8_t isShortRed = 0;
static uint32_t shortRedDuration = 1000;
static uint8_t enabled = 0;

static void updateNextState();
static void updateCurrentState();
static void turnOffAllLights();

Color trafficLightGetColor() {
    return currentState.color;
}

uint8_t trafficLightIsBlinking() {
    return currentState.blinking;
}

void trafficLightSetShortRed(uint32_t duration) {
    if (nextState.color == RED)
        nextState.duration = duration;
    else if (currentState.color == RED)
        currentState.duration = duration;
    else {
        isShortRed = 1;
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
        ledSetStateConstantly(lights[currentState.color], LED_TURN_OFF);
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
    enabled = 1;
    nextState.color = RED;
    nextState.blinking = 0;
    nextState.duration = durations[RED];
    trafficLightUpdate();
}

void trafficLightStop() {
    enabled = 0;
    turnOffAllLights();
}

char * colorGetName(Color color) {
    switch (color) {
        case RED:
            return "red";
        case GREEN:
            return "green";
        case YELLOW:
            return "yellow";
        default:
            return "unknown color";
    }
}

static void turnOffAllLights() {
    ledSetStateConstantly(lights[RED], LED_TURN_OFF);
    ledSetStateConstantly(lights[GREEN], LED_TURN_OFF);
    ledSetStateConstantly(lights[YELLOW], LED_TURN_OFF);
}

static void updateCurrentState() {
    currentState.color = nextState.color;
    currentState.blinking = nextState.blinking;
    if (currentState.color == RED && isShortRed) {
        currentState.duration = shortRedDuration;
        isShortRed = 0;
    } else {
        currentState.duration = nextState.duration;
    }
    // light updated current state periodically or constantly
    if (currentState.blinking) {
        ledSetPeriod(lights[currentState.color], currentState.duration >> 2, currentState.duration);
    } else {
        ledSetStateWithDuration(lights[currentState.color], LED_TURN_ON, currentState.duration);
    }
}

static void updateNextState() {
    switch (currentState.color) {
        case RED:
            nextState.color = GREEN;
            nextState.blinking = 0;
            break;
        case GREEN:
            if (currentState.blinking) {
                nextState.color = YELLOW;
                nextState.blinking = 0;
            } else {
                nextState.color = GREEN;
                nextState.blinking = 1;
            }
            break;
        case YELLOW:
            nextState.color = RED;
            nextState.blinking = 0;
            break;
    }
    nextState.duration = durations[nextState.color];
}
