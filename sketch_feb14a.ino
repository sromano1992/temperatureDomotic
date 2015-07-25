// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int temperaturaIn = A0;
int buttonStatoPin = 10;
int buttonMenoPin = 8;
int buttonPiuPin = 9;

int buttonMenoStato = 0;
int buttonPiuStato = 0;
int buttonstatoStato = 0;

int luminosita = 6;
const int caldaia   = 13;
int temperaturaValue;
  int autoTemperatura = 10;
float voltage;
float temperaturaC;
int stato =0;  //0=off,1=on,2=auto
int tempoLuminosita = 0;
int primoImpulso = 0;
int tempoAggiornamentoTemperatura = 0;


 
void setup() {
  //prova
  Serial.begin(9600);
  // set  up the LCD's number of columns and rows: 
  digitalWrite(10, LOW);
  lcd.begin(8, 2);
  pinMode(buttonPiuPin,INPUT);
  pinMode(buttonMenoPin,INPUT);
  pinMode(buttonStatoPin, INPUT);
  pinMode(caldaia, OUTPUT);
  pinMode(luminosita, OUTPUT);
  Serial.begin(9600); 
}

void automatico(){
  digitalWrite(caldaia, LOW);
  buttonstatoStato = digitalRead(buttonStatoPin);
  lcd.setCursor(0, 1);
  lcd.print("AUTO   ");
    
  while(stato == 2){
     Serial.println(tempoLuminosita);
     Serial.println("auto");
     if (tempoLuminosita >2000){
       digitalWrite(luminosita, LOW);
       tempoLuminosita = 0;
       primoImpulso = 0;
     }
    lcd.setCursor(5,1);
    lcd.print(autoTemperatura);
    
    //aggiorno la temperatura circa ogni 10 secondi(5000 cicli)
    if (tempoAggiornamentoTemperatura>5000){
      temperaturaValue = analogRead(temperaturaIn);
      voltage = temperaturaValue*5.00/1024;
      temperaturaC = voltage/0.01;
      lcd.setCursor(0, 0);
      lcd.print(temperaturaC);
      lcd.setCursor(5, 0);
      lcd.print(" C");      
      tempoAggiornamentoTemperatura = 0;
    }
    buttonstatoStato = digitalRead(buttonStatoPin);
    if (buttonstatoStato == HIGH){
      if (primoImpulso == 0){
        primoImpulso = 1;
        digitalWrite(luminosita, HIGH);  
        tempoLuminosita = 0;     
        delay(300);    
      }  
      else if (primoImpulso == 1){
        stato = 0;
        digitalWrite(luminosita, HIGH);
        tempoLuminosita = 0;
      }
    }        
    
    buttonPiuStato = digitalRead(buttonPiuPin);
    if(buttonPiuStato == HIGH){
      if (primoImpulso == 0){
        primoImpulso = 1;
        digitalWrite(luminosita, HIGH);  
        tempoLuminosita = 0;  
        delay(300);       
      }  
      else if (primoImpulso == 1){
        digitalWrite(luminosita, HIGH);
        tempoLuminosita = 0;
        if (autoTemperatura==35)  autoTemperatura = 10;
        else{
            autoTemperatura += 1;
         }
         delay(300);
      }
    }
    if (temperaturaC<autoTemperatura) {
      digitalWrite(caldaia, HIGH);
      lcd.setCursor(7,1);
      lcd.print("*");
    }
    else if(temperaturaC>autoTemperatura){
      digitalWrite(caldaia, LOW);
      lcd.setCursor(7,1);
      lcd.print(" ");
    }
  
  
   buttonMenoStato = digitalRead(buttonMenoPin);
    if(buttonMenoStato == HIGH){
      if (primoImpulso == 0){
        primoImpulso = 1;
        digitalWrite(luminosita, HIGH);  
        tempoLuminosita = 0;  
        delay(300);       
      }  
      else if (primoImpulso == 1){
        digitalWrite(luminosita, HIGH);
        tempoLuminosita = 0;
        if (autoTemperatura==10)  autoTemperatura = 35;
        else{
            autoTemperatura -= 1;
         }
         delay(300);
      }
    }
    
    if (temperaturaC<autoTemperatura) {
      digitalWrite(caldaia, HIGH);
      lcd.setCursor(7,1);
      lcd.print("*");
    }
    else if(temperaturaC>autoTemperatura){
      digitalWrite(caldaia, LOW);
      lcd.setCursor(7,1);
      lcd.print(" ");
    }
  
    tempoLuminosita++;  
    tempoAggiornamentoTemperatura++;
  }
  delay(300);
  spento();
}


