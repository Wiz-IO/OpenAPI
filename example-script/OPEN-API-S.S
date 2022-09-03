/*
   OpenAPI [ 03.09.2022 ] Georgi Angelov
   Version: 1.0.0
   Functions: 206
*/

.file "OPEN-API-S.S"
.syntax unified
.cpu cortex-m4
.thumb


#ifndef API_CODEER
#define API_CODEER 0xFEEDC0DE
#endif


.globl api_print
.section .api_ram_code.api_print
.type  api_print, %function
.func  api_print
api_print:
    .long API_CODEER
    .long 0x87A46DF6
.endfunc

.globl doReset
.section .api_ram_code.doReset
.type  doReset, %function
.func  doReset
doReset:
    .long API_CODEER
    .long 0xB0D9BA0E
.endfunc

.globl getBattery
.section .api_ram_code.getBattery
.type  getBattery, %function
.func  getBattery
getBattery:
    .long API_CODEER
    .long 0xD2361D70
.endfunc

.globl getIMEI
.section .api_ram_code.getIMEI
.type  getIMEI, %function
.func  getIMEI
getIMEI:
    .long API_CODEER
    .long 0x73C5D0F6
.endfunc

.globl yield
.section .api_ram_code.yield
.type  yield, %function
.func  yield
yield:
    .long API_CODEER
    .long 0x6A323B8A
.endfunc

.globl millis
.section .api_ram_code.millis
.type  millis, %function
.func  millis
millis:
    .long API_CODEER
    .long 0xAAC4FB6A
.endfunc

.globl micros
.section .api_ram_code.micros
.type  micros, %function
.func  micros
micros:
    .long API_CODEER
    .long 0x1C76F7B6
.endfunc

.globl delay
.section .api_ram_code.delay
.type  delay, %function
.func  delay
delay:
    .long API_CODEER
    .long 0x1F55D5A2
.endfunc

.globl delayMicroseconds
.section .api_ram_code.delayMicroseconds
.type  delayMicroseconds, %function
.func  delayMicroseconds
delayMicroseconds:
    .long API_CODEER
    .long 0x6A50FC40
.endfunc

.globl delay_ms
.section .api_ram_code.delay_ms
.type  delay_ms, %function
.func  delay_ms
delay_ms:
    .long API_CODEER
    .long 0xF6E09148
.endfunc

.globl delay_us
.section .api_ram_code.delay_us
.type  delay_us, %function
.func  delay_us
delay_us:
    .long API_CODEER
    .long 0xC575B380
.endfunc

.globl pinMode
.section .api_ram_code.pinMode
.type  pinMode, %function
.func  pinMode
pinMode:
    .long API_CODEER
    .long 0x6A973CA4
.endfunc

.globl digitalWrite
.section .api_ram_code.digitalWrite
.type  digitalWrite, %function
.func  digitalWrite
digitalWrite:
    .long API_CODEER
    .long 0xC9AE709C
.endfunc

.globl digitalRead
.section .api_ram_code.digitalRead
.type  digitalRead, %function
.func  digitalRead
digitalRead:
    .long API_CODEER
    .long 0x73501778
.endfunc

.globl analogRead
.section .api_ram_code.analogRead
.type  analogRead, %function
.func  analogRead
analogRead:
    .long API_CODEER
    .long 0x10A9DD60
.endfunc

.globl analogWrite
.section .api_ram_code.analogWrite
.type  analogWrite, %function
.func  analogWrite
analogWrite:
    .long API_CODEER
    .long 0x20F099D4
.endfunc

.globl attachInterrupt
.section .api_ram_code.attachInterrupt
.type  attachInterrupt, %function
.func  attachInterrupt
attachInterrupt:
    .long API_CODEER
    .long 0xEE10FDDC
.endfunc

.globl detachInterrupt
.section .api_ram_code.detachInterrupt
.type  detachInterrupt, %function
.func  detachInterrupt
detachInterrupt:
    .long API_CODEER
    .long 0x6A6C52D0
.endfunc

.globl uart_open
.section .api_ram_code.uart_open
.type  uart_open, %function
.func  uart_open
uart_open:
    .long API_CODEER
    .long 0xD631CF1E
.endfunc

.globl uart_close
.section .api_ram_code.uart_close
.type  uart_close, %function
.func  uart_close
uart_close:
    .long API_CODEER
    .long 0x403B17B4
.endfunc

.globl uart_write
.section .api_ram_code.uart_write
.type  uart_write, %function
.func  uart_write
uart_write:
    .long API_CODEER
    .long 0xD39D974C
.endfunc

.globl uart_read
.section .api_ram_code.uart_read
.type  uart_read, %function
.func  uart_read
uart_read:
    .long API_CODEER
    .long 0x738E8708
.endfunc

.globl uart_peek
.section .api_ram_code.uart_peek
.type  uart_peek, %function
.func  uart_peek
uart_peek:
    .long API_CODEER
    .long 0x5E25265E
.endfunc

.globl uart_available
.section .api_ram_code.uart_available
.type  uart_available, %function
.func  uart_available
uart_available:
    .long API_CODEER
    .long 0x9EF2D668
.endfunc

.globl uart_flush
.section .api_ram_code.uart_flush
.type  uart_flush, %function
.func  uart_flush
uart_flush:
    .long API_CODEER
    .long 0xB0F7D6BA
.endfunc

.globl pvPortMalloc
.section .api_ram_code.pvPortMalloc
.type  pvPortMalloc, %function
.func  pvPortMalloc
pvPortMalloc:
    .long API_CODEER
    .long 0x3CB9DF78
.endfunc

.globl vPortFree
.section .api_ram_code.vPortFree
.type  vPortFree, %function
.func  vPortFree
vPortFree:
    .long API_CODEER
    .long 0x2399B890
.endfunc

.globl pvPortCalloc
.section .api_ram_code.pvPortCalloc
.type  pvPortCalloc, %function
.func  pvPortCalloc
pvPortCalloc:
    .long API_CODEER
    .long 0x660DC30E
.endfunc

.globl pvPortRealloc
.section .api_ram_code.pvPortRealloc
.type  pvPortRealloc, %function
.func  pvPortRealloc
pvPortRealloc:
    .long API_CODEER
    .long 0xB5AE065A
.endfunc

.globl vPortExitCritical
.section .api_ram_code.vPortExitCritical
.type  vPortExitCritical, %function
.func  vPortExitCritical
vPortExitCritical:
    .long API_CODEER
    .long 0x76837E08
.endfunc

.globl vPortRaiseBASEPRI
.section .api_ram_code.vPortRaiseBASEPRI
.type  vPortRaiseBASEPRI, %function
.func  vPortRaiseBASEPRI
vPortRaiseBASEPRI:
    .long API_CODEER
    .long 0x57E1E79A
.endfunc

.globl vPortEnterCritical
.section .api_ram_code.vPortEnterCritical
.type  vPortEnterCritical, %function
.func  vPortEnterCritical
vPortEnterCritical:
    .long API_CODEER
    .long 0x79999B38
.endfunc

.globl ulPortRaiseBASEPRI
.section .api_ram_code.ulPortRaiseBASEPRI
.type  ulPortRaiseBASEPRI, %function
.func  ulPortRaiseBASEPRI
ulPortRaiseBASEPRI:
    .long API_CODEER
    .long 0xE9AB64CA
