# crypto-display-iot
Display on arduino
Connect: http://192.168.100.1

## Libraries 
Access `Sketch > Include Library > Manage Libraries...`, looking for:
 - [ArduinoJson](https://arduinojson.org/v6/example/) - v6.17.0 (by *Benoit Banchon*)
 - [Adafruit SSD1306](https://) - v2.4.4 (by *Adafruit*)
 - [PubSubClient](https://) - v2.8.0 (by *Nick O'Leary*)

## Create IOT (AWS)
- Create a policies
- FullAccess
    ```json
    {
    "Version": "2012-10-17",
    "Statement": [
        {
        "Effect": "Allow",
        "Action": "iot:*",
        "Resource": "*"
            }
        ]
        }
        ```
 - Create an device/thing in AWS IoT [ref](https://how2electronics.com/connecting-esp32-to-amazon-aws-iot-core-using-mqtt/)
    1. Create single thing 
    2. Set name
    3. Set shadow: unnamed shadow (classic)
    4. Auto generate certificate
    5. Link `FullAccess` policy
    5. Download certificates
        - Device certificate (client.crt)
        - Public key (pub.key)
        - Private key (pvt.key)
        - CA certificate: RSA 2048 bit key: Amazon Root CA 1 (ca.key)