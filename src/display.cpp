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

  gfx.setTextColor(WHITE);

  this->clear();
  this->storage.setup();

  StaticJsonDocument<256> config = this->config();
  this->load(config);
}
void Display::clear()
{
  for(unsigned short i = 0; i < DISPLAY_LINES; i++)
  {
    this->lines[i] = "";
  }
  
  this->scrollY = 0;
  gfx.clearDisplay();
  gfx.display();
}
void Display::text(String value)
{
  for(unsigned short i = 0; i < DISPLAY_LINES; i++)
  {
    if(
      this->lines[i].length() == 0
      || (
        this->lines[i].length() > 3 
        && value.length() > 3
        && this->lines[i].substring(0, 3) == value.substring(0, 3)
      )
    )
    {
      this->lines[i] = this->lines[0];
      break;
    }
  } 
  this->lines[0] = value;

  this->scrollY = 0;
  this->show();
}
bool Display::show(unsigned short index)
{
  String res = "";
  for(unsigned short i = index; i < DISPLAY_LINES; i++)
  {
    if(this->lines[i] == "")
    {
      break;
    }
    res += this->lines[i] + '\n';
  }
  
  if(res.length() == 0)
  {
    return false;
  }

  gfx.clearDisplay();
  gfx.setCursor(this->x, this->y);
  gfx.setTextSize(this->s);
  gfx.setTextWrap(this->w);
  gfx.print(res);
  gfx.display();

  //Serial.print("DISPLAY: ");
  //Serial.println(v);

  return true;
}
void Display::loop()
{
  if (millis() - this->lastMillis > 3000) //time to wait
  {
      if(this->show(this->scrollY++))
      {
        this->lastMillis = millis();
      }
      else
      {
        this->scrollY = 0;
      }
  }
}
