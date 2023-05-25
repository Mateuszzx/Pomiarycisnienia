#include <HoneywellTruStabilitySPI.h>
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

uint8_t odbior;
float x = 0; //wartość ciśnienia


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

void sending(uint pomiary){
  float p;
  uint32_t timing;
  for(uint8_t i =0; i<=40; i++){
    pomiary[i].x = p;
    pomiary[i].teta = timing;
    uint8_t pressure_count[4];
    uint8_t timing_count[4];
    for(uint8_t i=0; i<4; i++){
      uint8_t z = 8*i;
      pressure_count[i] = ( (p) >> z & 0xFF);
      timing_count[i] = ( (p) >> z & 0xFF);
      CAN_message_t msg;
      msg.id =1;
      for (uint8_t z = 0; z < 4; z++ ){
        msg.buf[z] = pressure_count[z];
        msg.buf[z+4] = timing_count[z];

      }
      Can0.write(msg);
    }
  } 
}

void loop() {
  Can0.events();

  if(odbior==0){
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
