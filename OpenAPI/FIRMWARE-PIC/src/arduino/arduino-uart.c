#include "arduino.h"

// Local functions, called from driver
// RING RX

#define PRINTF(...) kprintf(__VA_ARGS__)

static uart_ctx_t PERIPHERY_UART[UART_MAX] = {
    {
        .port = HAL_UART_0,
        .g_rx = HAL_GPIO_2,
        .m_rx = 1,
        .g_tx = HAL_GPIO_5,
        .m_tx = 1,
    },
    {
        .port = HAL_UART_1,
        .g_rx = HAL_GPIO_12,
        .m_rx = 3,
        .g_tx = HAL_GPIO_13,
        .m_tx = 3,
    },
    {
        .port = HAL_UART_2,
        .g_rx = HAL_GPIO_18,
        .m_rx = 3,
        .g_tx = HAL_GPIO_22,
        .m_tx = 5,
    },
};

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN
static uint8_t serial_rx_vfifo_buffer[UART_MAX][UART_VFIFO_SIZE];

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN
static uint8_t serial_tx_vfifo_buffer[UART_MAX][UART_VFIFO_SIZE];

static void UART_Callback(hal_uart_callback_event_t event, void *user) // ISR !!!
{
    if (user)
    {
        if (event == HAL_UART_EVENT_READY_TO_READ)
        {
            api_send_message(MSG_UART_RECEIVE, user, NULL);
        }
        else if (event == HAL_UART_EVENT_READY_TO_WRITE)
        {
            ((uart_ctx_t *)user)->event = 0;
        }
    }
}

// LOCAL //////////////////////////////////////////////////////////////////////

void UART_Receive(uart_ctx_t *ctx)
{
    uint32_t buffer[4], received;
    if (pdTRUE == MUTEX_TAKE(ctx->drv->mutex))
    {
        while (true)
        {
            if ((received = hal_uart_receive_dma(ctx->port, (uint8_t *)&buffer, sizeof(buffer))))
            {
                if (0 == ctx->drv->rx_ring.write(&ctx->drv->rx_ring, (uint8_t *)&buffer, received))
                {
                    break; // ring is full
                }
            }
            else
            {
                break; // nothing to receive
            }
        }
        MUTEX_GIVE(ctx->drv->mutex);
    }
}

// COMMON /////////////////////////////////////////////////////////////////////

int UART_Open(struct driver_s *drv, va_list list)
{
    int res = IO_ERROR;

    int brg = va_arg(list, int);
    int length = va_arg(list, int);
    int parity = va_arg(list, int);
    int stop_bit = va_arg(list, int);

    uart_ctx_t *ctx = &PERIPHERY_UART[drv->index];
    ctx->drv = drv; // self

    hal_uart_deinit(ctx->port);

    hal_uart_config_t uart_config = {0};
    hal_uart_dma_config_t dma_config = {0};
    static const uint32_t baudrate_map[] = {110, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 3000000}; // [14]

    uart_config.baudrate = HAL_UART_BAUDRATE_MAX;
    for (int i = 0; 0 < HAL_UART_BAUDRATE_MAX; i++)
    {
        if (baudrate_map[i] == brg)
        {
            uart_config.baudrate = (hal_uart_baudrate_t)i;
            break;
        }
    }
    if (HAL_UART_BAUDRATE_MAX == uart_config.baudrate)
    {
        return IO_ERROR - 1;
    }

    if (length < 5 || length > 8)
    {
        return IO_ERROR - 2;
    }
    uart_config.word_length = (hal_uart_word_length_t)length - 5;

    if (parity < 0 || parity > 2)
    {
        return IO_ERROR - 3;
    }
    uart_config.parity = (hal_uart_parity_t)parity;

    if (stop_bit < 1 || stop_bit > 2)
    {
        return IO_ERROR - 4;
    }
    uart_config.stop_bit = (hal_uart_stop_bit_t)stop_bit - 1;

    if (HAL_UART_STATUS_OK == (res = hal_uart_init(ctx->port, &uart_config)))
    {
        hal_uart_disable_flowcontrol(ctx->port);
        hal_uart_set_auto_baudrate(ctx->port, false);

        dma_config.receive_vfifo_alert_size = 20;
        dma_config.receive_vfifo_buffer = serial_rx_vfifo_buffer[drv->index];
        dma_config.receive_vfifo_buffer_size = UART_VFIFO_SIZE;
        dma_config.receive_vfifo_threshold_size = (UART_VFIFO_SIZE * 2) / 3;
        dma_config.send_vfifo_buffer = serial_tx_vfifo_buffer[drv->index];
        dma_config.send_vfifo_buffer_size = UART_VFIFO_SIZE;
        dma_config.send_vfifo_threshold_size = UART_VFIFO_SIZE / 10;

        if (HAL_UART_STATUS_OK == (res = hal_uart_set_dma(ctx->port, &dma_config)))
        {
            if (HAL_UART_STATUS_OK == (res = hal_uart_register_callback(ctx->port, UART_Callback, ctx)))
            {
                hal_pinmux_set_function(ctx->g_rx, ctx->m_rx);
                hal_pinmux_set_function(ctx->g_tx, ctx->m_tx);
                // res = 0; // done
            }
            else
            {
                // PRINTF("[ERROR][UART] CB: %d\n", res);
                res = -40;
            }
        }
        else
        {
            // PRINTF("[ERROR][UART] DMA: %d\n", res);
            res = -30;
        }
    }

    return res;
}

void UART_Close(struct driver_s *drv)
{
    uart_ctx_t *ctx = &PERIPHERY_UART[drv->index];
    hal_uart_deinit(ctx->port);
    hal_pinmux_set_function(ctx->g_rx, 0);
    hal_pinmux_set_function(ctx->g_tx, 0);
}

size_t UART_Write(struct driver_s *drv, const uint8_t *buffer, size_t size)
{
    uint32_t res = 0, left = size;
    uart_ctx_t *ctx = &PERIPHERY_UART[drv->index];
    while (1)
    {
        ctx->event = 1;
        uint32_t cnt = hal_uart_send_dma(ctx->port, buffer, left);
        if (0 == cnt && left > 0)
        {
            res = 0; // error
            break;
        }
        left -= cnt;
        buffer += cnt;
        if (0 == left)
        {
            res = size;
            break;
        }
        while (ctx->event)
            ;
    }
    return res;
}

int UART_Syscall(struct driver_s *d, io_commands io, va_list list)
{
    // uart_ctx_t *ctx = &PERIPHERY_UART[drv->index];
    return IO_ERROR;
}
