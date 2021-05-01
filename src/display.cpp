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
  this->cache = "";
}
void Display::text(String value)
{
  this->cache = value + "\n" + this->cache;
  this->show();
}
void Display::show()
{
  String res = this->cache; //.substring(0, 20);
  
  gfx.clearDisplay();
  gfx.setCursor(this->x, this->y);
  gfx.setTextSize(this->s);
  gfx.setTextWrap(this->w);
  gfx.print(res);
  gfx.display();

  //Serial.print("DISPLAY: ");
  //Serial.println(v);
}
