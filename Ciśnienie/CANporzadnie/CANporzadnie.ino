#include <FlexCAN_T4.h>
#include <isotp.h>
isotp<RX_BANKS_16, 512> tp; /* 16 slots for multi-ID support, at 512bytes buffer each payload rebuild */

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> myCan;  //bieda edition
//FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> myFD;     //tylko porty na CAN 3 wspierają tą wersję
CAN_message_t msg1;
CAN_message_t msg2;

void canSniff(const CAN_message_t &msg) {
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
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
  
  msg1.id=0x0;

  // Ustawienie RX i TX: myCan.setRX(ALT); or myCAN.setTX(ALT)
}


String x;
uint8_t star;
uint8_t i;

void loop() {
  myCan.events();

  star = 0;
  x="";

  Serial.println("wpisz coś");
  while(star==0){                           //Inicjalizacja procesu   
  
      x = Serial.read();
      x.toInt();
      if(x==1){
        Serial.println("Siema");
        myCan.write(msg1);
        
        star=1;
        static uint32_t timeout = millis();
        if ( millis() - timeout < 200 ) {
            CAN_message_t msg;
            msg.id =1;
            msg.id = random(0x1,0x7FE);
            for ( uint8_t i = 0; i < 8; i++ ) msg.buf[i] = i + 1;
            myCan.write(msg);
            
            
  }
        delay(1000);    
      }
      else if(x==2){
        Serial.println("stop");
        star=2;
      }
      else{
      }
      delay(500);
  }
  

  
  if(star==1){
    i = 0;
    while(i<3){
      Serial.println("dawaj hajs urządzenie");
      //myCan.write(MB15, myFrame); 
      // Write to mailbox 15 (provided it's a transmit mailbox)
      i=i+1;
    }
  }
  else{ 
    delay(1000);
  }
}
