#include <LiquidCrystal_I2C.h>
#include <RtcDS1302.h>
#include <notasMusicales.h>  
LiquidCrystal_I2C lcd(0x27, 20, 4);
ThreeWire myWire(3, 4, 5); // DAT, SCLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);     

byte reloj[] = {B00000,B01110,B10001,B11001,B10111,B10001,B11111,B10001};

#define boton2 6
#define boton3  7
#define boton4  9
#define boton1  8
#define pinBuzzer  10
#define esperaBoton 250
#define esperaInformacion 4000

char bandFecha = 'F';
notasMusicales miObjeto(pinBuzzer, boton4, 0.6);

void temporizador(){
  int8_t aux = 0;
  int8_t horas = 0, minutos = 0, segundos = 0;
  int segundosFinal = 0;
  char buffer[8];

  lcd.clear();
  delay(esperaBoton);
  lcd.home(); lcd.print("Modo alarma?");
  lcd.setCursor(5, 1); lcd.print(">"); 
  lcd.setCursor(6, 1); lcd.print("Si"); 
  lcd.setCursor(14, 1); lcd.print("No");

  do{
    if(digitalRead(boton1) == LOW){
      delay(esperaBoton);
      aux++;
      if(aux == 1){
        lcd.setCursor(5, 1); lcd.print(" ");
        lcd.setCursor(13, 1); lcd.print(">");
      }else{
        aux = 0;
        lcd.setCursor(13, 1); lcd.print(" ");
        lcd.setCursor(5, 1); lcd.print(">");
      }
    }
  }while(digitalRead(boton4) == HIGH);
  delay(esperaBoton);
  lcd.clear();

  lcd.home(); lcd.print("Bot1 = Horas");
  lcd.setCursor(0, 1); lcd.print("Bot2 = Minutos");
  lcd.setCursor(0, 2); lcd.print("Bot3 = Segundos");
  lcd.setCursor(0, 3); lcd.print("Bot4 = Ok");
  delay(esperaInformacion);
  lcd.clear();
  lcd.setCursor(2, 0); lcd.print("Escoge el tiempo");

  do{
    
    if(digitalRead(boton1) == LOW){
      delay(esperaBoton);
      horas++;
      if(horas == 24)
        horas = 0;
    }

    if(digitalRead(boton2) == LOW){
      delay(esperaBoton);
      minutos++;
      if(minutos == 60){
        minutos = 0;
        horas++;
      }
    }

    if(digitalRead(boton3) == LOW){
      delay(esperaBoton);
      segundos++;
      if(segundos == 60){
        segundos = 0;
        minutos++;
      }
    }

    sprintf(buffer, "%02i:%02i:%02i", horas, minutos, segundos);
    lcd.setCursor(5, 1); lcd.print(buffer);

  }while(digitalRead(boton4) == HIGH);
  delay(esperaBoton);
  lcd.clear();
  lcd.home(); lcd.print("Tiempo restante");

  segundosFinal = segundos +(minutos*60) +(horas*60*60);

  while(segundosFinal > 0){
    horas = (segundosFinal / 3600);
    minutos = (segundosFinal / 60) % 60;
    segundos = segundosFinal % 60;

    if(digitalRead(boton4) == LOW){
      lcd.setCursor(7, 3); lcd.print("Pausa");
      delay(esperaBoton+250);
      while(digitalRead(boton4) == HIGH){;}
      lcd.setCursor(7, 3); lcd.print("     ");
      delay(esperaBoton+250);
    }

    sprintf(buffer, "%02i:%02i:%02i", horas, minutos, segundos);
    lcd.setCursor(4, 1); lcd.print(buffer);

    delay(980);
    segundosFinal--;
  }

  lcd.clear(); lcd.setCursor(0,0); lcd.print("El tiempo ha"); 
  lcd.setCursor(0,1); lcd.print("ha finalizado");

  if(aux == 0){
    miObjeto.encendido();
  }
  else{
      tone(pinBuzzer, 440);
      delay(3000);
      noTone(pinBuzzer);
  }
}

