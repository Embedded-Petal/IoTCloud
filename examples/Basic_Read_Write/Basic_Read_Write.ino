#include <Petal.h>

Petal petal("WIFI", "PASS", "TOKEN");

void setup() {
  Serial.begin(115200);
  petal.begin();
  petal.setPollInterval(5000);
}

void loop() {
  petal.pushPin("V0", 1);
  String v = petal.readPin("V1");
  Serial.println(v);
  delay(2000);
}
