#include <FlexCAN_T4.h>

#include <FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

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
}

void canSniff(const CAN_message_t &msg) {
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}



String x;
uint8_t star;
uint8_t i;

void loop() {
 Can0.events();

  star = 0;
  x="";

  Serial.println("wpisz coś");
  while(star==0){                           //Inicjalizacja procesu   
  
      x = Serial.read();
      x.toInt();
      if(x==1){
        Serial.println("Siema");
       
        
        
        star=1;
  
        CAN_message_t msg;
        msg.id =1;
        for ( uint8_t i = 0; i < 8; i++ ) msg.buf[i] = 1;
        Can0.write(msg);
            
            
  
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
      CAN_message_t msg1;
      msg1.id =1;
      for (uint8_t z = 0; z < 8; z++ ) msg1.buf[z] = i+2;
      Can0.write(msg1);
      i=i+1;
    }
  }
  else{ 
    delay(1000);
  }
}