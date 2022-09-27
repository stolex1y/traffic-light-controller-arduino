#include <Arduino.h>
#include <../.pio/libdeps/nanoatmega328/GyverBME280/src/GyverBME280.h>

#include "TrafficLight.h"
#include "hardware/ButtonHardware.h"

#define RED_PIN 3
#define YELLOW_PIN 4
#define GREEN_PIN 5
#define BUTTON 2

#define RED_DURATION_START 6000

static void onButtonClick();

static uint16_t durations[3] = {
        [RED] = RED_DURATION_START,
        [YELLOW] = 1500,
        [GREEN] = 1500
};

void setup() {
    Serial.begin(9600);
    trafficLightInit(RED_PIN, YELLOW_PIN, GREEN_PIN);
    trafficLightSetDuration(RED, durations[RED]);
    trafficLightSetDuration(GREEN, durations[GREEN]);
    trafficLightSetDuration(YELLOW, durations[YELLOW]);
    buttonInit(BUTTON);
    buttonSetOnClick(&onButtonClick);
    trafficLightStart();
}

void loop() {
    trafficLightUpdate();

}

static void onButtonClick() {
    durations[RED] >>= 2;
    if (durations[RED] < RED_DURATION_START >> 4) {
        durations[RED] = RED_DURATION_START;
    }
    trafficLightSetDuration(RED, durations[RED]);
}