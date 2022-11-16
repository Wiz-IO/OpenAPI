#include "arduino.h"

// Local functions, called from driver
// RING RX & TX

#define PRINTF(...) kprintf(__VA_ARGS__)

static i2c_ctx_t PERIPHERY_I2C[I2C_MAX] = {
    {
        .port = HAL_I2C_MASTER_0,
        .g_clock = HAL_GPIO_6, // RI
        .g_data = HAL_GPIO_7,  // DCD
        .m_clock = 4,
        .m_data = 4,
    },
    {
        .port = HAL_I2C_MASTER_1,
        .g_clock = HAL_GPIO_MAX, // TODO
        .g_data = HAL_GPIO_MAX,
        .m_clock = 0,
        .m_data = 0,
    },
    {
        .port = HAL_I2C_MASTER_2,
        .g_clock = HAL_GPIO_MAX, // TODO
        .g_data = HAL_GPIO_MAX,
        .m_clock = 0,
        .m_data = 0,
    },
};

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN
static uint8_t i2c_vfifo_buffer[I2C_MAX][I2C_VFIFO_SIZE];

static void I2C_Callback(uint8_t slave_address, hal_i2c_callback_event_t event, void *user)
{
    if (user)
        ((i2c_ctx_t *)user)->event = event;
}

static hal_i2c_frequency_t I2C_Translate_frequency(uint32_t frequency_hz)
{
    hal_i2c_frequency_t F;
    switch (frequency_hz)
    {
    case 1000000:
        F = HAL_I2C_FREQUENCY_1M;
        break;
    case 400000:
        F = HAL_I2C_FREQUENCY_400K;
        break;
    case 200000:
        F = HAL_I2C_FREQUENCY_200K;
        break;
    case 100000:
        F = HAL_I2C_FREQUENCY_100K;
        break;
    default:
        F = HAL_I2C_FREQUENCY_50K;
        break;
    }
    return F;
}

// IO /////////////////////////////////////////////////////////////////////////

static int I2C_io_set_frequency(struct driver_s *drv, uint32_t frequency)
{
    i2c_ctx_t *ctx = &PERIPHERY_I2C[drv->index];
    return hal_i2c_master_set_frequency(ctx->port, I2C_Translate_frequency(frequency));
}

// COMMON /////////////////////////////////////////////////////////////////////

void I2C_Close(struct driver_s *drv)
{
    i2c_ctx_t *ctx = &PERIPHERY_I2C[drv->index];
    hal_i2c_master_deinit(ctx->port);
    hal_pinmux_set_function(ctx->g_clock, 0);
    hal_pinmux_set_function(ctx->g_data, 0);
}

int I2C_Open(struct driver_s *drv, va_list list)
{
    int res = IO_ERROR;

    int frequency = va_arg(list, int);

    hal_i2c_config_t cfg = {0};

    i2c_ctx_t *ctx = &PERIPHERY_I2C[drv->index];
    ctx->drv = drv; // self

    hal_i2c_master_deinit(ctx->port);

    cfg.frequency = I2C_Translate_frequency(frequency);
    res = hal_i2c_master_init(ctx->port, &cfg);
    if (HAL_I2C_STATUS_OK == res)
    {
        res = hal_i2c_master_register_callback(ctx->port, I2C_Callback, ctx);
        if (HAL_I2C_STATUS_OK == res)
        {
            hal_pinmux_set_function(ctx->g_clock, ctx->m_clock);
            hal_pinmux_set_function(ctx->g_data, ctx->m_data);
        }
    }
    return res;
}

int I2C_Syscall(struct driver_s *drv, io_commands io, va_list list)
{
    int res = IO_ERROR;
    switch (io)
    {
    case IO_I2C_SET_FREQUENCY:
        res = I2C_io_set_frequency(drv, va_arg(list, uint32_t));
        break;
    } // switch
    return res;
}

// FAST ///////////////////////////////////////////////////////////////////////

int I2C_Transmit(void *x, uint32_t address_7)
{
    driver_t *drv = (driver_t *)x;

    int res = -100, timeout = 100;
    if (drv && address_7 && drv->tx_ring.buffer) // test drv !
    {
        i2c_ctx_t *ctx = &PERIPHERY_I2C[drv->index];
        if (0 == drv->tx_ring.len)
            return 0; // empty ?

        memcpy(i2c_vfifo_buffer[drv->index], drv->tx_ring.buffer, drv->tx_ring.len);
        ctx->event = 1;
        res = hal_i2c_master_send_dma(ctx->port, address_7, i2c_vfifo_buffer[drv->index], drv->tx_ring.len);
        if (HAL_I2C_STATUS_OK == res)
        {
            while (1 == ctx->event && timeout--)
            {
                vTaskDelay(1); // ???
            }
            if (0 == timeout)
            {
                res = -101;
            }
            else
            {
                res = ctx->event;
                if (res)
                {
                    res -= 10;
                }
            }
        }
        drv->tx_ring.reset(&drv->tx_ring);
    }
    return res;
}

int I2C_Receive(void *x, uint32_t address_7, size_t size)
{
    driver_t *drv = (driver_t *)x;

    int res = -100, timeout = 100;
    if (drv && address_7 && size) // test drv !
    {
        i2c_ctx_t *ctx = &PERIPHERY_I2C[drv->index];
        drv->tx_ring.reset(&drv->rx_ring);
        memset(i2c_vfifo_buffer[drv->index], 0, I2C_VFIFO_SIZE);
        if (size > drv->rx_ring.capacity || size > I2C_VFIFO_SIZE)
            return -11; // no space

        ctx->event = 1;
        res = hal_i2c_master_receive_dma(ctx->port, address_7, i2c_vfifo_buffer[drv->index], size);
        if (HAL_I2C_STATUS_OK == res)
        {
            while (1 == ctx->event && timeout--)
            {
                vTaskDelay(1); // ???
            }
            if (0 == timeout)
            {
                res = -101;
            }
            else
            {
                res = ctx->event;
                if (res)
                {
                    res -= -10;
                }
                else
                {
                    drv->rx_ring.write(&drv->rx_ring, i2c_vfifo_buffer[drv->index], size);
                }
            }
        }
    }
    return res;
}
