
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
#define RTC_CLOCK 6
#define RTC_DATA 7
#define RTC_RST 8

virtuabotixRTC myRTC(RTC_CLOCK, RTC_DATA, RTC_RST);

int timeMin;
void setup() {
 // Open serial communications and wait for port to open:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(baundrate);//9600  // start serial port
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the data rate for the SoftwareSerial port
  esp.begin(baundrate); // start serial port
  esp.println("AT\r");

  connectWifi();
  setupRTC();
  getDateTime();
  setupDs18b20();
  timeMin= myRTC.minutes;
}



void loop() {
   // Le as informacoes do CI
   myRTC.updateTime();  
   
if (esp.available()) {
    Serial.write(esp.read());
  }
  if (Serial.available()) {
    esp.write(Serial.read());
    
  }

  int timeMinNow= myRTC.minutes;
  if(timeMinNow> timeMin){
    Serial.print("*******Passou minuto ");
    Serial.print(timeMin);
    Serial.println(" ********");
    timeMin=timeMinNow;
//    getDateTime(); 
    coletaDados();
    }else{
//    Serial.println("No memso minuto "); getTime();
    }
//  getDateTime();
}
