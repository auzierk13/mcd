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
#define WATER_SENSOR A2

#define ALERTA_BPM 9
#define ALERTA_TEMP 8
#define ALERTA_UMIDADE 7



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
#define HIPOTERMIA "Bebê com hipotermia"

#define URINA "Bebê esta urinado"
#define Ritimia "Coração acelerado"

virtuabotixRTC myRTC(RTC_CLOCK, RTC_DATA, RTC_RST);
//byte 0- 255
byte timeHor, timeMin, timeSeg;

float mediaTemperatura=0;
int mediaUmidade=0;
int mediaBPM=0;
unsigned int contAmostra=0;


// __      __           _    __               _            _                     __        //\               _        
// \ \    / /          (_)  /_/              (_)          | |                   / _|      |/ \|             (_)       
//  \ \  / /__ _  _ __  _   __ _ __   __ ___  _  ___    __| |  ___   _ __  ___ | |_  _ __  ___  _ __    ___  _   __ _ 
//   \ \/ // _` || '__|| | / _` |\ \ / // _ \| |/ __|  / _` | / _ \ | '__|/ _ \|  _|| '__|/ _ \| '_ \  / __|| | / _` |
//    \  /| (_| || |   | || (_| | \ V /|  __/| |\__ \ | (_| ||  __/ | |  |  __/| |  | |  |  __/| | | || (__ | || (_| |
//     \/  \__,_||_|   |_| \__,_|  \_/  \___||_||___/  \__,_| \___| |_|   \___||_|  |_|   \___||_| |_| \___||_| \__,_|
////                                                                                                                    
//
//  Temperatura Normal
//  Pele 35,5 - 37,0 C <--
#define temp_min 35.5
#define temp_max  37
// 

//  BPM Normal
//  Recém nascido 100 - 160 BPM
//  0 - 5  meses   90 - 150 BPM
//  6 - 12 meses   80 - 140 BPM
//  1 - 3  meses   80 - 130 BPM <-- 
#define bpm_min 80
#define bpm_max 130
//



void setup() {
 // Open serial communications and wait for port to open:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(baundrate);//9600  // start serial port
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(ALERTA_BPM, OUTPUT);
  pinMode(ALERTA_TEMP, OUTPUT);
  pinMode(ALERTA_UMIDADE, OUTPUT);  
  
 
  testLed();
  setupWifi();       //Inicia o modulo wifi 
  setupRTC();        //Inicia o relógio 
  getDateTime();     //Mostra o horario atual
  setupDs18b20();    //Inicia o sensor de temperatura

  setupTest();
  
  timeMin= myRTC.minutes;
  timeSeg= myRTC.seconds;
  timeHor= myRTC.hours;
  
 
}
void testLed(){
  digitalWrite(ALERTA_BPM, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(ALERTA_BPM, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);    

  digitalWrite(ALERTA_TEMP, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(ALERTA_TEMP, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);    
  
  digitalWrite(ALERTA_UMIDADE, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(ALERTA_UMIDADE, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);      

  }


void loop() {
  
  myRTC.updateTime();  // Le as informacoes do CI
  byte timeHorNow= myRTC.hours;
  byte timeMinNow= myRTC.minutes;
  byte timeSegNow= myRTC.seconds;

  checkButton();
  
if (esp.available()) {
    Serial.write(esp.read());
  }
  if (Serial.available()) {
    esp.write(Serial.read());
    
  }

  if(timeHorNow> timeHor || timeHorNow == 0 ){ //timeHorNow = 0 Passou um dia
    Serial.print(F("*******Passou hora ")); Serial.print(timeHor); Serial.println(F(" ********"));
    timeMin=0;
    timeHor=timeHorNow;
  }
  
  if(timeMinNow> timeMin  ){
    Serial.print(F("*******Passou minuto ")); Serial.print(timeMin); Serial.println(F(" ********"));
    
    if(!contAmostra==0){    
        Serial.print(F("********Media Tempearatura*******"));
        mediaTemperatura= mediaTemperatura/contAmostra;
        Serial.println( mediaTemperatura);
        
        Serial.print(F("********Media Umidade*******"));
        mediaUmidade= mediaUmidade/contAmostra;
        Serial.println( mediaUmidade);
        
        coletaDados();
    }
    
    timeMin=timeMinNow;
    timeSeg=mediaTemperatura=contAmostra=0;
    
    }else{
//    Serial.println("No memso minuto "); getTime();
      if(timeSegNow> timeSeg){
      contAmostra++;
      printTemperature(); // Use a simple function to print out the data
      printUmidade();
      timeSeg=timeSegNow;
      getDateTime();  
      }
    }
}



