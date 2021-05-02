#ifndef STORAGE_H
#define STORAGE_H

#include <FS.h>

class Storage
{
public:
  void setup();
  void list();
  void put(String key, String value);
  String get(String key);
  bool remove(String key);
};

#endif
