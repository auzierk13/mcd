
///AT+CWJAP_DEF="netvirtua87","2097527020"
//AT+CIFSR – Get local IP address
//AT+CIPSTATUS – Information about connection
//AT+CWMODE_DEF?
//AT+CWMODE_DEF=1 mode station
//AT+CWDHCP_DEF


//  req = "GET /batimento&90/mensagem&Coração acelerado/statusAlerta&true/temperatuera&37.7/tipoalerta&2/umidadade&10 HTTP/1.1";

//String uri = "/batimento&90/mensagem&Coração acelerado/statusAlerta&true/temperatuera&37.7/tipoalerta&2/umidadade&10";// our example is /esppost.php



//connect to your wifi network

void setupWifi() {

   // set the data rate for the SoftwareSerial port
  esp.begin(baundrate); // start serial port
  esp.println(F("AT\r"));


  String WIFI_SSID = F("NET_2G7A5441");
  String WIFI_PASSWORD = F("B37A5441");

  String cmd = "AT+CWJAP_DEF=\"" + WIFI_SSID + "\",\"" + WIFI_PASSWORD + "\"";

  esp.println(cmd);

  delay(4000);
  char ok[]= "OK"; 
  if (esp.find(ok)) {

    Serial.println(F("Connected!"));
    Serial.println("Comando= " +cmd);
    Serial.println(F("Comando=AT+CIFSR"));
    esp.println("AT+CIFSR");

  }

  else {

    setupWifi();

    Serial.println(F("Cannot connect to wifi"));
  }

}
  

void coletaDados(){
  String noSensor="";
  noSensor.concat(F("GET "));

////////////////////Tratamento da BPM////////////////////////////////////
mediaBPM=80;
  noSensor.concat(F("/batimento&")); noSensor.concat(mediaBPM); //envio da frequência cardíaca
if((mediaBPM >= bpm_min ) && (mediaBPM<= bpm_max) ){
  noSensor.concat(F("/tipoAlerta1&")); noSensor.concat(NOALERTA);
  noSensor.concat(F("/mensagem1&")); noSensor.concat("");
  digitalWrite(ALERTA_BPM, LOW);   

}else if(mediaBPM> bpm_max){
    noSensor.concat(F("/tipoAlerta1&")); noSensor.concat(HEART);
    noSensor.concat(F("/mensagem1&")); noSensor.concat(Ritimia); 
    digitalWrite(ALERTA_BPM, HIGH);   
    
  }else if((mediaBPM < bpm_min )){
    noSensor.concat(F("/tipoAlerta1&")); noSensor.concat(HEART);
    noSensor.concat(F("/mensagem1&")); noSensor.concat(Ritimia);
    digitalWrite(ALERTA_BPM, HIGH);    
        
  }else{
    Serial.println(F("Erro na captura do batimento"));
 }


////////////////////Tratamento da temperatura////////////////////////////////////

 noSensor.concat(F("/temperatura&")); noSensor.concat(mediaTemperatura); //envio da temperatura
 if ((mediaTemperatura >= temp_min ) && (mediaTemperatura<= temp_max) ){
    noSensor.concat(F("/tipoAlerta2&"));noSensor.concat(NOALERTA);
    noSensor.concat(F("/mensagem2&"));noSensor.concat("");
    digitalWrite(ALERTA_TEMP, LOW);
  }else if(mediaTemperatura> temp_max){
    noSensor.concat(F("/tipoAlerta2&"));noSensor.concat(TEMPERATURA);
    noSensor.concat(F("/mensagem2&"));noSensor.concat(FEBRE);
    digitalWrite(ALERTA_TEMP, HIGH);   
  }else if((mediaTemperatura < temp_min )){
    noSensor.concat(F("/tipoAlerta2&")); noSensor.concat(TEMPERATURA);
    noSensor.concat(F("/mensagem2&"));  noSensor.concat(HIPOTERMIA);
    digitalWrite(ALERTA_TEMP, HIGH);
        
  }else{
    Serial.println(F("Erro na captura da temperatura"));
 }
//////////////////////////////////////////////////////////////////////////////

 
  noSensor.concat(F("/time&")); noSensor.concat(getDateTime());
  ////Tratamento de mensagem

////////////////////Tratamento da umidade ///////////////////////////////////
    noSensor.concat(F("/umidade&")); noSensor.concat(mediaUmidade); //envio da umidade
    
  if(mediaUmidade<= 30){
    noSensor.concat(F("/tipoAlerta3&"));noSensor.concat(NOALERTA);
    noSensor.concat(F("/mensagem3&"));noSensor.concat("");
    digitalWrite(ALERTA_UMIDADE, LOW  );
  }else{
    noSensor.concat(F("/umidade&")); noSensor.concat(mediaUmidade);
    noSensor.concat(F("/tipoAlerta3&"));noSensor.concat(UMIDADE);
    noSensor.concat(F("/mensagem3&"));noSensor.concat(URINA);
    digitalWrite(ALERTA_UMIDADE, HIGH);   
  
  }
  
  


  noSensor.concat(F(" HTTP/1.1\r\n"));
  noSensor.concat(F(" Content-Type: charset=utf-8\r\n"));
 
 //  String HOST_PORT   "80";
  String HOST_NAME= "192.168.0.15";

  esp.println("AT+CIPSTART=\"TCP\",\"" + HOST_NAME + "\",80");//start a TCP connection.
  char receive[] = "OK";
  if ( esp.find(receive)) {

    Serial.println(F("TCP connection ready"));

  } delay(1000);


  
  Serial.print(F("GET: "));
  Serial.println(noSensor);
  Serial.println(F("Fim do Get: "));
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

  esp.print(sendCmd);

  esp.println(noSensor.length() );

  delay(500);
  char receiveMaior[] = ">";
  if (esp.find(receive)) {
    Serial.println(noSensor);
    Serial.println(F("Sending..")); esp.print(noSensor);
    char sendOk[] = "SEND OK";
    if ( esp.find(sendOk)) {
      Serial.println(F("Packet sent"));

      while (esp.available()) {

        String tmpResp = esp.readString();

        Serial.println(tmpResp);

      }//fim do while

      // close the connection

      esp.println(F("AT+CIPCLOSE"));

    }
  }
}
