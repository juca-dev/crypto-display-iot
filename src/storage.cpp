#include "storage.h"

void Storage::setup()
{
  if (!SPIFFS.begin())
  {
    Serial.println("### ERR: Storage - SPIFFS not configured!");
  }
  Serial.println("### Storage - SPIFFS: total %u | used %u | free %u", SPIFFS.totalBytes(), SPIFFS.usedBytes(), (SPIFFS.totalBytes() - SPIFFS.usedBytes()));
}
void Storage::list()
{
  File dir = SPIFFS.open("/");
  File file = dir.openNextFile();
  while (file)
  {
    Serial.println("### Storage - File: %s (%u)", file.name(), file.size());
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
String Storage::get(String key)
{
  String path = String("/" + key);
  Serial.println("get " + path);
  //check for stored credentials
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
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  return buf.get();
}