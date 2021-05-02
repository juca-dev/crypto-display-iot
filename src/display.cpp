#include "display.h"

Adafruit_SSD1306 gfx(128, 32, &Wire, 16);

void Display::reset()
{
  this->storage.remove("display.json");
}
StaticJsonDocument<256> Display::config()
{
  String data = this->storage.get("display.json");
  StaticJsonDocument<256> json;
  DeserializationError err = deserializeJson(json, data);
  if (err)
  {
    Serial.print("### ERR: Display - ");
    Serial.println(err.c_str());
  }
  return json;
}
bool Display::load(StaticJsonDocument<256> json)
{
  if (json.isNull() || !json.containsKey("x") || !json.containsKey("y") || !json.containsKey("s") || !json.containsKey("w"))
  {
    Serial.println("Display: No config");
    return false;
  }

  this->x = json["x"].as<uint16_t>();
  this->y = json["y"].as<uint16_t>();
  this->s = json["s"].as<uint8_t>();
  this->w = json["w"].as<bool>();
  this->show();

  return true;
}
void Display::save()
{
    StaticJsonDocument<256> json;
    json["x"] = this->x;
    json["y"] = this->y;
    json["s"] = this->s;
    json["w"] = this->w;
    String value;
    serializeJson(json, value);
    this->storage.put("display.json", value);
}
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

  this->storage.setup();

    StaticJsonDocument<256> config = this->config();
    this->load(config);
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