.endfunc

.globl xTaskGenericCreate
.section .api_ram_code.xTaskGenericCreate
.type  xTaskGenericCreate, %function
.func  xTaskGenericCreate
xTaskGenericCreate:
    .long API_CODEER
    .long 0x81D828BC
.endfunc

.globl vTaskDelete
.section .api_ram_code.vTaskDelete
.type  vTaskDelete, %function
.func  vTaskDelete
vTaskDelete:
    .long API_CODEER
    .long 0xE635C6E2
.endfunc

.globl uxTaskPriorityGet
.section .api_ram_code.uxTaskPriorityGet
.type  uxTaskPriorityGet, %function
.func  uxTaskPriorityGet
uxTaskPriorityGet:
    .long API_CODEER
    .long 0x922C2128
.endfunc

.globl uxTaskPriorityGetFromISR
.section .api_ram_code.uxTaskPriorityGetFromISR
.type  uxTaskPriorityGetFromISR, %function
.func  uxTaskPriorityGetFromISR
uxTaskPriorityGetFromISR:
    .long API_CODEER
    .long 0x51312B9A
.endfunc

.globl vTaskPrioritySet
.section .api_ram_code.vTaskPrioritySet
.type  vTaskPrioritySet, %function
.func  vTaskPrioritySet
vTaskPrioritySet:
    .long API_CODEER
    .long 0x3B35C8B8
.endfunc

.globl vTaskResume
.section .api_ram_code.vTaskResume
.type  vTaskResume, %function
.func  vTaskResume
vTaskResume:
    .long API_CODEER
    .long 0x5671A872
.endfunc

.globl xTaskResumeFromISR
.section .api_ram_code.xTaskResumeFromISR
.type  xTaskResumeFromISR, %function
.func  xTaskResumeFromISR
xTaskResumeFromISR:
    .long API_CODEER
    .long 0x3F5BC636
.endfunc

.globl xTaskGetTickCount
.section .api_ram_code.xTaskGetTickCount
.type  xTaskGetTickCount, %function
.func  xTaskGetTickCount
xTaskGetTickCount:
    .long API_CODEER
    .long 0xF31844CE
.endfunc

.globl xTaskGetTickCountFromISR
.section .api_ram_code.xTaskGetTickCountFromISR
.type  xTaskGetTickCountFromISR, %function
.func  xTaskGetTickCountFromISR
xTaskGetTickCountFromISR:
    .long API_CODEER
    .long 0x26EDA558
.endfunc

.globl vTaskDelayUntil
.section .api_ram_code.vTaskDelayUntil
.type  vTaskDelayUntil, %function
.func  vTaskDelayUntil
vTaskDelayUntil:
    .long API_CODEER
    .long 0xF3D05B14
.endfunc

.globl vTaskDelay
.section .api_ram_code.vTaskDelay
.type  vTaskDelay, %function
.func  vTaskDelay
vTaskDelay:
    .long API_CODEER
    .long 0xBDCDC95E
.endfunc

.globl vTaskSuspend
.section .api_ram_code.vTaskSuspend
.type  vTaskSuspend, %function
.func  vTaskSuspend
vTaskSuspend:
    .long API_CODEER
    .long 0x3EEB0DDA
.endfunc

.globl xTaskGetCurrentTaskHandle
.section .api_ram_code.xTaskGetCurrentTaskHandle
.type  xTaskGetCurrentTaskHandle, %function
.func  xTaskGetCurrentTaskHandle
xTaskGetCurrentTaskHandle:
    .long API_CODEER
    .long 0x92B7755E
.endfunc

.globl ulTaskNotifyTake
.section .api_ram_code.ulTaskNotifyTake
.type  ulTaskNotifyTake, %function
.func  ulTaskNotifyTake
ulTaskNotifyTake:
    .long API_CODEER
    .long 0x83AECCC4
.endfunc

.globl xTaskNotifyWait
.section .api_ram_code.xTaskNotifyWait
.type  xTaskNotifyWait, %function
.func  xTaskNotifyWait
xTaskNotifyWait:
    .long API_CODEER
    .long 0x79E2F126
.endfunc

.globl xTaskNotify
.section .api_ram_code.xTaskNotify
.type  xTaskNotify, %function
.func  xTaskNotify
xTaskNotify:
    .long API_CODEER
    .long 0xA3CC6396
.endfunc

.globl xTaskNotifyFromISR
.section .api_ram_code.xTaskNotifyFromISR
.type  xTaskNotifyFromISR, %function
.func  xTaskNotifyFromISR
xTaskNotifyFromISR:
    .long API_CODEER
    .long 0xCC374560
.endfunc

.globl vTaskNotifyGiveFromISR
.section .api_ram_code.vTaskNotifyGiveFromISR
.type  vTaskNotifyGiveFromISR, %function
.func  vTaskNotifyGiveFromISR
vTaskNotifyGiveFromISR:
    .long API_CODEER
    .long 0x728C8DD4
.endfunc

.globl xTimerCreate
.section .api_ram_code.xTimerCreate
.type  xTimerCreate, %function
.func  xTimerCreate
xTimerCreate:
    .long API_CODEER
    .long 0x3B00BDB0
.endfunc

.globl xTimerGenericCommand
.section .api_ram_code.xTimerGenericCommand
.type  xTimerGenericCommand, %function
.func  xTimerGenericCommand
xTimerGenericCommand:
    .long API_CODEER
    .long 0x51DDB5AE
.endfunc

.globl xTimerIsTimerActive
.section .api_ram_code.xTimerIsTimerActive
.type  xTimerIsTimerActive, %function
.func  xTimerIsTimerActive
xTimerIsTimerActive:
    .long API_CODEER
    .long 0x6BE2089A
.endfunc

.globl pvTimerGetTimerID
.section .api_ram_code.pvTimerGetTimerID
.type  pvTimerGetTimerID, %function
.func  pvTimerGetTimerID
pvTimerGetTimerID:
    .long API_CODEER
    .long 0xE2972E2A
.endfunc

.globl xEventGroupCreate
.section .api_ram_code.xEventGroupCreate
.type  xEventGroupCreate, %function
.func  xEventGroupCreate
xEventGroupCreate:
    .long API_CODEER
    .long 0x4062019C
.endfunc

.globl xEventGroupWaitBits
.section .api_ram_code.xEventGroupWaitBits
.type  xEventGroupWaitBits, %function
.func  xEventGroupWaitBits
xEventGroupWaitBits:
    .long API_CODEER
    .long 0x61166970
.endfunc

.globl xEventGroupClearBits
.section .api_ram_code.xEventGroupClearBits
.type  xEventGroupClearBits, %function
.func  xEventGroupClearBits
xEventGroupClearBits:
    .long API_CODEER
    .long 0x4BC05422
.endfunc

.globl xEventGroupClearBitsFromISR
.section .api_ram_code.xEventGroupClearBitsFromISR
.type  xEventGroupClearBitsFromISR, %function
.func  xEventGroupClearBitsFromISR
xEventGroupClearBitsFromISR:
    .long API_CODEER
    .long 0x89BBA0AC
