#include "arduino.h"

#define PRINTF(...) kprintf(__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////

uint8_t api_sleep_handler = 0;
static TaskHandle_t api_handle = NULL;
static QueueHandle_t api_queue = NULL;

void api_send_message(uint32_t id, void *context, void *param)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (api_queue == NULL)
    {
        return;
    }
    api_message_t M;
    M.msg = id;
    M.ctx = context;
    M.prm = param;
    if (0 == hal_nvic_query_exception_number())
    {
        xQueueSend(api_queue, &M, 0);
    }
    else
    {
        xQueueSendFromISR(api_queue, &M, &xHigherPriorityTaskWoken);
    }
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR(pdTRUE);
    }
}

/// @brief Translate ISR to user space
/// @param handle of main() task
static void api_mesages_handler(void *handle)
{
    PRINTF("[ARDUINO] %s()\n", __func__);
    if (handle)
    {
        vTaskResume(handle); // play main()
    }

    api_message_t M;
    uint32_t size;
    while (1)
    {
        if (xQueueReceive(api_queue, &M, portMAX_DELAY) == pdTRUE)
        {
            PRINTF("[ARDUINO] MESSAGE %d\n", M.msg);

            switch (M.msg)
            {
#if 1
            case MSG_UART_RECEIVE:
                UART_Receive(M.ctx);
                break;
#endif

#if 1
            case MSG_EINT_CALLBACK:
                if (M.ctx)
                {

                    hal_eint_mask(((pins_t *)(M.ctx))->eint);
                    if (((pins_t *)(M.ctx))->eint_cb)
                        ((pins_t *)(M.ctx))->eint_cb();
                    hal_eint_unmask(((pins_t *)(M.ctx))->eint);
                }
                break;
#endif
            }
        }
    }
}

/// @brief Create task messages handler, syscall
static bool api_init(void)
{
    extern int api_variable;
    api_variable = rtc_power_on_result(); // just for test

    PRINTF("[ARDUINO] %s()\n", __func__);
    static bool once = true;
    if (once)
    {
        once = false;
        void *current_task = xTaskGetCurrentTaskHandle();
        api_sleep_handler = hal_sleep_manager_set_sleep_handle("Arduino");
        if (NULL == api_handle)
        {
            if (NULL == (api_queue = xQueueCreate(10, sizeof(api_message_t))))
            {
                return false;
            }
            if (pdFAIL == xTaskCreate(api_mesages_handler, "AMH", 1024, current_task, TASK_PRIORITY_NORMAL, &api_handle))
            {
                PRINTF("[ERROR][ARDUINO] AMT\n");
                api_handle = NULL;
                return -2;
            }
            vTaskSuspend(current_task); // pause main(), wait AMH to start
            return true;
        }
    }
    return once;
}

/// @brief TRNG Random, syscall
static uint32_t api_rand(void)
{
    hal_trng_init();
    uint32_t random_number = rand();
    if (HAL_TRNG_STATUS_OK != hal_trng_get_generated_random_number(random_number))
    {
        random_number = rand();
    }
    hal_trng_deinit();
    return random_number;
}

///////////////////////////////////////////////////////////////////////////////

/// @brief Arduino compatible
void yield(void)
{
    vTaskDelay(1);
}

/// @brief Arduino compatible
unsigned int millis(void)
{
    return xTaskGetTickCount() * 10;
}

/// @brief Arduino compatible
void delay(unsigned int ms)
{
    vTaskDelay(ms / 10);
}

/// @brief Arduino compatible
unsigned int micros(void) // RAM/TCM ?
{
    uint32_t count = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &count);
    return count;
}

/// @brief Arduino compatible
void delayMicroseconds(unsigned int us) // RAM/TCM ?
{
    hal_gpt_delay_us(us);
}

