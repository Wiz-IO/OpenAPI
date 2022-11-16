/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"

#include "hal.h"
#include "syslog.h"
#include "memory_attribute.h"
#include "bsp_gpio_ept_config.h"
 
#ifdef MTK_NVDM_ENABLE
#include "nvdm.h"
#endif

#ifdef MTK_NVDM_MODEM_ENABLE
#include "nvdm_modem.h"
#endif

#ifdef MTK_NB_MODEM_ENABLE
extern bool N1RfShouldSetPinmux(void);
#endif

#ifdef MTK_SYSTEM_CLOCK_SET
static const uint32_t target_freq = MTK_SYSTEM_CLOCK_SET;
#else
/* target system frequency selection */
#ifdef MTK_SYSTEM_CLOCK_156M
static const uint32_t target_freq = 156000;
#elif defined(MTK_SYSTEM_CLOCK_78M)
static const uint32_t target_freq = 78000;
#elif defined(MTK_SYSTEM_CLOCK_26M)
static const uint32_t target_freq = 26000;
#else
static const uint32_t target_freq = 104000;
#endif
#endif

#ifdef HAL_SLEEP_MANAGER_ENABLED
uint8_t sys_lock_handle = 0xFF;
#endif

/* Private functions -----------------------------------------------------------*/
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
/* Enable port service feature */
#ifdef MTK_PORT_SERVICE_ENABLE
    log_write((char*)&ch, 1);
    return ch;
#else
    /* Place your implementation of fputc here */
    /* e.g. write a character to the HAL_UART_0 one at a time */
    hal_uart_put_char(HAL_UART_0, ch);
    return ch;
#endif
}

#ifndef MTK_DEBUG_LEVEL_NONE

log_create_module(main, PRINT_LEVEL_ERROR);

LOG_CONTROL_BLOCK_DECLARE(main);
LOG_CONTROL_BLOCK_DECLARE(common);


log_control_block_t *syslog_control_blocks[] = {
    &LOG_CONTROL_BLOCK_SYMBOL(main),
    &LOG_CONTROL_BLOCK_SYMBOL(common),
    NULL
};
#endif

static void SystemClock_Config(void)
{
#ifndef FPGA_ENV
    hal_clock_init();//for FPGA need mask this line
#endif
}

static void cache_init(void)
{
    hal_cache_region_t region, region_number;

    /* Max region number is 16 */
    hal_cache_region_config_t region_cfg_tbl[] = {
        /* cacheable address, cacheable size(both MUST be 4k bytes aligned) */
        /* Flash memory */
        {RTOS_BASE, RTOS_LENGTH},
        /* virtual memory for AP */
        {VRAM_BASE, VRAM_LENGTH + VRAM_MD_LENGTH},
        /* RTC SRAM */
        {RETSRAM_BASE, RETSRAM_LENGTH + RETSRAM_MD_LENGTH}
    };

    region_number = (hal_cache_region_t)(sizeof(region_cfg_tbl) / sizeof(region_cfg_tbl[0]));

    hal_cache_init();
    hal_cache_set_size(HAL_CACHE_SIZE_32KB);
    for (region = HAL_CACHE_REGION_0; region < region_number; region++) {
        hal_cache_region_config(region, &region_cfg_tbl[region]);
        hal_cache_region_enable(region);
    }
    for (; region < HAL_CACHE_REGION_MAX; region++) {
        hal_cache_region_disable(region);
    }
    hal_cache_enable();
}

/**
* @brief       caculate actual bit value of region size.
* @param[in]   region_size: actual region size.
* @return      corresponding bit value of region size for MPU setting.
*/
static uint32_t caculate_mpu_region_size(uint32_t region_size)
{
    uint32_t count;

    if (region_size < 32) {
        return 0;
    }
    for (count = 0; ((region_size  & 0x80000000) == 0); count++, region_size  <<= 1);
    return 30 - count;
}

