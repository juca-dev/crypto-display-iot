#include "iot.h"
#include "cert.h"

WiFiClientSecure net;

IoT::IoT(byte pin)
{
    this->ledPin = pin;
    this->client.setClient(net);
    Serial.println("IoT: ready");
}
void IoT::setCertCA(String value)
{
    this->certCA = value;
    this->storage.put("ca.pem", value);
}
void IoT::setCertClient(String value)
{
    this->certClient = value;
    this->storage.put("client.pem", value);
}
void IoT::setCertKey(String value)
{
    this->certKey = value;
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
        return false;
    }

    this->id = json["id"].as<String>();
    this->port = json["port"].as<uint16_t>();
    this->host = json["host"].as<String>();
    this->topicPub = json["pub"].as<String>();
    this->topicSub = json["sub"].as<String>();

    //    this->certCA = this->storage.get("ca.pem");
    //    this->certClient = CERT_CLIENT; //this->storage.get("client.pem");
    //    this->certKey = CERT_KEY; //this->storage.get("key.pem");

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
    int8_t TIME_ZONE = 3; //BRA: 3 UTC
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
        this->content = data;
    }
    else
    {
        Serial.print("NO-DATA => ");
        Serial.print(doc.as<String>());
    }
    Serial.println();
    this->sendData();
}

void IoT::pubSubErr(int8_t MQTTErr)
{
    if (MQTTErr == MQTT_CONNECTION_TIMEOUT)
        Serial.print("Connection tiemout");
    else if (MQTTErr == MQTT_CONNECTION_LOST)
        Serial.print("Connection lost");
    else if (MQTTErr == MQTT_CONNECT_FAILED)
        Serial.print("Connect failed");
    else if (MQTTErr == MQTT_DISCONNECTED)
        Serial.print("Disconnected");
    else if (MQTTErr == MQTT_CONNECTED)
        Serial.print("Connected");
    else if (MQTTErr == MQTT_CONNECT_BAD_PROTOCOL)
        Serial.print("Connect bad protocol");
    else if (MQTTErr == MQTT_CONNECT_BAD_CLIENT_ID)
        Serial.print("Connect bad Client-ID");
    else if (MQTTErr == MQTT_CONNECT_UNAVAILABLE)
        Serial.print("Connect unavailable");
    else if (MQTTErr == MQTT_CONNECT_BAD_CREDENTIALS)
        Serial.print("Connect bad credentials");
    else if (MQTTErr == MQTT_CONNECT_UNAUTHORIZED)
        Serial.print("Connect unauthorized");
}

void IoT::connectToMqtt(bool nonBlocking)
{
    Serial.print("MQTT connecting ");
    while (!this->client.connected())
    {
        if (this->client.connect(this->id.c_str()))
        {
            Serial.println("connected!");
            String topic = this->id + '/' + this->topicSub;
            if (this->client.subscribe(topic.c_str()))
            {
                Serial.print("subscribed: ");
                Serial.println(topic);
            }
            else
            {
                this->pubSubErr(this->client.state());
            }
        }
        else
        {
            Serial.print("failed, reason -> ");
            this->pubSubErr(this->client.state());
            if (!nonBlocking)
            {
                Serial.println(" < try again in 5 seconds");
                delay(5000);
            }
            else
            {
                Serial.println(" <");
            }
        }
        if (nonBlocking)
            break;
    }
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

    this->ntpConnect();

    //    BearSSL::X509List cert(this->certCA.c_str());
    //    BearSSL::X509List certClient(this->certClient.c_str());
    //    BearSSL::PrivateKey certKey(this->certKey.c_str());

    BearSSL::X509List cert(CERT_CA);
    BearSSL::X509List certClient(CERT_CLIENT);
    BearSSL::PrivateKey certKey(CERT_KEY);

    net.setTrustAnchors(&cert);
    net.setClientRSACert(&certClient, &certKey);

    this->client.setServer(this->host.c_str(), this->port);

    auto cb = [&](char *topic, byte *payload, unsigned int length) {
        this->messageReceived(topic, payload, length);
    };

    this->client.setCallback(cb);

    this->connectToMqtt(true);
}
void IoT::loop()
{
    now = time(nullptr);
    if (!this->client.connected())
    {
        this->connectToMqtt(true);
        this->ledOn = !this->ledOn;
        digitalWrite(this->ledPin, this->ledOn ? HIGH : LOW); //show working
    }
    else
    {
        this->content = "";
        this->client.loop();
        if (millis() - this->lastMillis > 3000) //time to wait
        {
            this->ledOn = !this->ledOn;
            digitalWrite(this->ledPin, this->ledOn ? HIGH : LOW); //show working
            this->lastMillis = millis();
        }
    }
}
