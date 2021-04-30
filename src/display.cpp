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
void Storage::print(String text, uint8_t x = 1, uint8_t y = 1)
{
  gfx.setCursor(x, y);
  gfx.print(text);
  gfx.display();
}
