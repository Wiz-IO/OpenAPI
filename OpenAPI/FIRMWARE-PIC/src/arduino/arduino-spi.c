#include "arduino.h"

// Local functions, called from driver
// NO RINGS

#define PRINTF(...) kprintf(__VA_ARGS__)

static spi_ctx_t PERIPHERY_SPI[SPI_MAX] = {
    {
        .port = HAL_SPI_MASTER_0,
        .g_miso = HAL_GPIO_9,
        .g_mosi = HAL_GPIO_10,
        .g_sck = HAL_GPIO_11,
        .g_cs = HAL_GPIO_8,
    },
    {
        .port = HAL_SPI_MASTER_1,
        .g_miso = HAL_GPIO_20,
        .g_mosi = HAL_GPIO_21,
        .g_sck = HAL_GPIO_22,
        .g_cs = HAL_GPIO_19,
    }};

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN
static uint8_t spi_dma_tx_buffer[SPI_MAX][SPI_VFIFO_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN
static uint8_t spi_dma_rx_buffer[SPI_MAX][SPI_VFIFO_SIZE];

extern SPIM_REGISTER_T *const g_spi_master_register[HAL_SPI_MASTER_MAX]; // register for fast access

#define SPI_PIN_NC -1
#define SPI_PIN_HARD -2

static inline bool SPI_get_bit_order(SPIM_REGISTER_T *reg)
{
    return reg->CTRL0_UNION.CTRL0_CELLS.CTRL0 && SPIM_CTRL0_TXMSBF_MASK; // CTRL0[3][2]
}

// IO /////////////////////////////////////////////////////////////////////////

static int SPI_io_set_frequency(struct driver_s *drv, uint32_t frequency)
{
    spi_ctx_t *ctx = &PERIPHERY_SPI[drv->index];
    uint32_t sck_count = 104000000 / (frequency * 2) - 1;
    g_spi_master_register[ctx->port]->CFG1_UNION.CFG1 = ((sck_count << SPIM_CFG1_SCK_LOW_COUNT_OFFSET) | sck_count);
    return 0;
}

static int SPI_io_set_mode(struct driver_s *drv, uint8_t mode)
{
    spi_ctx_t *ctx = &PERIPHERY_SPI[drv->index];
    if (mode & 1) // PHASE
    {
        g_spi_master_register[ctx->port]->CTRL0_UNION.CTRL0_CELLS.CTRL0 |= (SPIM_CTRL0_CPHA_MASK);
    }
    else
    {
        g_spi_master_register[ctx->port]->CTRL0_UNION.CTRL0_CELLS.CTRL0 &= (~SPIM_CTRL0_CPHA_MASK);
    }
    if (mode & 2) // POL
    {
        g_spi_master_register[ctx->port]->CTRL0_UNION.CTRL0_CELLS.CTRL0 |= (SPIM_CTRL0_CPOL_MASK);
    }
    else
    {
        g_spi_master_register[ctx->port]->CTRL0_UNION.CTRL0_CELLS.CTRL0 &= (~SPIM_CTRL0_CPOL_MASK);
    }
    return 0;
}

static int SPI_io_set_bit_order(struct driver_s *drv, bool bit_order)
{
    spi_ctx_t *ctx = &PERIPHERY_SPI[drv->index];
    if (bit_order) // MSB FIRST = 1
    {
        g_spi_master_register[ctx->port]->CTRL0_UNION.CTRL0_CELLS.CTRL0 |= (SPIM_CTRL0_TXMSBF_MASK | SPIM_CTRL0_RXMSBF_MASK);
    }
    else
    {
        g_spi_master_register[ctx->port]->CTRL0_UNION.CTRL0_CELLS.CTRL0 &= (~(SPIM_CTRL0_TXMSBF_MASK | SPIM_CTRL0_RXMSBF_MASK));
    }
    return 0;
}

// COMMON /////////////////////////////////////////////////////////////////////

int SPI_Open(struct driver_s *drv, va_list params)
{
    int res = IO_ERROR;

    uint32_t frequency = va_arg(params, int); // in Hz
    int order = va_arg(params, int);          // bit_order = MSBFIRST;
    int mode = va_arg(params, int);           // cpol & cpha
    int miso = va_arg(params, int);           // -1 = dont use
    int mosi = va_arg(params, int);           // -1 = dont use
    int sck = va_arg(params, int);            // -1 = dont use
    int cs = va_arg(params, int);             // -1 = dont use, -2 use hard pin,  >-1 ArduinoPin

    spi_ctx_t *ctx = &PERIPHERY_SPI[drv->index];
    hal_spi_master_config_t cfg = {0};

    hal_spi_master_deinit(ctx->port);

    ctx->u_miso = miso;
    ctx->u_mosi = mosi;
    ctx->u_sck = sck;
    ctx->u_cs = cs;

    cfg.clock_frequency = frequency;
    cfg.bit_order = (hal_spi_master_bit_order_t)(order & 1);
    cfg.phase = (hal_spi_master_clock_phase_t)(mode & 1);
    cfg.polarity = (hal_spi_master_clock_polarity_t)((mode >> 1) & 1);

    res = hal_spi_master_init(ctx->port, &cfg);
    if (HAL_SPI_MASTER_STATUS_OK == res)
    {
        // res = hal_spi_master_register_callback(ctx->port, spi_master_callback, p); // NOT NEED, blocked
        // if (HAL_SPI_MASTER_STATUS_OK == res)
        {
            if (miso == SPI_PIN_HARD) // use pin ?
            {
                // PRINTF("[SPI] MISO HARD\n", __func__);
                hal_pinmux_set_function(ctx->g_miso, 2); // MODE always 2
            }
            if (mosi == SPI_PIN_HARD) // use pin ?
            {
                // PRINTF("[SPI] MOSI HARD\n", __func__);
                hal_pinmux_set_function(ctx->g_mosi, 2); // MODE always 2
            }
            if (sck == SPI_PIN_HARD) // use pin ?
            {
                // PRINTF("[SPI] CLOCK HARD\n", __func__);
                hal_pinmux_set_function(ctx->g_sck, 2); // MODE always 2
            }
            if (cs == SPI_PIN_HARD) // -2 use hard pin
            {
                // PRINTF("[SPI] CS HARD\n", __func__);
                hal_pinmux_set_function(ctx->g_cs, 2); // MODE always 2
            }
        }
    }
    if (res)
    {
        PRINTF("[ERROR][SPI] %s( %d )\n", __func__, res);
    }
    return res;
}

void SPI_Close(struct driver_s *drv)
{
    spi_ctx_t *ctx = &PERIPHERY_SPI[drv->index];
    hal_spi_master_deinit(ctx->port);
    drv->is_open = false;

    if (ctx->u_miso == SPI_PIN_HARD)
    {
        hal_pinmux_set_function(ctx->g_miso, 0);
    }
    if (ctx->u_mosi == SPI_PIN_HARD)
    {
        hal_pinmux_set_function(ctx->g_mosi, 0);
    }
    if (ctx->u_sck == SPI_PIN_HARD)
    {
        hal_pinmux_set_function(ctx->g_sck, 0);
    }
    if (ctx->u_cs == SPI_PIN_HARD)
    {
        hal_pinmux_set_function(ctx->g_cs, 0);
    }
}

size_t SPI_Write(struct driver_s *drv, const uint8_t *buffer, size_t size)
{
    int res = IO_ERROR;
    size_t count;
    while (size)
    {
        count = (size / SPI_VFIFO_SIZE) ? SPI_VFIFO_SIZE : size;
        memcpy(spi_dma_tx_buffer[drv->index], buffer, count);
        buffer += count;
        if (HAL_SPI_MASTER_STATUS_OK == (res = hal_spi_master_send_dma_blocking(drv->index, spi_dma_tx_buffer[drv->index], count)))
        {
            size -= count;
        }
        else
        {
            break; // error
        }
    }
    return res; // int
}

int SPI_Syscall(struct driver_s *drv, io_commands io, va_list list)
{
    int res = IO_ERROR;
    switch (io)
    {
    case IO_SPI_SET_FREQUENCY:
        res = SPI_io_set_frequency(drv, va_arg(list, uint32_t));
        break;
    case IO_SPI_SET_MODE:
        res = SPI_io_set_mode(drv, va_arg(list, uint32_t));
        break;
    case IO_SPI_SET_ORDER:
        res = SPI_io_set_bit_order(drv, va_arg(list, uint32_t));
        break;
    } // switch
    return res;
}

// FAST ///////////////////////////////////////////////////////////////////////

int SPI_Transfer(void *x, uint8_t *tx_buffer, uint8_t *rx_buffer, size_t size)
{
    driver_t *drv = (driver_t *)x;
    spi_ctx_t *ctx = &PERIPHERY_SPI[drv->index];

    int res = IO_ERROR;
    size_t count;
    if (drv && size) // test drv !
    {
        hal_spi_master_send_and_receive_config_t cfg;

        if (NULL == tx_buffer)
        {
            memset(spi_dma_tx_buffer[drv->index], 0, SPI_VFIFO_SIZE);
        }

        cfg.send_data = spi_dma_tx_buffer[drv->index];
        cfg.receive_buffer = spi_dma_rx_buffer[drv->index];

        while (size)
        {
            count = (size / SPI_VFIFO_SIZE) ? SPI_VFIFO_SIZE : size;
            cfg.send_length = count;
            cfg.receive_length = count; // ?!? SDK bug
            if (tx_buffer)
            {
                memcpy(spi_dma_tx_buffer[drv->index], tx_buffer, count);
                tx_buffer += count;
            }
            if (HAL_SPI_MASTER_STATUS_OK == (res = hal_spi_master_send_and_receive_dma_blocking(ctx->port, &cfg)))
            {
                if (rx_buffer)
                {
                    memcpy(rx_buffer, spi_dma_rx_buffer[drv->index], count);
                    rx_buffer += count;
                }
                size -= count;
            }
            else
            {
                break; // error
            }
        }
    }
    if (res)
    {
        PRINTF("[ERROR][SPI] %s( %d )\n", __func__, res);
    }
    return res;
}

int SPI_Fill(void *x, uint32_t data_len, uint32_t data, size_t size)
{
    int res = IO_ERROR;
    driver_t *drv = (driver_t *)x;
    spi_ctx_t *ctx = &PERIPHERY_SPI[drv->index];
    size_t count;
    char *D = (char *)&data;
    if (drv && data_len && size) // test drv !
    {
        uint8_t *dst = spi_dma_tx_buffer[drv->index];
        if (SPI_get_bit_order(g_spi_master_register[drv->index]))
            data = __REV16(data); // TODO check
        for (int i = 0; i < size; i++)
        {
            *dst++ = D[0];
            if (data_len)
                *dst++ = D[1];
// #define COLOR_32
#ifdef COLOR_32
            if (data_len > 3)
            {
                *dst++ = D[2];
                *dst++ = D[3];
            }
#endif
        }
        while (size)
        {
            count = (size / SPI_VFIFO_SIZE) ? SPI_VFIFO_SIZE : size;
            if (HAL_SPI_MASTER_STATUS_OK == (res = hal_spi_master_send_dma_blocking(ctx->port, spi_dma_tx_buffer[drv->index], count)))
            {
                size -= count;
            }
            else
            {
                break; // error
            }
        }
    }
    if (res)
    {
        PRINTF("[ERROR][SPI] %s( %d )\n", __func__, res);
    }
    return res;
}
