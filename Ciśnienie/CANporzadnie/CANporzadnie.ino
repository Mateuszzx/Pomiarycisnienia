#include <FlexCAN_T4.h>
#include <isotp.h>
isotp<RX_BANKS_16, 512> tp; /* 16 slots for multi-ID support, at 512bytes buffer each payload rebuild */
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

void setup(void) {
  Serial.begin(115200); delay(400);
  pinMode(6, OUTPUT); digitalWrite(6, LOW); /* optional tranceiver enable pin */
  Can0.begin();
  Can1.setClock(CLK_60MHz);
  Can0.setBaudRate(1000000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();
  tp.begin();
  tp.setWriteBus(&Can0); /* we write to this bus */
  tp.onReceive(myCallback); /* set callback */
}

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
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
  /*CAN_message_t msg2;
  msg2.id =1;
  for ( uint8_t i = 0; i < 8; i++ ) msg2.buf[i] = 8;
  Can0.write(msg2);
  */

}



String x;
uint8_t star;
uint8_t i;

void loop() {
 Can0.events();

  star = 0;
  x="";

  Serial.println("Wpisz 1 aby rozpocząć pomiar, lub 2 by pobrać nowe dane");
  while(star==0){                           //Inicjalizacja procesu   
  
      x = Serial.read();
      x.toInt();

      if(x==1){
        Serial.println("Rozpoczynam wykonanie pomiaru");
        star=1;
        CAN_message_t msg;
        msg.id =1;
        for ( uint8_t i = 0; i < 8; i++ ) msg.buf[i] = 1;
        Can0.write(msg);
        delay(1000);    
      }
      else if(x==2){
        Serial.println("Wysyłam prośbę o wyniki pomiaru");
        star=2;
      }
      else{
      }
      delay(500);
  }
  
  if(star==2){
    i = 0;
    while(i<3){
      
      CAN_message_t msg1;
      msg1.id =1;
      for (uint8_t z = 0; z < 8; z++ ){
        msg1.buf[z] = i+2; 
      }
      Can0.write(msg1);
      i=i+1;
      
    
      delay(1000);
    }
  }
  else{
    delay(1000);
  }
}