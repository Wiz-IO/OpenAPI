////////////////////////////////////////////////////////////////////////////////////////
//
//      2022 Georgi Angelov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

////////////////////////////////////////////////////////////////////////////
// INIT CPP
////////////////////////////////////////////////////////////////////////////

extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void) __attribute__((weak));
extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));
extern void _init(void) __attribute__((weak));

void __libc_init_array(void)
{
    size_t count;
    size_t i;
    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
        __preinit_array_start[i]();

    _init();

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i]();
}

#if 0
extern void (*__fini_array_start[])(void) __attribute__((weak));
extern void (*__fini_array_end[])(void) __attribute__((weak));
extern void _fini(void) __attribute__((weak));

void __libc_fini_array(void)
{
    size_t count;
    size_t i;
    count = __fini_array_end - __fini_array_start;
    for (i = count; i > 0; i--)
        __fini_array_start[i - 1]();
    _fini();
}
#endif

////////////////////////////////////////////////////////////////////////////
// MEMORY
////////////////////////////////////////////////////////////////////////////

void *_sbrk(int incr) { return (void *)-1; }

extern void *pvPortMalloc(size_t xWantedSize);
extern void vPortFree(void *pv);
extern void *pvPortCalloc(size_t nmemb, size_t size);
extern void *pvPortRealloc(void *pv, size_t size);

void *malloc(size_t size)
{
    return pvPortMalloc(size);
}
void *_malloc_r(struct _reent *ignore, size_t size) { return malloc(size); }

void free(void *p)
{
    if (p)
        vPortFree(p);
}
void _free_r(struct _reent *ignore, void *ptr) { free(ptr); }

void *realloc(void *pv, size_t size)
{
    return pvPortRealloc(pv, size);
}
void *_realloc_r(struct _reent *ignored, void *ptr, size_t size) { return realloc(ptr, size); }

void *calloc(size_t nmemb, size_t size)
{
    return pvPortCalloc(nmemb, size);
}
void *_calloc_r(struct _reent *ignored, size_t element, size_t size) { return calloc(element, size); }
