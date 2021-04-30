#ifndef IOT_H
#define IOT_H

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <time.h>
#include "storage.h"

WiFiClientSecure net;

class IoT
{
private:
  byte ledPin;
  Storage storage;
  int port = 8883;
  String id;
  String host;
  String topicSub;
  String topicPub;
  String cert;
  String certClient;
  String certKey;
  PubSubClient client(net);
  unsigned long lastMillis = 0;
  time_t now;
  time_t nowish = 1510592825;
  bool ledOn = false;

public:
  IoT(byte pin);
  StaticJsonDocument<256> config();
  void ntpConnect();
  void messageReceived(char *topic, byte *payload, unsigned int length);
  void pubSubErr(int8_t MQTTErr);
  void connectToMqtt(bool nonBlocking = false);
  void sendData();
  void setup();
  void loop();
};