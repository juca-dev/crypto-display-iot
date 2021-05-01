#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display
{
private:
  uint16_t x = 0;
  uint16_t y = 0;
  uint8_t s = 1;
  bool w = true;
  String cache;
public:
  void setup();
  void show();
  void clear();
  void text(String value);
};

#endif