// clang-format off
pins_t PINS[] = {
    {.gpio=HAL_GPIO_9,  .eint=HAL_EINT_NUMBER_9,    }, // 00, spi-miso
    {.gpio=HAL_GPIO_10, .eint=HAL_EINT_NUMBER_MAX,  }, // 01, spi-mosi
    {.gpio=HAL_GPIO_11, .eint=HAL_EINT_NUMBER_MAX,  }, // 02, spi-clk
    {.gpio=HAL_GPIO_8,  .eint=HAL_EINT_NUMBER_MAX,  }, // 03, spi-cs
    {.gpio=HAL_GPIO_1,  .eint=HAL_EINT_NUMBER_MAX,  .adc_channel=HAL_ADC_CHANNEL_MAX, .pwm_channel=HAL_PWM_0, .pwm_pin_mode=5 }, // 04, LED, pwm
    {.gpio=HAL_GPIO_6,  .eint=HAL_EINT_NUMBER_MAX,  }, // 05, i2c-c
    {.gpio=HAL_GPIO_7,  .eint=HAL_EINT_NUMBER_MAX,  }, // 06, i2c-d
    {.gpio=HAL_GPIO_14, .eint=HAL_EINT_NUMBER_MAX,  }, // 07
    {.gpio=HAL_GPIO_15, .eint=HAL_EINT_NUMBER_MAX,  }, // 08
    {.gpio=HAL_GPIO_28, .eint=HAL_EINT_NUMBER_MAX,  }, // 09
    {.gpio=HAL_GPIO_12, .eint=HAL_EINT_NUMBER_MAX,  }, // 10,
    {.gpio=HAL_GPIO_13, .eint=HAL_EINT_NUMBER_MAX,  }, // 11,
    {.gpio=HAL_GPIO_27, .eint=HAL_EINT_NUMBER_MAX,  }, // 12
    {.gpio=HAL_GPIO_26, .eint=HAL_EINT_NUMBER_MAX,  }, // 13
    {.gpio=HAL_GPIO_24, .eint=HAL_EINT_NUMBER_MAX,  }, // 14
    {.gpio=HAL_GPIO_25, .eint=HAL_EINT_NUMBER_MAX,  }, // 15
    {.gpio=HAL_GPIO_18, .eint=HAL_EINT_NUMBER_MAX,  }, // 16,
    {.gpio=HAL_GPIO_22, .eint=HAL_EINT_NUMBER_MAX,  }, // 17,
    /* EX */
    {.gpio=HAL_GPIO_29, .eint=HAL_EINT_NUMBER_MAX,  }, // 18,
    {.gpio=HAL_GPIO_0,  .eint=HAL_EINT_NUMBER_MAX,  }, // 19,
    {.gpio=HAL_GPIO_31, .eint=HAL_EINT_NUMBER_MAX,  }, // 20,
    {.gpio=HAL_GPIO_33, .eint=HAL_EINT_NUMBER_MAX,  }, // 21,
    {.gpio=HAL_GPIO_34, .eint=HAL_EINT_NUMBER_MAX,  }, // 22,
};
// clang-format on
#define PIN_MAX sizeof(PINS) / sizeof(PINS[0])

static inline pins_t *getPIN(uint32_t pin)
{
    return (pin < PIN_MAX) ? &PINS[pin] : NULL;
}

static inline hal_gpio_pin_t getGPIO(uint32_t pin)
{
    return (pin < PIN_MAX) ? PINS[pin].gpio : HAL_GPIO_MAX; // ???
}

void pwmMode(uint32_t pin, uint32_t frequency, uint8_t duty, bool clock_13M_32k, uint8_t divider)
{
    if (pin < PIN_MAX)
    {
        divider = (divider > 4) ? 4 : divider;
        duty = (duty > 100) ? 100 : duty;
        hal_pwm_init(PINS[pin].pwm_channel, (hal_pwm_source_clock_t)clock_13M_32k);
        hal_pwm_set_advanced_config(PINS[pin].pwm_channel, (hal_pwm_advanced_config_t)divider);
        hal_pwm_set_frequency(PINS[pin].pwm_channel, frequency, &PINS[pin].pwm_total_count);
        hal_pwm_set_duty_cycle(PINS[pin].pwm_channel, (PINS[pin].pwm_total_count * duty) / 100);
        hal_pwm_start(PINS[pin].pwm_channel);
        hal_pinmux_set_function(PINS[pin].gpio, PINS[pin].pwm_pin_mode);
    }
}

