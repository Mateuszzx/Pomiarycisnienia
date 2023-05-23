#include <FlexCAN_T4.h>

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> myCan;  //bieda edition
//FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> myFD;     //tylko porty na CAN 3 wspierają tą wersję

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
  /*      Dla CANFD
  CANFD_timings_t config;
  config.clock = CLK_24MHz;
  config.baudrate = 1000000;  
  config.baudrateFD = 2000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 70;
  FD.setBaudRate(config);
  */
  
  myCan.onReceive(canSniff); 
  // allows all FIFO/message box messages to be received in the supplied callback.
  
  myCan.enableMBInterrupts(); // enables all mailboxes to be interrupt enabled
  myCan.setMaxMB(5); //stawienie liczby odbiorców
  myCan.mailboxStatus();  // to get an idea of a default initialization of the mailboxes
  delay(200);
  

  // Ustawienie RX i TX: myCan.setRX(ALT); or myCAN.setTX(ALT)

}

/*
There are 2 different message structures for CAN2.0 and CANFD.

CANFD: CANFD_message_t

CAN2.0: CAN_message_t

Both are very similar, except CANFD structure has a 64 byte payload, edl, and brs switching.

brs by default is set to 1, unless changed in the message structure.
This bit, when set, allows the higher bitrate for data to be used in CANFD mode,
otherwise nominal rate is used.

edl by default is set to 1, unless changed in the message structure.
This bit, when set, allows CANFD frames to be sent, otherwise, sends as a CAN2.0 frame,
with a truncated payload to max 8 bytes.


*/
String x;
uint8_t star;
uint8_t i;

void loop() {
  myCan.events();
  star = 0;
  x="";
  Serial.println("wpisz coś");
  while(star==0){                           //Inicjalizacja procesu   
    if (Serial.available() > 0) {
      x = Serial.readString();
    }
    if(x=="start"){
      Serial.println("Siema");
      //myCan.write(myFrame); 
      // Write to any available transmit mailbox, Note, sequential frames must use this function only.
      star=1;    
    }
    else if(x=="stop"){
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
    delay(100);
  }
}
