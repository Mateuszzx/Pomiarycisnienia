#include <HoneywellTruStabilitySPI.h>
#include <FlexCAN_T4.h>
#include <isotp.h>
isotp<RX_BANKS_16, 512> tp; /* 16 slots for multi-ID support, at 512bytes buffer each payload rebuild */


FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

uint8_t odbior;
float x = 0;
int y;
uint32_t teta;
//int i=0;

int pomiary;

struct dp{
  uint32_t teta;
  int32_t x;
};

dp tab[41]={};

#define SLAVE_SELECT_PIN SS
TruStabilityPressureSensor sensor( SLAVE_SELECT_PIN, -15.0, 15.0 );

void setup(void) {
  Serial.begin(115200); delay(400);
  pinMode(6, OUTPUT); digitalWrite(6, LOW); /* optional tranceiver enable pin */
  Can0.begin();
  Can0.setClock(CLK_60MHz);
  Can0.setBaudRate(1000000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  //Can0.mailboxStatus();
  SPI.begin(); // start SPI communication
  sensor.begin(); // run sensor initialization
  tp.begin();
  tp.setWriteBus(&Can0); /* we write to this bus */
  tp.onReceive(myCallback); /* set callback */
  odbior = 5;
}

void myCallback(const ISOTP_data &config, const uint8_t *buf) {
  /*Serial.print("ID: ");
  Serial.print(config.id, HEX);
  Serial.print("\tLEN: ");
  Serial.print(config.len);
  Serial.print("\tFINAL ARRAY: ");
  for ( int i = 0; i < config.len; i++ ) {
    Serial.print(buf[i], HEX);
    Serial.print(" ");
  } Serial.println();
}
  */
}

void canSniff(const CAN_message_t &msg) {
  odbior = msg.buf[0];
  /*Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
  */
  
}

/*
void measure(){
  if( sensor.readSensor() == 0 ) {
    x = sensor.pressure();
    x=x-1.46;
    x=x*0.702829;
    Serial.print("Pomiar ciśnienia w mH2O ");
    Serial.println(x);
  }
}
*/

/*
void sending(){
  dp pomiary[41]={};
  uint32_t p;
  uint32_t timing;
  tab = pomiary;
  for(uint8_t i =0; i<=40; i++){
    p = pomiary[i].x;
    timing = pomiary[i].teta;
    uint8_t pressure_count[4];
    uint8_t timing_count[4];
    
    for(uint8_t j=0; j<4; j++){
      uint8_t z = 8*i;
      pressure_count[j] = ( (p) >> z & 0xFF);
      timing_count[j] = ( (p) >> z & 0xFF);
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
*/

void loop() {
  Can0.events();
  
  if(odbior==1){
    //dp tab[41]={};
    for(uint8_t i=0;i<=40;i++){
      teta = micros();
      tab[i].teta=teta;
        if( sensor.readSensor() == 0 ) {
          x = sensor.pressure();         
          tab[i].x=x;
          }
      //Serial.println(tab[i].teta);
      //Serial.println(tab[i].x);  
      
      }
      Serial.println("Wykonałem pomiary");
      
      odbior =5;
  }

  else if(odbior==2){
    Serial.println("Wysyłam");
    
    //sending();
    
    /*
    uint32_t p;
    uint32_t timing;
   for(uint8_t h=0;h<=40; h++){
      Serial.print(tab[h].x);
    } 
    */
    
    uint8_t czas = 0;

    while(czas<41){
      uint32_t p;
      uint32_t timing;
      
      p = tab[czas].x;
      timing = tab[czas].teta;
      
      /*
      uint8_t pressure_count[4]={};
      uint8_t timing_count[4]={};
      //uint8_t count[8]={};
    
      for(uint8_t j=0; j<4; j++){
        uint8_t z ;
        z= 8*j;
        pressure_count[j] = ( (p) >> z & 0xFF);
        timing_count[j] = ( (timing) >> z & 0xFF);
        Serial.println(timing_count[j]);
        }
      */
      uint8_t p1;
      uint8_t p2;
      uint8_t p3;
      uint8_t p4;
      uint8_t t1;
      uint8_t t2;
      uint8_t t3;
      uint8_t t4;

      p1 = ((p) >> 24 & 0xFF);
      p2 = ((p) >> 16 & 0xFF);
      p3 = ((p) >> 8 & 0xFF);
      p4 = ((p)  & 0xFF);

      t1 = ( (timing) >> 24 & 0xFF);
      t2 = ( (timing) >> 16 & 0xFF);
      t3 = ( (timing) >> 8 & 0xFF);
      t4 = ( (timing)  & 0xFF);
      /*CAN_message_t msg5;

   
      msg5.id =5;
      msg5.buf[0]=p1;
      msg5.buf[1]=p2;
      msg5.buf[2]=p3;
      msg5.buf[3]=p4;
      msg5.buf[4]=t1;
      msg5.buf[5]=t2;
      msg5.buf[6]=t3;
      msg5.buf[7]=t4;
      */
//Serial.println("zawartosc buf: " + p4 + msg5.buf[0] + msg5.buf[1] );
//Serial.print(msg5.len);
      //for (uint8_t z = 0; z < 4; z++ ){
        //msg5.buf[z] = pressure_count[z];
        //msg5.buf[z+4] = timing_count[z];    
      //}
      /*for (uint8_t d = 0; d < 4; d++ ){
        Serial.println(msg5.buf[d]);
           
      }
      */
      /*
      msg5.seq=1;
      Can0.write(msg5);
      */
      ISOTP_data config;
      uint8_t buf[] ={p1, p2, p3, p4, t1, t2, t3, t4};
      config.id = 0x666;
      config.flags.extended = 0; /* standard frame */
      config.separation_time = 10; /* time between back-to-back frames in millisec */
      tp.write(config, buf, sizeof(buf));
    
      

      Serial.print("Wysłalem pomiar nr ");
      Serial.println(czas);
      //Serial.print(p);
      //Serial.print(tab[i].x);
      czas=czas+1;
      delay(1);
    }
    odbior =5;
  }
  else{

  }
  delay(1000);
}