void adcMode(uint32_t pin, uint32_t samples)
{
    if (pin < PIN_MAX)
    {
        samples = (samples == 0) ? 1 : samples;
        samples = (samples > 16) ? 16 : samples;
        PINS[pin].adc_samples = samples;
        hal_pinmux_set_function(PINS[pin].gpio, 5); // always 5
    }
}

/// @brief Arduino compatible
void pinMode(uint32_t pin, uint32_t mode)
{
    int res = IO_ERROR;
    if (pin < PIN_MAX)
    {
        hal_gpio_pin_t gpio = PINS[pin].gpio;
        hal_gpio_direction_t dir = HAL_GPIO_DIRECTION_INPUT;
        if (mode == INPUT)
        {
            hal_gpio_disable_pull(gpio);
        }
        else if (mode & INPUT_PULLUP)
        {
            hal_gpio_pull_up(gpio);
        }
        else if (mode & INPUT_PULLDOWN)
        {
            hal_gpio_pull_down(gpio);
        }
        else if ((mode & OUTPUT) || (mode & OUTPUT_LOW) || (mode & OUTPUT_HIGH))
        {
            dir = HAL_GPIO_DIRECTION_OUTPUT;
            hal_gpio_set_output(gpio, HAL_GPIO_DATA_LOW);
        }
        else if (mode & OUTPUT_HIGH)
        {
            hal_gpio_set_output(gpio, HAL_GPIO_DATA_HIGH);
        }
        if (HAL_GPIO_STATUS_OK == (res = hal_gpio_set_direction(gpio, dir)))
        {
            res = hal_pinmux_set_function(gpio, 0);
        }
    }
    if (res)
    {
        PRINTF("[ERROR][PIN] %s() pin=%u, mode=%u, res=%d\n", __func__, pin, mode, res);
    }
}

/// @brief Arduino compatible
void digitalWrite(uint32_t pin, uint32_t val)
{
    hal_gpio_set_output(getGPIO(pin), (hal_gpio_data_t)((bool)val));
}

void digitalToggle(uint32_t pin, uint32_t pause)
{
    hal_gpio_toggle_pin(getGPIO(pin));
    if (pause)
        delay(pause);
}

/// @brief Arduino compatible
int digitalRead(uint32_t pin)
{
    hal_gpio_get_input(getGPIO(pin), (hal_gpio_data_t *)&pin);
    return (bool)pin;
}

static void api_eint_callback_t(void *user) // ISR
{
    api_send_message(MSG_EINT_CALLBACK, user, NULL);
}

/// @brief Arduino compatible
void detachInterrupt(uint32_t pin)
{
    pins_t *n = getPIN(pin);
    if (n)
    {
        if (n->eint < HAL_EINT_NUMBER_MAX)
        {
            hal_eint_deinit(n->eint);
            n->eint_cb = NULL;
            hal_pinmux_set_function(n->gpio, 0);
        }
    }
}

/// @brief Arduino compatible
void attachInterrupt(uint32_t pin, void (*cb)(void), int mode)
{
    pins_t *n = getPIN(pin);
    if (n && cb)
    {
        if (n->eint < HAL_EINT_NUMBER_MAX) // ?
        {
            hal_eint_config_t cfg;
            cfg.debounce_time = 0;
            cfg.trigger_mode = (hal_eint_trigger_mode_t)mode;
            cfg.firq_enable = 0;
            n->eint_cb = NULL;
            hal_eint_mask(n->eint);
            if (HAL_EINT_STATUS_OK == hal_eint_init(n->eint, &cfg))
            {
                if (HAL_EINT_STATUS_OK == hal_eint_register_callback(n->eint, api_eint_callback_t, n))
                {
                    n->eint_cb = cb;
                    hal_pinmux_set_function(n->gpio, 7);
                }
                else
                {
                    hal_eint_deinit(n->eint);
                }
            }
            hal_eint_unmask(n->eint);
        }
    }
}

/// @brief Arduino compatible, Read ADC
int analogRead(uint32_t pin)
{
    int res = IO_ERROR;
    uint32_t data, result = 0;
    if (pin < PIN_MAX && HAL_ADC_STATUS_OK == (res = hal_adc_init()))
    {
        for (int i = 0, res = 0; i < PINS[pin].adc_samples; i++)
        {
            data = 0;
            if (HAL_ADC_STATUS_OK != (res = hal_adc_get_data_polling(PINS[pin].adc_channel, &data)))
                break;
            result += data;
        }
        hal_adc_deinit();
        return result /= PINS[pin].adc_samples;
    }
    return res;
}

