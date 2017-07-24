/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.
//netvirtua87
//2097527020
 */
///AT+CWJAP_DEF="NET_2G7A5441","B37A5441"
//AT+CIFSR – Get local IP address
//AT+CIPSTATUS – Information about connection
//AT+CWMODE_DEF?
//AT+CWMODE_DEF=1 mode station
//AT+CWDHCP_DEF
//AT+CIPSTART="TCP","www.baidu.com",80,1
//AT+RST – Restart module

//#define CH_PD 4 //sinal de controle de CH_PD
//#define RST 5 //sinal de controle de RST

#define baundrate 9600 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9,10); // RX, TX

void setup() {

//   pinMode(CH_PD,OUTPUT);
//    pinMode(RST,OUTPUT);
//    
//  digitalWrite(CH_PD,HIGH); //Setado em alto - funcionamento normal
//  digitalWrite(RST,HIGH); //RST em alto - funcionamento normal
  
  // Open serial communications and wait for port to open:
  Serial.begin(baundrate);//9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(baundrate);
  mySerial.println("AT\r");
}

void loop() { // run over and over
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

   
}

