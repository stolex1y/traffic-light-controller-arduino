#include "hardware/ButtonHardware.h"

#include <Arduino.h>

#include "hardware/InterraptionHardware.h"

static uint8_t pin;
static buttonActionListener onClickHandler = NULL;
static buttonActionListener onLongClickHandler = NULL;

static void buttonOnChange();

void buttonInit(uint8_t p) {
    pin = p;
    pinMode(pin, INPUT_PULLUP);
    pcintSetHandler(pin, &buttonOnChange);
    onClickHandler = NULL;
    onLongClickHandler = NULL;
}

void buttonSetOnClick(buttonActionListener actionListener) {
    onClickHandler = actionListener;
}

void buttonSetOnLongClick(buttonActionListener actionListener) {
    onLongClickHandler = actionListener;
}

bool buttonIsPressed() {
    return !digitalRead(pin);
}

static volatile uint32_t clickStart = 0;

static void buttonOnChange() {
    uint32_t time = millis();
    bool isPressed = buttonIsPressed();

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



