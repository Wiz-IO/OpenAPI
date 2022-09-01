# OpenAPI<br>
_( will be more of an explanation than copy / paste code and the association with Arduino is as an example application )_

OpenAPI is the sharing of Kernel ( Firmware ) functions to Userware applications<br>
or dynamically linking statically compiled applications<br>

Traditionally called: **Position Independent Code** ( PIC )<br>
https://en.wikipedia.org/wiki/Position-independent_code <br>
In order for applications to work regardless of their absolute address relative to the kernel, they use:<br>
* **Procedure Linkage Table PLT** (  PLT )
* **Global Offset Table** ( GOT ) 
* **Dynamic Linker** ( Kernel Procedure )

## Very briefly<br>
When you compile your application, the compiler does not know where the shared kernel functions that the application will use are located<br>
for example: the kernel functions **pinMode()**, **digitalRead()**, **digitalWrite()** are in the Kernel and are shared for use<br>
The compiler creates a PLT table, more precisely veneer functions by renaming their names to:<br>
**pinMode@plt**, **digitalRead@plt**, **digitalWrite@plt** and their code looks like:<br>

PLT or RAM functions
```
pinMode@plt:        jump got[1]
digitalRead@plt:    jump got[2]
digitalWrite@plt:   jump got[3]
...
```
GOT[] or array
```
got[1] = 0 ? ZERO does not know the address of the function
got[2] = 0 ?
got[3] = 0 ?
...
```
![plt got before](https://raw.githubusercontent.com/Wiz-IO/OpenAPI/main/images/plt_got-1.png)

and so your application compiles without errors, and the code looks like:<br>
pinMode() --> pinMode_veneer() --> pinMode@plt: jump got[1]=0 <-- no address

**How to launch the app**<br>
The kernel loads the application somewhere in RAM and<br>
the Dynamic Linker overwrites the GOT table with the absolute addresses of the shared functions<br>
pinMode() --> pinMode_veneer() --> pinMode@plt: jump got[1]=**0x82001342** <-- real address

![plt got after](https://raw.githubusercontent.com/Wiz-IO/OpenAPI/main/images/plt_got-2.png)

Unfortunately, all this uses a lot of resources, and more detailed information can be found on the web<br>
as example: https://eli.thegreenplace.net/2011/11/03/position-independent-code-pic-in-shared-libraries

## How to use this on "small" systems with limited resources<br>
For example: ARM Cortex M4 with several megabytes of ROM & RAM<br>
Another example: a GSM LPWA NB-IoT module that integrates an Arduino Core,<br>
which is shared for use by Userware Arduino applications

In Kernel we create a simple table (array) with records:
```c
API_TABLE = {
    { 0x10A9DD60, analogRead        }, // mean: HASH32( "function_name" ), adddress of function_name()
    { 0x1C76F7B6, micros            },
    { 0x1F55D5A2, delay             },
    { 0x20F099D4, analogWrite       },
    { 0x6A50FC40, delayMicroseconds },
    { 0x6A973CA4, pinMode           },
    { 0x73501778, digitalRead       },
    { 0xAAC4FB6A, millis            },
    { 0xC9AE709C, digitalWrite      },
    ...
}
```
looks like **GOT table**

We will use HASH with which we will search the addresses,<br>
and the trick with HASH is that we don't care about the string name of the functions<br>
We also have a function:<br>
```c
uint32_t getFunctionAddressByHash( uint32_t hash ) // note: I will write interactively to make it understandable
{ 
    for everything from от API_TABLE[]: 
        if API_TABLE[i].hash == hash 
            return API_TABLE[i].function 
    or return error
}
```
The Kernel only needs a procedure to start the Userware application - we'll come back to that later

## Userware Application
```c
int main(void)
{
    static int T = 0;
    pinMode(LED, OUTPUT); 
    while (true)
    {
        digitalWrite(LED, ++T & 1);
        delay(250); 
    }
}
```
Static User Space is allocated for these applications - USER-ROM & USER-RAM, free space somewhere in system memory<br>
where in USER-ROM we will upload the compiled BIN file<br>
and note - the system does not use MMU ( Memory management unit ) - there are no such resources.<br>
**Userware is just an extension of Firmware**, but Userware doesn't know where the shared functions are<br>

We make an ASM file ( or library from it ) with RAM functions in RAM section **.api_ram_code**<br>
to keep the compiler from crying and will be used for "**Dynamic Linking**"

```S
#define API_CODEER 0xFEEDC0DE ; it can just be zero, it is used here to hide information

.globl millis
.section .api_ram_code.millis
.type  millis, %function
.func  millis
millis:
    .long API_CODEER ; mean: just simple number
    .long 0xAAC4FB6A ; mean: HASH32( "millis" ) this HASH is also in the above API_TABLE[]
.endfunc

.globl micros
.section .api_ram_code.micros
.type  micros, %function
.func  micros
micros:
    .long API_CODEER ; mean: just simple number
    .long 0x1C76F7B6 ; mean: HASH32( "micros" )
.endfunc

// etc all other shared functions
```
looks like **PLT table**

or binary RAM ARRAY looks like:<br>
```c
0xFEEDC0DE, 0xAAC4FB6A, 0xFEEDC0DE, 0x1C76F7B6 ... 0xFFFFFFFF, 0xFFFFFFFF // this last means section EOF<br>
```
This RAM section is described in the linker script and has parameters API-BEGIN, API-END, API-SIZE, like a normal **.data** section<br>
Why EOF - the compiler will remove the unused functions and we need it for the end of the array<br>

![asm](https://raw.githubusercontent.com/Wiz-IO/OpenAPI/main/images/asm.jpg)

## Nothing complicated, right ?

What the above ASM functions actually are?<br>
**user millis: ldr.w ps, =(kernel millis address)**<br>
or a 4-byte Thumb32 (or Arm) instruction ( for Thumb16 it's 16 bytes - we won't go into this )<br>
binary for Cortex M4 it seems<br>
0xF000F85F = ldr.w ps, =(label)<br>
0x12345678 = label address<br>
**But we don't know the address !!!**<br>
0xF000F85F we will hide it with 0xFEEDC0DE, and instead of the address we will write HASH32( "function_name" )<br>

Compile --> ELF --> BIN --> UPLOAD ... for the above LED BLINK is about 400 bytes

**We're rebooting**<br>
Kernel initializes the system and will attempt to [start](https://github.com/Wiz-IO/OpenAPI/blob/main/example-kernel/OpenAPI-core.c#L113) Userware located at USER-ROM and use USER-RAM allocated<br>
At the beginning of the BIN file ( USER-ROM ) there is a **HEADER** with information about the Userware Application:<br>
**MAGIC, API-VERSION, .api_ram_code, .data, bss, entry-point**<br>
The Kernel will [check MAGIC and API-VERSION](https://github.com/Wiz-IO/OpenAPI/blob/main/example-kernel/OpenAPI-core.c#L121) if the application is valid<br>
and will initialize the **.api_ram_code**, .data, bss sections<br>

so<br>

We scan **.api_ram_code**, in this example:
```c
**0xFEEDC0DE, 0xAAC4FB6A**, 0xFEEDC0DE, 0x1C76F7B6 ... 0xFFFFFFFF, 0xFFFFFFFF<br>
```
IF **API_TABLE[i].hash == hash** THEN we overwrite **0xFEEDC0DE** with **0xF000F85F**(instruction code) and replace **HASH** with the **real function address**<br>
Now Userware is ready to Start --> call entry-point --> Arduino blink or ... driveRoverAtMars()<br>

**Basic and simple !** ( watch in Youtube )

[![OpenAPI](https://img.youtube.com/vi/E_ITLNXYudA/0.jpg)](https://www.youtube.com/watch?v=E_ITLNXYudA "OpenAPI DEMO")

Resources:
* RAM & ROM 8 bytes for shared functions ( for Arm or Thumb32 )
* Kernel control
* Hidden information
* Option for signed applications
* etc

The examples in the folders have been tested with
* Mediatek **MT2625** - GSM LPWA NB-IoT SoC ( ARMv7 Cortex M4 )
* Mediatek MT2503 - GSM GPRS SoC ( ARMv6 )
* Can be used with any ARM SoC modules...

Similarly OpenAPI is OpenCPU:<br>
* Chinese author - unknown,<br>
* It is used by Quectel - a manufacturer of GSM modules<br>
* Unfortunately - Closed Source with writing peculiarities and underdeveloped SDK over the years<br>

and sorry for my bad English...

>Here you can treat me to Coffee or Year Old Whisky :)
[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=ESUP9LCZMZTD6)
