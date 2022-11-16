# OpenAPI<br>
In the first version I had done an imitation of Position Independent Code,<br> 
but it turned out that there is a much simpler and more elegant solution - the one provided by the compiler itself

**Project Version 2.0.0**

OpenAPI is the sharing of Kernel ( Firmware ) functions to Userware applications<br>
_Note: no Memory Management Unit ( MMU )_

**Suitable for IoT** ( WiFi, GSM, LoRa ... etc ) **modules**<br>
You manufacture IoT modules<br>
They are probably controlled with an external MCU and AT commands - technology from the last century<br>
There is probably free, unusable space in their memory<br>
Integrate OpenAPI and your customers will be able to write their own applications ( without external management )<br>

<hr><br>

## Where does the idea come from?

Traditionally called: **Position Independent Code** ( PIC )<br>
https://en.wikipedia.org/wiki/Position-independent_code <br>
In order for applications to work regardless of their absolute address relative to the kernel, they use:<br>
* **Procedure Linkage Table PLT** (  PLT )
* **Global Offset Table** ( GOT ) 
* **Dynamic Linker** ( Kernel Procedure )

### Very briefly<br>

NOTE: I will use Arduino as an integration example

When you compile your application, the compiler does not know where the shared kernel functions that the application will use are located<br>
for example: the kernel functions **pinMode()**, **digitalRead()**, **digitalWrite()** are in the Kernel and are shared for use<br>
The compiler creates a PLT table, more precisely veneer functions by renaming their names to:<br>
**pinMode@plt**, **digitalRead@plt**, **digitalWrite@plt** and their code looks like:<br>

