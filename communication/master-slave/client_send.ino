#include <ESP8266WiFi.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson (use v6.xx)
#include <time.h>
#ifndef STASSID
#define STASSID "Fiifi"
#define STAPSK  "fiifi1234"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "master-ip"; //IP-address of the master
const uint16_t port = 17;

void getData(float current, float temperature, float voltage){
    // Get the data from sensor and store as json 
    current = 48.748010;
    temperature = 120.89;
    voltage = 9.3; 

        // Allocate JsonBuffer
    const int capacity = JSON_ARRAY_SIZE(2) + 4 * JSON_OBJECT_SIZE(2);
    StaticJsonBuffer<capacity> jb;
    // Create JsonObject
    JsonObject &root = jb.createObject();
    // Add data
    JsonObject &data = root.createNestedObject("data");
    data["current"] = current;
    data["voltage"] = voltage;
    data["temperature"] = temperature; 

}

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  // This will send sensor data to the master
  Serial.println("sending data to server");
  if (client.connected()) {



  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  delay(300000); // execute once every 5 minutes, don't flood remote service
}