/// @brief Arduino compatible, Set PWM duty
void analogWrite(uint32_t pin, int val)
{
    if (pin < PIN_MAX)
    {
        val = (val < 0) ? 0 : val;
        val = (val > 100) ? 100 : val;
        hal_pwm_set_duty_cycle(PINS[pin].pwm_channel, (PINS[pin].pwm_total_count * val) / 100);
    }
}

// NON ARDUINO FUNCTIONS //////////////////////////////////////////////////////

int api_syscall(io_commands io, uint32_t variable, va_list list)
{
    static bool api_debug = false;
    int res = IO_ERROR;
    switch (io) // always break/return
    {
    case IO_INIT:
        if (IO_KEY == variable)
            return api_init();
        break;

    case IO_DEBUG_SET:
        api_debug = (bool)variable;
        break;

    case IO_DEBUG:
        if (api_debug)
            return api_vprintf((const char *)variable, list);
        break;

    case IO_GET_API_VERSION:
        return ((app_rom_t *)APP_ROM)->api_version;

    case IO_GET_RAND:
        return api_rand();

    case IO_GET_POWER_REASON:
        res = rtc_power_on_result();
        /*
            0: power
            1: deep
            2: deeper
            3: reset
            4: wdt hw reset
            5: wdt sw reset
        */
        break;

    case IO_POWER_OFF:
        if (IO_KEY == variable)
        {
            // TODO
        }
        break;

    case IO_RESET:
        if (IO_KEY == variable)
        {
            hal_wdt_config_t cfg;
            cfg.mode = 0;
            cfg.seconds = 1;
            hal_wdt_disable(0xCAFE0065);
            hal_wdt_init(&cfg);
            hal_wdt_software_reset();
            while (1)
                ;
        }
        break;

    case IO_KICK_DOG:
        if (IO_KEY == variable)
            return hal_wdt_feed(HAL_WDT_FEED_MAGIC);
        break;

    case IO_GET_UID:
        if (variable && 16u == va_arg(list, uint32_t))
        {
            extern bool hal_efuse_read_uid(uint8_t *, uint32_t); // 0xA20A0610[4]
            hal_efuse_read_uid((uint8_t *)variable, 16);
            res = 0;
        }
        break;

    case IO_GET_BATTERY:
        variable = 0;
        hal_adc_init();
        hal_adc_get_data_polling(HAL_ADC_CHANNEL_6, &variable);
        hal_adc_deinit();
        return (5600 * variable + 2047) / 4095;

    default:
        res = IO_ERROR;
        break;
    } // switch
    return res;
}

// DRIVERS - PERIPHERI ////////////////////////////////////////////////////////

driver_t DRIVER[] = {
    /*00 UART0 */ {
        .index = 0,
        .name = "com:0",
        .open = UART_Open,
        .close = UART_Close,
        .write = UART_Write,
        .syscall = UART_Syscall,
    },
    /*01 UART1 */ {
        .index = 1,
        .name = "com:1",
        .open = UART_Open,
        .close = UART_Close,
        .write = UART_Write,
        .syscall = UART_Syscall,
    },
    /*02 UART2 */ {
        .index = 2,
        .name = "com:2",
        .open = UART_Open,
        .close = UART_Close,
        .write = UART_Write,
        .syscall = UART_Syscall,
    },
    /*03 SPI0 */ {
        .index = 0,
        .name = "spi:0",
        .open = SPI_Open,
        .close = SPI_Close,
        .write = SPI_Write,
        .syscall = SPI_Syscall,
        .spi.transfer = SPI_Transfer,
        .spi.fill = SPI_Fill,
    },
    /*04 SPI1 */ {
        .index = 1,
        .name = "spi:0",
        .open = SPI_Open,
        .close = SPI_Close,
        .write = SPI_Write,
        .syscall = SPI_Syscall,
        .spi.transfer = SPI_Transfer,
        .spi.fill = SPI_Fill,
    },
    /*05 I2C0 */ {
        .index = 0,
        .name = "i2c:0",
        .open = I2C_Open,
        .close = I2C_Close,
        .syscall = I2C_Syscall,
        .i2c.transmit = I2C_Transmit,
        .i2c.receive = I2C_Receive,
    },
    /*06 I2C1 */ {
        .index = 1,
        .name = "i2c:1",
        .open = I2C_Open,
        .close = I2C_Close,
        .syscall = I2C_Syscall,
        .i2c.transmit = I2C_Transmit,
        .i2c.receive = I2C_Receive,
    },
    /*07 I2C2 */ {
        .index = 2,
        .name = "i2c:2",
        .open = I2C_Open,
        .close = I2C_Close,
        .syscall = I2C_Syscall,
        .i2c.transmit = I2C_Transmit,
        .i2c.receive = I2C_Receive,
    },
    /*08 RIL */
    /*09 USB */
};
#define DRIVERS_MAX sizeof(DRIVER) / sizeof(DRIVER[0])
#define DRIVER_RING_MAX_SIZE 4096

