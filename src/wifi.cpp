#include "wifi.h"

const char WIFI_PWD[] = "123123123";
const char WIFI_SSID[] = "crypto.juca.dev";

IPAddress WIFI_LOCAL_IP(192, 168, 100, 1);
IPAddress WIFI_GATEWAY(192, 168, 100, 1);
IPAddress WIFI_NETMASK(255, 255, 255, 0);

WifiService::WifiService(byte pin)
{
    this->ledPin = pin;
    Serial.println("Wifi: ready");
}
String WifiService::ipToString(IPAddress ip)
{
    String res = "";
    for (int i = 0; i < 4; i++)
        res += i ? "." + String(ip[i]) : String(ip[i]);
    return res;
}
void WifiService::setup()
{
    this->storage.setup();

    //reset networking
    WiFi.softAPdisconnect(true);
    WiFi.disconnect();
    delay(1000);
    //check for stored credentials
    StaticJsonDocument<256> config = this->config();
    if (this->load(config))
    {
        this->isAP = false;
        this->ip = this->ipToString(WiFi.localIP());

        digitalWrite(this->ledPin, HIGH);
        Serial.println("WIFI: OK");
        Serial.println(this->ip);
    }
    else
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(WIFI_LOCAL_IP, WIFI_GATEWAY, WIFI_NETMASK);
        WiFi.softAP(WIFI_SSID, WIFI_PWD);

        this->isAP = true;
        this->ip = "192.168.100.1";

        digitalWrite(this->ledPin, LOW);
        Serial.println("Connected: NOK");
    }
    Serial.println("");
    WiFi.printDiag(Serial);
}

void WifiService::reset()
{
  this->storage.remove("wifi.json");
}
StaticJsonDocument<256> WifiService::config()
{
    String data = this->storage.get("wifi.json");
    StaticJsonDocument<256> json;
    DeserializationError err = deserializeJson(json, data);
    if (err)
    {
        Serial.print("### ERR: WIFI - ");
        Serial.println(err.c_str());
    }
    return json;
}
void WifiService::update(String ssid, String password)
{
    StaticJsonDocument<256> json;
    json["ssid"] = ssid;
    json["password"] = password;
    String value;
    serializeJson(json, value);
    this->storage.put("wifi.json", value);
}
bool WifiService::load(StaticJsonDocument<256> json)
{
    if (json.isNull() || !json.containsKey("ssid") || !json.containsKey("password"))
    {
        Serial.println("wifi: No config");
        return false;
    }

    const char *ssid = "", *pwd = "";
    ssid = json["ssid"];
    pwd = json["password"];
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwd);
    unsigned long startTime = millis();
    Serial.print("WIFI: Wait");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        digitalWrite(this->ledPin, !digitalRead(this->ledPin));
        if ((unsigned long)(millis() - startTime) >= 15000)
        {
            Serial.println("wifi: timed out (15s)");
            return false;
        }
    }
    Serial.println("");

    this->update(json["ssid"], json["password"]);
    return true;
}
