#include <SPI.h>

const int SlavePin = 10;
SPISettings settings(2000000, MSBFIRST, SPI_MODE1);

void setup() {
  pinMode(SlavePin, OUTPUT);
  SPI.begin();
  SPI.pinIsMISO(11);
  Serial.begin(9600);
}

uint8_t stat;

void loop() {
  SPI.beginTransaction(settings);
  digitalWrite(SlavePin, LOW);
  stat = SPI.transfer(0);
  digitalWrite(SlavePin, HIGH);
  SPI.endTransaction();
  
  Serial.println(stat);
  delay(500);
}
