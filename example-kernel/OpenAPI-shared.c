#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "memory_attribute.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

int Printf(const char *f, ...);
#define PRINT Printf

/*** DEVICE HELPERS ***/

void doReset(uint8_t type)
{
    if (0 == type)
    {
        hal_wdt_config_t cfg;
        cfg.mode = 0;
        cfg.seconds = 1;
        hal_wdt_disable(0xCAFE0065);
        hal_wdt_init(&cfg);
        hal_wdt_software_reset();
        while (1)
        {
        }
    }
}

int getBattery(void)
{
    uint32_t data;
    if (hal_adc_init())
        return -1;
    if (hal_adc_get_data_polling(HAL_ADC_CHANNEL_6, &data))
        return -1;
    hal_adc_deinit();
    return (5600 * data + 2047) / 4095;
}

void getIMEI(char imei[16], uint8_t size)
{
    if (imei && size == 16)
    {
        memset(imei, '0', 15);
        imei[15] = 0;
        // TODO
    }
}

/*** ARDUINO COMPATABLE FUNCTIONS ***/

void yield(void)
{
    vTaskDelay(1);
}

unsigned int millis(void)
{
    return xTaskGetTickCount() * 10;
}

unsigned int micros(void)
{
    uint32_t count = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &count);
    return count;
}

void delay(unsigned int ms)
{
    vTaskDelay(ms / 10);
}

void delayMicroseconds(unsigned int us) // ?
{
    hal_gpt_delay_us(us);
}

void delay_ms(unsigned int ms) // helper
{
    hal_gpt_delay_ms(ms);
}

void delay_us(unsigned int us) // helper
{
    hal_gpt_delay_us(us);
}

// Wiring /////////////////////////////////////////////////////////////////////////////////////

// clang-format off
#define INPUT           (0)
#define INPUT_PULLUP    (1 << 0)
#define INPUT_PULLDOWN  (1 << 1)
#define OUTPUT          (1 << 2)
#define OUTPUT_LO       (1 << 3)
#define OUTPUT_HI       (1 << 4)
// clang-format on

#define ARRAYLEN(x) (sizeof(x) / sizeof((x)[0]))

typedef struct
{
    hal_gpio_pin_t gpio;
    hal_eint_number_t eint;
    void (*eint_cb)(void);
    hal_adc_channel_t adc_channel;
    int adc_pin_mode;
    int pwm;
    int pwm_pin_mode;
} PinDescription;

