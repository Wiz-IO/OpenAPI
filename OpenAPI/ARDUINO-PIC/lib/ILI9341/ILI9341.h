#ifndef _ILI9341_H_
#define _ILI9341_H_

#include "Arduino.h"
#include "Print.h"
#include <Adafruit_GFX.h>

// clang-format off
#define SPI_SPEED 52000000
#define SPI_NO_CS

#define ILI9341_TFTWIDTH    240
#define ILI9341_TFTHEIGHT   320

#define RGBto565(r, g, b) ((((r)&0xF8) << 8) | (((g)&0xFC) << 3) | ((b) >> 3))
#define RGBIto565(r, g, b, i) ((((((r) * (i)) / 255) & 0xF8) << 8) | ((((g) * (i) / 255) & 0xFC) << 3) | ((((b) * (i) / 255) & 0xFC) >> 3))

#define BLACK               0x0000
#define WHITE               0xFFFF

#define BLUE                0x001F
#define RED                 0xF800
#define GREEN               0x07E0
#define CYAN                0x07FF
#define MAGENTA             0xF81F
#define YELLOW              0xFFE0

// ILI9341 commands
#define ILI9341_NOP         0x00
#define ILI9341_SWRESET     0x01
#define ILI9341_RDDID       0x04
#define ILI9341_RDDST       0x09
#define ILI9341_RDMODE      0x0A
#define ILI9341_RDMADCTL    0x0B
#define ILI9341_RDPIXFMT    0x0C
#define ILI9341_RDIMGFMT    0x0A
#define ILI9341_RDSELFDIAG  0x0F
#define ILI9341_SLPIN       0x10
#define ILI9341_SLPOUT      0x11
#define ILI9341_PTLON       0x12
#define ILI9341_NORON       0x13
#define ILI9341_INVOFF      0x20
#define ILI9341_INVON       0x21
#define ILI9341_GAMMASET    0x26
#define ILI9341_DISPOFF     0x28
#define ILI9341_DISPON      0x29
#define ILI9341_CASET       0x2A
#define ILI9341_RASET       0x2B
#define ILI9341_RAMWR       0x2C
#define ILI9341_RAMRD       0x2E
#define ILI9341_PTLAR       0x30
#define ILI9341_VSCRDEF     0x33
#define ILI9341_MADCTL      0x36
#define ILI9341_VSCRSADD    0x37
#define ILI9341_PIXFMT      0x3A
#define ILI9341_FRMCTR1     0xB1
#define ILI9341_FRMCTR2     0xB2
#define ILI9341_FRMCTR3     0xB3
#define ILI9341_INVCTR      0xB4
#define ILI9341_DFUNCTR     0xB6
#define ILI9341_PWCTR1      0xC0
#define ILI9341_PWCTR2      0xC1
#define ILI9341_PWCTR3      0xC2
#define ILI9341_PWCTR4      0xC3
#define ILI9341_PWCTR5      0xC4
#define ILI9341_VMCTR1      0xC5
#define ILI9341_VMCTR2      0xC7
#define ILI9341_PWCTRA      0xCB
#define ILI9341_CMD_CF      0xCF
#define ILI9341_RDID1       0xDA
#define ILI9341_RDID2       0xDB
#define ILI9341_RDID3       0xDC
#define ILI9341_RDID4       0xDD
#define ILI9341_GMCTRP1     0xE0
#define ILI9341_GMCTRN1     0xE1
#define ILI9341_TIMCTRA     0xE8
#define ILI9341_TIMCTRB     0xE9
#define ILI9341_TIMCTRC     0xEA
#define ILI9341_POWSEQ      0xED
#define ILI9341_CMD_EF      0xEF
#define ILI9341_EN3GAM      0xF2
#define ILI9341_PUMPRAT     0xF7
#define ILI9341_MADCTL_MY   0x80
#define ILI9341_MADCTL_MX   0x40
#define ILI9341_MADCTL_MV   0x20
#define ILI9341_MADCTL_ML   0x10
#define ILI9341_MADCTL_RGB  0x00
#define ILI9341_MADCTL_BGR  0x08
#define ILI9341_MADCTL_MH   0x04

#define LCD_CMD_DELAY 0x80

