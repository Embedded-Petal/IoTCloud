#ifndef PETAL_H
#define PETAL_H

#include "Petal_Config.h"

class Petal {
  public:
    struct RGB {
      uint8_t r;
      uint8_t g;
      uint8_t b;
    };

    Petal(const char* ssid,
          const char* password,
          const char* token);

    void begin();
    void setPollInterval(unsigned long intervalMs);

    // WRITE
    bool pushPin(String pin, int value);

    // READ
    String readPin(String pin);   // normal string
    RGB getRGB(String pin);       // RGB only

  private:
    const char* _ssid;
    const char* _password;
    const char* _token;

    unsigned long _pollInterval;

    String _lastReadValue[10];
    unsigned long _lastReadTime[10];
    int _lastWriteValue[10];
    RGB _lastRGB[10];

    int pinIndex(String pin);
    void fetchFromServer(String pin);

    bool isHexColor(String value);
    uint8_t hexToByte(String hex);
};

#endif
