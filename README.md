# OpenAPI<br>
_( will be more of an explanation than copy / paste code and the association with Arduino is as an example application )_

By the way, there is a more elegant solution... ( coming soon )

**Project Version 2.0.0**

OpenAPI is the sharing of Kernel ( Firmware ) functions to Userware applications<br>
or dynamically linking statically compiled applications<br>
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

In Kernel we create a simple [table](https://github.com/Wiz-IO/OpenAPI/blob/main/example-script/OPEN-API-C.h) (array) with records:
```c
API_TABLE = { // sorted by hash for fast binary-search
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
Looks like **PIC GOT table**

We will use HASH with which we will search the addresses,<br>
and the trick with HASH is that we don't care about the string name of the functions<br>
We also have a function:<br>
```c
static uint32_t getFunctionByHash(uint32_t hash) // binary-search
{
    int mid, low = 0, high = API_COUNT - 1;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        if (API_TABLE[mid].hash == hash)
            return API_TABLE[mid].func;
        if (API_TABLE[mid].hash < hash)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0;
}
```
The Kernel only needs a procedure to start the Userware application - we'll come back to that later

## Userware Application
```c
#include "Arduino.h"

void setup(void)
{
    Serial.begin(115200);
    Serial.println("\n[ARDUINO] Mediatek MT2625 OpenAPI 2022 Georgi Angelov");
    pinMode(LED, OUTPUT);
}

void loop(void)
{
    static int T = 0;
    digitalWrite(LED, T & 1); // blink
    delay(250);
    if (++T % 20 == 0)
    {
        Serial.printf("[ARDUINO] millis = %u\n", millis());
    }
    if (Serial.available()) // echo for test
    {
        Serial.print("[ARDUINO] Serial echo: ");
        while (Serial.available())
            Serial.print((char)Serial.read());
        Serial.println();
    }
}
```
Static User Space is allocated for these applications - USER-ROM & USER-RAM, free space somewhere in system memory<br>
where in USER-ROM we will upload the compiled BIN file<br>
and note - the system does not use MMU ( Memory management unit ) - there are no such resources.<br>
**Userware is just an extension of Firmware**, but Userware doesn't know where the shared functions are<br>

We make an [ASM file](https://github.com/Wiz-IO/OpenAPI/blob/main/example-script/OPEN-API-S.S) ( or library from it ) with RAM functions in RAM section **.api_ram_code**<br>
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
Looks like **PIC PLT table**

or binary RAM ARRAY looks like:<br>
```c
0xFEEDC0DE, 0xAAC4FB6A, 0xFEEDC0DE, 0x1C76F7B6 ... 0 // last zero means section EOF<br>
```
This RAM section is described in the [linker script](https://github.com/Wiz-IO/OpenAPI/blob/main/example-user/lib/cpp.ld#L110) and has parameters API-BEGIN, API-END, API-SIZE, like a normal **.data** section<br>
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
and will [initialize](https://github.com/Wiz-IO/OpenAPI/blob/main/example-kernel/OpenAPI-core.c#L137) the **.api_ram_code**, .data, bss sections<br>

and will patch veneers<br>

IF **API_TABLE[i].hash == hash** THEN we overwrite **0xFEEDC0DE** with **0xF000F85F**( instruction code ) <br>
and replace **HASH** with the **real function address**<br>
Now Userware is ready to Start --> call entry-point --> Arduino blink or ... driveRoverAtMars()<br>
Looks like **Dynamic Linker**

**Basic and simple !** ( watch in Youtube )

[![OpenAPI](https://img.youtube.com/vi/E_ITLNXYudA/0.jpg)](https://www.youtube.com/watch?v=E_ITLNXYudA "OpenAPI DEMO")

Resources:
* RAM & ROM 8 bytes for shared functions ( for Arm or Thumb32 )
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
