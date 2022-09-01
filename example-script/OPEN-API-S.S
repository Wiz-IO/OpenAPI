/*
   OpenAPI [ 31.08.2022 ] Georgi Angelov
   Version: 1.0.0
   Functions: 19
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

