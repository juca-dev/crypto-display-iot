#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#include "storage.h"

const uint16_t DISPLAY_LINES = 10;
const uint16_t CHAR_SIZE = 6;

class Display
{
private:
  int16_t x = 0;
  int16_t y = 0;
  uint8_t s = 1; // size, char size = 6px
  bool w = false; // text wrap
  uint16_t speed = 250;
  unsigned short scrollY = 0;
  unsigned long lastMillis = 0;
  Storage storage;
  String lines[DISPLAY_LINES];
public:
  void setup();
  bool show();
  void clear();
  void text(String value);
  String getText();
  StaticJsonDocument<256> config();
  bool load(StaticJsonDocument<256> json);
  void save();
  void reset();
  void loop();
};

#endif
