#include <Mqtt.h>

#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

const char* mqtt_server   = "10.29.223.67";
const char* mqtt_username = "wangziyi";
const char* mqtt_password = "123456abcd";

extern bool mqtt_status;
extern bool mqtt_init;

WiFiClient   esp_client;
PubSubClient client(esp_client);

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) { Serial.print((char)payload[i]); }
    Serial.println();
}

void mqttInit(){
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    mqtt_init = true;
}

void updateMqtt() {
    if(!client.connected()){
        mqtt_status = false;
        client.connect("ESP8266Client", mqtt_username, mqtt_password);
    } else {
        if(!mqtt_status) client.subscribe("test/topic1");
        mqtt_status = true;
    }
}

void handleMqtt(){
    client.loop();
}