.endfunc

.globl xEventGroupGetBitsFromISR
.section .api_ram_code.xEventGroupGetBitsFromISR
.type  xEventGroupGetBitsFromISR, %function
.func  xEventGroupGetBitsFromISR
xEventGroupGetBitsFromISR:
    .long API_CODEER
    .long 0xAA23D64C
.endfunc

.globl xEventGroupSetBits
.section .api_ram_code.xEventGroupSetBits
.type  xEventGroupSetBits, %function
.func  xEventGroupSetBits
xEventGroupSetBits:
    .long API_CODEER
    .long 0x6432C156
.endfunc

.globl xEventGroupSync
.section .api_ram_code.xEventGroupSync
.type  xEventGroupSync, %function
.func  xEventGroupSync
xEventGroupSync:
    .long API_CODEER
    .long 0xA1C038EC
.endfunc

.globl vEventGroupDelete
.section .api_ram_code.vEventGroupDelete
.type  vEventGroupDelete, %function
.func  vEventGroupDelete
vEventGroupDelete:
    .long API_CODEER
    .long 0x1A8E8C7C
.endfunc

.globl xEventGroupSetBitsFromISR
.section .api_ram_code.xEventGroupSetBitsFromISR
.type  xEventGroupSetBitsFromISR, %function
.func  xEventGroupSetBitsFromISR
xEventGroupSetBitsFromISR:
    .long API_CODEER
    .long 0xA4C98A20
.endfunc

.globl xQueueGenericReset
.section .api_ram_code.xQueueGenericReset
.type  xQueueGenericReset, %function
.func  xQueueGenericReset
xQueueGenericReset:
    .long API_CODEER
    .long 0xC560E65E
.endfunc

.globl xQueueGenericCreate
.section .api_ram_code.xQueueGenericCreate
.type  xQueueGenericCreate, %function
.func  xQueueGenericCreate
xQueueGenericCreate:
    .long API_CODEER
    .long 0x477098B8
.endfunc

.globl xQueueCreateCountingSemaphore
.section .api_ram_code.xQueueCreateCountingSemaphore
.type  xQueueCreateCountingSemaphore, %function
.func  xQueueCreateCountingSemaphore
xQueueCreateCountingSemaphore:
    .long API_CODEER
    .long 0x3BA49D34
.endfunc

.globl xQueueGenericSend
.section .api_ram_code.xQueueGenericSend
.type  xQueueGenericSend, %function
.func  xQueueGenericSend
xQueueGenericSend:
    .long API_CODEER
    .long 0xB3C28844
.endfunc

.globl xQueueCreateMutex
.section .api_ram_code.xQueueCreateMutex
.type  xQueueCreateMutex, %function
.func  xQueueCreateMutex
xQueueCreateMutex:
    .long API_CODEER
    .long 0x76F047F4
.endfunc

.globl xQueueGiveMutexRecursive
.section .api_ram_code.xQueueGiveMutexRecursive
.type  xQueueGiveMutexRecursive, %function
.func  xQueueGiveMutexRecursive
xQueueGiveMutexRecursive:
    .long API_CODEER
    .long 0xC2A52E80
.endfunc

.globl xQueueGenericSendFromISR
.section .api_ram_code.xQueueGenericSendFromISR
.type  xQueueGenericSendFromISR, %function
.func  xQueueGenericSendFromISR
xQueueGenericSendFromISR:
    .long API_CODEER
    .long 0x348B17BE
.endfunc

.globl xQueueGiveFromISR
.section .api_ram_code.xQueueGiveFromISR
.type  xQueueGiveFromISR, %function
.func  xQueueGiveFromISR
xQueueGiveFromISR:
    .long API_CODEER
    .long 0x2327E12A
.endfunc

.globl xQueueGenericReceive
.section .api_ram_code.xQueueGenericReceive
.type  xQueueGenericReceive, %function
.func  xQueueGenericReceive
xQueueGenericReceive:
    .long API_CODEER
    .long 0x2CFEA550
.endfunc

.globl xQueueTakeMutexRecursive
.section .api_ram_code.xQueueTakeMutexRecursive
.type  xQueueTakeMutexRecursive, %function
.func  xQueueTakeMutexRecursive
xQueueTakeMutexRecursive:
    .long API_CODEER
    .long 0x4F80CAA6
.endfunc

.globl xQueueReceiveFromISR
.section .api_ram_code.xQueueReceiveFromISR
.type  xQueueReceiveFromISR, %function
.func  xQueueReceiveFromISR
xQueueReceiveFromISR:
    .long API_CODEER
    .long 0xB7B0FFB0
.endfunc

.globl xQueuePeekFromISR
.section .api_ram_code.xQueuePeekFromISR
.type  xQueuePeekFromISR, %function
.func  xQueuePeekFromISR
xQueuePeekFromISR:
    .long API_CODEER
    .long 0x9CBE3C78
.endfunc

.globl uxQueueMessagesWaiting
.section .api_ram_code.uxQueueMessagesWaiting
.type  uxQueueMessagesWaiting, %function
.func  uxQueueMessagesWaiting
uxQueueMessagesWaiting:
    .long API_CODEER
    .long 0xC8F138B8
.endfunc

.globl uxQueueSpacesAvailable
.section .api_ram_code.uxQueueSpacesAvailable
.type  uxQueueSpacesAvailable, %function
.func  uxQueueSpacesAvailable
uxQueueSpacesAvailable:
    .long API_CODEER
    .long 0xB91493D6
.endfunc

.globl uxQueueMessagesWaitingFromISR
.section .api_ram_code.uxQueueMessagesWaitingFromISR
.type  uxQueueMessagesWaitingFromISR, %function
.func  uxQueueMessagesWaitingFromISR
uxQueueMessagesWaitingFromISR:
    .long API_CODEER
    .long 0x52BBD42A
.endfunc

.globl ucQueueGetQueueType
.section .api_ram_code.ucQueueGetQueueType
.type  ucQueueGetQueueType, %function
.func  ucQueueGetQueueType
ucQueueGetQueueType:
    .long API_CODEER
    .long 0x8A2217C4
.endfunc

.globl xQueueIsQueueEmptyFromISR
.section .api_ram_code.xQueueIsQueueEmptyFromISR
.type  xQueueIsQueueEmptyFromISR, %function
.func  xQueueIsQueueEmptyFromISR
xQueueIsQueueEmptyFromISR:
    .long API_CODEER
    .long 0x044C1A6A
.endfunc

.globl xQueueIsQueueFullFromISR
.section .api_ram_code.xQueueIsQueueFullFromISR
.type  xQueueIsQueueFullFromISR, %function
.func  xQueueIsQueueFullFromISR
xQueueIsQueueFullFromISR:
    .long API_CODEER
    .long 0xA0CDF1A4
.endfunc

.globl vQueueUnregisterQueue
.section .api_ram_code.vQueueUnregisterQueue
.type  vQueueUnregisterQueue, %function
.func  vQueueUnregisterQueue
vQueueUnregisterQueue:
    .long API_CODEER
    .long 0xD63BC4BC
.endfunc

.globl vQueueDelete
.section .api_ram_code.vQueueDelete
.type  vQueueDelete, %function
.func  vQueueDelete
vQueueDelete:
    .long API_CODEER
    .long 0x689D7036
