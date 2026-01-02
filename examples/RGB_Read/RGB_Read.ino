#include <Petal.h>

Petal petal("WIFI", "PASS", "TOKEN");

void setup() {
  Serial.begin(115200);
  petal.begin();
  petal.setPollInterval(5000);
}

void loop() {
  Petal::RGB c = petal.getRGB("V3");
  Serial.print("R=");
  Serial.print(c.r);
  Serial.print(" G=");
  Serial.print(c.g);
  Serial.print(" B=");
  Serial.println(c.b);
  delay(2000);
}
