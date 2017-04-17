
///AT+CWJAP_DEF="netvirtua87","2097527020"
//AT+CIFSR – Get local IP address
//AT+CIPSTATUS – Information about connection
//AT+CWMODE_DEF?
//AT+CWMODE_DEF=1 mode station
//AT+CWDHCP_DEF
//AT+CIPSTART="TCP","www.baidu.com",80,1

//  req = "GET /batimento&90/mensagem&Coração acelerado/statusAlerta&true/temperatuera&37.7/tipoalerta&2/umidadade&10 HTTP/1.1";




//connect to your wifi network

void connectWifi() {

  String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";

  esp.println(cmd);

  delay(4000);

  if (esp.find("OK")) {

    Serial.println("Connected!");

  }

  else {

    connectWifi();

    Serial.println("Cannot connect to wifi");
  }

}

void coletaDados(){
  noSensor.concat("GET ");
  noSensor.concat("/batimento&90");
  noSensor.concat("/mensagem&Coração acelerado");
  noSensor.concat("/statusAlerta&true");
  noSensor.concat("/temperatuera&"); noSensor.concat(printTemperature());
  noSensor.concat("/tipoalerta&2");
  noSensor.concat("/umidadade&"); noSensor.concat(printUmidade());
  noSensor.concat(" HTTP/1.1\r\n");
  noSensor.concat(" Content-Type: charset=utf-8\r\n");
  httpGet (); //Envio do GET

}


void httpGet () {

  esp.println("AT+CIPSTART=\"TCP\",\"" + HOST_NAME + "\",80");//start a TCP connection.

  if ( esp.find("OK")) {

    Serial.println("TCP connection ready");

  } delay(1000);


  
  Serial.print("GET: ");
  Serial.println(noSensor);
  Serial.println("Fim do Get: ");
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

  esp.print(sendCmd);

  esp.println(noSensor.length() );

  delay(500);

  if (esp.find(">")) {
    Serial.println(noSensor);
    Serial.println("Sending.."); esp.print(noSensor);

    if ( esp.find("SEND OK")) {
      Serial.println("Packet sent");

      while (esp.available()) {

        String tmpResp = esp.readString();

        Serial.println(tmpResp);

      }//fim do while

      // close the connection

      esp.println("AT+CIPCLOSE");

    }
  }
}