.endfunc

.globl vQueueWaitForMessageRestricted
.section .api_ram_code.vQueueWaitForMessageRestricted
.type  vQueueWaitForMessageRestricted, %function
.func  vQueueWaitForMessageRestricted
vQueueWaitForMessageRestricted:
    .long API_CODEER
    .long 0x86613A4A
.endfunc

.globl xQueueCreateSet
.section .api_ram_code.xQueueCreateSet
.type  xQueueCreateSet, %function
.func  xQueueCreateSet
xQueueCreateSet:
    .long API_CODEER
    .long 0x2A780D3C
.endfunc

.globl xQueueAddToSet
.section .api_ram_code.xQueueAddToSet
.type  xQueueAddToSet, %function
.func  xQueueAddToSet
xQueueAddToSet:
    .long API_CODEER
    .long 0xA79C5DBE
.endfunc

.globl xQueueRemoveFromSet
.section .api_ram_code.xQueueRemoveFromSet
.type  xQueueRemoveFromSet, %function
.func  xQueueRemoveFromSet
xQueueRemoveFromSet:
    .long API_CODEER
    .long 0xAE1696F0
.endfunc

.globl xQueueSelectFromSet
.section .api_ram_code.xQueueSelectFromSet
.type  xQueueSelectFromSet, %function
.func  xQueueSelectFromSet
xQueueSelectFromSet:
    .long API_CODEER
    .long 0xD0B4F182
.endfunc

.globl xQueueSelectFromSetFromISR
.section .api_ram_code.xQueueSelectFromSetFromISR
.type  xQueueSelectFromSetFromISR, %function
.func  xQueueSelectFromSetFromISR
xQueueSelectFromSetFromISR:
    .long API_CODEER
    .long 0x46BBDB4C
.endfunc

.globl hal_wdt_init
.section .api_ram_code.hal_wdt_init
.type  hal_wdt_init, %function
.func  hal_wdt_init
hal_wdt_init:
    .long API_CODEER
    .long 0x5EFEDC84
.endfunc

.globl hal_wdt_deinit
.section .api_ram_code.hal_wdt_deinit
.type  hal_wdt_deinit, %function
.func  hal_wdt_deinit
hal_wdt_deinit:
    .long API_CODEER
    .long 0x9B15DCF6
.endfunc

.globl hal_wdt_register_callback
.section .api_ram_code.hal_wdt_register_callback
.type  hal_wdt_register_callback, %function
.func  hal_wdt_register_callback
hal_wdt_register_callback:
    .long API_CODEER
    .long 0x0B797F4E
.endfunc

.globl hal_wdt_feed
.section .api_ram_code.hal_wdt_feed
.type  hal_wdt_feed, %function
.func  hal_wdt_feed
hal_wdt_feed:
    .long API_CODEER
    .long 0x909ADE88
.endfunc

.globl hal_wdt_software_reset
.section .api_ram_code.hal_wdt_software_reset
.type  hal_wdt_software_reset, %function
.func  hal_wdt_software_reset
hal_wdt_software_reset:
    .long API_CODEER
    .long 0x241418E8
.endfunc

.globl hal_wdt_enable
.section .api_ram_code.hal_wdt_enable
.type  hal_wdt_enable, %function
.func  hal_wdt_enable
hal_wdt_enable:
    .long API_CODEER
    .long 0xEBC01B48
.endfunc

.globl hal_wdt_disable
.section .api_ram_code.hal_wdt_disable
.type  hal_wdt_disable, %function
.func  hal_wdt_disable
hal_wdt_disable:
    .long API_CODEER
    .long 0x232263BE
.endfunc

.globl hal_wdt_get_reset_status
.section .api_ram_code.hal_wdt_get_reset_status
.type  hal_wdt_get_reset_status, %function
.func  hal_wdt_get_reset_status
hal_wdt_get_reset_status:
    .long API_CODEER
    .long 0x5374D686
.endfunc

.globl hal_wdt_get_enable_status
.section .api_ram_code.hal_wdt_get_enable_status
.type  hal_wdt_get_enable_status, %function
.func  hal_wdt_get_enable_status
hal_wdt_get_enable_status:
    .long API_CODEER
    .long 0x97C4CEDA
.endfunc

.globl hal_wdt_get_mode
.section .api_ram_code.hal_wdt_get_mode
.type  hal_wdt_get_mode, %function
.func  hal_wdt_get_mode
hal_wdt_get_mode:
    .long API_CODEER
    .long 0xE8502C5C
.endfunc

.globl hal_nvic_enable_irq
.section .api_ram_code.hal_nvic_enable_irq
.type  hal_nvic_enable_irq, %function
.func  hal_nvic_enable_irq
hal_nvic_enable_irq:
    .long API_CODEER
    .long 0x68FCD728
.endfunc

.globl hal_nvic_disable_irq
.section .api_ram_code.hal_nvic_disable_irq
.type  hal_nvic_disable_irq, %function
.func  hal_nvic_disable_irq
hal_nvic_disable_irq:
    .long API_CODEER
    .long 0x193AF526
.endfunc

.globl hal_nvic_set_priority
.section .api_ram_code.hal_nvic_set_priority
.type  hal_nvic_set_priority, %function
.func  hal_nvic_set_priority
hal_nvic_set_priority:
    .long API_CODEER
    .long 0x0AACA598
.endfunc

.globl hal_nvic_get_priority
.section .api_ram_code.hal_nvic_get_priority
.type  hal_nvic_get_priority, %function
.func  hal_nvic_get_priority
hal_nvic_get_priority:
    .long API_CODEER
    .long 0x87EC7BCC
.endfunc

.globl hal_nvic_register_isr_handler
.section .api_ram_code.hal_nvic_register_isr_handler
.type  hal_nvic_register_isr_handler, %function
.func  hal_nvic_register_isr_handler
hal_nvic_register_isr_handler:
    .long API_CODEER
    .long 0xDBDEC93A
.endfunc

.globl hal_nvic_save_and_set_interrupt_mask
.section .api_ram_code.hal_nvic_save_and_set_interrupt_mask
.type  hal_nvic_save_and_set_interrupt_mask, %function
.func  hal_nvic_save_and_set_interrupt_mask
hal_nvic_save_and_set_interrupt_mask:
    .long API_CODEER
    .long 0xA92B937C
.endfunc

.globl hal_nvic_restore_interrupt_mask
.section .api_ram_code.hal_nvic_restore_interrupt_mask
.type  hal_nvic_restore_interrupt_mask, %function
.func  hal_nvic_restore_interrupt_mask
hal_nvic_restore_interrupt_mask:
    .long API_CODEER
    .long 0x77C0B514
.endfunc

.globl hal_gpio_init
.section .api_ram_code.hal_gpio_init
.type  hal_gpio_init, %function
.func  hal_gpio_init
hal_gpio_init:
    .long API_CODEER
    .long 0x9EBF2E12
.endfunc

.globl hal_gpio_deinit
.section .api_ram_code.hal_gpio_deinit
.type  hal_gpio_deinit, %function
.func  hal_gpio_deinit
hal_gpio_deinit:
    .long API_CODEER
    .long 0x3BD54BC8
