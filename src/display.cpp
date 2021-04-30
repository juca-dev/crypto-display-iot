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
void Storage::print(String text, uint16_t x = 1, uint16_t y = 1, uint8_t s = 3, bool w = false)
{
  gfx.setCursor(x, y);
  gtx.setTextSize(s);
  gtx.setTextWrap(w);
  gfx.print(text);
  gfx.display();
}
