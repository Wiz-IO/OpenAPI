#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "mt2625.h"
#include "hal.h"
#include "memory_attribute.h"
#include "syslog.h"
#ifdef MTK_USB_DEMO_ENABLED
#include "usb.h"
#endif

#include "openapi/OpenAPI-core.h"
#include "arduino/arduino.h"

#define PRINTF(...) kprintf(__VA_ARGS__)

#define APP_ENABLE (1)

static void wizio_task(void *param)
{
    static int T = 0;
    LOG_I(common, "[W] BEGIN: WiZIO Task");
    vTaskDelay(1);

    driver_t *d = (driver_t*)drv_create("com:2", 256, 0);
    int res = drv_open(d, 115200, 8, 0, 1);
    drv_write(d, 0, "TEST 12345678\n", 14);

    hal_pinmux_set_function(HAL_GPIO_1, 0);
    hal_gpio_set_direction(HAL_GPIO_1, HAL_GPIO_DIRECTION_OUTPUT);
    while (1)
    {
        hal_gpio_toggle_pin(HAL_GPIO_1);
        if (++T % 100 == 0)
        {
            LOG_I(common, "[W] LOOP: %d mS", xTaskGetTickCount() * 10);
        }
        vTaskDelay(20);
    }
}

int main(void)
{
    extern void system_init(void);
    system_init();

    LOG_I(common, "[M] Mediatek MT2625 Hello World 2022 Georgi Angelov\r\n");
    SysInitStatus_Set();

#ifdef MTK_USB_DEMO_ENABLED
    extern void usb_boot_init(void);
    usb_boot_init();
#endif

    if (api_run_application(APP_ENABLE))
    { // run other
        xTaskCreate(wizio_task, "wizio_task", 1024, NULL, TASK_PRIORITY_NORMAL, NULL);
    }
    else
    {
        //PRINTF("API START\n");
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