static void *drv_get(const char *name)
{
    if (name)
    {
        for (int i = 0; i < DRIVERS_MAX; i++)
        {
            if (0 == strcmp(name, DRIVER[i].name))
            {
                // PRINTF("[DRV] %s( %s )\n", __func__, name);
                return &DRIVER[i];
            }
        }
    }
    PRINTF("[ERROR][DRV] %s( %s ) NULL\n", __func__, name);
    return NULL;
}

void *drv_create(const char *name, uint16_t rx_ring_size, uint16_t tx_ring_size)
{
    driver_t *d = drv_get(name);
    if (d)
    {
        if (d->mutex || d->is_open) // already
        {
            PRINTF("[DRV] %s( ALREADY ) %p, %d\n", __func__, d->mutex, d->is_open);
            return d;
        }

        rx_ring_size = (rx_ring_size > DRIVER_RING_MAX_SIZE) ? DRIVER_RING_MAX_SIZE : rx_ring_size;
        tx_ring_size = (tx_ring_size > DRIVER_RING_MAX_SIZE) ? DRIVER_RING_MAX_SIZE : tx_ring_size;

        if (NULL == (d->mutex = xSemaphoreCreateMutex()))
            goto error;

        if (rx_ring_size)
        {
            if (NULL == (d->rx_buffer = pvPortMalloc(rx_ring_size)))
                goto error;
            ring_buffer_init(&d->rx_ring, rx_ring_size, d->rx_buffer);
        }
        if (tx_ring_size)
        {
            if (NULL == (d->tx_buffer = pvPortMalloc(tx_ring_size)))
                goto error;
            ring_buffer_init(&d->tx_ring, tx_ring_size, d->tx_buffer);
        }
        d->is_open = false;
    }
    PRINTF("[DRV] %s( '%s' )\n", __func__, name);
    return d;
error:
    assert("driver constructor" && 0);
}

void drv_free(driver_t *d)
{
    if (d)
    {
        if (NULL == d->mutex)
            return;
        if (MUTEX_TAKE(d->mutex) == pdTRUE)
        {
            if (d->close)
                d->close(d);
            d->is_open = false;
            if (d->rx_buffer)
            {
                vPortFree(d->rx_buffer);
                d->rx_buffer = NULL;
            }
            if (d->tx_buffer)
            {
                vPortFree(d->tx_buffer);
                d->rx_buffer = NULL;
            }
            MUTEX_GIVE(d->mutex);
            vSemaphoreDelete(d->mutex);
            d->mutex = NULL;
        }
    }
}

int drv_open(driver_t *d, ...)
{
    int res = IO_ERROR;
    if (d)
    {
        if (false == d->is_open)
        {
            if (MUTEX_TAKE(d->mutex) == pdTRUE)
            {
                if (d->tx_buffer)
                    d->tx_ring.reset(&d->tx_ring);
                if (d->rx_buffer)
                    d->rx_ring.reset(&d->rx_ring);
                if (d->open)
                {
                    va_list list;
                    va_start(list, d);
                    d->is_open = (0 == (res = d->open(d, list)));
                    va_end(list);
                }
                else
                {
                    d->is_open = true;
                    res = 0;
                }
                MUTEX_GIVE(d->mutex);
            }
            PRINTF("[DRV] %s( %s )\n", __func__, d->name);
        }
    }
    if (res)
    {
        PRINTF("[ERROR][DRV] %s( %d )\n", __func__, res);
    }
    return res;
}

