#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display
{
public:
  void setup();
  void clear();
  void print(String text, uint16_t x = 1, uint16_t y = 1, uint8_t s = 3, bool w = false);
};

#endif