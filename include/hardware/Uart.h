#ifndef HARDWARE_UART_H
#define HARDWARE_UART_H

#include <stdint.h>

extern void uartReceiveIntCallback(uint8_t data);
extern void uartTransmitIntCallback();

void usartHardwareInit();
uint8_t usartHardwareTransmitPolling(uint8_t data, uint16_t timeOut);
void usartHardwareTransmitInterrupt(uint8_t buffer);
uint8_t usartHardwareReceivePolling(uint8_t * data, uint16_t timeOut);
void usartHardwareReceiveInterrupt();
void usartHardwareDisableInterrupts();

#endif