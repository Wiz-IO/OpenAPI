#ifndef __ARDUINO_I2C_H__
#define __ARDUINO_I2C_H__

#include "arduino.h"

#define I2C_MAX 3
#define I2C_VFIFO_SIZE 256 /* transaction max size */

typedef struct i2c_ctx_s // private
{
    driver_t *drv;
    const hal_i2c_port_t port; // sort
    const hal_gpio_pin_t g_clock, g_data;
    int m_clock, m_data;
    int event;
} i2c_ctx_t;

// COMMON /////////////////////////////////////////////////////////////////////
void I2C_Close(struct driver_s *);
int I2C_Open(struct driver_s *, va_list);
int I2C_Syscall(struct driver_s *, io_commands, va_list);

// FAST ///////////////////////////////////////////////////////////////////////
int I2C_Transmit(void *, uint32_t address_7);
int I2C_Receive(void *, uint32_t address_7, size_t size);

#endif // __ARDUINO_I2C_H__