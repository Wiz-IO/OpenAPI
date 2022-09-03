## as example for LPWA Quectel BC66NB module

It will probably work with SIMcom, Fibocom, Neoway ... etc. modules with **MT2625 SoC** - I don't have modules for testing

The **FIRMWARE**
* NB modem is disabled - just for periphery test
* The firmware use default boot loader
* Make NVDM backup ( just in case )
* Upload FIRMWARE


The Userware application is simple **ARDUINO** program
* Upload ARDUINO
* Watch uarts...
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
    digitalWrite(LED, T & 1);
    delay(250);
    if (++T % 20 == 0)
    {
        Serial.printf("[ARDUINO] millis = %u\n", millis());
    }
    if (Serial.available())
    {
        Serial.print("[ARDUINO] Serial echo: ");
        while (Serial.available())
            Serial.print((char)Serial.read());
        Serial.println();
    }
}
```

```
[ARDUINO] Mediatek MT2625 OpenAPI 2022 Georgi Angelov
[ARDUINO] millis = 5220
[ARDUINO] millis = 10420
 ...etc
```
![ScreenShot](https://raw.githubusercontent.com/Wiz-IO/Arduino_MT2625_BC66/master/board.jpg)
