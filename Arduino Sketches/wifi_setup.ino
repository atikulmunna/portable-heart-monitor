#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- Network & Cloud Credentials ---
const char* WIFI_SSID = "EXACT WIFI NAME";
const char* WIFI_PASS = "135792468";
const char* UBIDOTS_TOKEN = "BBUS-a4RBW6xMmq0YYajUjzLzOJTJFk3eEL";
const char* MQTT_BROKER = "industrial.api.ubidots.com";
const char* DEVICE_LABEL = "stm32-monitor"; // Ubidots will auto-create a device with this name

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
}

void reconnect() {
  // Loop until we are reconnected to the Ubidots broker
  while (!client.connected()) {
    Serial.print("Connecting to Ubidots MQTT...");
   
    // Generate a random client ID to avoid collisions
    String clientId = "ESP8266-Health-";
    clientId += String(random(0xffff), HEX);
   
    // Connect! (Username is Token, Password is an empty string)
    if (client.connect(clientId.c_str(), UBIDOTS_TOKEN, "")) {
      Serial.println(" Connected!");
    } else {
      Serial.print(" Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 3 seconds.");
      delay(3000);
    }
  }
}

void setup() {
  // Initialize Serial at 115200 to exactly match the STM32's transmission speed
  Serial.begin(115200);
 
  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Listen to the UART hardware port for incoming data from the STM32
  if (Serial.available()) {
    // Read the incoming string until the carriage return/newline
    String incomingData = Serial.readStringUntil('\n');
    incomingData.trim(); // Clean up hidden \r or trailing spaces

    // Validate that it looks like our expected JSON payload: {"bpm":X, ...}
    if (incomingData.startsWith("{") && incomingData.endsWith("}")) {
     
      // Parse the JSON payload
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, incomingData);

      if (!error) {
        int bpm = doc["bpm"];
        int spo2 = doc["spo2"];
        int ecg = doc["ecg"];

        // Format the payload exactly how Ubidots expects it
        char payload[128];
        snprintf(payload, sizeof(payload), "{\"bpm\":%d,\"spo2\":%d,\"ecg-signal\":%d}", bpm, spo2, ecg);

        // Define the auto-provisioning topic
        String topic = String("/v1.6/devices/") + DEVICE_LABEL;
       
        // Publish the data!
        if (client.publish(topic.c_str(), payload)) {
          // This will print to your PC serial monitor for easy debugging
          Serial.println("Data successfully published to Ubidots: " + String(payload));
        }
      }
    }
  }
}