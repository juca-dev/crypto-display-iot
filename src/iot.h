#ifndef IOT_H
#define IOT_H

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <time.h>
#include "storage.h"

class IoT
{
private:
  byte ledPin;
  Storage storage;
  uint16_t port = 8883;
  String id;
  String host;
  String topicSub;
  String topicPub;
  String certCA;
  String certClient;
  String certKey;
  unsigned long lastMillis = 0;
  time_t now;
  time_t nowish = 1510592825;
  bool ledOn = false;
  PubSubClient client;
public:
  IoT(byte pin);
  String content;
  StaticJsonDocument<256> config();
  bool load(StaticJsonDocument<256> json);
  void save();
  void reset();
  void ntpConnect();
  void messageReceived(char *topic, byte *payload, unsigned int length);
  void pubSubErr(int8_t MQTTErr);
  void connectToMqtt(bool nonBlocking = false);
  void sendData();
  void setup();
  void loop();
  void setCertCA(String value);
  void setCertClient(String value);
  void setCertKey(String value);
};

#endif
