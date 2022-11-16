/*
    OpenAPI 2022 Georgi Angelov
    Position Independent Code
*/

#include "FreeRTOS.h"
#include "task.h"
#include "OpenAPI-core.h"

// shared platform
#include "../arduino/arduino.h"

// DEBUG //////////////////////////////////////////////////////////////////////

#define PRINTF(...) kprintf(__VA_ARGS__)

void kprinth(const char *txt, const char *buf, uint8_t size)
{
    if (buf && size)
    {
        if (txt)
        {
            kprintf("%s ", txt);
        }
        for (int i = 0; i < size; i++)
        {
            kprintf("%02X ", (int)buf[i] & 0xFF);
        }
        kprintf("\n");
    }
}

int kprintf(const char *f, ...)
{
    va_list a;
    va_start(a, f);
    int res = vprintf(f, a); // change kernel log
    va_end(a);
    return res;
}

int api_vprintf(const char *frm, va_list list) // private debug, shared to app
{
    // PRINTF("F: %p, L: %p\n", frm, list);
    // #define APP_ROM 0x08292000
    // #define APP_RAM 0x001E7000
    if ((uint32_t)frm > APP_ROM) // F: 0x8292223, L: 0x1000B15C
    {
        return vprintf(frm, list); // change kernel log
    }
    return -1;
}

// API TABLE //////////////////////////////////////////////////////////////////

int api_variable = 42u;               // test PiC variable
const int api_constant = 0x11223300u; // test PiC constant

static const struct
{
    const char *function_name;
    const void *function_address;
} API_TABLE[] = {
#include "OpenAPI-shared.h"
};

static void *api_get_function_by_name(const char *name)
{
    if (name)
        for (int i = 0; i < (sizeof(API_TABLE) / sizeof(API_TABLE[0])); i++)
            if (0 == strcmp(name, API_TABLE[i].function_name))
                return (void *)API_TABLE[i].function_address;
    return NULL;
}

// APPLICATION LOADER /////////////////////////////////////////////////////////

TaskHandle_t app_handle = NULL; // optional, external use

static int api_check_app(app_rom_t *rom)
{
    PRINTF("[API] Starting OpenAPI Application ( ver %08X )\n", API_VERSION);
    if (APP_MAGIC != rom->magic) // check? || rom->api_version != API_VERSION
    {
        PRINTF("[ERROR][API] Application not exist ( ver %08X )\n", rom->magic);
        return -1;
    }
    if (rom->app_entry < APP_ROM || rom->app_entry > APP_ROM + APP_MAX)
    {
        PRINTF("[ERROR][API] Application Wrong Entry\n");
        return -2;
    }
    return 0;
}

static int api_load_app(app_rom_t *rom)
{
    uint32_t *src = rom->data_load;
    uint32_t *dst = rom->data_start;
    if (src != dst)
        while ((uint32_t)dst < rom->data_end)
            *(dst++) = *(src++); /* copy app .data segment into ram */

    dst = rom->bss_start;
    if (dst)
        while ((uint32_t)dst < rom->bss_end)
            *(dst++) = 0; /* clear app .bss */

    return 0;
}

static int api_relocate_app(app_rom_t *rom)
{
    int counter = 0;
    uint32_t index, max;
    void *function_address;
    Elf32_Rel *REL = rom->rel_start;
    Elf32_Sym *DYN = rom->dyn_start;
    const char *STR = rom->str_start;
    if ((uint32_t)REL != (uint32_t)DYN && STR)
    {
        PRINTF("[API] Library: %s\n", (char *)(STR + 1)); // "libopenapi.a"
        max = ((uint32_t)DYN - (uint32_t)REL) / sizeof(Elf32_Rel);
        for (; (uint32_t)REL < (uint32_t)DYN; REL++)
        {
            index = (REL->r_info >> 8); // 1, 2, 3 ...
            if ((0 == index) || (index - 1 > max))
            {
                PRINTF("[ERROR][API] REL INDEX: %u\n", index);
                return -10;
            }
            function_address = api_get_function_by_name((const char *)(STR + DYN[index].st_name));
            if (NULL == function_address)
            {
                PRINTF("[ERROR][API] EXPORT: '%s' NOT EXIST\n", (const char *)(STR + DYN[index].st_name));
                return -11;
            }
            else
            {
                PRINTF("[API] Relocate[%02d] %s\n", index, (const char *)(STR + DYN[index].st_name));
            }
            switch (REL->r_info & 0xFF)
            {
            case R_ARM_GLOB_DAT:                               // variables
            case R_ARM_JUMP_SLOT:                              // functions
                *(uint32_t *)REL->r_offset = function_address; // replace address
                break;
            default:
                PRINTF("[ERROR][API] REL TYPE: %d\n", REL->r_info & 0xFF);
                return -12;
            }
            counter++;
        } // switch
    }
    PRINTF("[API] Use %d Exports\n", counter);
    return 0;
}

int api_run_application(bool enabled)
{
    int res = -100;
    app_rom_t *rom = (app_rom_t *)APP_ROM;
    if (enabled)
    {
        if ((res = api_check_app(rom)))
            return res;
        if ((res = api_load_app(rom)))
            return res;
        if ((res = api_relocate_app(rom)))
            return res;
        uint32_t stack = rom->app_stack;
        stack = (stack < 1024) ? 1024 : stack;
        stack = (stack > 8192) ? 8192 : stack;
        if (pdPASS == xTaskCreate((TaskFunction_t)rom->app_entry, "APPLICATION", stack, NULL, TASK_PRIORITY_NORMAL, &app_handle))
        {
            PRINTF("[API] Application Ready\n");
            res = 0; // done
        }
        else
        {
            PRINTF("[ERROR][API] Failed to create Application task\n");
        }
    }
    return res;
}