int drv_close(driver_t *d)
{
    int res = IO_ERROR;
    if (d)
    {
        if (d->is_open)
        {
            if (MUTEX_TAKE(d->mutex) == pdTRUE)
            {
                if (d->close)
                    d->close(d);
                d->is_open = false;
                MUTEX_GIVE(d->mutex);
                res = 0;
            }
        }
    }
    return res;
}

size_t drv_read(driver_t *d, direction_e where, void *buf, size_t count)
{
    int res = 0;
    if (d && buf && count && d->is_open)
    {
        if (MUTEX_TAKE(d->mutex) == pdTRUE)
        {
            if (RW_RING_RX == where && d->rx_buffer)
            {
                res = d->rx_ring.read(&d->rx_ring, buf, count);
            }
            else if (RW_RING_TX == where && d->tx_buffer)
            {
                res = d->tx_ring.read(&d->tx_ring, buf, count);
            }
            else
            {
                if (d->read)
                    res = d->read(d, buf, count);
            }
            MUTEX_GIVE(d->mutex);
        }
    }
    return res;
}

size_t drv_write(driver_t *d, direction_e where, const void *buf, size_t count)
{
    int res = 0;
    if (d && buf && count && d->is_open)
    {
        if (MUTEX_TAKE(d->mutex) == pdTRUE)
        {
            if (RW_RING_RX == where && d->rx_buffer)
            {
                res = d->rx_ring.write(&d->rx_ring, (void *)buf, count);
            }
            else if (RW_RING_TX == where && d->tx_buffer)
            {
                res = d->tx_ring.write(&d->tx_ring, (void *)buf, count);
            }
            else
            {
                if (d->write)
                    res = d->write(d, buf, count);
            }
            MUTEX_GIVE(d->mutex);
        }
    }
    return res;
}

int drv_syscall(driver_t *d, io_commands io, ...)
{
    int res = IO_ERROR,
        val = -1;
    if (d)
    {
        if (d->is_open)
        {
            if (MUTEX_TAKE(d->mutex) == pdTRUE)
            {
                switch (io)
                {
                case IO_DRIVER_RX_AVAILABLE:
                    res = 0;
                    if (d->rx_buffer)
                        res = d->rx_ring.size(&d->rx_ring);
                    break;

                case IO_DRIVER_TX_AVAILABLE:
                    res = 0;
                    if (d->tx_buffer)
                        res = d->tx_ring.size(&d->tx_ring);
                    break;

                case IO_DRIVER_RX_PEEK:
                    if (d->rx_buffer)
                        res = (1 == d->rx_ring.peek(&d->rx_ring, &val, 1)) ? val & 0xFF : IO_ERROR;
                    break;

                case IO_DRIVER_TX_PEEK:
                    if (d->tx_buffer)
                        res = (1 == d->tx_ring.peek(&d->tx_ring, &val, 1)) ? val & 0xFF : IO_ERROR;
                    break;

                case IO_DRIVER_FLUSH:
                    if (d->rx_buffer)
                        d->rx_ring.reset(&d->rx_ring);
                    if (d->tx_buffer)
                        d->tx_ring.reset(&d->tx_ring);
                    res = 0;
                    break;

                case IO_DRIVER_RX_FLUSH: // TODO read
                    if (d->rx_buffer)
                        d->rx_ring.reset(&d->rx_ring);
                    res = 0;
                    break;

                case IO_DRIVER_TX_FLUSH: // TODO write
                    if (d->tx_buffer)
                        d->tx_ring.reset(&d->tx_ring);
                    res = 0;
                    break;

                default:
                    if (d->syscall)
                    {
                        va_list list;
                        va_start(list, io);
                        res = d->syscall(d, io, list);
                        va_end(list);
                    }
                    break;

                } // switch
                MUTEX_GIVE(d->mutex);
            }
        }
    }
    return res;
}

///////////////////////////////////////////////////////////////////////////////