void cronometro(){ 
  lcd.clear(); 
  int8_t k = 0;
  int horas = 0, minutos = 0, segundos = 0;
  char bufferC[8];
  lcd.home(); lcd.print("Bot1 = Inicio"); lcd.setCursor(0,1); lcd.print("Bot2 = Pausa"); lcd.setCursor(0,2); lcd.print("Bot4 = Reinicio");
  delay(esperaInformacion); lcd.clear();
  lcd.setCursor(3,0); lcd.write(3); lcd.setCursor(5,0); lcd.print("Cronometro");
  do{
      if(digitalRead(boton4) == LOW){
        k = 0;
      }
 
      if(digitalRead(boton2) == LOW){
        k = 1;
      }

      if(digitalRead(boton1) == LOW){
        k = 2;
      }

      switch(k){
        case 0: 
        horas = 0; minutos = 0; segundos = 0; lcd.setCursor(5,1); lcd.print("00:00:00"); 
        lcd.setCursor(7, 3); lcd.print("     ");
        break;
        case 1: break;
        case 2: 
        lcd.setCursor(7, 3); lcd.print("     ");
        while(digitalRead(boton2) == HIGH){ 
              delay(975);
              segundos++;
                
              if(segundos == 60) {
                segundos = 0;
                minutos++;
              }

              if(minutos == 60) {
                minutos = 0;
                horas++;
              }

              sprintf(bufferC, "%02i:%02i:%02i", horas, minutos, segundos);    
              lcd.setCursor(5, 1);
              lcd.print(bufferC);   
        }
        k = 1;
        lcd.setCursor(7, 3); lcd.print("Pausa");
        break;
    }
  }while(digitalRead(boton3) == HIGH);
  delay(esperaBoton);// menu();
}

void fechaYhora(){
  char buffer1[8], buffer2[16];
  uint8_t totalDias = 0;
  do{
      lcd.clear();
      RtcDateTime now = Rtc.GetDateTime();

      sprintf(buffer1, "%02i:%02i:%02i", now.Hour(), now.Minute(), now.Second());
      lcd.home(); lcd.print("Hora"); lcd.setCursor(0, 1); lcd.print(buffer1);

      if(bandFecha == 'F'){
        snprintf(buffer2, sizeof(buffer2), "%s %02i %s %i", now.NameDay(now.DayOfWeek()).c_str(), now.Day(), now.NameMonth(now.Month()).c_str(), now.Year());
        bandFecha = 'T';  
        totalDias = now.DaysInMonth(now.Year(), now.Month());
      }
      lcd.setCursor(0, 2);  lcd.print("Fecha"); lcd.setCursor(0, 3); lcd.print(buffer2); 
      lcd.setCursor(18, 0); lcd.print(totalDias);
      delay(975);  
  }while(digitalRead(boton3) == HIGH);
  bandFecha = 'F'; delay(esperaBoton); //menu();
}

void menu(){
  int8_t filaFlecha = 0;
  int8_t seleccionMenu = 0;

  lcd.clear();
  lcd.home(); lcd.print(">"); 
  lcd.setCursor(1,0); lcd.print("Hora");
  lcd.setCursor(1,1); lcd.print("Cronometro");
  lcd.setCursor(1,2); lcd.print("Temporizador");

  while(digitalRead(boton4) == HIGH){
    if(digitalRead(boton1) == LOW){
      delay(esperaBoton);
      seleccionMenu++;
      lcd.setCursor(0,filaFlecha); lcd.print(" ");
      filaFlecha++;
      lcd.setCursor(0,filaFlecha); lcd.print(">");
    }
    if(digitalRead(boton2) == LOW){
      delay(esperaBoton);
      seleccionMenu--;
      lcd.setCursor(0,filaFlecha); lcd.print(" ");
      filaFlecha--;
      lcd.setCursor(0,filaFlecha); lcd.print(">"); 
    }
  }

  switch(seleccionMenu){
    case 0: fechaYhora(); break;
    case 1: cronometro(); break;
    case 2: temporizador(); break;
  }
}

void setup() {
  Wire.begin(); 
  lcd.init(); 
  lcd.backlight();
  lcd.createChar(3, reloj);
  Serial.begin(9600);
  Rtc.Begin(); 
  if (!Rtc.IsDateTimeValid()) {
    lcd.home(); lcd.print("RtcDs1302 fallo");
    while(digitalRead(boton4) == HIGH){;}
  }
  pinMode(boton2,INPUT_PULLUP);
  pinMode(boton3,INPUT_PULLUP);
  pinMode(boton4,INPUT_PULLUP);
  pinMode(boton1,INPUT_PULLUP);
  lcd.home(); lcd.print("Reloj Valji");  delay(850);  lcd.clear();
}

void loop() {
  lcd.clear();
  lcd.home(); lcd.print("Bot1 = Adelante");
  lcd.setCursor(0, 1); lcd.print("Bot2 = Atras");
  lcd.setCursor(0, 2); lcd.print("Bot3 = Salir");
  lcd.setCursor(0, 3); lcd.print("Bot4 = OK");
  delay(esperaInformacion);
  menu();
}