// clang-format off
static const uint8_t ILI9341_commands[] PROGMEM = {
    22,
    ILI9341_SWRESET, LCD_CMD_DELAY, 100,                                                                           // 1
    ILI9341_CMD_EF, 3, 0x03, 0x80, 0x02,                                                                           // 2
    ILI9341_CMD_CF, 3, 0x00, 0xC1, 0x30,                                                                           // 3
    ILI9341_POWSEQ, 4, 0x64, 0x03, 0x12, 0x81,                                                                     // 4
    ILI9341_TIMCTRA, 3, 0x85, 0x00, 0x78,                                                                          // 5
    ILI9341_PWCTRA, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,                                                               // 6
    ILI9341_PUMPRAT, 1, 0x20,                                                                                      // 7
    ILI9341_TIMCTRC, 2, 0x00, 0x00,                                                                                // 8
    ILI9341_PWCTR1, 1, 0x23,                                                                                       // 9  power control VRH[5:0]
    ILI9341_PWCTR2, 1, 0x10,                                                                                       // 10 power control SAP[2:0]; BT[3:0]
    ILI9341_VMCTR1, 2, 0x3E, 0x28,                                                                                 // 11 VCM control
    ILI9341_VMCTR2, 1, 0x86,                                                                                       // 12 VCM control2
    ILI9341_MADCTL, 1, ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR,                                                     // 13
    ILI9341_PIXFMT, 1, 0x55,                                                                                       // 14
    ILI9341_FRMCTR1, 2, 0x00, 0x18,                                                                                // 15
    ILI9341_DFUNCTR, 3, 0x08, 0x82, 0x27,                                                                          // 16
    ILI9341_EN3GAM, 1, 0x00,                                                                                       // 17 3Gamma Function Disable
    ILI9341_GAMMASET, 1, 0x01,                                                                                     // 18 Gamma curve selected
    ILI9341_GMCTRP1, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // 19 Set Gamma
    ILI9341_GMCTRN1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // 20
    ILI9341_SLPOUT, LCD_CMD_DELAY, 100,                                                                            // 21
    ILI9341_DISPON, LCD_CMD_DELAY, 255,                                                                            // 22
};

// clang-format on

class ILI9341 : public Adafruit_GFX
{
#ifdef SPI_NO_CS
#define CS_IDLE
#define CS_ACTIVE
#else
#define CS_IDLE   \
  if (csPin > -1) \
    digitalWrite(csPin, 1);

#define CS_ACTIVE \
  if (csPin > -1) \
    digitalWrite(csPin, 0);
#endif

#define DC_DATA digitalWrite(dcPin, 1)
#define DC_COMMAND digitalWrite(dcPin, 0)

public:
  ILI9341(int8_t DC, int8_t RST, int8_t CS = -1) : Adafruit_GFX(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT)
  {
    csPin = CS;
    dcPin = DC;
    rstPin = RST;
  }

