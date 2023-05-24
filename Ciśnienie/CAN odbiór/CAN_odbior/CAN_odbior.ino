#include <HoneywellTruStabilitySPI.h>
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

uint8_t odbior;
float x = 0;

#define SLAVE_SELECT_PIN SS
TruStabilityPressureSensor sensor( SLAVE_SELECT_PIN, -15.0, 15.0 );

void setup(void) {
  Serial.begin(115200); delay(400);
  pinMode(6, OUTPUT); digitalWrite(6, LOW); /* optional tranceiver enable pin */
  Can0.begin();
  Can0.setBaudRate(1000000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();
  SPI.begin(); // start SPI communication
  sensor.begin(); // run sensor initialization
}

void canSniff(const CAN_message_t &msg) {
  odbior = msg.buf[0];
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
  
}


void measure(){
  if( sensor.readSensor() == 0 ) {
    x = sensor.pressure();
    x=x-1.46;
    x=x*0.702829;
    Serial.print("Pomiar ciśnienia w mH2O ");
    Serial.println(x);
  }
}


void loop() {
  Can0.events();

  if(odbior==5){
    measure();
    Serial.println("Wykonałem pomiary");
  }
  else if(odbior==2){
    Serial.println("Wysyłam");
  }
  else{

  }
  delay(1000);
}
