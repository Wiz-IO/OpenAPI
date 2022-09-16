/*
    OpenAPI 2022 Georgi Angelov
*/

#ifndef __OPEN_API_CORE_H__
#define __OPEN_API_CORE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

// Application space: edit
#define APP_ROM 0x08292000
#define APP_MAX 0x00032000
#define APP_RAM 0x001E7000

#define API_VERSION 0x100     /* your version*/
#define APP_MAGIC 0xCAFECAFE  /* mean: Application exist */
#define API_CODEER 0xFEEDC0DE /* hide veneer code */
#define API_VENEER 0xF000F85F /* LDR.W PC, =(func) for thumb func+1 */

typedef struct // BIN Header
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
    uint32_t veneer; /* LDR.W PC, =(func) */
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

extern bool IS_OPEN_API_ENABLED;
extern bool IS_OPEN_API_READY;

int api_run_application(void);

int Printf(const char *f, ...);
void Printh(const char *txt, const char *buf, uint8_t size);

#endif // __OPEN_API_CORE_H__