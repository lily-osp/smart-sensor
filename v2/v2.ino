#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <DHT.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

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

ESP8266WebServer server(80);

void handleRoot() {
  String html = "<html><body><h1>Enter WiFi Credentials</h1>";
  html += "<form method='post'>";
  html += "SSID: <input type='text' name='ssid'><br>";
  html += "Password: <input type='text' name='password'><br>";
  html += "<input type='submit' value='Submit'></form></body></html>";
  server.send(200, "text/html", html);
}

void handlePost() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");
  String html = "<html><body><h1>Credentials Received</h1>";
  html += "<p>SSID: " + ssid + "</p>";
  html += "<p>Password: " + password + "</p></body></html>";
  server.send(200, "text/html", html);

  // Connect to the WiFi network with received credentials
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Connecting to WiFi network ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("Connected to WiFi");

  mqttClient.connect();
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  dht.begin();

  // Start the web server
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ESP8266_AP", "password");
  Serial.println("Access point started with SSID: ESP8266_AP and password: password");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/", HTTP_POST, handlePost);
  server.begin();
}

void loop() {
  server.handleClient();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int lightIntensity = analogRead(LIGHT_INTENSITY_PIN);

  // Map the values to a range of 0 to 100
  int mappedLightIntensity = map(lightIntensity, 0, 1023, 0, 100);

  temperaturePublish.publish(temperature);
  humidityPublish.publish(humidity);
  lightIntensityPublish.publish(mappedLightIntensity);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Light Intensity: ");
  Serial.print(mappedLightIntensity);
  Serial.println("%");

  delay(10000); // Publish sensor data every 10 seconds
}