void acceso(){
  digitalWrite(caldaia, HIGH);
  buttonstatoStato = digitalRead(buttonStatoPin);
  lcd.setCursor(0, 1);
  lcd.print("ON     *");
    
  while(stato == 1){
     Serial.println(tempoLuminosita);
     Serial.println("on");
     if (tempoLuminosita >2000){
       digitalWrite(luminosita, LOW);
       tempoLuminosita = 0;
       primoImpulso = 0;
     }
    
    //aggiorno la temperatura circa ogni 10 secondi(5000 cicli)
    if (tempoAggiornamentoTemperatura>5000){
      temperaturaValue = analogRead(temperaturaIn);
      voltage = temperaturaValue*5.00/1024;
      temperaturaC = voltage/0.01;
      lcd.setCursor(0, 0);
      lcd.print(temperaturaC);
      lcd.setCursor(5, 0);
      lcd.print(" C");      
      tempoAggiornamentoTemperatura = 0;
    }
    buttonstatoStato = digitalRead(buttonStatoPin);
    if (buttonstatoStato == HIGH){ 
      if (primoImpulso == 0){
        primoImpulso = 1;
        digitalWrite(luminosita, HIGH);  
        tempoLuminosita = 0;    
        delay(300);     
      }  
      else if (primoImpulso == 1){
        stato = 2;
        digitalWrite(luminosita, HIGH);
        tempoLuminosita = 0;
      }
    }    
    tempoLuminosita++;    
    tempoAggiornamentoTemperatura++;
  }
  delay(300);
  automatico();
}

void spento(){
   digitalWrite(caldaia, LOW);
   buttonstatoStato = digitalRead(buttonStatoPin);
   lcd.setCursor(0, 1);
   lcd.print("OFF     ");
   
   while(stato == 0){
     Serial.println(tempoLuminosita);
     Serial.println("off");
     if (tempoLuminosita >2000){
       digitalWrite(luminosita, LOW);
       tempoLuminosita = 0;
       primoImpulso = 0;
     }
     
     //aggiorno la temperatura circa ogni 10 secondi(5000 cicli)
    if (tempoAggiornamentoTemperatura>5000){
      temperaturaValue = analogRead(temperaturaIn);
      voltage = temperaturaValue*5.00/1024;
      temperaturaC = voltage/0.01;
      lcd.setCursor(0, 0);
      lcd.print(temperaturaC);
      lcd.setCursor(5, 0);
      lcd.print(" C");     
      tempoAggiornamentoTemperatura = 0; 
    }
    
    buttonstatoStato = digitalRead(buttonStatoPin);
    if (buttonstatoStato == HIGH){
      if (primoImpulso == 0){
        primoImpulso = 1;
        digitalWrite(luminosita, HIGH);    
        tempoLuminosita = 0;  
        delay(300);    
      }  
      else if (primoImpulso == 1){
        stato = 1;
        digitalWrite(luminosita, HIGH);
        tempoLuminosita = 0;
      }
    }
    tempoLuminosita++;  
    tempoAggiornamentoTemperatura++;
  }
  delay(300);
  acceso();
}

void loop() {
  spento();
}

