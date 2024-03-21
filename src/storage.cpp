#include "storage.h"

void Storage::setup()
{
  if (!SPIFFS.begin())
  {
    Serial.println("### ERR: Storage - SPIFFS not configured!");
  }
  else
  {
    Serial.println("### Storage - SPIFFS OK");
  }
  //Serial.println("### Storage - SPIFFS: total %u | used %u | free %u", SPIFFS.totalBytes(), SPIFFS.usedBytes(), (SPIFFS.totalBytes() - SPIFFS.usedBytes()));
}
void Storage::list()
{
  File dir = SPIFFS.open("/", "r");
  File file = dir.openNextFile();
  while (file)
  {
      // Serial.println("### Storage - File: %s (%u)", file.name(), file.size());
      Serial.print("### Storage - File ");
      Serial.print(file.name());
      Serial.print(" | Size ");
      Serial.println(file.size());

    file = dir.openNextFile();
  }

  if (dir)
  {
    dir.close();
  }
}
void Storage::put(String key, String value)
{
  String path = String("/" + key);
  Serial.println("put " + path);
  File file = SPIFFS.open(path, "w");
  if (!file)
  {
    Serial.println("### ERR: Storage - opening file for writing");
    return;
  }
  int bytesWritten = file.print(value);
  if (bytesWritten <= 0)
  {
    Serial.println("### ERR: Storage - File write failed");
  }
  file.close();
}
char* Storage::get(String key)
{
  String path = String("/" + key);
  Serial.println("get2 " + path);
  
  if (!SPIFFS.exists(path))
  {
    return "";
  }

  File file = SPIFFS.open(path, "r");
  if (!file)
  {
    Serial.println("### ERR: Storage - opening file for reading");
    return "";
  }
  
  char* buf = (char*)malloc(file.size());
  if (!buf) {
    return nullptr;
  }

  file.readString().toCharArray(buf, file.size());
  file.close();

  return buf;
}
bool Storage::remove(String key)
{
  String path = String("/" + key);
  Serial.println("remove " + path);
  //check for stored credentials
  if (!SPIFFS.exists(path))
  {
    return false;
  }

  SPIFFS.remove(path);
  return true;
}
