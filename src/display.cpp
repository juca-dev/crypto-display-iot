#include "display.h"

Adafruit_SSD1306 gfx(128, 32, &Wire, 16);

void Display::setup()
{
  if (!gfx.begin(SSD1306_SWITCHCAPVCC, 0x3c))
  {
    Serial.println("Display not allocated - check connections");
    for (;;)
      yield();
  }

  gfx.clearDisplay();
  gfx.setTextColor(WHITE);
}
void Display::clear()
{
  gfx.clearDisplay();
}
void Display::print(String text, uint16_t x, uint16_t y, uint8_t s, bool w)
{
  gfx.clearDisplay();
  gfx.setCursor(x, y);
  gfx.setTextSize(s);
  gfx.setTextWrap(w);
  gfx.print(text);
  gfx.display();
}
