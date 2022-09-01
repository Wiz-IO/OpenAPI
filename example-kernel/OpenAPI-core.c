/*
    OpenAPI 2022 Georgi Angelov
    PLT ( Procedure Linkage Table ) и GOT ( Global Offset Table ) imitation for static linked userware applications

    Userware cost: RAM(executable): 8 bytes & ROM: 8 bytes - per function

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
#include "OpenAPI-common.h"

int Printf(const char *f, ...)
{
    va_list a;
    va_start(a, f);
    return vprintf(f, a);
}
#define PRINT Printf

static int api_print(const char *f, va_list a) // private api debug
{
    return vprintf(f, a);
}

// Application space: edit
#define APP_ROM 0x08292000
#define APP_MAX 0x00032000
#define APP_RAM 0x001E7000

typedef struct
{
    uint32_t magic;       // 0xCAFECAFE
    uint32_t api_version; // 0x100
    uint32_t app_stack;   // FreeRTOS app stack
    uint32_t app_entry;   // FreeRTOS app entry

    uint32_t api_load;
    uint32_t api_start;
    uint32_t api_end;

    uint32_t data_load;
    uint32_t data_start;
    uint32_t data_end;

    uint32_t bss_start;
    uint32_t bss_end;
} app_rom_t;

typedef struct
{
    uint32_t veneer; /* LDR.W PC, =(function) */
    union
    {
        uint32_t hash; // table must be sorted by hash & binary-search for fast
        uint32_t func;
    };
} api_veneer_t;

typedef struct
{
    uint32_t hash;
    uint32_t func;
} api_table_t;

#define APP_MAG 0xCAFECAFE /* mean: Application exist ?? */
#define API_EOF 0xDEADDEAD
#define API_VERSION 0x100

#define API_CODEER 0xFEEDC0DE /* do not show veneer code */
#define API_VENEER 0xF000F85F /* LDR.W PC, =(function) for thumb func+1 */

static const api_table_t API_TABLE[] = {
#include "OPEN-API-C.h"
};

#define API_COUNT sizeof(API_TABLE) / sizeof(api_table_t)

// API_TABLE must be sorted by hash
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

    int res = -1;
    volatile app_rom_t *rom = (app_rom_t *)APP_ROM;

    // check magic, api version, app entry & stack
    PRINT("[A] APP: magic = 0x%08X, version = 0x%X\n", rom->magic, rom->api_version);
    if (rom->magic != APP_MAG)
    {
        PRINT("[ERROR] Application not exist\n");
        res = -1;
        goto END;
    }
    if (rom->api_version != API_VERSION)
    {
        PRINT("[ERROR] OpenAPI Version\n");
        res = -2;
        goto END;
    }
    PRINT("[A] APP: entry = 0x%08X, stack request = %u\n", rom->app_entry, rom->app_stack);
    if (rom->app_stack < 1024 || rom->app_stack > 8096) // stack limit
    {
        PRINT("[ERROR] Application Wrong Stack Size: %u\n", rom->app_stack);
        res = -3;
        goto END;
    }
    if (-1 == rom->app_entry || 0 == rom->app_entry || rom->app_entry < APP_ROM || rom->app_entry > APP_ROM + APP_MAX)
    {
        PRINT("[ERROR] Application Wrong Entry\n");
        res = -4;
        goto END;
    }

    // PRINT("[A] API: 0x%08X, 0x%08X, 0x%08X\n", rom->api_load, rom->api_start, rom->api_end);
    // PRINT("[A] DAT: 0x%08X, 0x%08X, 0x%08X\n", rom->data_load, rom->data_start, rom->data_end);
    // PRINT("[A] BSS: 0x%08X, 0x%08X\n", rom->bss_start, rom->bss_end);

    /* copy the API segment into ram - mandatory */
    uint32_t *src = rom->api_load;
    uint32_t *dst = rom->api_start;
    if (src != dst)
        while ((uint32_t)dst < rom->api_end)
            *(dst++) = *(src++);

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

    // patch app venners
    int counter = 0;
    volatile api_veneer_t *api = (api_veneer_t *)APP_RAM;
    for (int i = 0; i < API_COUNT + 1; i++)
    {
        if (API_EOF == api->veneer || API_EOF == api->hash)
        {
            PRINT("[A] API EOF: APP use %d functions\n", counter);
            break;
        }

        // PRINT("[A]    line: %08X, %08X\n", api->veneer, api->hash);
        if (API_CODEER == api->veneer)
        {
            uint32_t func = getFunctionByHash(api->hash);
            if (func)
            {

                api->veneer = API_VENEER; // patch: LDR.W PC, =(function)
                api->func = func;         // patch real kernel function
                // PRINT("[A]    func: %08X, %08X\n", api->veneer, api->func);
                counter++;
                api++; // next
            }
            else
            {
                PRINT("[ERROR] OpenAPI Function not exist: %u\n", api->hash);
                res = -5;
                goto END;
            }
        }
        else
        {
            PRINT("[ERROR] OpenAPI Wrong Code\n");
            res = -6;
            goto END;
        }
    }
    PRINT("[A] Creating Application Task and Run\n");
    if (pdPASS == xTaskCreate((TaskFunction_t)rom->app_entry, "APPLICATION", (uint16_t)rom->app_stack, NULL, TASK_PRIORITY_NORMAL, &app_handle))
    {
        res = 0; // done
    }
    else
    {
        PRINT("[ERROR] Failed to create Application task\n");
    }
END:
    return res;
}