.endfunc

.globl hal_gpio_set_direction
.section .api_ram_code.hal_gpio_set_direction
.type  hal_gpio_set_direction, %function
.func  hal_gpio_set_direction
hal_gpio_set_direction:
    .long API_CODEER
    .long 0xC3854538
.endfunc

.globl hal_gpio_get_direction
.section .api_ram_code.hal_gpio_get_direction
.type  hal_gpio_get_direction, %function
.func  hal_gpio_get_direction
hal_gpio_get_direction:
    .long API_CODEER
    .long 0x4F27C85C
.endfunc

.globl hal_pinmux_set_function
.section .api_ram_code.hal_pinmux_set_function
.type  hal_pinmux_set_function, %function
.func  hal_pinmux_set_function
hal_pinmux_set_function:
    .long API_CODEER
    .long 0x102099FA
.endfunc

.globl hal_gpio_get_input
.section .api_ram_code.hal_gpio_get_input
.type  hal_gpio_get_input, %function
.func  hal_gpio_get_input
hal_gpio_get_input:
    .long API_CODEER
    .long 0xC3429468
.endfunc

.globl hal_gpio_set_output
.section .api_ram_code.hal_gpio_set_output
.type  hal_gpio_set_output, %function
.func  hal_gpio_set_output
hal_gpio_set_output:
    .long API_CODEER
    .long 0x7B3219BE
.endfunc

.globl hal_gpio_get_output
.section .api_ram_code.hal_gpio_get_output
.type  hal_gpio_get_output, %function
.func  hal_gpio_get_output
hal_gpio_get_output:
    .long API_CODEER
    .long 0xEC58E8F2
.endfunc

.globl hal_gpio_toggle_pin
.section .api_ram_code.hal_gpio_toggle_pin
.type  hal_gpio_toggle_pin, %function
.func  hal_gpio_toggle_pin
hal_gpio_toggle_pin:
    .long API_CODEER
    .long 0xBE537F7A
.endfunc

.globl hal_gpio_set_clockout
.section .api_ram_code.hal_gpio_set_clockout
.type  hal_gpio_set_clockout, %function
.func  hal_gpio_set_clockout
hal_gpio_set_clockout:
    .long API_CODEER
    .long 0x54FAB730
.endfunc

.globl hal_gpio_set_pupd_register
.section .api_ram_code.hal_gpio_set_pupd_register
.type  hal_gpio_set_pupd_register, %function
.func  hal_gpio_set_pupd_register
hal_gpio_set_pupd_register:
    .long API_CODEER
    .long 0xE47C2FBE
.endfunc

.globl hal_gpio_pull_up
.section .api_ram_code.hal_gpio_pull_up
.type  hal_gpio_pull_up, %function
.func  hal_gpio_pull_up
hal_gpio_pull_up:
    .long API_CODEER
    .long 0x9E0BC438
.endfunc

.globl hal_gpio_pull_down
.section .api_ram_code.hal_gpio_pull_down
.type  hal_gpio_pull_down, %function
.func  hal_gpio_pull_down
hal_gpio_pull_down:
    .long API_CODEER
    .long 0x4D9AFC6C
.endfunc

.globl hal_gpio_disable_pull
.section .api_ram_code.hal_gpio_disable_pull
.type  hal_gpio_disable_pull, %function
.func  hal_gpio_disable_pull
hal_gpio_disable_pull:
    .long API_CODEER
    .long 0x66325C9E
.endfunc

.globl hal_gpio_set_high_impedance
.section .api_ram_code.hal_gpio_set_high_impedance
.type  hal_gpio_set_high_impedance, %function
.func  hal_gpio_set_high_impedance
hal_gpio_set_high_impedance:
    .long API_CODEER
    .long 0x7995189C
.endfunc

.globl hal_gpio_clear_high_impedance
.section .api_ram_code.hal_gpio_clear_high_impedance
.type  hal_gpio_clear_high_impedance, %function
.func  hal_gpio_clear_high_impedance
hal_gpio_clear_high_impedance:
    .long API_CODEER
    .long 0x5B8BA918
.endfunc

.globl hal_gpio_set_driving_current
.section .api_ram_code.hal_gpio_set_driving_current
.type  hal_gpio_set_driving_current, %function
.func  hal_gpio_set_driving_current
hal_gpio_set_driving_current:
    .long API_CODEER
    .long 0xA1B7EF22
.endfunc

.globl hal_gpio_get_driving_current
.section .api_ram_code.hal_gpio_get_driving_current
.type  hal_gpio_get_driving_current, %function
.func  hal_gpio_get_driving_current
hal_gpio_get_driving_current:
    .long API_CODEER
    .long 0x2AF30706
.endfunc

.globl hal_eint_register_callback
.section .api_ram_code.hal_eint_register_callback
.type  hal_eint_register_callback, %function
.func  hal_eint_register_callback
hal_eint_register_callback:
    .long API_CODEER
    .long 0xF1172120
.endfunc

.globl hal_eint_set_trigger_mode
.section .api_ram_code.hal_eint_set_trigger_mode
.type  hal_eint_set_trigger_mode, %function
.func  hal_eint_set_trigger_mode
hal_eint_set_trigger_mode:
    .long API_CODEER
    .long 0x8242E81E
.endfunc

.globl hal_eint_set_debounce_count
.section .api_ram_code.hal_eint_set_debounce_count
.type  hal_eint_set_debounce_count, %function
.func  hal_eint_set_debounce_count
hal_eint_set_debounce_count:
    .long API_CODEER
    .long 0x9BCD50BC
.endfunc

.globl hal_eint_set_debounce_time
.section .api_ram_code.hal_eint_set_debounce_time
.type  hal_eint_set_debounce_time, %function
.func  hal_eint_set_debounce_time
hal_eint_set_debounce_time:
    .long API_CODEER
    .long 0x2EFDBAB6
.endfunc

.globl hal_eint_init
.section .api_ram_code.hal_eint_init
.type  hal_eint_init, %function
.func  hal_eint_init
hal_eint_init:
    .long API_CODEER
    .long 0x8FEF06EA
.endfunc

.globl hal_eint_set_software_trigger
.section .api_ram_code.hal_eint_set_software_trigger
.type  hal_eint_set_software_trigger, %function
.func  hal_eint_set_software_trigger
hal_eint_set_software_trigger:
    .long API_CODEER
    .long 0x7E8E7CD2
.endfunc

.globl hal_eint_clear_software_trigger
.section .api_ram_code.hal_eint_clear_software_trigger
.type  hal_eint_clear_software_trigger, %function
.func  hal_eint_clear_software_trigger
hal_eint_clear_software_trigger:
    .long API_CODEER
    .long 0x22F0F4FE
.endfunc

.globl hal_eint_mask
.section .api_ram_code.hal_eint_mask
.type  hal_eint_mask, %function
.func  hal_eint_mask
hal_eint_mask:
    .long API_CODEER
    .long 0x56DDEC40
.endfunc

.globl hal_eint_deinit
.section .api_ram_code.hal_eint_deinit
.type  hal_eint_deinit, %function
.func  hal_eint_deinit
hal_eint_deinit:
    .long API_CODEER
    .long 0x3D20D370