  void begin()
  {
    pinMode(dcPin, OUTPUT);
    if (csPin > -1)
      pinMode(csPin, OUTPUT);
    SPI.begin();
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0)); //
    CS_ACTIVE;
    if (rstPin != -1)
    {
      pinMode(rstPin, OUTPUT);
      digitalWrite(rstPin, HIGH);
      delay(5);
      digitalWrite(rstPin, LOW);
      delay(500);
      digitalWrite(rstPin, HIGH);
      delay(10);
    }
    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;
    displayInit(ILI9341_commands);
  }

  void setAddrWindow(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
  {
    CS_ACTIVE;

    LCD_COMMAND(ILI9341_CASET);
    LCD_DATA_16(xs);
    LCD_DATA_16(xe);

    LCD_COMMAND(ILI9341_RASET);
    LCD_DATA_16(ys);
    LCD_DATA_16(ye);

    LCD_COMMAND(ILI9341_RAMWR);

    DC_DATA;
  }

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) // 18 fps
  {
    if (x >= _width || y >= _height || w <= 0 || h <= 0)
      return;
    if (x + w - 1 >= _width)
      w = _width - x;
    if (y + h - 1 >= _height)
      h = _height - y;
    setAddrWindow(x, y, x + w - 1, y + h - 1);
#if 0
    uint32_t size = (uint32_t)w * h;
    while (size--) // slow
      SPI.transfer(color);
#else
    SPI.fill(sizeof(color), color, w * h * sizeof(color));
#endif
    CS_IDLE;
  }

  void fillScreen(uint16_t color = BLACK)
  {
    fillRect(0, 0, _width, _height, color);
  }

  void clearScreen() { fillScreen(BLACK); }

  void pushColor(uint16_t color) { SPI.fill(sizeof(color), color, sizeof(color)); }

  void drawPixel(int16_t x, int16_t y, uint16_t color)
  {
    if (x < 0 || x >= _width || y < 0 || y >= _height)
      return;
    setAddrWindow(x, y, x + 1, y + 1);
    LCD_DATA_16(color);
    CS_IDLE;
  }

  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
  {
    if (x >= _width || y >= _height || h <= 0)
      return;
    if (y + h - 1 >= _height)
      h = _height - y;
    setAddrWindow(x, y, x, y + h - 1);
    SPI.fill(sizeof(color), color, h * sizeof(color));
    CS_IDLE;
  }

  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
  {
    if (x >= _width || y >= _height || w <= 0)
      return;
    if (x + w - 1 >= _width)
      w = _width - x;
    setAddrWindow(x, y, x + w - 1, y);
    SPI.fill(sizeof(color), color, w * sizeof(color));
    CS_IDLE;
  }

  void drawImage(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *buffer) // 18 fps
  {
    if (x >= _width || y >= _height || w <= 0 || h <= 0 || buffer == NULL)
      return;
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    int size = w * h;
    int sz = size;
#if 0 
    while (size--) // slow
      SPI.transfer(*img++);
#else
    uint16_t *p = (uint16_t *)frame_buffer;
    uint16_t *b = (uint16_t *)buffer;
    int count;
    while (size)
    {
      count = (size / sizeof(frame_buffer) / sizeof(uint16_t)) ? sizeof(frame_buffer) / sizeof(uint16_t) : size;
      for (unsigned int i = 0; i < sizeof(frame_buffer) / sizeof(uint16_t); i++)
      {
        p[i] = __REV16(*b++); // image L.M, display M.L
      }
      SPI.write((uint8_t *)p, count * sizeof(uint16_t));
      size -= count;
    }
#endif
    CS_IDLE;
  }

  void setRotation(uint8_t rot)
  {
    CS_ACTIVE;
    LCD_COMMAND(ILI9341_MADCTL);
    rotation = rot & 3;
    switch (rotation)
    {
    case 0:
      LCD_DATA(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
      _width = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 1:
      LCD_DATA(ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
      _width = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
    case 2:
      LCD_DATA(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
      _width = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 3:
      LCD_DATA(ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
      _width = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
    }
    CS_IDLE;
  }

  void invertDisplay(boolean mode)
  {
    CS_ACTIVE;
    LCD_COMMAND(mode ? ILI9341_INVON : ILI9341_INVOFF);
    CS_IDLE;
  }

  void partialDisplay(boolean mode)
  {
    CS_ACTIVE;
    LCD_COMMAND(mode ? ILI9341_PTLON : ILI9341_NORON);
    CS_IDLE;
  }

  void sleepDisplay(boolean mode)
  {
    CS_ACTIVE;
    LCD_COMMAND(mode ? ILI9341_SLPIN : ILI9341_SLPOUT);
    CS_IDLE
    delay(5);
  }

  void enableDisplay(boolean mode) {}
  void idleDisplay(boolean mode) {}
  void resetDisplay() {}
  void setScrollArea(uint16_t tfa, uint16_t bfa) {}
  void setScroll(uint16_t vsp) {}
  void setPartArea(uint16_t sr, uint16_t er) {}

  uint16_t Color565(uint8_t r, uint8_t g, uint8_t b)
  {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  }

  uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
  {
    return Color565(r, g, b);
  }

protected:
  uint8_t frame_buffer[SPI_DMA_SIZE];

  inline void LCD_COMMAND(uint8_t c)
  {
    DC_COMMAND;
    SPI.transfer(c);
  }

  inline void LCD_DATA(uint8_t d)
  {
    DC_DATA;
    SPI.transfer(d);
  }

  inline void LCD_DATA_16(uint16_t d)
  {
    DC_DATA;
    SPI.transfer(d);
  }

  void displayInit(const uint8_t *address)
  {
    uint8_t *addr = (uint8_t *)address;
    uint8_t numCommands, numArgs;
    uint16_t ms;
    numCommands = *addr++;
    CS_ACTIVE;
    while (numCommands--)
    {
      LCD_COMMAND(*addr++);
      numArgs = *addr++;
      ms = numArgs & LCD_CMD_DELAY;
      numArgs &= ~LCD_CMD_DELAY;
      while (numArgs--)
      {
        LCD_DATA(*addr++);
      }
      if (ms)
      {
        ms = *addr++;
        if (ms == 255)
          ms = 500;
        delay(ms);
      }
    }
    CS_IDLE;
  }

private:
  int8_t csPin, dcPin, rstPin;
};

#endif // _ILI9341_H_