/**
* @brief       This function is to initialize MPU.
* @param[in]   None.
* @return      None.
*/
static void mpu_init(void)
{
    hal_mpu_region_t region, region_number;
    hal_mpu_region_config_t region_config;
    typedef struct {
        uint32_t mpu_region_base_address;/**< MPU region start address */
        uint32_t mpu_region_end_address;/**< MPU region end address */
        hal_mpu_access_permission_t mpu_region_access_permission; /**< MPU region access permission */
        uint8_t mpu_subregion_mask; /**< MPU sub region mask*/
        bool mpu_xn;/**< XN attribute of MPU, if set TRUE, execution of an instruction fetched from the corresponding region is not permitted */
    } mpu_region_information_t;

    //VRAM: CODE+RO DATA for AP
    extern uint32_t Image$$RAM_TEXT$$Base;
    extern uint32_t Image$$RAM_TEXT$$Limit;

    //VRAM: CODE+RO DATA for AP
    extern uint32_t Image$$MD_CACHED_RAM_TEXT$$Base;
    extern uint32_t Image$$MD_CACHED_RAM_TEXT$$Limit;

    //TCM: CODE+RO DATA for AP
    extern uint32_t Image$$TCM$$RO$$Base;
    extern uint32_t Image$$TCM$$RO$$Limit;

    //TCM: CODE+RO DATA for MD
    extern uint32_t Image$$MD_TCM$$RO$$Base;
    extern uint32_t Image$$MD_TCM$$RO$$Limit;

    //STACK END
    extern unsigned int Image$$STACK$$ZI$$Base;

    /* MAX region number is 8 */
    mpu_region_information_t region_information[] = {
        /* mpu_region_start_address, mpu_region_end_address, mpu_region_access_permission, mpu_subregion_mask, mpu_xn */
        {(uint32_t) &Image$$RAM_TEXT$$Base, (uint32_t) &Image$$RAM_TEXT$$Limit, HAL_MPU_READONLY, 0x0, FALSE}, //Virtual memory for AP
        {(uint32_t) &Image$$RAM_TEXT$$Base - VRAM_BASE, (uint32_t) &Image$$RAM_TEXT$$Limit - VRAM_BASE, HAL_MPU_NO_ACCESS, 0x0, TRUE}, //RAM code+RAM rodata for AP
        {(uint32_t) &Image$$MD_CACHED_RAM_TEXT$$Base, (uint32_t) &Image$$MD_CACHED_RAM_TEXT$$Limit, HAL_MPU_READONLY, 0x0, FALSE}, //Virtual memory for MD
        {(uint32_t) &Image$$MD_CACHED_RAM_TEXT$$Base - VRAM_BASE, (uint32_t) &Image$$MD_CACHED_RAM_TEXT$$Limit - VRAM_BASE, HAL_MPU_NO_ACCESS, 0x0, TRUE}, //RAM code+RAM rodata for MD

        {(uint32_t) &Image$$TCM$$RO$$Base, (uint32_t) &Image$$TCM$$RO$$Limit, HAL_MPU_READONLY, 0x0, FALSE},//TCM code+TCM rodata for MD
        {(uint32_t) &Image$$MD_TCM$$RO$$Base, (uint32_t) &Image$$MD_TCM$$RO$$Limit, HAL_MPU_READONLY, 0x0, FALSE},//MD TCM code + MD TCM rodata
        {(uint32_t) &Image$$STACK$$ZI$$Base, (uint32_t) &Image$$STACK$$ZI$$Base + 32, HAL_MPU_READONLY, 0x0, TRUE}, //Stack end check for stack overflow
    };

    hal_mpu_config_t mpu_config = {
        /* PRIVDEFENA, HFNMIENA */
        TRUE, TRUE
    };

    region_number = (hal_mpu_region_t)(sizeof(region_information) / sizeof(region_information[0]));

    hal_mpu_init(&mpu_config);
    for (region = HAL_MPU_REGION_0; region < region_number; region++) {
        /* Updata region information to be configured */
        region_config.mpu_region_address = region_information[region].mpu_region_base_address;
        region_config.mpu_region_size = (hal_mpu_region_size_t) caculate_mpu_region_size(region_information[region].mpu_region_end_address - region_information[region].mpu_region_base_address);
        region_config.mpu_region_access_permission = region_information[region].mpu_region_access_permission;
        region_config.mpu_subregion_mask = region_information[region].mpu_subregion_mask;
        region_config.mpu_xn = region_information[region].mpu_xn;

        if (hal_mpu_region_configure(region, &region_config) != HAL_MPU_STATUS_OK) {
            hal_mpu_region_disable(region);
        } else {
            hal_mpu_region_enable(region);
        }
    }
    /* make sure unused regions are disabled */
    for (; region < HAL_MPU_REGION_MAX; region++) {
        hal_mpu_region_disable(region);
    }
    hal_mpu_enable();
}

static void prvSetupHardware(void)
{
/* sleep manager init*/
#ifdef HAL_SLEEP_MANAGER_ENABLED
    hal_sleep_manager_init();
#endif

    hal_dvfs_init();
    hal_dvfs_target_cpu_frequency(target_freq, HAL_DVFS_FREQ_RELATION_L);

    /* System HW initialization */

    cache_init();
#ifndef FPGA_ENV
    mpu_init();
#endif
    /* Peripherals initialization */
    log_uart_init(HAL_UART_0);

#ifdef HAL_FLASH_MODULE_ENABLED
    hal_flash_init();
#endif
    hal_nvic_init();

    /* Board HW initialization */
}

/**
* @brief       This function is to do system initialization, eg: system clock, hardware and logging port.
* @param[in]   None.
* @return      None.
*/
void system_init(void)
{
    /* Configure system clock */
    SystemClock_Config();

    SystemCoreClockUpdate();

    /* Configure the hardware */
    prvSetupHardware();

    /*ept init*/
    bsp_ept_gpio_setting_init();

#ifdef MTK_NB_MODEM_ENABLE
    /* Apply to MT2625 E2 EVB pinmux */
    if (true == N1RfShouldSetPinmux()) {
        /* Set GPIO_4 KP_ROW1 */
        hal_pinmux_set_function(HAL_GPIO_4, 4);
        /* Set GPIO_32 UART2_TXD */
        hal_pinmux_set_function(HAL_GPIO_32, 3);
        /* Disbale pull */
        hal_gpio_disable_pull(HAL_GPIO_32);
    }
#endif

#ifndef MTK_DEBUG_LEVEL_NONE
    log_init(NULL, NULL, syslog_control_blocks);
#endif

    /* Enable NVDM feature */
#ifdef MTK_NVDM_ENABLE
    nvdm_init();
#endif

#ifdef MTK_NVDM_MODEM_ENABLE
    /*modem nvdm init */
    nvdm_modem_init();
#endif

    hal_dvfs_target_cpu_frequency(26000, HAL_DVFS_FREQ_RELATION_L);
    hal_dcxo_load_calibration();
    hal_dvfs_target_cpu_frequency(target_freq, HAL_DVFS_FREQ_RELATION_L);

    ////LOG_I(common, "System Initialize DONE @ %d.\r\n", hal_dvfs_get_cpu_frequency());
}


