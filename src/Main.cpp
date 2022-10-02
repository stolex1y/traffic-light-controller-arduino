#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <Arduino.h>

#include "hardware/Button.h"
#include "TrafficLight.h"
#include "Uart.h"
#include "Command.h"
#include "Menu.h"

#define RED_PIN 3
#define YELLOW_PIN 4
#define GREEN_PIN 5
#define BUTTON 2

#define UART_BUF_SIZE 128

static char * getTrafficLightInfo();
static char * changeButtonMode(const uint32_t *intArg);
static char * changeInterruptionMode(const OnOffStatus *onOffStatus);
static char * changeRedTimeout(const uint32_t *intArg);
static void onButtonClick();
static void sendMessage(const char * msg);
static void receiveAndSendChar();
static void sendCommandResult(const char * commandResult);

static Command *commands[] = {
        commandCreate("?", (CommandAction) getTrafficLightInfo, NONE),
        commandCreate("set mode", (CommandAction) changeButtonMode, INT),
        commandCreate("set interrupts", (CommandAction) changeInterruptionMode, ON_OFF),
        commandCreate("set timeout", (CommandAction) changeRedTimeout, INT)
};
static uint8_t uartBuf[UART_BUF_SIZE];
static uint8_t uartBufLast = 0;
static uint8_t buttonEnabled = 1;
static uint8_t hasLine = 0;

static uint32_t durations[3] = {
        [RED] = 6000,
        [YELLOW] = 1500,
        [GREEN] = 1500
};

void setup() {
    uartInit();

    trafficLightInit(RED_PIN, YELLOW_PIN, GREEN_PIN);
    trafficLightSetDuration(RED, durations[RED]);
    trafficLightSetDuration(GREEN, durations[GREEN]);
    trafficLightSetDuration(YELLOW, durations[YELLOW]);
    trafficLightStart();

    buttonInit(BUTTON);
    buttonSetOnClick(&onButtonClick);
    buttonEnable();

    menuInit(commands, sizeof(commands));
    sendMessage("Started\n");
}

void loop() {
    if (trafficLightGetColor() == GREEN && !trafficLightIsBlinking())
        buttonDisable();
    else {
        if (buttonEnabled)
            buttonEnable();
        else
            buttonDisable();
    }

    trafficLightUpdate();
    buttonUpdateState();
    uartPooling();

    if (uartHasNext()) {
        receiveAndSendChar();
    }

    if (hasLine) {
        uartBuf[uartBufLast] = '\0';
        const char *commandResult = menuExecuteCommand((char *) uartBuf);
        sendMessage("\n");
        sendCommandResult(commandResult);
    }
}

static void onButtonClick() {
    trafficLightSetShortRed(durations[RED] >> 2);
}

static char cmdBuf[128];
static char * getTrafficLightInfo() {
    char *bufLast = cmdBuf;
    uint8_t isBlinking = trafficLightIsBlinking();
    char *currentColor = colorGetName(trafficLightGetColor());
    bufLast += sprintf(bufLast, "Light: %s%s\n",
            isBlinking ? "blinking " : "",
            currentColor
    );
    bufLast += sprintf(bufLast, "Button mode: mode %" PRIu8 " (%s)\n",
                       buttonIsEnabled() + 1,
                       buttonIsEnabled() ? "enabled" : "disabled"
    );
    bufLast += sprintf(bufLast, "Red timeout: %" PRIu32 " s\n",
                       durations[RED] / (uint32_t) 1000
    );
    bufLast += sprintf(bufLast, "Interrupts mode: %s\n\r",
                       uartIsInterruptionEnabled()  ? "I" : "P"
    );
    return cmdBuf;
}

static char * changeButtonMode(const uint32_t *intArg) {
    if (*intArg == 2) {
        buttonEnabled = 1;
        return "Enable button\n";
    } else if (*intArg == 1) {
        buttonEnabled = 0;
        return "Disable button\n";
    } else {
        return "The int argument must be 1 or 2\n";
    }
}

static char * changeInterruptionMode(const OnOffStatus *onOffStatus) {
    if (*onOffStatus == ON) {
        uartEnableInterruption();
        return "Enable int\n";
    } else if (*onOffStatus == OFF) {
        uartDisableInterruption();
        return "Disable int\n";
    } else {
        return "The argument must be 'on' or 'off'\n";
    }
}

static char * changeRedTimeout(const uint32_t *intArg) {
    durations[RED] = *intArg * 1000;
    trafficLightSetDuration(RED, durations[RED]);
    return "OK\n";
}

static void sendMessage(const char * msg) {
    uartTransmit((uint8_t *) msg, strlen(msg));
}

static void receiveAndSendChar() {
    if (uartReceive(uartBuf + uartBufLast, 1)) {
        uint8_t received = uartBuf[uartBufLast];
        uartBufLast++;
        uartTransmit(&received, 1);

        if (received == '\r')
            hasLine = 1;
    }
}

static void sendCommandResult(const char * commandResult) {
    uartTransmit((uint8_t *) commandResult, strlen(commandResult));
    uartBufLast = 0;
    hasLine = 0;
}