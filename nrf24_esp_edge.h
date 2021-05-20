#ifndef NRF24_ESP_EDGE_H_
#define NRF24_ESP_EDGE_H_

#include "RF24.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include "mqtt_settings.h"


void setup_wifi_and_mqtt();
void reconnect_mqtt();
void blink(int count);


#endif /* NRF24_ESP_EDGE_H_ */
