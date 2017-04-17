
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define baundrate 9600
#define HOST_PORT   "80"
String HOST_NAME= "10.208.5.167";


String ssid = "ICOMP_1A";
#define password  "1comp@14#"


String uri = "/batimento&90/mensagem&Coração acelerado/statusAlerta&true/temperatuera&37.7/tipoalerta&2/umidadade&10";// our example is /esppost.php
String noSensor;

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 10
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

SoftwareSerial esp(2,3); // RX, TX


void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(baundrate);//9600  // start serial port
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the data rate for the SoftwareSerial port
  esp.begin(baundrate); // start serial port
  esp.println("AT\r");

  connectWifi();
  setupDs18b20();
  coletaDados();
 
}



void loop() {
if (esp.available()) {
    Serial.write(esp.read());
  }
  if (Serial.available()) {
    esp.write(Serial.read());
    
  }

}
