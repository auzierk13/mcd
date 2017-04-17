



void setupRTC()  {      

// Informacoes iniciais de data e hora
  // Apos setar as informacoes, comente a linha abaixo
  // (segundos, minutos, hora, dia da semana, dia do mes, mes, ano)
//  myRTC.setDS1302Time(0, 0, 12, 11, 30, 3, 2017);

 Serial.println(F("DS1302RTC Read Test"));
  Serial.println("-------------------");
    
  Serial.println(F("RTC module activated"));
  Serial.println();
  delay(500);
  
  if (RTC.haltRTC()) {
    Serial.println(F("The DS1302 is stopped.  Please run the SetTime"));
    Serial.println(F("example to initialize the time and begin running."));
    Serial.println();
  }
  if (!RTC.writeEN()) {
    Serial.println(F("The DS1302 is write protected. This normal."));
    Serial.println();
  }
}

long getDateTime()  {                                                                                                                                                                                                      

    tmElements_t tm; 
    Serial.print(F("UNIX Time: "));
    Serial.print(RTC.get());

  if (! RTC.read(tm)) {
    Serial.write(' ');
    Serial.print(tm.Day); Serial.write('/');
    Serial.print(tm.Month); Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.write(' ');
    print2digits(tm.Hour); Serial.write(':');
    print2digits(tm.Minute); Serial.write(':');
    print2digits(tm.Second);
    Serial.println(' ');
  } else {
    Serial.println(F("DS1302 read error!  Please check the circuitry."));
    Serial.println();
    delay(9000);
  }
  
  // Wait one second before repeating :)
  delay (1000);
  return RTC.get();
      
// 
}       

                                                                                                  
void print2digits(int number) {
  if (number >= 0 && number < 10)
    Serial.write('0');
  Serial.print(number);
}
  