.endfunc

.globl hal_eint_unmask
.section .api_ram_code.hal_eint_unmask
.type  hal_eint_unmask, %function
.func  hal_eint_unmask
hal_eint_unmask:
    .long API_CODEER
    .long 0x1A107C70
.endfunc

.globl hal_uart_set_baudrate
.section .api_ram_code.hal_uart_set_baudrate
.type  hal_uart_set_baudrate, %function
.func  hal_uart_set_baudrate
hal_uart_set_baudrate:
    .long API_CODEER
    .long 0x7BC3A7E2
.endfunc

.globl hal_uart_set_format
.section .api_ram_code.hal_uart_set_format
.type  hal_uart_set_format, %function
.func  hal_uart_set_format
hal_uart_set_format:
    .long API_CODEER
    .long 0x88D64E0E
.endfunc

.globl hal_uart_init
.section .api_ram_code.hal_uart_init
.type  hal_uart_init, %function
.func  hal_uart_init
hal_uart_init:
    .long API_CODEER
    .long 0xF13A676E
.endfunc

.globl hal_uart_deinit
.section .api_ram_code.hal_uart_deinit
.type  hal_uart_deinit, %function
.func  hal_uart_deinit
hal_uart_deinit:
    .long API_CODEER
    .long 0xD79D4D2C
.endfunc

.globl hal_uart_put_char
.section .api_ram_code.hal_uart_put_char
.type  hal_uart_put_char, %function
.func  hal_uart_put_char
hal_uart_put_char:
    .long API_CODEER
    .long 0x09C4D436
.endfunc

.globl hal_uart_send_polling
.section .api_ram_code.hal_uart_send_polling
.type  hal_uart_send_polling, %function
.func  hal_uart_send_polling
hal_uart_send_polling:
    .long API_CODEER
    .long 0x25B87BB4
.endfunc

.globl hal_uart_send_dma
.section .api_ram_code.hal_uart_send_dma
.type  hal_uart_send_dma, %function
.func  hal_uart_send_dma
hal_uart_send_dma:
    .long API_CODEER
    .long 0xD5BC6390
.endfunc

.globl hal_uart_get_char
.section .api_ram_code.hal_uart_get_char
.type  hal_uart_get_char, %function
.func  hal_uart_get_char
hal_uart_get_char:
    .long API_CODEER
    .long 0x321BBEC6
.endfunc

.globl hal_uart_get_char_unblocking
.section .api_ram_code.hal_uart_get_char_unblocking
.type  hal_uart_get_char_unblocking, %function
.func  hal_uart_get_char_unblocking
hal_uart_get_char_unblocking:
    .long API_CODEER
    .long 0x9BF66D58
.endfunc

.globl hal_uart_receive_polling
.section .api_ram_code.hal_uart_receive_polling
.type  hal_uart_receive_polling, %function
.func  hal_uart_receive_polling
hal_uart_receive_polling:
    .long API_CODEER
    .long 0x11297A30
.endfunc

.globl hal_uart_receive_dma
.section .api_ram_code.hal_uart_receive_dma
.type  hal_uart_receive_dma, %function
.func  hal_uart_receive_dma
hal_uart_receive_dma:
    .long API_CODEER
    .long 0xA9F76D44
.endfunc

.globl hal_uart_register_callback
.section .api_ram_code.hal_uart_register_callback
.type  hal_uart_register_callback, %function
.func  hal_uart_register_callback
hal_uart_register_callback:
    .long API_CODEER
    .long 0x0309F26C
.endfunc

.globl hal_uart_get_available_send_space
.section .api_ram_code.hal_uart_get_available_send_space
.type  hal_uart_get_available_send_space, %function
.func  hal_uart_get_available_send_space
hal_uart_get_available_send_space:
    .long API_CODEER
    .long 0xE627854C
.endfunc

.globl hal_uart_get_available_receive_bytes
.section .api_ram_code.hal_uart_get_available_receive_bytes
.type  hal_uart_get_available_receive_bytes, %function
.func  hal_uart_get_available_receive_bytes
hal_uart_get_available_receive_bytes:
    .long API_CODEER
    .long 0x2D28C852
.endfunc

.globl hal_uart_set_hardware_flowcontrol
.section .api_ram_code.hal_uart_set_hardware_flowcontrol
.type  hal_uart_set_hardware_flowcontrol, %function
.func  hal_uart_set_hardware_flowcontrol
hal_uart_set_hardware_flowcontrol:
    .long API_CODEER
    .long 0x7A820EB6
.endfunc

.globl hal_uart_set_software_flowcontrol
.section .api_ram_code.hal_uart_set_software_flowcontrol
.type  hal_uart_set_software_flowcontrol, %function
.func  hal_uart_set_software_flowcontrol
hal_uart_set_software_flowcontrol:
    .long API_CODEER
    .long 0x19F97098
.endfunc

.globl hal_uart_disable_flowcontrol
.section .api_ram_code.hal_uart_disable_flowcontrol
.type  hal_uart_disable_flowcontrol, %function
.func  hal_uart_disable_flowcontrol
hal_uart_disable_flowcontrol:
    .long API_CODEER
    .long 0x296B191A
.endfunc

.globl hal_uart_set_dma
.section .api_ram_code.hal_uart_set_dma
.type  hal_uart_set_dma, %function
.func  hal_uart_set_dma
hal_uart_set_dma:
    .long API_CODEER
    .long 0x815203CE
.endfunc

.globl hal_uart_set_dma_timeout
.section .api_ram_code.hal_uart_set_dma_timeout
.type  hal_uart_set_dma_timeout, %function
.func  hal_uart_set_dma_timeout
hal_uart_set_dma_timeout:
    .long API_CODEER
    .long 0xB0290D9E
.endfunc

.globl hal_uart_set_auto_baudrate
.section .api_ram_code.hal_uart_set_auto_baudrate
.type  hal_uart_set_auto_baudrate, %function
.func  hal_uart_set_auto_baudrate
hal_uart_set_auto_baudrate:
    .long API_CODEER
    .long 0x056B87CA
.endfunc

.globl hal_spi_master_init
.section .api_ram_code.hal_spi_master_init
.type  hal_spi_master_init, %function
.func  hal_spi_master_init
hal_spi_master_init:
    .long API_CODEER
    .long 0x90FE8CA6
.endfunc

.globl hal_spi_master_set_advanced_config
.section .api_ram_code.hal_spi_master_set_advanced_config
.type  hal_spi_master_set_advanced_config, %function
.func  hal_spi_master_set_advanced_config
hal_spi_master_set_advanced_config:
    .long API_CODEER
    .long 0x5D4ECCAE
.endfunc

.globl hal_spi_master_deinit
.section .api_ram_code.hal_spi_master_deinit
.type  hal_spi_master_deinit, %function
.func  hal_spi_master_deinit
hal_spi_master_deinit:
    .long API_CODEER
    .long 0xF5B74564
.endfunc

.globl hal_spi_master_send_polling
.section .api_ram_code.hal_spi_master_send_polling
.type  hal_spi_master_send_polling, %function
.func  hal_spi_master_send_polling
hal_spi_master_send_polling:
    .long API_CODEER
    .long 0x67FA431C
