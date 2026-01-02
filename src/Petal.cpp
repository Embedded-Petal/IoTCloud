#include "Petal.h"

/* ================= CONSTRUCTOR ================= */
Petal::Petal(const char* ssid,
             const char* password,
             const char* token)
{
  _ssid = ssid;
  _password = password;
  _token = token;

  _pollInterval = 5000; // default

  for (int i = 0; i < 10; i++) {
    _lastReadValue[i] = "";
    _lastReadTime[i]  = 0;
    _lastWriteValue[i] = -1;
    _lastRGB[i] = {0, 0, 0};
  }
}

/* ================= WIFI ================= */
void Petal::begin()
{
  Serial.print("WiFi Connecting");
  WiFi.begin(_ssid, _password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());
}

/* ================= CONFIG ================= */
void Petal::setPollInterval(unsigned long intervalMs)
{
  _pollInterval = intervalMs;
}

/* ================= PIN INDEX ================= */
int Petal::pinIndex(String pin)
{
  pin.replace("V", "");
  return pin.toInt();
}

/* ================= WRITE ================= */
bool Petal::pushPin(String pin, int value)
{
  int idx = pinIndex(pin);
  if (_lastWriteValue[idx] == value) return true;

  if (WiFi.status() != WL_CONNECTED) return false;

  String url = String("https://")+ PETAL_SERVER_IP + "/api/iot/mcu?token=" + String(_token) + "&" + pin + "=" + String(value);

#ifdef PETAL_BOARD_ESP32
  HTTPClient https;
  https.begin(url);
#endif

#ifdef PETAL_BOARD_ESP8266
  WiFiClient client;
  HTTPClient https;
  https.begin(client, url);
#endif

  https.setTimeout(8000);
  int code = https.GET();
  https.end();

  if (code > 0) {
    _lastWriteValue[idx] = value;
    return true;
  }
  return false;
}

/* ================= COMMON FETCH ================= */
void Petal::fetchFromServer(String pin)
{
  int idx = pinIndex(pin);

  if (millis() - _lastReadTime[idx] < _pollInterval) return;
  _lastReadTime[idx] = millis();

  if (WiFi.status() != WL_CONNECTED) return;

  String url = String("https://")+ PETAL_SERVER_IP+"/api/iot/get?token=" + String(_token) +"&pin=" + pin;

#ifdef PETAL_BOARD_ESP32
  HTTPClient https;
  https.begin(url);
#endif

#ifdef PETAL_BOARD_ESP8266
  WiFiClient client;
  HTTPClient https;
  https.begin(client, url);
#endif

  https.setTimeout(8000);
  int code = https.GET();
  if (code <= 0) {
    https.end();
    return;
  }

  String payload = https.getString();
  https.end();

  DynamicJsonDocument doc(512);
  deserializeJson(doc, payload);

  if (!doc["success"]) return;

  String value = doc["data"]["value"].as<String>();
  _lastReadValue[idx] = value;

  // RGB parse only if HEX
  if (isHexColor(value)) {
    value.replace("#", "");
    _lastRGB[idx].r = hexToByte(value.substring(0, 2));
    _lastRGB[idx].g = hexToByte(value.substring(2, 4));
    _lastRGB[idx].b = hexToByte(value.substring(4, 6));
  }
}

/* ================= READ STRING ================= */
String Petal::readPin(String pin)
{
  fetchFromServer(pin);
  return _lastReadValue[pinIndex(pin)];
}

/* ================= READ RGB ================= */
Petal::RGB Petal::getRGB(String pin)
{
  fetchFromServer(pin);
  return _lastRGB[pinIndex(pin)];
}

/* ================= HELPERS ================= */
bool Petal::isHexColor(String value)
{
  value.replace("#", "");
  if (value.length() != 6) return false;

  for (int i = 0; i < 6; i++) {
    if (!isxdigit(value[i])) return false;
  }
  return true;
}

uint8_t Petal::hexToByte(String hex)
{
  return (uint8_t) strtol(hex.c_str(), NULL, 16);
}
