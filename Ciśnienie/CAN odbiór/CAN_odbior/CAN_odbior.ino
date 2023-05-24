#include <HoneywellTruStabilitySPI.h>
#include <FlexCAN_T4.h>



float x = 0;
int y;

#define SLAVE_SELECT_PIN SS
TruStabilityPressureSensor sensor( SLAVE_SELECT_PIN, -15.0, 15.0 );

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> myCan;




void canSniff(const CAN_message_t &msg) {
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print(" OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print(" LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" BUS: "); Serial.print(msg.bus);
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
void setup() {
  Serial.begin(115200);
  delay(400);

  myCan.begin();
  myCan.setBaudRate(1000000);         //dla CAN2.0
  myCan.setMaxMB(16);
  myCan.enableFIFO();
  myCan.enableFIFOInterrupt();
  myCan.onReceive(canSniff); 
  myCan.mailboxStatus();
 
  delay(200);
  
  //tp.begin();
  //tp.setWriteBus(&Can1); /* we write to this bus */
  //tp.onReceive(myCallback); /* set callback */
  SPI.begin(); // start SPI communication
  sensor.begin(); // run sensor initialization
}

void loop() {
  myCan.events();
  y = Serial.read();
  if(y==5){
    measure();
    
  }
  delay(1000);
}
