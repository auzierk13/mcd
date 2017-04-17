//  __  __   __      _       _                   _      _              _            _           _           
// |  \/  | /_/     | |     | |                 | |    | |            | |          | |         | |          
// | \  / | ___   __| |_   _| | ___     ___ ___ | | ___| |_ __ _    __| | ___    __| | __ _  __| | ___  ___ 
// | |\/| |/ _ \ / _` | | | | |/ _ \   / __/ _ \| |/ _ | __/ _` |  / _` |/ _ \  / _` |/ _` |/ _` |/ _ \/ __|
// | |  | | (_) | (_| | |_| | | (_) | | (_| (_) | |  __| || (_| | | (_| |  __/ | (_| | (_| | (_| | (_) \__ \
// |_|  |_|\___/ \__,_|\__,_|_|\___/   \___\___/|_|\___|\__\__,_|  \__,_|\___|  \__,_|\__,_|\__,_|\___/|___/
//                                                                                                          
//                                                                                                          


#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <virtuabotixRTC.h>    

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
// myRTC(clock, data, rst)
byte RTC_CLOCK= 6;
byte RTC_DATA= 5;
byte RTC_RST= 4;

#define NOALERTA  -1
#define TEMPERATURA  0
#define UMIDADE  1
#define HEART  2

#define FEBRE "Bebê com febre"
#define URINA "Bebê esta urinado"
#define Ritimia "Coração acelerado"

virtuabotixRTC myRTC(RTC_CLOCK, RTC_DATA, RTC_RST);
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
  
  timeMin= myRTC.minutes;
  timeSeg= myRTC.seconds;
}



void loop() {
   // Le as informacoes do CI
   myRTC.updateTime();
  byte nowsegundo= myRTC.seconds;
  byte timeHorNow= myRTC.hours;
  byte timeMinNow= myRTC.minutes;
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
//    Serial.println("No memso minuto "); getTime();
      if(nowsegundo> timeSeg){
      index++;
      printTemperature(); // Use a simple function to print out the data
      printUmidade();
      timeSeg=nowsegundo;
      getDateTime();  
      }
    }
}



