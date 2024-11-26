#include <Temperature.h>

#include <Adafruit_AHTX0.h>
#include <Arduino.h>
#include <Wire.h>
#include <Commons.h>

Adafruit_AHTX0 aht;
sensors_event_t humidity, temp;
extern Temperature t_data;

void tempInit() {
    Wire.begin(AHT_SDA, AHT_SCL);
    aht.begin();
}

void updateTemperature(){
    aht.getEvent(&humidity,&temp);
    t_data.humidity = humidity.relative_humidity;
    t_data.temperature = temp.temperature;
    Serial.print("Humidity: ");
    Serial.println(t_data.humidity);
    Serial.print("Temperature: ");
    Serial.println(t_data.temperature);
}