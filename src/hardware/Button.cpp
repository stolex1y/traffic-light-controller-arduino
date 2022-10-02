#include "hardware/Button.h"

#include <Arduino.h>

static volatile uint8_t enabled = 0;
static uint8_t pin;
static ButtonActionListener onClickHandler = NULL;
static ButtonActionListener onLongClickHandler = NULL;

static uint8_t buttonIsPressed();

void buttonInit(const uint8_t p) {
    pin = p;
    pinMode(pin, INPUT_PULLUP);
    onClickHandler = NULL;
    onLongClickHandler = NULL;
}

void buttonSetOnClick(const ButtonActionListener actionListener) {
    onClickHandler = actionListener;
}

void buttonSetOnLongClick(const ButtonActionListener actionListener) {
    onLongClickHandler = actionListener;
}

void buttonDisable() {
    enabled = 0;
}

void buttonEnable() {
    enabled = 1;
}

uint8_t buttonIsEnabled() {
    return enabled;
}

static volatile uint32_t clickStart = 0;

void buttonUpdateState() {
    if (!enabled)
        return;
    uint32_t time = millis();
    uint8_t isPressed = buttonIsPressed();

    if (!isPressed && clickStart > 0 && (time - clickStart) >= LONG_CLICK) {
        clickStart = 0;
        if (onLongClickHandler != NULL) {
            (*onLongClickHandler)();
        }
    } else if (!isPressed && clickStart > 0 && (time - clickStart) >= SHORT_CLICK) {
        clickStart = 0;
        if (onClickHandler != NULL)
            (*onClickHandler)();
    } else if (isPressed && clickStart == 0) {
        clickStart = time;
    }
}

static uint8_t buttonIsPressed() {
    return !digitalRead(pin);
}