.endfunc

.globl hal_spi_master_send_and_receive_polling
.section .api_ram_code.hal_spi_master_send_and_receive_polling
.type  hal_spi_master_send_and_receive_polling, %function
.func  hal_spi_master_send_and_receive_polling
hal_spi_master_send_and_receive_polling:
    .long API_CODEER
    .long 0xEA497528
.endfunc

.globl hal_spi_master_register_callback
.section .api_ram_code.hal_spi_master_register_callback
.type  hal_spi_master_register_callback, %function
.func  hal_spi_master_register_callback
hal_spi_master_register_callback:
    .long API_CODEER
    .long 0xECD7B694
.endfunc

.globl hal_spi_master_send_dma
.section .api_ram_code.hal_spi_master_send_dma
.type  hal_spi_master_send_dma, %function
.func  hal_spi_master_send_dma
hal_spi_master_send_dma:
    .long API_CODEER
    .long 0xF3C2CD18
.endfunc

.globl hal_spi_master_send_dma_blocking
.section .api_ram_code.hal_spi_master_send_dma_blocking
.type  hal_spi_master_send_dma_blocking, %function
.func  hal_spi_master_send_dma_blocking
hal_spi_master_send_dma_blocking:
    .long API_CODEER
    .long 0xFD317F8E
.endfunc

.globl hal_spi_master_send_and_receive_dma
.section .api_ram_code.hal_spi_master_send_and_receive_dma
.type  hal_spi_master_send_and_receive_dma, %function
.func  hal_spi_master_send_and_receive_dma
hal_spi_master_send_and_receive_dma:
    .long API_CODEER
    .long 0x654CD56C
.endfunc

.globl hal_spi_master_send_and_receive_dma_blocking
.section .api_ram_code.hal_spi_master_send_and_receive_dma_blocking
.type  hal_spi_master_send_and_receive_dma_blocking, %function
.func  hal_spi_master_send_and_receive_dma_blocking
hal_spi_master_send_and_receive_dma_blocking:
    .long API_CODEER
    .long 0x8A51A3E2
.endfunc

.globl hal_spi_master_get_running_status
.section .api_ram_code.hal_spi_master_get_running_status
.type  hal_spi_master_get_running_status, %function
.func  hal_spi_master_get_running_status
hal_spi_master_get_running_status:
    .long API_CODEER
    .long 0xB4DF1D36
.endfunc

.globl hal_spi_master_set_chip_select_timing
.section .api_ram_code.hal_spi_master_set_chip_select_timing
.type  hal_spi_master_set_chip_select_timing, %function
.func  hal_spi_master_set_chip_select_timing
hal_spi_master_set_chip_select_timing:
    .long API_CODEER
    .long 0xC3ADDFE4
.endfunc

.globl hal_spi_master_set_deassert
.section .api_ram_code.hal_spi_master_set_deassert
.type  hal_spi_master_set_deassert, %function
.func  hal_spi_master_set_deassert
hal_spi_master_set_deassert:
    .long API_CODEER
    .long 0x11AADF32
.endfunc

.globl hal_spi_master_set_mode
.section .api_ram_code.hal_spi_master_set_mode
.type  hal_spi_master_set_mode, %function
.func  hal_spi_master_set_mode
hal_spi_master_set_mode:
    .long API_CODEER
    .long 0x1A54A49A
.endfunc

.globl hal_spi_master_set_dummy_bits
.section .api_ram_code.hal_spi_master_set_dummy_bits
.type  hal_spi_master_set_dummy_bits, %function
.func  hal_spi_master_set_dummy_bits
hal_spi_master_set_dummy_bits:
    .long API_CODEER
    .long 0xA4478E0E
.endfunc

.globl hal_spi_master_set_command_bytes
.section .api_ram_code.hal_spi_master_set_command_bytes
.type  hal_spi_master_set_command_bytes, %function
.func  hal_spi_master_set_command_bytes
hal_spi_master_set_command_bytes:
    .long API_CODEER
    .long 0xC1197F90
.endfunc

.globl hal_i2c_master_init
.section .api_ram_code.hal_i2c_master_init
.type  hal_i2c_master_init, %function
.func  hal_i2c_master_init
hal_i2c_master_init:
    .long API_CODEER
    .long 0xB5B81288
.endfunc

.globl hal_i2c_master_deinit
.section .api_ram_code.hal_i2c_master_deinit
.type  hal_i2c_master_deinit, %function
.func  hal_i2c_master_deinit
hal_i2c_master_deinit:
    .long API_CODEER
    .long 0x9DB9324A
.endfunc

.globl hal_i2c_master_set_frequency
.section .api_ram_code.hal_i2c_master_set_frequency
.type  hal_i2c_master_set_frequency, %function
.func  hal_i2c_master_set_frequency
hal_i2c_master_set_frequency:
    .long API_CODEER
    .long 0xD4B7E038
.endfunc

.globl hal_i2c_master_register_callback
.section .api_ram_code.hal_i2c_master_register_callback
.type  hal_i2c_master_register_callback, %function
.func  hal_i2c_master_register_callback
hal_i2c_master_register_callback:
    .long API_CODEER
    .long 0x1ACEAADA
.endfunc

.globl hal_i2c_master_send_polling
.section .api_ram_code.hal_i2c_master_send_polling
.type  hal_i2c_master_send_polling, %function
.func  hal_i2c_master_send_polling
hal_i2c_master_send_polling:
    .long API_CODEER
    .long 0xAB5BDCFE
.endfunc

.globl hal_i2c_master_send_dma
.section .api_ram_code.hal_i2c_master_send_dma
.type  hal_i2c_master_send_dma, %function
.func  hal_i2c_master_send_dma
hal_i2c_master_send_dma:
    .long API_CODEER
    .long 0x1246FE3E
.endfunc

.globl hal_i2c_master_receive_polling
.section .api_ram_code.hal_i2c_master_receive_polling
.type  hal_i2c_master_receive_polling, %function
.func  hal_i2c_master_receive_polling
hal_i2c_master_receive_polling:
    .long API_CODEER
    .long 0x5A954A9E
.endfunc

.globl hal_i2c_master_receive_dma
.section .api_ram_code.hal_i2c_master_receive_dma
.type  hal_i2c_master_receive_dma, %function
.func  hal_i2c_master_receive_dma
hal_i2c_master_receive_dma:
    .long API_CODEER
    .long 0x2CB0BFDE
.endfunc

.globl hal_i2c_master_send_to_receive_polling
.section .api_ram_code.hal_i2c_master_send_to_receive_polling
.type  hal_i2c_master_send_to_receive_polling, %function
.func  hal_i2c_master_send_to_receive_polling
hal_i2c_master_send_to_receive_polling:
    .long API_CODEER
    .long 0xF85B905E
.endfunc

.globl hal_i2c_master_send_to_receive_dma
.section .api_ram_code.hal_i2c_master_send_to_receive_dma
.type  hal_i2c_master_send_to_receive_dma, %function
.func  hal_i2c_master_send_to_receive_dma
hal_i2c_master_send_to_receive_dma:
    .long API_CODEER
    .long 0x9482319E
.endfunc

