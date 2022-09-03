/*
    OpenAPI 2022 Georgi Angelov
    Dynamically linking statically compiled applications
    PLT ( Procedure Linkage Table ) & GOT ( Global Offset Table ) imitation
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "hal.h"
#include "hal_rtc_internal.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"

#include "syslog.h"
#include "OpenAPI-shared.h"

int Printf(const char *f, ...) // private case
{
    va_list a;
    va_start(a, f);
    return vprintf(f, a);
}
#define PRINT Printf

static int api_print(const char *f, va_list a) // private debug, shared to app
{
    return vprintf(f, a);
}

// Application space: edit
#define APP_ROM 0x08292000
#define APP_MAX 0x00032000
#define APP_RAM 0x001E7000

typedef struct // APP_ROM Header
{
    uint32_t magic;       // APP_MAG
    uint32_t api_version; // API_VERSION
    uint32_t app_entry;   // FreeRTOS app entry

    uint16_t app_stack;    // FreeRTOS app stack - optional
    uint16_t app_priority; // FreeRTOS app priority - optional

    uint32_t api_load;
    uint32_t api_start;
    uint32_t api_end;

    uint32_t data_load;
    uint32_t data_start;
    uint32_t data_end;

    uint32_t bss_start;
    uint32_t bss_end;
} app_rom_t;

typedef struct /*PLT*/
{
    uint32_t veneer; /* LDR.W PC, =(function) */
    union
    {
        uint32_t hash; // table must be sorted by hash & binary-search for fast
        uint32_t func;
    };
} api_veneer_t;

typedef struct /*GOT*/
{
    uint32_t hash;
    uint32_t func;
} api_table_t;

#define API_VERSION 0x100
#define APP_MAGIC 0xCAFECAFE /* mean: Application exist ?? */

#define API_CODEER 0xFEEDC0DE /* hide veneer code */
#define API_VENEER 0xF000F85F /* LDR.W PC, =(function) for thumb func+1 */

static const api_table_t API_TABLE[] = {
#include "OPEN-API-C.h"
};

#define API_COUNT sizeof(API_TABLE) / sizeof(api_table_t)

// API_TABLE must be sorted by Hash
// https://gist.github.com/sgsfak/9ba382a0049f6ee885f68621ae86079b
// https://www.programiz.com/dsa/binary-search
static uint32_t getFunctionByHash(uint32_t hash)
{
    int mid, low = 0, high = API_COUNT - 1;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        if (API_TABLE[mid].hash == hash)
            return API_TABLE[mid].func;
        if (API_TABLE[mid].hash < hash)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0;
}

TaskHandle_t app_handle = NULL; // optional

int run_application(void)
{
    PRINT("[A] Starting OpenAPI Application ( v1.0.0 )\n");

    int counter = 0;
    volatile app_rom_t *rom = (app_rom_t *)APP_ROM;

    // check magic, api version, app entry
    if (rom->magic != APP_MAGIC && rom->api_version != API_VERSION)
    {
        PRINT("[ERROR] Application not exist\n");
        return -1;
    }
    if (rom->app_entry < APP_ROM || rom->app_entry > APP_ROM + APP_MAX)
    {
        PRINT("[ERROR] Application Wrong Entry\n");
        return -2;
    }
    if (rom->app_stack < 1024 || rom->app_stack > 8096 || rom->app_priority != TASK_PRIORITY_NORMAL) // optional
    {
        PRINT("[ERROR] Application Wrong Params\n");
        return -3;
    }

    /* copy the API segment into ram and pach it - mandatory */
    uint32_t *src = rom->api_load;  // ROM
    uint32_t *dst = rom->api_start; // RAM
    if (src != dst)
    {
        while ((uint32_t)dst < rom->api_end)
        {
            if (0 == *src) // zero must exist eof, gcc remove section .api ?!
            {
                PRINT("[A] API EOF: Application use %d functions\n", counter);
                break;
            }
            if (API_CODEER == *src)
            {
                *(dst++) = API_VENEER;
                src++;
                uint32_t function = getFunctionByHash(*src);
                if (function)
                {
                    *(dst++) = function;
                    src++;
                    counter++;
                }
                else
                {
                    PRINT("[ERROR] API FUNCTION NOT EXIST ( HASH: %08X )\n", *src);
                    return -10;
                }
            }
            else
            {
                PRINT("[ERROR] API CODEER\n");
                return -11;
            }
        }
    }

    /* copy the app DATA segment into ram - optional */
    src = rom->data_load;
    dst = rom->data_start;
    if (src != dst)
        while ((uint32_t)dst < rom->data_end)
            *(dst++) = *(src++);

    /* clear app BSS - optional */
    dst = rom->bss_start;
    if (dst)
        while ((uint32_t)dst < rom->bss_end)
            *(dst++) = 0;

    PRINT("[A] Creating Application Task and Run\n");
    if (pdPASS == xTaskCreate((TaskFunction_t)rom->app_entry, "APPLICATION", (uint16_t)rom->app_stack, NULL, rom->app_priority, &app_handle))
    {
        return 0; // done
    }
    else
    {
        PRINT("[ERROR] Failed to create Application task\n");
    }
    return -100;
}
