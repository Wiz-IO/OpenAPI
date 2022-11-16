#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "hal.h"
#include "hal_nvic_internal.h"
#include "hal_rtc_internal.h"
#include "hal_rtc_external.h"
#include "hal_sleep_manager_internal.h"
#include "memory_attribute.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

#include "ring-buffer.h"
#include "minmax.h"

#include "../openapi/OpenAPI-core.h"

#define MUTEX_TAKE(S) xSemaphoreTake(S, portMAX_DELAY)
#define MUTEX_GIVE(S) xSemaphoreGive(S);

typedef enum
{
    MSG_REMOTE_CALL = 1,

    MSG_UART_RECEIVE = 10,
    MSG_UART_TRANSMIT,
    MSG_EINT_CALLBACK,
    MSG_RTC_CALLBACK,
    MSG_GPT_CALLBACK,

    MSG_USB = 90,
    MSG_USB_RECEIVE = MSG_USB, // 90
    MSG_USB_TRANSMIT,          // x1
    MSG_USB_OPEN,              // x2
    MSG_USB_CLOSE,             // x3
    MSG_USB_CONNECTION,        // x4
    MSG_USB_DISCONNECTION,     // x5

} api_messages_e;

typedef struct
{
    uint32_t msg;
    void *ctx;
    void *prm;
} api_message_t;
void api_send_message(uint32_t id, void *context, void *param);

typedef struct pins_s
{
    const hal_gpio_pin_t gpio;
    const hal_eint_number_t eint;
    void (*eint_cb)(void);

    const hal_adc_channel_t adc_channel;
    int adc_samples;

    const hal_pwm_channel_t pwm_channel;
    const int pwm_pin_mode;
    uint32_t pwm_total_count;
} pins_t;

extern pins_t PINS[];

typedef enum
{
    /*** VALUES ***/
    IO_VOID = 0,
    IO_KEY = 42,

    /*** COMMANDS ***/
    IO_INIT = 1,
    IO_DEBUG,
    IO_DEBUG_SET,

    __IO_FUN = 100,
    IO_GET_API_VERSION = __IO_FUN,
    IO_GET_RAND,
    IO_GET_UID,
    IO_GET_BATTERY,

    IO_GET_POWER_REASON,

    IO_SLEEP_ENABLE,
    IO_SLEEP_DISABLE,
    IO_SLEEP_BEFORE,

    IO_FLASH_ERASE,
    IO_FLASH_WRITE,
    IO_FLASH_READ,

    IO_KICK_DOG = 900,
    IO_RESET = 901,
    IO_POWER_OFF = 902,

    /*** DRIVERS COMMON ***/
    __IO_DRIVER = 1000,
    IO_DRIVER_RX_AVAILABLE = __IO_DRIVER,
    IO_DRIVER_TX_AVAILABLE,
    IO_DRIVER_RX_PEEK,
    IO_DRIVER_TX_PEEK,
    IO_DRIVER_RX_FLUSH,
    IO_DRIVER_TX_FLUSH,
    IO_DRIVER_FLUSH,

    /*** HARDWARE ***/
    __IO_UART = 1100,

    __IO_SPI = 1200,
    IO_SPI_SET_FREQUENCY = __IO_SPI,
    IO_SPI_SET_MODE,
    IO_SPI_SET_ORDER,
    
    __IO_I2C = 1300,
    IO_I2C_SET_FREQUENCY = __IO_I2C,

    /*** EOF ***/
    IO_ERROR = -10,
    __IO_END = 0xFFFFFFFF
} io_commands;

typedef struct driver_s
{
    // PUBLIC /////////////////////////////////////////////////////////////////
    const char *name;
    union
    {
        struct
        {
            // public
        } uart;
        struct
        {
            // public
            int (*transfer)(void *, uint8_t *tx, uint8_t *rx, size_t size);
            int (*fill)(void *, uint32_t data_len, uint32_t data, size_t size);
        } spi;
        struct
        {
            // public
            int (*transmit)(void *, uint32_t address_7);
            int (*receive)(void *, uint32_t address_7, size_t size);
        } i2c;
    };
    // PRIVATE ////////////////////////////////////////////////////////////////
    uint32_t index; // index to the PERIPHERY_NAME[N] table
    int (*open)(struct driver_s *, va_list);
    void (*close)(struct driver_s *);
    size_t (*write)(struct driver_s *, const uint8_t *buffer, size_t size);
    size_t (*read)(struct driver_s *, char *buffer, size_t size);
    int (*syscall)(struct driver_s *, io_commands io, va_list);
    SemaphoreHandle_t mutex; // is_init
    ring_buffer_t rx_ring, tx_ring;
    char *rx_buffer, *tx_buffer;
    int is_open; // bool, align
} driver_t;

#include "arduino-uart.h"
#include "arduino-spi.h"
#include "arduino-i2c.h"

// INTERFACE //////////////////////////////////////////////////////////////////

// clang-format off
#define INPUT               (0)
#define INPUT_PULLUP        (1 << 0)
#define INPUT_PULLDOWN      (1 << 1)
#define OUTPUT              (1 << 2)
#define OUTPUT_LOW          (1 << 3)
#define OUTPUT_HIGH         (1 << 4)

#define FALLING             (2)
#define RISING              (3)
#define CHANGE              (4)
// clang-format on

void yield(void);
unsigned int millis(void);
unsigned int micros(void);
void delay(unsigned int);
void delayMicroseconds(unsigned int);
void pwmMode(uint32_t pin, uint32_t frequency, uint8_t duty, bool clock_13MHz, uint8_t divider);
void adcMode(uint32_t pin, uint32_t mode);
void pinMode(uint32_t pin, uint32_t mode);
int digitalRead(uint32_t pin);
void digitalWrite(uint32_t pin, uint32_t val);
void digitalToggle(uint32_t pin, uint32_t pause);
void detachInterrupt(uint32_t pin);
void attachInterrupt(uint32_t pin, void (*cb)(void), int mode);
int analogRead(uint32_t pin);
void analogWrite(uint32_t pin, int val);

// alias in heap_4.c
void *api_malloc(size_t);
void *api_realloc(void *, size_t);
void *api_calloc(size_t, size_t);
void api_free(void *p);

int api_syscall(io_commands, uint32_t variable_0, va_list);

typedef enum
{
    RW_HARDWARE = 0,
    RW_RING_RX = 1,
    RW_RING_TX = 2,
} direction_e;

void *drv_create(const char *name, uint16_t rx_ring_size, uint16_t tx_ring_size);
void drv_free(driver_t *);
int drv_close(driver_t *);
int drv_open(driver_t *, ...);
int drv_syscall(driver_t *, io_commands, ...);
size_t drv_read(driver_t *, direction_e, void *buf, size_t count);
size_t drv_write(driver_t *, direction_e, const void *buf, size_t count);

///////////////////////////////////////////////////////////////////////////////

#endif // _ARDUINO_H_