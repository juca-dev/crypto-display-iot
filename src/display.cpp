#include "display.h"

Adafruit_SSD1306 gfx(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET); // onboard display
// Adafruit_SSD1306 gfx(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET); // external display
int x, minX;

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
  if (json.isNull() || !json.containsKey("x") || !json.containsKey("y") || !json.containsKey("s") || !json.containsKey("w")|| !json.containsKey("d"))
  {
    Serial.println("Display: No config");
    return false;
  }

  this->x = json["x"].as<uint16_t>();
  this->y = json["y"].as<uint16_t>();
  this->s = json["s"].as<uint8_t>();
  this->w = json["w"].as<bool>();
  this->speed = json["d"].as<uint16_t>();

  gfx.clearDisplay();
  gfx.drawPixel(this->x, this->y, OLED_COLOR);
  gfx.setCursor(this->x, this->y);
  gfx.setTextSize(this->s);
  gfx.setTextWrap(this->w);
  gfx.display();
  Serial.println("Display: configurated");
  delay(1000);

  return true;
}
void Display::save()
{
  StaticJsonDocument<256> json;
  json["x"] = this->x;
  json["y"] = this->y;
  json["s"] = this->s;
  json["w"] = this->w;
  json["d"] = this->speed;
  String value;
  serializeJson(json, value);
  this->storage.put("display.json", value);

  this->show();
}
void Display::setup()
{
  if (!gfx.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
  {
    Serial.println("Display not allocated - check connections");
    for (;;)
      yield();
  }
  // the library initializes this with an Adafruit splash screen.
  gfx.display();
  gfx.setTextColor(OLED_COLOR);
  delay(2000); // Pause for 2 seconds

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

  this->show();
}
String Display::getText()
{
  String res = "";
  for(unsigned short i = 0; i < DISPLAY_LINES; i++)
  {
    if(this->lines[i] == "")
    {
      break;
    }
    if(i > 0){
      res += " | "; // separator
    }
    res += this->lines[i];
  }

  return res;
}
bool Display::show()
{
  String res = this->getText();
  
  if(res.length() == 0)
  {
    return false;
  }

  x = gfx.width();
  minX = (-1 * (this->s * CHAR_SIZE)) * res.length(); 

  gfx.clearDisplay();
  gfx.setCursor(this->x, this->y);
  gfx.print(res);
  gfx.display();

  Serial.print("PRINT: ");
  Serial.println(res);
  delay(500);

  return true;
}
void Display::loop()
{
  if (millis() - this->lastMillis > this->speed) //time to wait
  {
    this->lastMillis = millis();

    gfx.clearDisplay();
    gfx.setCursor(x, this->y);
    gfx.print(this->getText());
    gfx.display();

    x -= CHAR_SIZE;
    if(x < minX) x = gfx.width();
  }
}