// clang-format off
static PinDescription pinsMap[] = {
    // Mediatek,    EINT pin    user callback, ADC channel   pin mode,
    {HAL_GPIO_9,    HAL_EINT_NUMBER_9,   NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 00,
    {HAL_GPIO_10,   HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 01, 
    {HAL_GPIO_11,   HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 02, 
    {HAL_GPIO_8,    HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 03, 
    {HAL_GPIO_1,    HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 04, LED
    {HAL_GPIO_6,    HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 05, i2c-c
    {HAL_GPIO_7,    HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 06, i2c-d
    {HAL_GPIO_14,   HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 07
    {HAL_GPIO_15,   HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 08
    {HAL_GPIO_28,   HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 09
    {0x0C,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 10, 
    {0x0D,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 11, 
    {0x1B,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 12
    {0x1A,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 13
    {0x18,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 14
    {0x19,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 15
    {0x12,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 16, 
    {0x16,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 17, 
    /* EX */
    {0x1D,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 18, 
    {HAL_GPIO_0,    HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 19, 
    {0x1F,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 20, 
    {0x21,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 21, 
    {0x22,          HAL_EINT_NUMBER_MAX, NULL, HAL_ADC_CHANNEL_MAX, 7, }, // 22, n
};
// clang-format on

static PinDescription *getPin(uint8_t arduino_pin)
{
    return (arduino_pin >= ARRAYLEN(pinsMap)) ? NULL : &pinsMap[arduino_pin];
}

void pinMode(uint8_t pin, uint8_t mode)
{
    PinDescription *n = getPin(pin);
    if (n)
    {
        hal_pinmux_set_function(n->gpio, 0);
        hal_gpio_direction_t dir = HAL_GPIO_DIRECTION_INPUT;
        // hal_gpio_clear_high_impedance(n->gpio); // PSM

        if (mode == INPUT)
        {
            hal_gpio_disable_pull(n->gpio);
        }
        if (mode & INPUT_PULLUP)
        {
            hal_gpio_pull_up(n->gpio);
        }
        if (mode & INPUT_PULLDOWN)
        {
            hal_gpio_pull_down(n->gpio);
        }
        if ((mode & OUTPUT) || (mode & OUTPUT_LO) || (mode & OUTPUT_HI))
        {
            dir = HAL_GPIO_DIRECTION_OUTPUT;
            hal_gpio_set_output(n->gpio, HAL_GPIO_DATA_HIGH);
        }
        if (mode & OUTPUT_HI)
        {
            hal_gpio_set_output(n->gpio, HAL_GPIO_DATA_HIGH);
        }
        hal_gpio_set_direction(n->gpio, dir);
    }
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    PinDescription *n = getPin(pin);
    if (n)
    {
        hal_gpio_set_output(n->gpio, (hal_gpio_data_t)val);
    }
}

int digitalRead(uint8_t pin)
{
    hal_gpio_data_t val = -1;
    PinDescription *n = getPin(pin);
    if (n)
    {
        if (0 == hal_gpio_get_input(n->gpio, &val))
            return val;
    }
    return val;
}

// EINT //////////////////////////////////////////////////////////////////////////////////////

static void api_eint_callback_t(void *user_data) // ISR !!!
{

    if (user_data)
    {
        hal_eint_mask(((PinDescription *)(user_data))->eint);
        if (((PinDescription *)(user_data))->eint_cb)
            ((PinDescription *)(user_data))->eint_cb();
        hal_eint_unmask(((PinDescription *)(user_data))->eint);
    }
}

void detachInterrupt(uint8_t pin)
{
    PinDescription *n = getPin(pin);
    if (n)
    {
        if (n->eint < HAL_EINT_NUMBER_MAX)
        {
            hal_eint_deinit(n->eint);
            n->eint_cb = NULL;
            hal_pinmux_set_function(n->gpio, 0);
            // hal_gpio_set_high_impedance(n->gpio); // PSM
        }
    }
}

void attachInterrupt(uint8_t pin, void (*cb)(void), int mode)
{
    PinDescription *n = getPin(pin);
    if (n)
    {
        if (n->eint < HAL_EINT_NUMBER_MAX)
        {
            hal_pinmux_set_function(n->gpio, 7);
            hal_eint_mask(n->eint);
            hal_eint_config_t cfg;
            cfg.debounce_time = 0;
            cfg.trigger_mode = (hal_eint_trigger_mode_t)mode;
            cfg.firq_enable = 0;
            if (HAL_EINT_STATUS_OK == hal_eint_init(n->eint, &cfg))
            {
                if (HAL_EINT_STATUS_OK == hal_eint_register_callback(n->eint, api_eint_callback_t, n))
                {
                    n->eint_cb = cb;
                }
                else
                {
                }
            }
            else
            {
            }
        }
    }
}

// ADC[5+2]
int analogRead(uint8_t pin)
{
    uint32_t res = -1, data = 0;
    PinDescription *n = getPin(pin);
    if (n)
    {
        if (n->adc_channel >= HAL_ADC_CHANNEL_MAX)
            return -2;
        if (n->adc_channel < HAL_ADC_CHANNEL_6)
        {
            PinDescription *n = getPin(n->gpio);
            // hal_gpio_clear_high_impedance(n->gpio); // PSM
            hal_pinmux_set_function(n->gpio, n->adc_pin_mode);
        }
        if (HAL_ADC_STATUS_OK == hal_adc_init())
        {
#define CNT 3
            for (int i = 0, res = 0; i < CNT; i++)
            {
                if (HAL_ADC_STATUS_OK != hal_adc_get_data_polling(n->adc_channel, &data))
                {
                    return -3;
                }
                res += data;
            }
            res /= CNT;
            hal_adc_deinit();
            // hal_gpio_set_high_impedance(n->gpio); // PSM
        }
    }
    return res;
}

// PWM[4]
void analogWrite(uint8_t pin, int val) // TODO
{
    PinDescription *n = getPin(pin);
    if (n)
    {
        hal_pinmux_set_function(n->gpio, 0);
        // TODO
    }
}

// Serial ////////////////////////////////////////////////////////////////////////////////////

#include "ring-buffer.h"
typedef struct
{
    const hal_uart_port_t port;
    hal_gpio_pin_t pin_rx, pin_tx;
    int mode_rx, mode_tx;

    hal_uart_config_t uart_config;
    hal_uart_dma_config_t dma_config;
    bool is_open;

    uint16_t rx_ring_size;
    ring_buffer_t rx_ring;
    char *rx_buffer;

    bool tx_notice;
} uart_ctx_t;

#define SERIAL_CNT 3
#define VFIFO_SIZE 256
#define RING_SIZE_MAX 1024

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t rx_vfifo_buffer[SERIAL_CNT][VFIFO_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t tx_vfifo_buffer[SERIAL_CNT][VFIFO_SIZE];

static uart_ctx_t SERIAL[SERIAL_CNT] = {
    {
        .port = HAL_UART_0,
        .pin_rx = HAL_GPIO_2,
        .mode_rx = 1,
        .pin_tx = HAL_GPIO_5,
        .mode_tx = 1,
        .is_open = 0,
    },
    {
        .port = HAL_UART_1,
        .pin_rx = HAL_GPIO_12, // U1:PINNAME_RXD_AUX= 0xC:3
        .mode_rx = 3,
        .pin_tx = HAL_GPIO_13, // U1:PINNAME_TXD_AUX = 0xD:3
        .mode_tx = 3,
        .is_open = 0,
    },
    {
        .port = HAL_UART_2,
        .pin_rx = HAL_GPIO_18, // TODO: U2:PINNAME_RXD_DBG = 0x12:3
        .mode_rx = 3,
        .pin_tx = HAL_GPIO_22, // TODO: U2:PINNAME_TXD_DBG = 0x16:5
        .mode_tx = 5,
        .is_open = 0,
    },
};

static TaskHandle_t u_handle = NULL;
static QueueHandle_t u_queue = NULL;
static SemaphoreHandle_t u_mutex = NULL;
static uint8_t u_buffer[VFIFO_SIZE];
static void uart_task(void *param)
{
    uart_ctx_t *u;
    uint32_t size;
    PRINT("[UART] Task Started\n");
    while (1)
    {
        if (xQueueReceive(u_queue, &u, -1) == pdTRUE)
        {
            if (xSemaphoreTake(u_mutex, -1) == pdTRUE)
            {
                if ((size = hal_uart_get_available_receive_bytes(u->port)))
                {
                    if ((size = hal_uart_receive_dma(u->port, u_buffer, size)))
                    {
                        size = u->rx_ring.write(&u->rx_ring, u_buffer, size);
                    }
                }
                xSemaphoreGive(u_mutex);
            }
        }
    }
}

static void uart_cb(hal_uart_callback_event_t event, void *user_data) // ISR !!!
{
    if (user_data)
    {
        uart_ctx_t *u = (uart_ctx_t *)user_data;
        if (event == HAL_UART_EVENT_READY_TO_READ)
        {
            xQueueSendFromISR(u_queue, &user_data, NULL);
        }
        if (event == HAL_UART_EVENT_READY_TO_WRITE)
        {
            u->tx_notice = true;
        }
    }
}

void uart_close(uint8_t n)
{
    if (n < SERIAL_CNT)
    {
        uart_ctx_t *u = &SERIAL[n];
        hal_uart_deinit(u->port);
        hal_pinmux_set_function(u->pin_rx, 0); // GPIO
        // hal_gpio_set_high_impedance(u->pin_rx); // PSM
        hal_pinmux_set_function(u->pin_tx, 0); // GPIO
        // hal_gpio_set_high_impedance(u->pin_tx); // PSM
        if (u->rx_buffer)
        {
            vPortFree(u->rx_buffer);
            u->rx_buffer = NULL;
        }
        u->is_open = false;
    }
}

int uart_open(uint8_t n, int brg, int length, int parity, int stop_bit, uint16_t ring_size)
{
    // Create task for RX ring
    if (NULL == u_handle)
    {
        u_queue = xQueueCreate(10, sizeof(void *)); // abort
        u_mutex = xSemaphoreCreateMutex();          // abort
        xTaskCreate(uart_task, "UART-TASK", 1024, NULL, TASK_PRIORITY_NORMAL, &u_handle);
        vTaskDelay(1);
    }

    int res = -10;
    uart_ctx_t *u;
    static const uint32_t baudrate_map[] = {110, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 3000000}; // [14]
    if (n < SERIAL_CNT)
    {
        u = &SERIAL[n];
        if (u->is_open)
            return 0; // is open
        uart_close(n);

        u->uart_config.baudrate = HAL_UART_BAUDRATE_MAX;
        for (int i = 0; 0 < HAL_UART_BAUDRATE_MAX; i++)
        {
            if (baudrate_map[i] == brg)
            {
                u->uart_config.baudrate = (hal_uart_baudrate_t)i;
                break;
            }
        }
        if (HAL_UART_BAUDRATE_MAX == u->uart_config.baudrate)
            return -11;

        // TODO CHECK & SET VALUES
        if (length < 5 || length > 8)
            return -12;
        u->uart_config.word_length = HAL_UART_WORD_LENGTH_8; // 5, 6, 7, 8
        u->uart_config.parity = HAL_UART_PARITY_NONE;        // ODD = 1, EVEN = 2
        u->uart_config.stop_bit = HAL_UART_STOP_BIT_1;       // 0, 1

        if (ring_size > RING_SIZE_MAX || NULL == (u->rx_buffer = pvPortCalloc(ring_size, 1)))
        {
            return -20;
        }

        ring_buffer_init(&u->rx_ring, ring_size, u->rx_buffer);
        u->rx_ring_size = ring_size;

        res = hal_uart_init(u->port, &u->uart_config);
        if (HAL_UART_STATUS_OK == res)
        {
            hal_uart_disable_flowcontrol(u->port);
            hal_uart_set_auto_baudrate(u->port, false);

            u->dma_config.receive_vfifo_alert_size = 20;
            u->dma_config.receive_vfifo_buffer = rx_vfifo_buffer[n];
            u->dma_config.receive_vfifo_buffer_size = VFIFO_SIZE;
            u->dma_config.receive_vfifo_threshold_size = (VFIFO_SIZE * 2) / 3;
            u->dma_config.send_vfifo_buffer = tx_vfifo_buffer[n];
            u->dma_config.send_vfifo_buffer_size = VFIFO_SIZE;
            u->dma_config.send_vfifo_threshold_size = VFIFO_SIZE / 10;

            res = hal_uart_set_dma(u->port, &u->dma_config);
            if (HAL_UART_STATUS_OK == res)
            {
                res = hal_uart_register_callback(u->port, uart_cb, u);
                u->is_open = HAL_UART_STATUS_OK == res;
                if (u->is_open)
                {
                    hal_pinmux_set_function(u->pin_rx, u->mode_rx);
                    hal_pinmux_set_function(u->pin_tx, u->mode_tx);
                    res = 0;
                }
                else
                {
                    PRINT("[UART] CB ERROR: %d\n", res);
                    res = -40;
                }
            }
            else
            {
                PRINT("[UART] DMA ERROR: %d\n", res);
                res = -30;
            }
        }
    }
    if (res)
    {
        uart_close(n);
        PRINT("[UART] ERROR: %d\n", res);
    }
    else
    {
        PRINT("[UART] Open\n");
    }
    return res;
}

uint32_t uart_write(uint8_t n, const uint8_t *buffer, uint32_t size)
{
    int res = 0;
    if (n < SERIAL_CNT)
    {
        uart_ctx_t *u = &SERIAL[n];
        if (u->is_open && buffer && size)
        {
            uint32_t left = size;
            uint8_t *p = (uint8_t *)buffer;
            while (1)
            {
                u->tx_notice = false;
                uint32_t cnt = hal_uart_send_dma(u->port, p, left);
                left -= cnt;
                p += cnt;
                if (left == 0)
                {
                    res = size;
                    break;
                }
                while (false == u->tx_notice)
                {
                    // vTaskDelay(1);
                }
            }
        }
    }
    return res;
}

int uart_read(uint8_t n, char *buffer, size_t size)
{
    int res = -1;
    if (n < SERIAL_CNT)
    {
        uart_ctx_t *u = &SERIAL[n];
        if (u->is_open && buffer && size)
        {
            if (xSemaphoreTake(u_mutex, -1) == pdTRUE)
            {
                res = u->rx_ring.read(&u->rx_ring, buffer, size);
                xSemaphoreGive(u_mutex);
            }
        }
    }
    return res;
}

int uart_peek(uint8_t n)
{
    int res = -1;
    if (n < SERIAL_CNT)
    {
        uart_ctx_t *u = &SERIAL[n];
        if (u->is_open)
        {
            if (xSemaphoreTake(u_mutex, -1) == pdTRUE)
            {
                if (1 == u->rx_ring.peek(&u->rx_ring, &res, 1))
                {
                    res &= 0xFF;
                }
                else
                {
                    res = -1;
                }
                xSemaphoreGive(u_mutex);
            }
        }
    }
    return res;
}

uint32_t uart_available(uint8_t n)
{
    int res = 0;
    if (n < SERIAL_CNT)
    {
        uart_ctx_t *u = &SERIAL[n];
        if (u->is_open)
        {
            if (xSemaphoreTake(u_mutex, -1) == pdTRUE)
            {
                res = u->rx_ring_size - u->rx_ring.available(&u->rx_ring);
                xSemaphoreGive(u_mutex);
            }
        }
    }
    return res;
}

void uart_flush(uint8_t n)
{
    if (n < SERIAL_CNT)
    {
        if (xSemaphoreTake(u_mutex, -1) == pdTRUE)
        {
            SERIAL[n].rx_ring.reset(&SERIAL[n].rx_ring);
        }
        xSemaphoreGive(u_mutex);
    }
}

// Wire ////////////////////////////////////////////////////////////////////
// SPI /////////////////////////////////////////////////////////////////////

// OS //////////////////////////////////////////////////////////////////////

// MODEM //
// RIL VIRTUAL UART ////////////////////////////////////////////////////////
// DATA CALL ///////////////////////////////////////////////////////////////
// LWIP ////////////////////////////////////////////////////////////////////
// SSL /////////////////////////////////////////////////////////////////////

// OPTIONALS //
// HTTP ////////////////////////////////////////////////////////////////////
// MQTT ////////////////////////////////////////////////////////////////////
// LWM2M ///////////////////////////////////////////////////////////////////