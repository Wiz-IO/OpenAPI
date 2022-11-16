#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
SPIClass SPI("spi:0", SPI_PIN_NC, SPI_PIN_HARD, SPI_PIN_HARD, SPI_PIN_NC);
#define TFT_DC 0
#define TFT_RST 3
#include <ILI9341.h>
ILI9341 ILI = ILI9341(TFT_DC, TFT_RST);
#include <pacman.h>
void PACMAN_drawImage(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *buffer)
{
  ILI.drawImage(x, y, w, h, buffer);
}

extern const int api_constant;
extern int api_variable;

void setup()
{
  Serial.begin();
  Serial.println("Mediatek MT2526 2022 Georgi Angelov");
  Serial.printf("Version: %08X\n", getVersion());
  pinMode(LED, OUTPUT);
  ILI.begin();
  ILI.setRotation(2);
  ILI.clearScreen();
  pacman_setup();

  Serial.printf("VARIABLE: %d\n", api_variable);
  Serial.printf("CONSTANT: %X\n", api_constant);
}

void loop()
{
  pacman_loop();
  
  digitalToggle(LED, 1000);
  // Serial.println(__func__);
  Serial.printf("TEST VARIABLE: %d\n", api_variable++);
  // ILI.fillScreen(random(0xFFFF));
}
