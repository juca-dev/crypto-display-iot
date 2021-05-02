#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#include "storage.h"

class Display
{
private:
  uint16_t x = 0;
  uint16_t y = 0;
  uint8_t s = 1;
  bool w = true;
  String cache;
  Storage storage;
public:
  void setup();
  void show();
  void clear();
  void text(String value);
  StaticJsonDocument<256> config();
  bool load(StaticJsonDocument<256> json);
  void save();
  void reset();
};

#endif
