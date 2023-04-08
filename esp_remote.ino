#include <WiFi.h>
#include <PubSubClient.h>
#define sub "/robot/enggang"
const char* ssid = "Msys";
const char* password = "monitor260";
const char* mqttServer = "192.168.1.100";
const char* mqttUserName = "arrafii";
const char* mqttPwd = "arrafii14";
const char* clientID = "robot";
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (80)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Terhubung");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("Server MQTT Terhubung");
      // ... and resubscribe
      client.subscribe(sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" Mencoba Ulang Dalam 5 Detik");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strstr(topic, sub)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
  } else {
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
