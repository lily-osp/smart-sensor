#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <DHT.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"
#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_USERNAME "your_username"
#define MQTT_KEY "your_key"
#define TEMP_HUMIDITY_PIN D5
#define LIGHT_INTENSITY_PIN A0

#define DHTTYPE DHT11
DHT dht(TEMP_HUMIDITY_PIN, DHTTYPE);

WiFiClient wifiClient;
Adafruit_MQTT_Client mqttClient(&wifiClient, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_KEY);

Adafruit_MQTT_Publish temperaturePublish = Adafruit_MQTT_Publish(&mqttClient, MQTT_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidityPublish = Adafruit_MQTT_Publish(&mqttClient, MQTT_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish lightIntensityPublish = Adafruit_MQTT_Publish(&mqttClient, MQTT_USERNAME "/feeds/light-intensity");

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  dht.begin();
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  
  mqttClient.connect();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int lightIntensity = analogRead(LIGHT_INTENSITY_PIN);

  // Map the values to a range of 0 to 100
  int mappedLightIntensity = map(lightIntensity, 0, 1023, 0, 100);
  int mappedSoilMoisture = map(soilMoisture, 0, 1023, 0, 100);

  temperaturePublish.publish(temperature);
  humidityPublish.publish(humidity);
  lightIntensityPublish.publish(mappedLightIntensity);
  soilMoisturePublish.publish(mappedSoilMoisture);
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Light Intensity: ");
  Serial.print(mappedLightIntensity);
  Serial.println("%");
  
  delay(10000); // Publish sensor data every 10 seconds
}
