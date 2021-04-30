#include "api.h"

Api API(LED_BUILTIN, 5);

void setup()
{
  Serial.begin(115200);

  API.setup();
}

void loop()
{
  API.loop();
}