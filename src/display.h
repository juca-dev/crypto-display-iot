#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#include "storage.h"

const uint16_t DISPLAY_LINES = 100;

class Display
{
private:
  uint16_t x = 0;
  uint16_t y = 0;
  uint8_t s = 1;
  bool w = true;
  unsigned short scrollY = 0;
  unsigned long lastMillis = 0;
  Storage storage;
  String lines[DISPLAY_LINES];
public:
  void setup();
  bool show(unsigned short index = 0);
  void clear();
  void text(String value);
  StaticJsonDocument<256> config();
  bool load(StaticJsonDocument<256> json);
  void save();
  void reset();
  void loop();
};

#endif