.globl hal_i2c_master_send_dma_ex
.section .api_ram_code.hal_i2c_master_send_dma_ex
.type  hal_i2c_master_send_dma_ex, %function
.func  hal_i2c_master_send_dma_ex
hal_i2c_master_send_dma_ex:
    .long API_CODEER
    .long 0x849D74E8
.endfunc

.globl hal_i2c_master_receive_dma_ex
.section .api_ram_code.hal_i2c_master_receive_dma_ex
.type  hal_i2c_master_receive_dma_ex, %function
.func  hal_i2c_master_receive_dma_ex
hal_i2c_master_receive_dma_ex:
    .long API_CODEER
    .long 0x6F0EDD88
.endfunc

.globl hal_i2c_master_send_to_receive_dma_ex
.section .api_ram_code.hal_i2c_master_send_to_receive_dma_ex
.type  hal_i2c_master_send_to_receive_dma_ex, %function
.func  hal_i2c_master_send_to_receive_dma_ex
hal_i2c_master_send_to_receive_dma_ex:
    .long API_CODEER
    .long 0x3FC1C2C8
.endfunc

.globl hal_i2c_master_get_running_status
.section .api_ram_code.hal_i2c_master_get_running_status
.type  hal_i2c_master_get_running_status, %function
.func  hal_i2c_master_get_running_status
hal_i2c_master_get_running_status:
    .long API_CODEER
    .long 0x6AAC6954
.endfunc

.globl hal_adc_init
.section .api_ram_code.hal_adc_init
.type  hal_adc_init, %function
.func  hal_adc_init
hal_adc_init:
    .long API_CODEER
    .long 0x9521A70E
.endfunc

.globl hal_adc_deinit
.section .api_ram_code.hal_adc_deinit
.type  hal_adc_deinit, %function
.func  hal_adc_deinit
hal_adc_deinit:
    .long API_CODEER
    .long 0x4E3AEB4C
.endfunc

.globl hal_adc_get_data_polling
.section .api_ram_code.hal_adc_get_data_polling
.type  hal_adc_get_data_polling, %function
.func  hal_adc_get_data_polling
hal_adc_get_data_polling:
    .long API_CODEER
    .long 0x8C63FDEE
.endfunc

.globl hal_pwm_init
.section .api_ram_code.hal_pwm_init
.type  hal_pwm_init, %function
.func  hal_pwm_init
hal_pwm_init:
    .long API_CODEER
    .long 0x0E00BB46
.endfunc

.globl hal_pwm_deinit
.section .api_ram_code.hal_pwm_deinit
.type  hal_pwm_deinit, %function
.func  hal_pwm_deinit
hal_pwm_deinit:
    .long API_CODEER
    .long 0xE0BDD184
.endfunc

.globl hal_pwm_set_frequency
.section .api_ram_code.hal_pwm_set_frequency
.type  hal_pwm_set_frequency, %function
.func  hal_pwm_set_frequency
hal_pwm_set_frequency:
    .long API_CODEER
    .long 0xC59D69EA
.endfunc

.globl hal_pwm_set_duty_cycle
.section .api_ram_code.hal_pwm_set_duty_cycle
.type  hal_pwm_set_duty_cycle, %function
.func  hal_pwm_set_duty_cycle
hal_pwm_set_duty_cycle:
    .long API_CODEER
    .long 0x96EBA1E2
.endfunc

.globl hal_pwm_start
.section .api_ram_code.hal_pwm_start
.type  hal_pwm_start, %function
.func  hal_pwm_start
hal_pwm_start:
    .long API_CODEER
    .long 0x7562106C
.endfunc

.globl hal_pwm_stop
.section .api_ram_code.hal_pwm_stop
.type  hal_pwm_stop, %function
.func  hal_pwm_stop
hal_pwm_stop:
    .long API_CODEER
    .long 0x41D99330
.endfunc

.globl hal_pwm_get_frequency
.section .api_ram_code.hal_pwm_get_frequency
.type  hal_pwm_get_frequency, %function
.func  hal_pwm_get_frequency
hal_pwm_get_frequency:
    .long API_CODEER
    .long 0xA2A045AE
.endfunc

.globl hal_pwm_get_duty_cycle
.section .api_ram_code.hal_pwm_get_duty_cycle
.type  hal_pwm_get_duty_cycle, %function
.func  hal_pwm_get_duty_cycle
hal_pwm_get_duty_cycle:
    .long API_CODEER
    .long 0x67F4F816
.endfunc

.globl hal_pwm_get_running_status
.section .api_ram_code.hal_pwm_get_running_status
.type  hal_pwm_get_running_status, %function
.func  hal_pwm_get_running_status
hal_pwm_get_running_status:
    .long API_CODEER
    .long 0x63B88356
.endfunc

.globl hal_pwm_set_advanced_config
.section .api_ram_code.hal_pwm_set_advanced_config
.type  hal_pwm_set_advanced_config, %function
.func  hal_pwm_set_advanced_config
hal_pwm_set_advanced_config:
    .long API_CODEER
    .long 0x748A774E
.endfunc

.globl hal_rtc_get_time
.section .api_ram_code.hal_rtc_get_time
.type  hal_rtc_get_time, %function
.func  hal_rtc_get_time
hal_rtc_get_time:
    .long API_CODEER
    .long 0x55E55AA0
.endfunc

.globl rtc_sw_timer_create
.section .api_ram_code.rtc_sw_timer_create
.type  rtc_sw_timer_create, %function
.func  rtc_sw_timer_create
rtc_sw_timer_create:
    .long API_CODEER
    .long 0xA83EE0A2
.endfunc

.globl rtc_sw_timer_start
.section .api_ram_code.rtc_sw_timer_start
.type  rtc_sw_timer_start, %function
.func  rtc_sw_timer_start
rtc_sw_timer_start:
    .long API_CODEER
    .long 0x56F4AACA
.endfunc

.globl rtc_sw_timer_stop
.section .api_ram_code.rtc_sw_timer_stop
.type  rtc_sw_timer_stop, %function
.func  rtc_sw_timer_stop
rtc_sw_timer_stop:
    .long API_CODEER
    .long 0x46E108C6
.endfunc

.globl rtc_sw_timer_delete
.section .api_ram_code.rtc_sw_timer_delete
.type  rtc_sw_timer_delete, %function
.func  rtc_sw_timer_delete
rtc_sw_timer_delete:
    .long API_CODEER
    .long 0x47E4ECE8
.endfunc

.globl hal_trng_init
.section .api_ram_code.hal_trng_init
.type  hal_trng_init, %function
.func  hal_trng_init
hal_trng_init:
    .long API_CODEER
    .long 0x46F443E0
.endfunc

.globl hal_trng_deinit
.section .api_ram_code.hal_trng_deinit
.type  hal_trng_deinit, %function
.func  hal_trng_deinit
hal_trng_deinit:
    .long API_CODEER
    .long 0xCC6C9712
.endfunc

.globl hal_trng_get_generated_random_number
.section .api_ram_code.hal_trng_get_generated_random_number
.type  hal_trng_get_generated_random_number, %function
.func  hal_trng_get_generated_random_number
hal_trng_get_generated_random_number:
    .long API_CODEER
    .long 0xD8C34264
.endfunc

