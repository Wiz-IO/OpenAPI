# OpenAPI

OpenAPI is the sharing of Kernel ( Firmware ) functions to Userware applications<br>
or dynamically linking statically compiled applications<br>

Traditionally called: **Position Independent Code** ( PIC )<br>
https://en.wikipedia.org/wiki/Position-independent_code <br>
In order for applications to work regardless of their absolute address relative to the kernel, they use:<br>
* **Procedure Linkage Table PLT** (  PLT )
* **Global Offset Table** ( GOT ) 
* **Dynamic Linker** ( Kernel Procedure )

**Very briefly**<br>
When you compile your application, the compiler does not know where the shared kernel functions that the application will use are located<br>
for example: the kernel functions **pinMode()**, **digitalRead()**, **digitalWrite()** are in the Kernel and are shared for use<br>
The compiler creates a PLT table, more precisely veneer functions by renaming their names to:<br>
pinMode@plt, digitalRead@plt, digitalWrite@plt and their code looks like:<br>

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
