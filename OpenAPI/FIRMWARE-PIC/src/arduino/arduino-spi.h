#ifndef __ARDUINO_SPI_H__
#define __ARDUINO_SPI_H__

#include "arduino.h"

#define SPI_MAX 2
#define SPI_VFIFO_SIZE 1024

typedef struct spi_ctx_s // private
{
    const hal_spi_master_port_t port; // sort
    const hal_gpio_pin_t g_miso, g_mosi, g_sck, g_cs;
    int u_miso, u_mosi, u_sck, u_cs;
} spi_ctx_t;

// COMMON /////////////////////////////////////////////////////////////////////
int SPI_Open(struct driver_s *, va_list);
void SPI_Close(struct driver_s *);
size_t SPI_Write(struct driver_s *, const uint8_t *, size_t);
int SPI_Syscall(struct driver_s *, io_commands, va_list);

// FAST ///////////////////////////////////////////////////////////////////////
int SPI_Transfer(void *, uint8_t *, uint8_t *, size_t);
int SPI_Fill(void *, uint32_t, uint32_t, size_t);

#endif // __ARDUINO_SPI_H__