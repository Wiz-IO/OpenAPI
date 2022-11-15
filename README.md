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

TODO....


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
where **r_offset** Identifies the location of the object to be adjusted.<br>
or the address of the shared object (function, variable...) in the application address space<br>

<br><hr>

**Basic and simple !** ( watch in Youtube )

[![OpenAPI](https://img.youtube.com/vi/2D3_3b4-PVo/0.jpg)](https://www.youtube.com/watch?v=2D3_3b4-PVo "OpenAPI DEMO")

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
