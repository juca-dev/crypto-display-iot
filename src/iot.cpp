#include "iot.h"

WiFiClientSecure net;

IoT::IoT(byte pin)
{
    this->ledPin = pin;
    this->client.setClient(net);
    Serial.println("IoT: ready");
}
void IoT::setCertCA(String value)
{
//    this->certCA = value;
    this->storage.put("ca.pem", value);
}
void IoT::setCertClient(String value)
{
//    this->certClient = value;
    this->storage.put("client.pem", value);
}
void IoT::setCertKey(String value)
{
//    this->certKey = value;
    this->storage.put("key.pem", value);
}
void IoT::reset()
{
    this->storage.remove("iot.json");
    this->storage.remove("ca.pem");
    this->storage.remove("client.pem");
    this->storage.remove("key.pem");
}
StaticJsonDocument<256> IoT::config()
{
    String data = this->storage.get("iot.json");
    StaticJsonDocument<256> json;
    DeserializationError err = deserializeJson(json, data);
    if (err)
    {
        Serial.print("### ERR: IOT - ");
        Serial.println(err.c_str());
    }
    return json;
}
bool IoT::load(StaticJsonDocument<256> json)
{
    if (json.isNull() || !json.containsKey("id") || !json.containsKey("port") || !json.containsKey("host") || !json.containsKey("pub") || !json.containsKey("sub"))
    {
        Serial.println("iot: No config");
        this->ready = false;
        return false;
    }

    this->id = json["id"].as<String>();
    this->port = json["port"].as<uint16_t>();
    this->host = json["host"].as<String>();
    this->topicPub = json["pub"].as<String>();
    this->topicSub = json["sub"].as<String>();
    
    this->ready = true;

    return true;
}
void IoT::save()
{
    StaticJsonDocument<256> json;
    json["id"] = this->id;
    json["port"] = this->port;
    json["host"] = this->host;
    json["pub"] = this->topicPub;
    json["sub"] = this->topicSub;

    String value;
    serializeJson(json, value);
    this->storage.put("iot.json", value);
}
void IoT::ntpConnect()
{
    Serial.print("Setting time using SNTP");
    int8_t TIME_ZONE = -3; //BRA: 3 UTC
    configTime(TIME_ZONE * 3600, 0, "pool.ntp.org", "time.nist.gov");
    this->now = time(nullptr);
    while (this->now < this->nowish)
    {
        delay(500);
        Serial.print(".");
        this->now = time(nullptr);
    }
    Serial.println("done!");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}

void IoT::messageReceived(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Received [");
    Serial.print(topic);
    Serial.print("]: ");

    DynamicJsonDocument doc(JSON_OBJECT_SIZE(3) + 100);
    deserializeJson(doc, payload);
    if (doc.containsKey("data"))
    {
        String data = doc["data"].as<String>();
        Serial.print(data);
        this->message = data;
    }
    else
    {
        Serial.print("NO-DATA => ");
        Serial.print(doc.as<String>());
    }
    Serial.println();
//    this->sendData();
}

void IoT::pubSubErr(int8_t MQTTErr)
{
    if (MQTTErr == MQTT_CONNECTION_TIMEOUT)
        Serial.println("Connection tiemout");
    else if (MQTTErr == MQTT_CONNECTION_LOST)
        Serial.println("Connection lost");
    else if (MQTTErr == MQTT_CONNECT_FAILED)
        Serial.println("Connect failed");
    else if (MQTTErr == MQTT_DISCONNECTED)
        Serial.println("Disconnected");
    else if (MQTTErr == MQTT_CONNECTED)
        Serial.println("Connected");
    else if (MQTTErr == MQTT_CONNECT_BAD_PROTOCOL)
        Serial.println("Connect bad protocol");
    else if (MQTTErr == MQTT_CONNECT_BAD_CLIENT_ID)
        Serial.println("Connect bad Client-ID");
    else if (MQTTErr == MQTT_CONNECT_UNAVAILABLE)
        Serial.println("Connect unavailable");
    else if (MQTTErr == MQTT_CONNECT_BAD_CREDENTIALS)
        Serial.println("Connect bad credentials");
    else if (MQTTErr == MQTT_CONNECT_UNAUTHORIZED)
        Serial.println("Connect unauthorized");
    else 
        Serial.println("MQTT Error unknow: " + String(MQTTErr));
}

void IoT::connectToMqtt(bool nonBlocking)
{
    if (!this->ready)
    {
        Serial.println("MQTT not configured!");
        delay(5000);
        return;
    }
    
    Serial.print("Connecting to AWS IOT thing name: ");Serial.println(this->id.c_str());
    while (!this->client.connect(this->id.c_str()))
    {
        Serial.print(".");
        delay(1000);
    }
    
    if (!this->client.connected()) {
        Serial.println("AWS IoT Timeout!");
        return;
    }


    Serial.println("AWS IoT Connected!");

    String topic = this->id + '/' + this->topicSub;
    if (!this->client.subscribe(topic.c_str()))
    {
        Serial.print("subscribed failed");
        Serial.println(topic);
        this->pubSubErr(this->client.state());
        return;
    }

    Serial.print("subscribed: ");
    Serial.println(topic);
}

void IoT::sendData()
{
    DynamicJsonDocument jsonBuffer(JSON_OBJECT_SIZE(3) + 100);
    JsonObject root = jsonBuffer.to<JsonObject>();
    JsonObject data = root.createNestedObject("data");
    data["ok"] = true;
    data["id"] = this->id;
    Serial.printf("Sending  [%s]: ", this->topicPub.c_str());
    serializeJson(root, Serial);
    Serial.println();
    char shadow[measureJson(root) + 1];
    serializeJson(root, shadow, sizeof(shadow));
    if (!this->client.publish(this->topicPub.c_str(), shadow, false))
        this->pubSubErr(this->client.state());
}
void IoT::setup()
{
    this->storage.setup();

    StaticJsonDocument<256> config = this->config();
    if (!this->load(config))
    {
        Serial.println("IoT: Not configured!");
        return;
    }

    char* certCA = this->storage.get2("ca.pem");
    char* certCli = this->storage.get2("client.pem");
    char* certPvt = this->storage.get2("key.pem");
    if (!certCA || !certCli || !certPvt)
    {
        Serial.println("IoT: Not certificate!");
        return;
    }

    this->ntpConnect();

    BearSSL::X509List cert(certCA);
    BearSSL::X509List certClient(certCli);
    BearSSL::PrivateKey certKey(certPvt);

    net.setTrustAnchors(&cert);
    net.setClientRSACert(&certClient, &certKey);

    Serial.print("AWS IOT ENDPOINT: ");Serial.println(this->host.c_str());
    Serial.print("AWS IOT PORT: ");Serial.println(this->port);
    this->client.setServer(this->host.c_str(), this->port);

    auto cb = [&](char *topic, byte *payload, unsigned int length) {
        this->messageReceived(topic, payload, length);
    };

    this->client.setCallback(cb);

    this->connectToMqtt(true);
}
void IoT::loop()
{
    if (!this->client.connected())
    {
        Serial.println("AWS IoT not connected!");
        this->connectToMqtt(true);
        // digitalWrite(this->ledPin, HIGH); //show working
        return;
    }
    
    this->message = "";
    this->client.loop();
    if (millis() - this->lastMillis > 3000) //time to wait
    {
        this->ledOn = !this->ledOn;
        // digitalWrite(this->ledPin, this->ledOn ? HIGH : LOW); //show working
        this->lastMillis = millis();
    }
}
