

#define LINHA1 11
#define LINHA2 12
#define LINHA3 13

#define COLUNA1 A1
#define COLUNA2 A0

void setupTest()
{
  
  //Pinos ligados aos pinos 1, 2, 3 e 4 do teclado - Linhas
  pinMode(LINHA1, OUTPUT);
  pinMode(LINHA2, OUTPUT);
  pinMode(LINHA3, OUTPUT);

   
  //Pinos ligados aos pinos 5, 6, 7 e 8 do teclado - Colunas
  pinMode(COLUNA1, INPUT);
  pinMode(COLUNA2, INPUT);
   
  Serial.begin(9600);
  Serial.println("Aguardando acionamento das teclas...");
  Serial.println();
}

 void imprime_linha_coluna(int x, int y)
{
   Serial.print("Linha : ");
   Serial.print(x);
   Serial.print(" x Coluna : ");
   Serial.print(y);
   delay(10);
   Serial.println();
}

void checkButton()
{
    for (int ti = 11; ti<14; ti++)
    {
    //Alterna o estado dos pinos das linhas
    digitalWrite(LINHA3, LOW);
    digitalWrite(LINHA2, LOW);
    digitalWrite(LINHA1, LOW);
    digitalWrite(ti, HIGH);
    //Verifica se alguma tecla da coluna 1 foi pressionada
    if (digitalRead(COLUNA1) == HIGH)
    {
      if(ti==LINHA1){Serial.print("S13 ");} //matrix(1,1)
      if(ti==LINHA2){Serial.print("S14 ");} //matrix(2,1)
      if(ti==LINHA3){Serial.print("S15 ");} //matrix(3,1)
      
      imprime_linha_coluna(ti-10, 1);
      while(digitalRead(COLUNA1) == HIGH){}
    }
 
    //Verifica se alguma tecla da coluna 2 foi pressionada    
    if (digitalRead(COLUNA2) == HIGH)
    {
      if(ti==LINHA1){Serial.print("S09 ");} //matrix(1,2)
      if(ti==LINHA2){Serial.print("S10 ");} //matrix(2,2)
      if(ti==LINHA3){Serial.print("S11 ");} //matrix(3,2)
      
      imprime_linha_coluna(ti-10, 2);
      while(digitalRead(COLUNA2) == HIGH){};
    }
   }
   delay(10);
}