PLT or ram veneers
```
pinMode@plt:        jump got[1]
digitalRead@plt:    jump got[2]
digitalWrite@plt:   jump got[3]
...
```
GOT[] or indexed array
```
got[1] = 0 ? ZERO does not know the address of the function
got[2] = 0 ?
got[3] = 0 ?
...
```
![plt got before](https://raw.githubusercontent.com/Wiz-IO/OpenAPI/main/images/plt_got-1.png)

and so your application compiles without errors, and the code looks like:<br>
pinMode() --> pinMode@plt: jump got[1]=0 <-- no address

**How to launch the app**<br>
The kernel loads the application somewhere in RAM and<br>
the Dynamic Linker overwrites the GOT table with the absolute addresses of the shared functions<br>
pinMode() --> pinMode@plt: jump got[1]=**0x82001342** <-- real address

![plt got after](https://raw.githubusercontent.com/Wiz-IO/OpenAPI/main/images/plt_got-2.png)

**Unfortunately, all this uses a lot of resources**, and more detailed information can be found on the web<br>
as example: https://eli.thegreenplace.net/2011/11/03/position-independent-code-pic-in-shared-libraries

<hr><br>

## How to use this on "small" systems with limited resources<br>
For example: ARM Cortex M4 with several megabytes of ROM & RAM<br>
Another example: a GSM LPWA NB-IoT module that integrates an Arduino Core,<br>
which is shared for use by Userware Arduino applications

<br><hr>

Compilers have a perfect mechanism for compiling Position Independent Code, and we are only required to arrange the code.<br>
When we include the -fPIC flag, the compiler adds several sections necessary for the relocation of shared objects<br>
**.rel.dyn, .rel.dyn, .dynsym, .dynstr**<br>
These are tables of addresses - which, where is in the application<br>
for example **.rel.dyn** is an array (table) for all shared objects with structure:
```c
typedef struct {
    Elf32_Addr      r_offset;
    Elf32_Word      r_info;
} Elf32_Rel;
```
where **r_offset** identifies the location of the object to be adjusted.<br>
or the address of the shared object (function, variable...) in the application address space<br>
and **r_info** identifies the patch type and its index in the ELF Symbol Table<br>
Detailed information can be found on the web...<br>

![rel](https://raw.githubusercontent.com/Wiz-IO/OpenAPI/main/images/rel.gif)

As I shared above we are only asked to line up our code, and for this we need a modified two-part linker script<br>
The second part is a normal script like for a static application<br>
The first part begins with a header or information about the application and the addresses of certain sections,<br>
and they are the standard .bss and .data to initialize our variables.<br>
as well as the additional Position Independent Code tables, which we arrange immediately after the header<br>
![head](https://raw.githubusercontent.com/Wiz-IO/OpenAPI/main/images/USER-HEADER.jpg)
```
SECTIONS
{
    . = ROM_BASE;

/***** HEADER *****/
    .initdata :
    {
        LONG( 0xFECAFECA );                 /* APP MAGIC    */
        LONG( 0x12345678 );                 /* API VERSION  */
        LONG((ABSOLUTE(app_entry + 1)));    /* APP ENTRY    */
        LONG( APP_STACK );                  /* APP STACK optional */

        LONG((ABSOLUTE( _data_load )));     /* copy .data */
        LONG((ABSOLUTE( _data_start )));
        LONG((ABSOLUTE( _data_end )));

        LONG((ABSOLUTE( _bss_start )));     /* clear .bss */
        LONG((ABSOLUTE( _bss_end )));

        LONG((ABSOLUTE( _rel_dyn )));       /* ELF Relocation Table : Elf32_Rel    */
        LONG((ABSOLUTE( _dyn_start )));     /* ELF Symbol Table     : Elf32_Sym    */
        LONG((ABSOLUTE( _str_start )));     /* ELF String Table     : const char * */

        /* reserved, align 64 bytes */
        LONG(0);
        LONG(0);
        LONG(0);
        LONG(0);
    } > FLASH

    .rel.dyn : /* ELF REL Relocation Table : Elf32_Rel */
    {
        _rel_dyn = .;
        *(.rel.dyn)
        _rel_dyn_end = .;
    } > FLASH

    .rel.plt : /* ELF JMPREL Relocation Table : Elf32_Rel */
    {
        _rel_plt = .;
        *(.rel.plt)
        _rel_plt_end = .;
    } > FLASH

    .dynsym :  /* ELF Symbol Table : Elf32_Sym */
    {
        _dyn_start = .;
        *(.dynsym)
        _dyn_end = .;
    } > FLASH

    .dynstr :  /* ELF String Table */
    {
        _str_start = .;
        *(.dynstr)
    } > FLASH
    .................
```
APP MAGIC and API VERSION are information that this is a user application,<br>
APP ENTRY is the entry "reset" vector of the application itself<br>
With the first two, we inform the kernel that this is indeed a userware application to be launched from the address APP ENTRY<br><br>
All that's left is to trick the compiler ... make it import the shared objects ... very easy<br>
The compiler only needs to construct the relocation tables. <br>
We create a single C file with all shared objects without their types, just a void foo(void) function<br>
and compile it as -fPIC library<br>
```
void api_syscall(void){}
void api_malloc(void){}
void api_realloc(void){}
void api_calloc(void){}
void api_free(void){}
....etc, all shared objects
```

```
CC_OPTIONS=-mcpu=cortex-m4 -mthumb -msingle-pic-base -fPIC -Wall
GCC_PATH=C:/Users/1124/.platformio/packages/toolchain-gccarmnoneeabi/bin/

all:
	@echo   * Creating OpenAPI PIC Library
	$(GCC_PATH)arm-none-eabi-gcc $(CC_OPTIONS) -g -Os -c OpenAPI-shared.c -o OpenAPI-shared.o
	$(GCC_PATH)arm-none-eabi-gcc -shared -Wl,-soname,libopenapi.a -nostdlib -o libopenapi.a OpenAPI-shared.o
```
![library](https://raw.githubusercontent.com/Wiz-IO/OpenAPI/main/images/lib.jpg)
We have the linker script and the library, it remains to compile the application - like a normal application, <br>but with the -fPIC flag<br>
Something complicated?

**Kernel Application loader**<br>

The Loader are a few simple functions: check for valid app, initialize .data & .bss and relocate shared objects.<br>
The check in this example is simple - we check if a constant address in the flash contains<br>APP MAGIC and API VERSION and limit APP ENTRY
Then we copy the data for section data and reset the bss section<br>
Relocation is also not a big deal, this solution doesn't use MMU and we don't need complex address recalculation of shared objects<br>
With the indices of the shared objects from the ELF REL Relocation Table we check ( strcmp() ) whether we have shared the searched object<br>
and if yes: we overwrite its r_offset with the address of the real function<br>
```c
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
```
And finally we call APP ENTRY<br><br>
**That's all Folks...**

<br><hr>

**Basic and simple !** ( watch in Youtube )

[![OpenAPI](https://img.youtube.com/vi/2D3_3b4-PVo/0.jpg)](https://www.youtube.com/watch?v=2D3_3b4-PVo "OpenAPI DEMO")

Resources:
* RAM & ROM a few for shared functions ( for Arm or Thumb32 )
* Kernel control
* Hidden information
* Option for signed applications
* as example: Arduino peripheral functions: 36 ( GPIO, EINT, ADC, PWM, UART, I2C, SPI )  
* etc

The examples in the folders have been tested with
* Mediatek **MT2625** - GSM LPWA NB-IoT SoC ( ARMv7 Cortex M4 )
* Mediatek MT2503 - GSM GPRS SoC ( ARMv6 )
* Can be used with any ARM SoC modules...

Similarly OpenAPI is OpenCPU:<br>
* Chinese author - unknown,<br>
* It is used by Quectel - a manufacturer of GSM modules<br>
* Unfortunately - Closed Source with writing peculiarities and underdeveloped SDK over the years<br>

![openapi](https://raw.githubusercontent.com/Wiz-IO/LIB/master/images/openapi.jpg)

and sorry for my bad English...

>Here you can treat me to Coffee or 12 Year Old Whisky :)
[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=ESUP9LCZMZTD6)
