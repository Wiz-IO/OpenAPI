/*
    OpenAPI 2022 Georgi Angelov
    Dynamically linking statically compiled applications
    PLT ( Procedure Linkage Table ) & GOT ( Global Offset Table ) imitation
*/

#include "OpenAPI-core.h"
#include "arduino-main.h"
#include "arduino-wiring.h"
#include "arduino-periphery.h"

int Printf(const char *f, ...)
{
    va_list a;
    va_start(a, f);
    return vprintf(f, a);
}

void Printh(const char *txt, const char *buf, uint8_t size)
{
    if (buf && size)
    {
        if (txt)
        {
            Printf("%s ", txt);
        }
        for (int i = 0; i < size; i++)
        {
            Printf("%02X ", (int)buf[i] & 0xFF);
        }
        Printf("\n");
    }
}

static int api_print(const char *f, va_list a) // private debug, shared to app
{
    return vprintf(f, a);
}

#define PRINTF(...) Printf(__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////

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

bool IS_OPEN_API_ENABLED = true;
bool IS_OPEN_API_READY = false; // for external test

int api_run_application(void)
{
    if (false == IS_OPEN_API_ENABLED)
        return -1;

    PRINTF("[A] Starting OpenAPI Application ( v1.0.0 )\n");

    int counter = 0;
    volatile app_rom_t *rom = (app_rom_t *)APP_ROM;

    // check magic, api version, app entry
    if (rom->magic != APP_MAGIC && rom->api_version != API_VERSION)
    {
        PRINTF("[ERROR] Application not exist\n");
        return -1;
    }
    if (rom->app_entry < APP_ROM || rom->app_entry > APP_ROM + APP_MAX)
    {
        PRINTF("[ERROR] Application Wrong Entry\n");
        return -2;
    }

    uint16_t priority = TASK_PRIORITY_NORMAL; // rom->app_priority;
    uint16_t stack = rom->app_stack;
#if 1
    if (stack < 1024)
        stack = 1024;
    if (stack > 8192)
        stack = 8192;
#else
    if (stack < 1024 || stack > 8192 || rom->app_priority != TASK_PRIORITY_NORMAL) // optional
    {
        PRINTF("[ERROR] Application Wrong Params\n");
        return -3;
    }
#endif

    /* copy the API segment into ram and pach it - mandatory */
    uint32_t function;
    uint32_t *src = rom->api_load;  // ROM
    uint32_t *dst = rom->api_start; // RAM
    if (src != dst)
    {
        while ((uint32_t)dst < rom->api_end)
        {
            if (0 == *src) // zero must exist - eof, gcc remove section .api ?!
            {
                PRINTF("[A] API EOF: Application use %d functions\n", counter);
                break;
            }
            if (API_CODEER == *src)
            {
                *(dst++) = API_VENEER;
                src++;
                if ((function = getFunctionByHash(*src)))
                {
                    *(dst++) = function;
                    src++;
                    counter++;
                }
                else
                {
                    PRINTF("[ERROR] API FUNCTION NOT EXIST ( HASH: %08X )\n", *src);
                    return -10;
                }
            }
            else
            {
                PRINTF("[ERROR] API CODEER\n");
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

    if (pdPASS == xTaskCreate((TaskFunction_t)rom->app_entry, "APPLICATION", (uint16_t)stack, NULL, priority, &app_handle))
    {
        IS_OPEN_API_READY = true;
        PRINTF("[A] Application DONE\n");
        return 0; // done
    }
    else
    {
        PRINTF("[ERROR] Failed to create Application task\n");
    }
    return -100;
}
