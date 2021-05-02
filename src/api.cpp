#include "api.h"

const char ERR_400[] = "{\"error\":\"invalid params\"}";
const char ERR_405[] = "{\"error\":\"api power OFF\"}";

StaticJsonDocument<256> Api::getJson()
{
    String data = server.arg("plain");
    Serial.print("API: json ");
    Serial.println(data);
    StaticJsonDocument<256> json;
    DeserializationError err = deserializeJson(json, data);
    if (err)
    {
        Serial.print("### ERR: API - ");
        Serial.println(err.c_str());
    }
    
    return json;
}
String Api::getBody()
{
    String data = server.arg("plain");
    Serial.print("API: data ");
    Serial.println(data);
    
    return data;
}

Api::Api(byte ledPin)
{
    this->ledPin = ledPin;
    Serial.println("API: ready");
}

void Api::setup()
{
    pinMode(this->ledPin, OUTPUT);
    delay(250);

    this->display.setup();
    this->display.text("Wifi: loading");
    this->wifi.setup();
    if(this->wifi.isAP)
    {
      this->display.text("AP: " + this->wifi.ip);
    }
    else
    {
      this->display.text("Wifi: " + this->wifi.ip);
      this->display.text("IoT: loading");
      delay(500);
      this->iot.setup();
    }

    this->server.on("/", HTTP_GET, std::bind(&Api::conWeb, this));
    this->server.on("/", HTTP_DELETE, std::bind(&Api::conReset, this));
    this->server.on("/toggle", HTTP_POST, std::bind(&Api::conToggle, this));
    this->server.on("/wifi", HTTP_POST, std::bind(&Api::conWifiPut, this));
    this->server.on("/wifi", HTTP_GET, std::bind(&Api::conWifi, this));
    this->server.on("/wifi", HTTP_DELETE, std::bind(&Api::conWifiDel, this));
    this->server.on("/iot", HTTP_POST, std::bind(&Api::conIotPut, this));
    this->server.on("/iot", HTTP_GET, std::bind(&Api::conIot, this));
    this->server.on("/iot", HTTP_DELETE, std::bind(&Api::conIotDel, this));
    this->server.on("/iot/cert/ca", HTTP_PUT, std::bind(&Api::conIotPutCertCA, this));
    this->server.on("/iot/cert/client", HTTP_PUT, std::bind(&Api::conIotPutCertClient, this));
    this->server.on("/iot/cert/key", HTTP_PUT, std::bind(&Api::conIotPutCertKey, this));
    this->server.on("/display", HTTP_POST, std::bind(&Api::conDisplayPut, this));
    this->server.on("/display", HTTP_GET, std::bind(&Api::conDisplay, this));
    this->server.on("/display", HTTP_DELETE, std::bind(&Api::conDisplayDel, this));

    this->server.begin();
    
    this->display.text("API: ready");
}
void Api::loop()
{
    this->server.handleClient();
    //this->iot.loop();
}
void Api::conWeb()
{
    this->server.send_P(200, "text/html", WEB_HTML);
}
void Api::conToggle()
{
    digitalWrite(this->ledPin, !digitalRead(this->ledPin));
    this->server.send(204, "");
}
void Api::conIotPut()
{
    StaticJsonDocument<256> json = this->getJson();

    if (!this->iot.load(json))
    {
        this->server.send(400, "application/json", ERR_400);
        return;
    }

    this->iot.save();
    
    server.send(204, "");
}
void Api::conIotPutCertCA()
{
    String data = this->getBody();

    this->iot.setCertCA(data);
    
    server.send(204, "");
}
void Api::conIotPutCertClient()
{
    String data = this->getBody();

    this->iot.setCertClient(data);
    
    server.send(204, "");
}
void Api::conIotPutCertKey()
{
    String data = this->getBody();

    this->iot.setCertKey(data);
    
    server.send(204, "");
}
void Api::conIotDel()
{
    this->iot.reset();
    
    server.send(200, "");
}
void Api::conIot()
{
    StaticJsonDocument<256> json = this->iot.config();

    String value;
    serializeJson(json, value);

    this->server.send(json.isNull() ? 404 : 200, "application/json", value.c_str());
}
void Api::conWifiPut()
{
    if (!this->wifi.load(this->getJson()))
    {
        this->server.send(400, "application/json", ERR_400);
        return;
    }
    
    server.send(204, "");
}
void Api::conWifiDel()
{
    this->wifi.reset();
    
    server.send(200, "");
}
void Api::conWifi()
{
    StaticJsonDocument<256> json = this->wifi.config();

    String value;
    serializeJson(json, value);

    this->server.send(json.isNull() ? 404 : 200, "application/json", value.c_str());
}
void Api::conDisplayPut()
{
    StaticJsonDocument<256> json = this->getJson();

    if (!this->display.load(json))
    {
        this->server.send(400, "application/json", ERR_400);
        return;
    }
    
    this->display.save();
    
    server.send(204, "");
}
void Api::conDisplayDel()
{
    this->display.reset();
    
    server.send(200, "");
}
void Api::conDisplay()
{
    StaticJsonDocument<256> json = this->display.config();

    String value;
    serializeJson(json, value);

    this->server.send(json.isNull() ? 404 : 200, "application/json", value.c_str());
}
void Api::conReset()
{
    server.send(204, "");

    delay(500);
    Serial.println("API: Rebooting");
    ESP.restart(); //reload wifi
}
String Api::request(String url)
{
  Serial.print("url:  ");
  Serial.println(url.c_str());
  
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure(); //the magic line, use with caution
  client.connect(url, 433);

  http.begin(client, url);
  int httpCode = http.GET();
  
  Serial.print("httpCode:  ");
  Serial.println(httpCode);
  
  String payload;
  if (httpCode < 200 || httpCode >= 300) 
  {
    return payload;
  }

  payload = http.getString();
  Serial.print("payload:  ");
  Serial.println(payload.c_str());

  http.end();
  
  return payload;
}
