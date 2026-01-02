#ifndef PETAL_CONFIG_H
#define PETAL_CONFIG_H

/************* SERVER CONFIG *************/
#define PETAL_SERVER_IP   "api.iotcloud.petalred.com"
#define PETAL_SERVER_PORT 8080

/************* BOARD AUTO DETECTION *************/
#if defined(ESP32)
  #define PETAL_BOARD_ESP32
#elif defined(ESP8266)
  #define PETAL_BOARD_ESP8266
#else
  #error "Petal supports only ESP32 and ESP8266"
#endif

/************* COMMON LIBS *************/
#include <Arduino.h>
#include <ArduinoJson.h>

/************* BOARD SPECIFIC LIBS *************/
#ifdef PETAL_BOARD_ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif

#ifdef PETAL_BOARD_ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#endif
