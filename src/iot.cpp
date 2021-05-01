#include "iot.h"

WiFiClientSecure net;

IoT::IoT(byte pin)
{
    this->ledPin = pin;
    this->client.setClient(net);
    StaticJsonDocument<256> config = this->config();
    this->port = config["port"].as<int>();
    this->id = config["id"].as<String>();
    this->host = config["host"].as<String>();
    this->topicPub = config["topicPub"].as<String>();
    this->topicSub = config["topicSub"].as<String>();
    this->cert = config["cert"].as<String>();
    this->certClient = config["certClient"].as<String>();
    this->certKey = config["certKey"].as<String>();
    Serial.println("IoT: ready");
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
    if (json.isNull() || !json.containsKey("id") || !json.containsKey("port") || !json.containsKey("host"))
    {
        Serial.println("iot: No config");
        return false;
    }

    this->port = json["port"].as<int>();
    this->id = json["id"].as<String>();
    this->host = json["host"].as<String>();
    this->topicPub = json["topicPub"].as<String>();
    this->topicSub = json["topicSub"].as<String>();
    this->cert = json["cert"].as<String>();
    this->certClient = json["certClient"].as<String>();
    this->certKey = json["certKey"].as<String>();
    return true;
}
void IoT::ntpConnect(void)
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
        Serial.print(doc["data"].as<String>());
        if (doc["data"].containsKey("code"))
        {
            //            code = doc["data"]["code"].as<uint8_t>();
        }
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
            if (!this->client.subscribe(this->topicSub.c_str()))
                this->pubSubErr(this->client.state());
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
    //    data["code"] = code;
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
    this->ntpConnect();

    BearSSL::X509List cert(this->cert.c_str());
    BearSSL::X509List certClient(this->certClient.c_str());
    BearSSL::PrivateKey certKey(this->certKey.c_str());

    net.setTrustAnchors(&cert);
    net.setClientRSACert(&certClient, &certKey);

    this->client.setServer(this->host.c_str(), this->port);

    auto cb = [&](char *topic, byte *payload, unsigned int length) {
        this->messageReceived(topic, payload, length);
    };
    //    this->client.setCallback(this->messageReceived);
    this->client.setCallback(cb);

    this->connectToMqtt();
}
void IoT::loop()
{
    now = time(nullptr);
    if (!this->client.connected())
    {
        this->connectToMqtt();
        this->ledOn = !this->ledOn;
        digitalWrite(this->ledPin, this->ledOn ? HIGH : LOW); //show working
    }
    else
    {
        this->client.loop();
        if (millis() - this->lastMillis > 3000) //time to wait
        {
            this->ledOn = !this->ledOn;
            digitalWrite(this->ledPin, this->ledOn ? HIGH : LOW); //show working
            this->lastMillis = millis();
        }
    }
}
