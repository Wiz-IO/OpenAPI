/*
    OpenAPI 2022 Georgi Angelov
    Position Independent Code
*/

#ifndef __OPEN_API_CORE_H__
#define __OPEN_API_CORE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// Application space: EDIT
#define APP_ROM 0x08292000
#define APP_MAX 0x00032000
#define APP_RAM 0x001E7000

#define API_VERSION 0x12345678 /* your version*/
#define APP_MAGIC 0xFECAFECA   /* mean: Application exist */

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    uint32_t st_name;       // offset to string table
    uint32_t st_value;      // 0x00
    uint32_t st_size;       // 0x00
    unsigned char st_info;  // 0x12
    unsigned char st_other; // 0x00
    uint16_t st_shndx;      // 0x00
} Elf32_Sym;

typedef struct
{
    uint32_t r_offset; // R_ARM_JUMP_SLOT Address
    uint32_t r_info;   // Symbol table index and type of relocation to apply
} Elf32_Rel;

#define R_ARM_ABS32 2
#define R_ARM_GLOB_DAT 21
#define R_ARM_JUMP_SLOT 0x16

typedef struct // BIN Header
{
    uint32_t magic;       // APP_MAG
    uint32_t api_version; // API_VERSION
    uint32_t app_entry;   // FreeRTOS app entry
    uint32_t app_stack;   // FreeRTOS app stack

    uint32_t data_load; // section .data
    uint32_t data_start;
    uint32_t data_end;

    uint32_t bss_start; // section .bss
    uint32_t bss_end;

    Elf32_Rel *rel_start; // ELF info
    Elf32_Sym *dyn_start;
    char *str_start;

    uint32_t v[4]; // align header to 64 bytes
} app_rom_t;

extern TaskHandle_t app_handle;
int api_run_application(bool enabled);

void kprinth(const char *txt, const char *buf, uint8_t size);
int kprintf(const char *f, ...);
int api_vprintf(const char *, va_list);

#endif // __OPEN_API_CORE_H__