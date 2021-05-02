#ifndef API_H
#define API_H

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include "storage.h"
#include "web.h"
#include "iot.h"
#include "wifi.h"
#include "display.h"

class Api
{
private:
    ESP8266WebServer server;
    Storage storage;
    byte ledPin;
    byte powerPin;
    bool powerBkp;
    StaticJsonDocument<256> configBkp;
    IoT iot = IoT(0);
    WifiService wifi = WifiService(0);
    Display display;

    StaticJsonDocument<256> getJson();
    StaticJsonDocument<256> config();

public:
    Api(byte ledPin, byte powerPin);
    void setup();
    void loop();
    bool load(StaticJsonDocument<256> json);
    void conWeb();
    void conToggle();
    void conConfig();
    void conConfigPut();
    void conIot();
    void conIotPut();
    void conIotDel();
    void conWifi();
    void conWifiPut();
    void conWifiDel();
    void conDisplay();
    void conDisplayPut();
    void conDisplayDel();
    void conReset();
};

#endif
