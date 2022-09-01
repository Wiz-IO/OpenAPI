# OpenAPI<br>
_( will be more of an explanation than copy / paste code )_

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
looks like GOT table

We will use HASH with which we will search the addresses,<br>
and the trick with HASH is that we don't care about the string name of the functions<>
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
