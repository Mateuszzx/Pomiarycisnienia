#include <HoneywellTruStabilitySPI.h>

#include <FlexCAN_T4.h>
#include <isotp.h>
#include <HoneywellTruStabilitySPI.h>
float x = 0;
int y;
#define SLAVE_SELECT_PIN SS
TruStabilityPressureSensor sensor( SLAVE_SELECT_PIN, -15.0, 15.0 );
isotp<RX_BANKS_16, 512> tp; /* 16 slots for multi-ID support, at 512bytes buffer each payload rebuild */

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;


void myCallback(const ISOTP_data &config, const uint8_t *buf) {
  Serial.print("ID: ");
  Serial.print(config.id, HEX);
  Serial.print("\tLEN: ");
  Serial.print(config.len);
  Serial.print("\tFINAL ARRAY: ");
  for ( int i = 0; i < config.len; i++ ) {
    Serial.print(buf[i], HEX);
    Serial.print(" ");
  } Serial.println();
}

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
  Serial.begin(115200); delay(400);
  Can1.begin();
  Can1.setClock(CLK_60MHz);
  Can1.setBaudRate(95238);
  Can1.setMaxMB(16);
  Can1.enableFIFO();
  Can1.enableFIFOInterrupt();
  //  Can1.onReceive(canSniff);
  tp.begin();
  tp.setWriteBus(&Can1); /* we write to this bus */
  tp.onReceive(myCallback); /* set callback */
  SPI.begin(); // start SPI communication
  sensor.begin(); // run sensor initialization
}

void loop() {
  y = Serial.read();
  if(y==5){
    measure();
    
  }
  delay(1000);
}
