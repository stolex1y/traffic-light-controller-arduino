#include <Arduino.h>

#include "TrafficLight.h"
#include "hardware/ButtonHardware.h"

#define RED_PIN 3
#define YELLOW_PIN 4
#define GREEN_PIN 5
#define BUTTON 2

static void onButtonClick();

static uint16_t durations[3] = {
        [RED] = 6000,
        [YELLOW] = 1500,
        [GREEN] = 1500
};

void setup() {
    trafficLightInit(RED_PIN, YELLOW_PIN, GREEN_PIN);
    trafficLightSetDuration(RED, durations[RED]);
    trafficLightSetDuration(GREEN, durations[GREEN]);
    trafficLightSetDuration(YELLOW, durations[YELLOW]);
    buttonInit(BUTTON);
    buttonSetOnClick(&onButtonClick);
    buttonEnable();
    trafficLightStart();
}

void loop() {
    if (trafficLightGetColor() == GREEN && !trafficLightIsFlashing())
        buttonDisable();
    else
        buttonEnable();

    trafficLightUpdate();
    buttonUpdateState();
}

static void onButtonClick() {
    trafficLightSetShortRed(durations[RED] >> 2);
}