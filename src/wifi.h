#ifndef WIFISERVICE_H
#define WIFISERVICE_H

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "storage.h"

class WifiService
{
private:
    byte ledPin;
    Storage storage;

public:
    WifiService(byte pin);
    void setup();
    StaticJsonDocument<256> config();
    void update(String ssid, String password);
    bool load(StaticJsonDocument<256> json);
};

#endif