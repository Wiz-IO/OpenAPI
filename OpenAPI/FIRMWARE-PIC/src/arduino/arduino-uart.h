#ifndef __ARDUINO_UART_H__
#define __ARDUINO_UART_H__

#include "arduino.h"

#define UART_MAX 3
#define UART_VFIFO_SIZE 1024

typedef struct uart_ctx_s // private
{
    driver_t *drv;
    const hal_uart_port_t port; // sort
    const hal_gpio_pin_t g_rx, g_tx;
    const int m_rx, m_tx;
    int event;
} uart_ctx_t;

void UART_Receive(uart_ctx_t *);

// COMMON /////////////////////////////////////////////////////////////////////
int UART_Open(struct driver_s *, va_list);
void UART_Close(struct driver_s *);
size_t UART_Write(struct driver_s *, const uint8_t *, size_t);
int UART_Syscall(struct driver_s *, io_commands, va_list);

#endif // __ARDUINO_UART_H__