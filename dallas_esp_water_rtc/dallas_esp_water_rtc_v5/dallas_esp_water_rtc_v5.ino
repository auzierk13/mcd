
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Time.h>
#include <DS1302RTC.h>
#include <TimeLib.h>

#define baundrate 9600
#define maxAnalog 400


// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 10
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

SoftwareSerial esp(2,3); // RX, TX

// Creation of the Real Time Clock Object
// Determina os pinos ligados ao modulo
// myRTC(rst, data, clock )
byte RTC_CLOCK= 6;
byte RTC_DATA= 7;
byte RTC_RST= 8;

DS1302RTC RTC(RTC_RST, RTC_DATA, RTC_CLOCK);

#define NOALERTA  -1
#define TEMPERATURA  0
#define UMIDADE  1
#define HEART  2

#define FEBRE "Bebê com febre"
#define URINA "Bebê esta urinado"
#define Ritimia "Coração acelerado"


//byte 0- 255

byte timeMin;
byte timeSeg;
byte timeHor;

float mediaTemperatura=0;
int mediaUmidade=0;
unsigned int index=0;


void setup() {
 // Open serial communications and wait for port to open:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(baundrate);//9600  // start serial port
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the data rate for the SoftwareSerial port
  esp.begin(baundrate); // start serial port
  esp.println(F("AT\r"));

  connectWifi();
  setupRTC();
  getDateTime();
  setupDs18b20();
  
  tmElements_t tm; 
  if (! RTC.read(tm)) {
    timeMin=  tm.Minute;
    timeSeg= tm.Second;
    timeHor= tm.Hour;
    
  }
  
}



void loop() {
   // Le as informacoes do CI
   tmElements_t tm;
  
 byte nowsegundo;
 byte timeMinNow;
 byte timeHorNow;
 
  if (! RTC.read(tm)) {
    nowsegundo= tm.Second;
    timeMinNow= tm.Minute;
    timeHorNow=tm.Hour;
  }
if (esp.available()) {
    Serial.write(esp.read());
  }
  if (Serial.available()) {
    esp.write(Serial.read());
    
  }


  if(timeHorNow> timeHor){
    Serial.print(F("*******Passou hora "));
    Serial.print(timeHor);
    Serial.println(F(" ********"));
    timeMin=0;
    timeHor=timeHorNow;
  }
  
  if(timeMinNow> timeMin){
    Serial.print(F("*******Passou minuto "));
    Serial.print(timeMin);
    Serial.println(F(" ********"));
    
    if(!index==0){    
        Serial.print(F("********Media Tempearatura*******"));
        mediaTemperatura= mediaTemperatura/index;
        Serial.println( mediaTemperatura);
        
        Serial.print(F("********Media Umidade*******"));
        mediaUmidade= mediaUmidade/index;
        Serial.println( mediaUmidade);
        
        coletaDados();
    }
    
    timeMin=timeMinNow;
    timeSeg=mediaTemperatura=index=0;
    
    }else{
//    Serial.println(F("No memso minuto ")); getDateTime();
      if(nowsegundo> timeSeg){
      index++;
      printTemperature(); // Use a simple function to print out the data
      printUmidade();
      timeSeg=nowsegundo;
      getDateTime();  
      }
    }
}



