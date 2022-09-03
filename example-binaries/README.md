## as example for LPWA Quectel BC66NB module

**The FIRMWARE**
* NB modem is disabled - just for periphery test
* The firmware use default boot loader
* Make NVDM backup and upload FIRMWARE ( just in case )


The Userware application is simple **ARDUINO** program
* upload ARDUINO and watch uarts
```
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
