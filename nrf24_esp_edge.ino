#include "nrf24_esp_edge.h"


#if defined(ESP8266) |              \
    defined(ARDUINO_SAMD_MKRZERO) | \
    defined(ARDUINO_SAMD_ZERO) |    \
    defined(ARDUINO_SAM_DUE) |      \
    defined(ARDUINO_ARCH_ARC32) |   \
    defined(__TC27XX__) |           \
    (defined(TEENSYDUINO) && (TEENSYDUINO < 139))
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*(const void **)(addr))
#endif

#define CE_PIN D2
#define CSN_PIN D8
#define STATUS_LED D0

RF24 radio(CE_PIN, CSN_PIN);

uint8_t address[7] = "switch";
char uuid[50];

bool shouldSaveConfig = false;

WiFiClient espClient;
PubSubClient client(espClient);


void setup()
{
  Serial.begin(115200);
  pinMode(STATUS_LED, OUTPUT);

  blink(2);

  setup_wifi_and_mqtt();

  blink(2);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(true);
  radio.setRetries(15,15);
  radio.openReadingPipe(1, address);
  radio.startListening();

  client.publish(mqtt_topic, "radio_bridge online");

  blink(5);
}


void loop(void){
  if (!client.connected()) {
    Serial.println("Lost MQTT connection.");
    reconnect_mqtt();
    Serial.println("MQTT connection is restored.");
  }

  if(radio.available())
  {
    radio.read( &uuid, 16*2+1 );
    Serial.print("Got msg with id = ");
    Serial.println(uuid);

    Serial.print("Publish uuid ");
    Serial.print(uuid);
    Serial.print(" in topic ");
    Serial.println(mqtt_topic);
    client.publish(mqtt_topic, uuid);
    blink(2);
  }

  client.loop();
}

void setup_wifi_and_mqtt()
{
  // wifi
  WiFiManager wifiManager;
  Serial.println("Try to connect to Wifi");
  if(!wifiManager.autoConnect("esp_wifi_setup"))
  {
    Serial.println("Failed to connect to Wifi! Restart in 3 seconds.");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  Serial.println("Successfully connected to Wifi");

  // mqtt
  client.setServer(mqtt_server_address, mqtt_server_port);
  reconnect_mqtt();
}

void reconnect_mqtt() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_id, mqtt_username, mqtt_password)) {
      Serial.println(" connected");
      delay(20);
    } else {
      Serial.print(" failed, return code=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void blink(int count)
{
  digitalWrite(STATUS_LED, HIGH);
  delay(100);
  digitalWrite(STATUS_LED, LOW);
  if(count > 1)
  {
    delay(100);
    blink(count-1);
  }
}
