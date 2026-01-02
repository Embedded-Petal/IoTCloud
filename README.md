#  Petal Arduino Library

Petal is a universal IoT HTTP communication library for ESP32 and ESP8266.

## Features
- ESP32 & ESP8266 support
- Virtual pin read & write
- RGB HEX color parsing
- Common polling interval
- Automatic board detection

## Supported Boards
- ESP32
- ESP8266

## Installation
1. Open Arduino IDE
2. Go to Library Manager
3. Search for "Petal"
4. Install

## Example
```
#include <Petal.h>

Petal petal("WIFI", "PASS", "TOKEN");

void setup() {
  Serial.begin(115200);
  petal.begin();
  petal.setPollInterval(5000);
}

void loop() {
  String status = petal.readPin("V1");
  Petal::RGB c = petal.getRGB("V3");
}
```