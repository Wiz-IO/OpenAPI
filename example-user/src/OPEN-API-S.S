/*
   OpenAPI [ 16.09.2022 ] Georgi Angelov
   Version: 1.0.0
   Functions: 37
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
    .long 0x72B996C3
.endfunc

.globl arduino_init
.section .api_ram_code.arduino_init
.type  arduino_init, %function
.func  arduino_init
arduino_init:
    .long API_CODEER
    .long 0x8483CC5A
.endfunc

.globl ring_buffer_init
.section .api_ram_code.ring_buffer_init
.type  ring_buffer_init, %function
.func  ring_buffer_init
ring_buffer_init:
    .long API_CODEER
    .long 0x0DE4D751
.endfunc

.globl get_random
.section .api_ram_code.get_random
.type  get_random, %function
.func  get_random
get_random:
    .long API_CODEER
    .long 0xB2CA946C
.endfunc

.globl yield
.section .api_ram_code.yield
.type  yield, %function
.func  yield
yield:
    .long API_CODEER
    .long 0x42BABC8B
.endfunc

.globl millis
.section .api_ram_code.millis
.type  millis, %function
.func  millis
millis:
    .long API_CODEER
    .long 0xCC08ADF1
.endfunc

.globl micros
.section .api_ram_code.micros
.type  micros, %function
.func  micros
micros:
    .long API_CODEER
    .long 0x547DC08F
.endfunc

.globl delayMicroseconds
.section .api_ram_code.delayMicroseconds
.type  delayMicroseconds, %function
.func  delayMicroseconds
delayMicroseconds:
    .long API_CODEER
    .long 0xA021DDA0
.endfunc

.globl delay
.section .api_ram_code.delay
.type  delay, %function
.func  delay
delay:
    .long API_CODEER
    .long 0x10DB166E
.endfunc

.globl pinMode
.section .api_ram_code.pinMode
.type  pinMode, %function
.func  pinMode
pinMode:
    .long API_CODEER
    .long 0x450C863B
.endfunc

.globl digitalWrite
.section .api_ram_code.digitalWrite
.type  digitalWrite, %function
.func  digitalWrite
digitalWrite:
    .long API_CODEER
    .long 0x8BD387AE
.endfunc

.globl digitalRead
.section .api_ram_code.digitalRead
.type  digitalRead, %function
.func  digitalRead
digitalRead:
    .long API_CODEER
    .long 0x68CB1624
.endfunc

.globl analogRead
.section .api_ram_code.analogRead
.type  analogRead, %function
.func  analogRead
analogRead:
    .long API_CODEER
    .long 0x86FF11F7
.endfunc

.globl analogWrite
.section .api_ram_code.analogWrite
.type  analogWrite, %function
.func  analogWrite
analogWrite:
    .long API_CODEER
    .long 0xD5D1D61B
.endfunc

.globl attachInterrupt
.section .api_ram_code.attachInterrupt
.type  attachInterrupt, %function
.func  attachInterrupt
attachInterrupt:
    .long API_CODEER
    .long 0x6F6148FB
.endfunc

.globl detachInterrupt
.section .api_ram_code.detachInterrupt
.type  detachInterrupt, %function
.func  detachInterrupt
detachInterrupt:
    .long API_CODEER
    .long 0x1DB69DD1
.endfunc

.globl periphery_init
.section .api_ram_code.periphery_init
.type  periphery_init, %function
.func  periphery_init
periphery_init:
    .long API_CODEER
    .long 0xEB8617A1
.endfunc

.globl periphery_deinit
.section .api_ram_code.periphery_deinit
.type  periphery_deinit, %function
.func  periphery_deinit
periphery_deinit:
    .long API_CODEER
    .long 0x2E14F6F0
.endfunc

.globl periphery_close
.section .api_ram_code.periphery_close
.type  periphery_close, %function
.func  periphery_close
periphery_close:
    .long API_CODEER
    .long 0x263FB1F0
.endfunc

.globl periphery_open
.section .api_ram_code.periphery_open
.type  periphery_open, %function
.func  periphery_open
periphery_open:
    .long API_CODEER
    .long 0x0D0D04FF
.endfunc

.globl periphery_write
.section .api_ram_code.periphery_write
.type  periphery_write, %function
.func  periphery_write
periphery_write:
    .long API_CODEER
    .long 0xAF34DFBB
.endfunc

.globl periphery_read
.section .api_ram_code.periphery_read
.type  periphery_read, %function
.func  periphery_read
periphery_read:
    .long API_CODEER
    .long 0xF76EC44A
.endfunc

.globl periphery_available
.section .api_ram_code.periphery_available
.type  periphery_available, %function
.func  periphery_available
periphery_available:
    .long API_CODEER
    .long 0xBB094ED0
.endfunc

.globl periphery_peek
.section .api_ram_code.periphery_peek
.type  periphery_peek, %function
.func  periphery_peek
periphery_peek:
    .long API_CODEER
    .long 0x421294E8
.endfunc

.globl periphery_flush_clean
.section .api_ram_code.periphery_flush_clean
.type  periphery_flush_clean, %function
.func  periphery_flush_clean
periphery_flush_clean:
    .long API_CODEER
    .long 0xEE12DCE6
.endfunc

.globl periphery_spi_transfer
.section .api_ram_code.periphery_spi_transfer
.type  periphery_spi_transfer, %function
.func  periphery_spi_transfer
periphery_spi_transfer:
    .long API_CODEER
    .long 0x2A461C37
.endfunc

.globl periphery_spi_set_frequency
.section .api_ram_code.periphery_spi_set_frequency
.type  periphery_spi_set_frequency, %function
.func  periphery_spi_set_frequency
periphery_spi_set_frequency:
    .long API_CODEER
    .long 0x9DA939ED
.endfunc

.globl periphery_spi_set_mode
.section .api_ram_code.periphery_spi_set_mode
.type  periphery_spi_set_mode, %function
.func  periphery_spi_set_mode
periphery_spi_set_mode:
    .long API_CODEER
    .long 0x8FA9BCC2
.endfunc

.globl periphery_spi_set_bit_order
.section .api_ram_code.periphery_spi_set_bit_order
.type  periphery_spi_set_bit_order, %function
.func  periphery_spi_set_bit_order
periphery_spi_set_bit_order:
    .long API_CODEER
    .long 0xF86C81CE
.endfunc

.globl periphery_spi_set_byte_order
.section .api_ram_code.periphery_spi_set_byte_order
.type  periphery_spi_set_byte_order, %function
.func  periphery_spi_set_byte_order
periphery_spi_set_byte_order:
    .long API_CODEER
    .long 0x04EBD85D
.endfunc

.globl periphery_i2c_set_clock
.section .api_ram_code.periphery_i2c_set_clock
.type  periphery_i2c_set_clock, %function
.func  periphery_i2c_set_clock
periphery_i2c_set_clock:
    .long API_CODEER
    .long 0xA13C0912
.endfunc

.globl periphery_i2c_transmit
.section .api_ram_code.periphery_i2c_transmit
.type  periphery_i2c_transmit, %function
.func  periphery_i2c_transmit
periphery_i2c_transmit:
    .long API_CODEER
    .long 0xAE0E1789
.endfunc

.globl periphery_i2c_receive
.section .api_ram_code.periphery_i2c_receive
.type  periphery_i2c_receive, %function
.func  periphery_i2c_receive
periphery_i2c_receive:
    .long API_CODEER
    .long 0x8FF2A514
.endfunc

.globl pvPortMalloc
.section .api_ram_code.pvPortMalloc
.type  pvPortMalloc, %function
.func  pvPortMalloc
pvPortMalloc:
    .long API_CODEER
    .long 0xAEA806E1
.endfunc

.globl vPortFree
.section .api_ram_code.vPortFree
.type  vPortFree, %function
.func  vPortFree
vPortFree:
    .long API_CODEER
    .long 0x051CCDEC
.endfunc

.globl pvPortCalloc
.section .api_ram_code.pvPortCalloc
.type  pvPortCalloc, %function
.func  pvPortCalloc
pvPortCalloc:
    .long API_CODEER
    .long 0xD854B969
.endfunc

.globl pvPortRealloc
.section .api_ram_code.pvPortRealloc
.type  pvPortRealloc, %function
.func  pvPortRealloc
pvPortRealloc:
    .long API_CODEER
    .long 0x62998AD9
.endfunc

