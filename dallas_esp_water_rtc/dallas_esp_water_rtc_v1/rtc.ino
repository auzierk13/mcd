



void setupRTC()  {      

// Informacoes iniciais de data e hora
  // Apos setar as informacoes, comente a linha abaixo
  // (segundos, minutos, hora, dia da semana, dia do mes, mes, ano)
//  myRTC.setDS1302Time(0, 31, 11, 11, 15, 3, 2017);
}

String getDateTime()  {                                                                                            
// This allows for the update of variables for time or accessing the individual elements.                
//  myRTC.updateTime();                                                                                     
                                                                                                          
//// Delay so the program doesn't print non-stop                                                            
////  delay( 5000);
//"2010-10-15T09:27:37Z";
//  Serial.print("Current Date/Time: ");                                                                  

  String dateTime;
  
  Serial.print(myRTC.year);dateTime.concat(myRTC.year);
                                                                                 
  Serial.print("-");dateTime.concat("-");                                                                                      
  
  Serial.print(myRTC.month);dateTime.concat(myRTC.month);                                                                              
  
  Serial.print("-");dateTime.concat("-");                                                                                      
  
  Serial.print(myRTC.dayofmonth);dateTime.concat(myRTC.dayofmonth);                                                                         
  
  Serial.print("-");dateTime.concat("-");                                                                                     
  
  Serial.print(myRTC.hours); dateTime.concat(myRTC.hours);                                                                               
  
  Serial.print(":");dateTime.concat(":");                                                                                      
  
  Serial.print(myRTC.minutes);dateTime.concat(myRTC.minutes);                                                                            
  
  Serial.print(":");dateTime.concat(":");                                                                                      
  
  Serial.println(myRTC.seconds);dateTime.concat(myRTC.seconds);  
  
  return dateTime;
      
// 
}                                                                                                         
                                                                                                             
//=======================================================================================================    
//                                                                                                           
//                                   Printout using BUFFER objects BEGIN                                     
//                                                                                                           
//=======================================================================================================    


  
