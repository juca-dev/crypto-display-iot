#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display
{
public:
  void setup();
  void clear();
  void print(String text, uint8_t x = 1, uint8_t y = 1);
};

#endif