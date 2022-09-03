NOTE: libopenapi.a is pre-compiled OPEN-API-S.S 

```c
int main(void)
{
    static int T = 0;
    pinMode(LED, OUTPUT);             // Arduino pin 4
    while (true)
    {
        digitalWrite(LED, ++T & 1);
        delay(250); // 250 mS
    }
}
```


```
LOG FROM MT2656 as Quectel BC66 module

F1: 0000 0000
V0: 0000 0000 [0001]
00: 0006 000C
01: 0000 0000
U0: 0000 0001 [0000]
T0: 0000 00B4
Leaving the BROM

addr =0xa20d009c;  temp=0x00000000
CAPID init done, default value = 431 
Load CAPID from golden value ...  
Expect CAPID = 403 
decrease CAPID from 431 to 403 
now CAPID = 403 
[T: 14 M: common C: info F: system_init L: 313]: System Initialize DONE @ 104000.
[T: 21 M: common C: info F: main L: 53]: [M] Mediatek MT2625 Hello World 2022 Georgi Angelov
[A] Starting OpenAPI Application ( v1.0.0 )
[A] APP: magic = 0xCAFECAFE, version = 0x100
[A] APP: entry = 0x08292031, stack request = 1024
[A] API EOF: APP use 5 functions
[A] Creating Application Task and Run
[U] Arduino Application
[UART] Started: 0x100058d8 0x10005968
[UART] Closed
[UART] Opened
[U] Loop: 5020 mS
[U] Loop: 10020 mS
[U] Loop: 15020 mS
[U] Loop: 20020 mS

```
