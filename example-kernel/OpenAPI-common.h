#ifndef __API_H__
#define __API_H__

#include <stdint.h>

void doReset(uint8_t type);
int getBattery(void);
void getIMEI(char imei[16], uint8_t size);

unsigned int millis(void);
unsigned int micros(void);
void delay(unsigned int ms);
void delayMicroseconds(unsigned int us);

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);

int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);

void uart_close(uint8_t n);
int uart_open(uint8_t n, int brg, int length, int parity, int stop_bit, uint16_t ring_size);
uint32_t uart_write(uint8_t n, const uint8_t buffer, uint32_t size);
int uart_read(uint8_t n, char *buffer, size_t size);
int uart_peek(uint8_t n);
int uart_available(uint8_t n);
void uart_flush(uint8_t n);

#endif //__API_H__