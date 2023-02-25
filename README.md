# IoT Sensor Data Publisher with ESP8266
This code is an example of how to read temperature, humidity and light intensity data from sensors using an ESP8266 microcontroller and publish the data to the Adafruit IO MQTT broker.

## Dependencies
The following libraries must be installed before using this code:

- ESP8266WiFi.h
- Adafruit_MQTT.h
- Adafruit_MQTT_Client.h
- DHT.h
- WiFiClient.h
- ESP8266WebServer.h

## Configuration
Before using this code, you need to modify the following configuration parameters in the code:

- `MQTT_SERVER:` The MQTT broker server, in this case Adafruit IO MQTT broker.
- `MQTT_PORT:` The MQTT broker port, usually 1883.
- `MQTT_USERNAME:` Your Adafruit IO username.
- `MQTT_KEY:` Your Adafruit IO secret key.
- `TEMP_HUMIDITY_PIN:` The pin to which the temperature and humidity sensor is connected.
- `LIGHT_INTENSITY_PIN:` The pin to which the light intensity sensor is connected.
- `DHTTYPE:` The type of DHT sensor, which can be DHT11, DHT21 or DHT22.

## Usage
After modifying the configuration parameters, you need to upload the code to your ESP8266 board using the Arduino IDE or any other compatible IDE. Then, connect the temperature, humidity and light intensity sensors to the appropriate pins on the board.

When the board starts up, it creates a Wi-Fi access point and starts a web server. You can connect to the access point using your smartphone or computer and enter your Wi-Fi network credentials on the web server to connect the board to your Wi-Fi network `(on v2)`. After connecting to the network, the board will connect to the MQTT broker and start publishing sensor data every 10 seconds.

The temperature, humidity and light intensity data is published to the following MQTT topics:

- `your_username/feeds/temperature`
- `your_username/feeds/humidity`
- `your_username/feeds/light-intensity`

You can view the published data on the Adafruit IO dashboard or use it for other purposes.
