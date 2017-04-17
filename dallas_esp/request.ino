
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
  noSensor = "/batimento&90";
  noSensor.concat("/mensagem&Coração acelerado");
  noSensor.concat("/statusAlerta&true");
  noSensor.concat("/temperatuera&"); noSensor.concat(printTemperature());
  noSensor.concat("/tipoalerta&2");
  noSensor.concat("/umidadade&10") ;
  Serial.print("noSensor:");
  Serial.println(noSensor);
  httpGet (); //Envio do GET

}


void httpGet () {

  esp.println("AT+CIPSTART=\"TCP\",\"" + HOST_NAME + "\",80");//start a TCP connection.

  if ( esp.find("OK")) {

    Serial.println("TCP connection ready");

  } delay(1000);

  String postRequest ="GET " + noSensor + " HTTP/1.1\r\n Content-Type: text/html; charset=utf-8\r\n" ;
  Serial.print("GET: ");
  Serial.println(postRequest);
  
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

  esp.print(sendCmd);

  esp.println(postRequest.length() );

  delay(500);

  if (esp.find(">")) {
    Serial.println(postRequest);
    Serial.println("Sending.."); esp.print(postRequest);

    if ( esp.find("SEND OK")) {
      Serial.println("Packet sent");

      while (esp.available()) {

        String tmpResp = esp.readString();

        Serial.println(tmpResp);

      }

      // close the connection

      esp.println("AT+CIPCLOSE");

    }

  }
}
