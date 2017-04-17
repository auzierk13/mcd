// Programa : Sensor de temperatura DS18B20
// Autor : Bruno Auzier Cavalcante

#include <OneWire.h>
#include <DallasTemperature.h>


///Alertas
#define alerta_temperatura 2 //Alerta  na temperatura
#define alerta_umidade 3 //Alerta umidade
#define alerta_heart 4 //Alerta batimento cardiaco

///////////////////

//Fontes
#define fonte_umidade A3 // pino que ativa o sensor de umidade
/////

////Portas
#define portaUmidade A2    //Sensor A2 pin to Arduino pin A2
// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 10

//Limites 
#define limite_inf_Umi 100 //Saida minima para nao estar urinado
#define limite_inf_Tem 34.0 // Saida minima para nao estar com febre
#define limite_sup_Tem 37 //Saida maxima para nao estar com febre


#define ledControle 13

//Para analise

//  Variables
int pulsePin = A0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = false;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse 

// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress temperaturaSensor;

int cont=0;

void setup(void)
{
  Serial.begin(9600);
//  Serial.begin(115200);           // we agree to talk fast!
   analogReference(INTERNAL);
   
  //ALertas 
  pinMode(alerta_temperatura, OUTPUT);
  pinMode(alerta_umidade, OUTPUT);
/////////////////////////////////
  pinMode(ledControle, OUTPUT);

  //Pinos do sensores
  pinMode(fonte_umidade, OUTPUT);
///////////////////////////////////
 //Teste do sensor de temperatura
  sensors.begin();
  if (!sensors.getAddress(temperaturaSensor, 0))
    Serial.println("Sensores nao encontrados !");
  Serial.println();

//Para analise
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
//  Serial.begin(9600); //115200           // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
   // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE, 
   // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
//   analogReference(EXTERNAL);  

  
}



void ligarSensor(int fonte)
{
  digitalWrite(fonte, HIGH);
  digitalWrite(ledControle, HIGH);
  

}

void desligaSensor(int fonte)
{
  digitalWrite(fonte, LOW);
  digitalWrite(ledControle, LOW);
//  delay(2000);
}

void loop()
{

  
    serialOutput() ;       
    
  if (QS == true){     // A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
        fadeRate = 255;         // Makes the LED Fade Effect Happen
                                // Set 'fadeRate' Variable to 255 to fade LED with pulse
        serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.     
        QS = false;                      // reset the Quantified Self flag for next time    
  }
     
  ledFadeToBeat();                      // Makes the LED Fade Effect Happen 
  delay(20);                             //  take a break

  //cont>=3000 //Verifica acada 1 minuto
  //cont>=1500 //Verifica acada 30 seg
  //cont>=500 //Verifica acada 10 seg
  //cont>=500 //Verifica acada 5 seg
  if(cont>=250){  
    getTemperatura();  
    getUmidade();
    cont=0;
  }else{
    cont++;
  }
}


void ledFadeToBeat(){
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin,fadeRate);          //  fade LED
  }


void getUmidade() {

  ligarSensor(fonte_umidade);
  delay(500);

  int valorUmidade = analogRead(portaUmidade);
  Serial.print("Umidade: ");
  Serial.print(valorUmidade);

  if (valorUmidade <= limite_inf_Umi )
  {
    Serial.println(" Normal");
     digitalWrite(alerta_umidade, LOW);
  } else
  {
    Serial.println(" Alerta Ativo: Urina presente");
    digitalWrite(alerta_umidade, HIGH);
  
  }

  
  desligaSensor(fonte_umidade);

}


void getTemperatura()
{
  ligarSensor(ledControle);
  // Le a informacao do sensor
  delay(500);
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(temperaturaSensor);
 
  // Mostra dados no serial monitor
  Serial.print("Temperatura: ");
  Serial.print(tempC);
  Serial.print(" Celsius ");
  if(tempC>=limite_inf_Tem && tempC<=limite_sup_Tem)
  {
    Serial.println(" Normal");
    digitalWrite(alerta_temperatura, LOW);
    
  }else if(tempC>limite_sup_Tem)
  {
     Serial.println(" Alerta Ativo: Febre");
     digitalWrite(alerta_temperatura, HIGH);
 
  }else {
     Serial.println(" Alerta Ativo: Hipotermia");
     digitalWrite(alerta_temperatura, HIGH);
  
   }
  
  desligaSensor(ledControle);

}
