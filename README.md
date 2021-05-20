# nRF24 Esp Edge

This simple project uses a Esp8266 microcontroller in combination with a nRF24 radio module to listen for radio messages and publish them via MQTT. The code can be compiled with the Arduino IDE. Just make sure that you run

```commandline
mv mqtt_settings_sample.h mqtt_settings.h
```

and then modify the content of `mqtt_settings.h` before you compile the code. Also adapt the nRf24 pins and the pipe address in `nrf24_esp_edge.ino` according to your configuration.