#include <EEPROM.h>
#include <RtcDS1302.h>
#include <LiquidCrystal_I2C.h>
#include <notasMusicales.h>  

LiquidCrystal_I2C lcd(0x27, 20, 4);
ThreeWire myWire(3, 4, 5); // DAT, SCLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

#define boton2 6
#define boton3  7
#define boton4  9
#define boton1  8
#define pinBuzzer  10
#define esperaBoton 250
#define esperaInformacion 4000

notasMusicales miNota(pinBuzzer, boton4, 0.6);

class Alarma{
	private:
		static const size_t tamanio1 = 3, tamanio2 = 7; 
		int8_t hora[tamanio1], dias[tamanio2];
		bool alarmaActiva;
		
	public: 	
    Alarma(){
      for(size_t i = 0; i<tamanio1; i++)
        hora[i] = 0;
      for(size_t i = 0; i<tamanio2; i++)
				dias[i] = 0;
      alarmaActiva = false;
    }

    void setHora(int8_t (&arrHora)[tamanio1]){
      for(size_t i = 0; i<tamanio1; i++)
				hora[i] = arrHora[i];
    }

    void setDias(int8_t (&arrDias)[tamanio2]){
      for(size_t i = 0; i<tamanio2; i++)
				dias[i] = arrDias[i];
    }
		
		void estado(){
			this->alarmaActiva = !this->alarmaActiva;
		}

    bool getEstado(){
      return this->alarmaActiva;
    }

    String getHora(){
      char buffer[9]; //8
      sprintf(buffer, "%02i:%02i:%02i", this->hora[0], this->hora[1], this->hora[2]);
      return String(buffer);
    }

    /*int8_t* getDias(){
      int8_t size = this->diasHabiles();
      Serial.println(size);
      if(size <= 0)
        return NULL;
      int8_t* arrayDias = (int8_t*) malloc(size * sizeof(int8_t)); //reservando memoria
      if(arrayDias != NULL){ //asegurando
        for(size_t i = 0; i<tamanio2; i++){
          if(dias[i] != 0){
            arrayDias = dias[i];
            Serial.print(dias[i]);
          }
        }
        Serial.println();
        Serial.println("esos son los dias habiles"); 
      }
      return arrayDias;
    }*/

    String getDias(){
      int8_t band = 0;
      String resultado = "NI";
      for(int8_t i = 0; i<tamanio2; i++){
        if(dias[i] != 0){
          if(band == 0){
            resultado = "";
            band = 1;
          }
          resultado += String(dias[i]) + " ";
        }
      }
      return resultado;
    }
};

int8_t diasAlarma[7];
uint8_t dia;
bool comprobarAlarma1 = false, comprobarAlarma2 = false, comprobarAlarma3 = false, diaActualizado = false;
Alarma misAlarmas[] = {Alarma(), Alarma(), Alarma()};

void comprobarDias() {
  int8_t aux = 0;
  for (int8_t i = 0; i < 7; i++) { 
      for (int8_t j = 0; j < 7 - i - 1; j++) { // Evita comparar fuera de los límites
          if (diasAlarma[j] > diasAlarma[j + 1]) {
              aux = diasAlarma[j];
              diasAlarma[j] = diasAlarma[j + 1];
              diasAlarma[j + 1] = aux;
          }
      }
  }
}

void modificarAlarma(int8_t indice){
  char buffer[8], band = 'f';
  int8_t horaAlarma = 0, minutosAlarma = 0, segundosAlarma = 0, dias = 1, i = 0, columna = 0;
  Serial.println("en el modificador de alarmas");

  lcd.clear();
  lcd.home(); lcd.print("Bot1 = Hora");
  lcd.setCursor(0,1); lcd.print("Bot2 = Minutos");
  lcd.setCursor(0,2); lcd.print("Bot3 = Segundos");
  lcd.setCursor(0,3); lcd.print("Bot4 = Ok"); 
  delay(esperaInformacion); 
  lcd.clear();

  lcd.home(); lcd.print("Escoge la hora");

  do{
    if(digitalRead(boton1) == LOW){
      delay(esperaBoton);
      horaAlarma++;
      if(horaAlarma == 25){
        horaAlarma = 0; delay(100);
      }
    }

    if(digitalRead(boton2) == LOW){
      delay(esperaBoton);
      minutosAlarma++;
      if(minutosAlarma == 60){
        minutosAlarma = 0; horaAlarma++; delay(100);
      }
    }

    if(digitalRead(boton3) == LOW){
      delay(esperaBoton);
      segundosAlarma++;
      if(segundosAlarma == 60){
        segundosAlarma = 0; minutosAlarma++; delay(100);
      }
    }

    sprintf(buffer, "%02i:%02i:%02i", horaAlarma, minutosAlarma, segundosAlarma);
    lcd.setCursor(4,1);
    lcd.print(buffer);
  }while(digitalRead(boton4) == HIGH);

  lcd.clear(); lcd.setCursor(0,0); lcd.print("Has modificado la");
  lcd.setCursor(0,1); lcd.print("hora"); delay(esperaInformacion - 2000);

  lcd.clear(); lcd.home(); lcd.print("Bot1 = dia");
  lcd.setCursor(0,1); lcd.print("Bot4 = Ok");
  lcd.setCursor(0,2); lcd.print("Bot3 = Listo");
  delay(esperaInformacion);
  lcd.clear();
  lcd.home(); lcd.print("Ingresa los dias en");
  lcd.setCursor(0,1); lcd.print("orden ascendente");

  do{
    if(digitalRead(boton1) == LOW){
      delay(esperaBoton); 
      dias++;
      if(dias == 8)
        dias = 1;
    }

    if(digitalRead(boton4) == LOW){
      delay(esperaBoton); 
      diasAlarma[i] = dias;
      dias = 1; i++; columna += 2;
      band = 't';
    }

    lcd.setCursor(columna,3); lcd.print(dias);

    if(band == 't'){
      lcd.setCursor(columna-1, 3); lcd.print("-");
      band = 'f';
    }

  }while(digitalRead(boton3) == HIGH);
  lcd.clear();

  comprobarDias();

  for(int8_t i = 0; i<7; i++)
    Serial.print(diasAlarma[i]);
  Serial.println();
  Serial.println("Esos fueron los dias, ya ordenados");

  switch(indice){
    case 0:
        EEPROM.write(0, horaAlarma); EEPROM.write(1, minutosAlarma); EEPROM.write(2, segundosAlarma);
        for(int8_t i = 3; i<10; i++){
          EEPROM.write(i, diasAlarma[i-3]);
        }
        break;
    case 1:
        EEPROM.write(10, horaAlarma); EEPROM.write(11, minutosAlarma); EEPROM.write(12, segundosAlarma);
        for(int8_t i = 13; i<20; i++){
          EEPROM.write(i, diasAlarma[i-13]);
          Serial.print(diasAlarma[i-13]);
        }
        Serial.println();
        Serial.println("Esos dias ingresaron en la eprom");
        break;
    case 2:
        EEPROM.write(20, horaAlarma); EEPROM.write(21, minutosAlarma); EEPROM.write(22, segundosAlarma);
        for(int8_t i = 23; i<30; i++){
          EEPROM.write(i, diasAlarma[i-23]);
        }
        break;
  }

  lcd.home(); lcd.print("Reinicia el arduino"); lcd.setCursor(0, 1); lcd.print("para actualizar los"); lcd.setCursor(0, 2); lcd.print(" cambios");
  delay(esperaInformacion);
}

void infoAlarma(int8_t indice){
  int8_t contador = 0;

  lcd.home(); lcd.print("Alarma "); lcd.setCursor(7, 0); lcd.print(indice+1);
  lcd.setCursor(0, 2); lcd.print("Dias"); lcd.setCursor(5, 2);  lcd.print(misAlarmas[indice].getDias());
  lcd.setCursor(0, 1); lcd.print("Hora "); lcd.setCursor(5, 1); lcd.print(misAlarmas[indice].getHora());

  lcd.setCursor(0, 3); lcd.print(">");
  lcd.setCursor(1, 3); lcd.print("Estado"); 
  lcd.setCursor(10, 3); lcd.print("Modificar");

  do{
    if(digitalRead(boton1) == LOW){
      delay(esperaBoton);
      contador++;
      if(contador == 1){
        lcd.setCursor(0, 3); lcd.print(" ");
        lcd.setCursor(9, 3); lcd.print(">");
      }
      else if(contador == 2){
        contador = 0;
        lcd.setCursor(9, 3); lcd.print(" ");
        lcd.setCursor(0, 3); lcd.print(">");
      }
    }
    
    if(digitalRead(boton4) == LOW){
      delay(esperaBoton);
      lcd.clear();
      if(contador == 0){
        misAlarmas[indice].estado();
        lcd.home(); lcd.print("Modificaste ESTADO"); lcd.setCursor(0, 1); lcd.print("con exito");
        delay(1000);
        break;
      }
      else{
        modificarAlarma(indice);
        break;
      }
    }
  }while(digitalRead(boton2) == HIGH);
}

void comprobarAlarmas(){
  char bufferH[9];
  RtcDateTime now = Rtc.GetDateTime();
  sprintf(bufferH, "%02i:%02i:%02i", now.Hour(), now.Minute(), now.Second());

  if(now.Hour() == 0 && !diaActualizado){
    dia = now.DayOfWeek();
    dia++;
    lcd.noBacklight();
    Serial.print("Nuevo dia: "); Serial.println(dia);
    diaActualizado = true;
    comprobarAlarma1 = false; comprobarAlarma2 = false; comprobarAlarma3 = false; 
  }else if(now.Hour() == 4 && now.Minute() == 55){
    lcd.backlight();
  }

  if(misAlarmas[0].getEstado()){
    //Serial.println("Alarma 1 esta activa");
    if(!comprobarAlarma1){
      int8_t numAuxiliar = 0;
      String arregloDias = misAlarmas[0].getDias();
      if(arregloDias != "NI"){
        for(int8_t i = 0; i<arregloDias.length(); i++){
          if(arregloDias[i] != ""){
            numAuxiliar = arregloDias[i] - '0';
            if(numAuxiliar == dia){
            comprobarAlarma1 = true;
            Serial.println("Alarma 1 debe sonar este dia");
            break;
            }
          }
        }
      }else{
        Serial.println("Arreglo dias para la alarma 1 llego vacio");
      }
    }else{
      if(misAlarmas[0].getHora() == bufferH){
        Serial.println("Alarma 1 fue activada");
        lcd.clear(); lcd.home(); lcd.print("Alarma 1 activada");
        miNota.encendido();
      }
    }
  }
  if(misAlarmas[1].getEstado()){
    //Serial.println("Alarma 2 esta activa");
    if(!comprobarAlarma2){
      int8_t numAuxiliar2 = 0;
      String arregloDias2 = misAlarmas[1].getDias();
      if(arregloDias2 != "NI"){
        for(int8_t i = 0; i<arregloDias2.length(); i++){
          if(arregloDias2[i] != ""){
            numAuxiliar2 = arregloDias2[i] - '0';
            if(numAuxiliar2 == dia){
            comprobarAlarma2 = true;
            Serial.println("Alarma 2 debe sonar este dia");
            break;
            }
          }
        }
      }else{
        Serial.println("Arreglo dias para la alarma 2 llego vacio");
      }
    }else{
      if(misAlarmas[1].getHora() == bufferH){
        Serial.println("Alarma 2 fue activada");
        lcd.clear(); lcd.home(); lcd.print("Alarma 2 activada");
        miNota.encendido();
      }
    }
  }
  if(misAlarmas[2].getEstado()){
    //Serial.println("Alarma 3 esta activa");
    if(!comprobarAlarma3){
      int8_t numAuxiliar3 = 0;
      String arregloDias3 = misAlarmas[2].getDias();
      if(arregloDias3 != "NI"){
        for(int8_t i = 0; i<arregloDias3.length(); i++){
          if(arregloDias3[i] != ""){
            numAuxiliar3 = arregloDias3[i] - '0';
            if(numAuxiliar3 == dia){
            comprobarAlarma3 = true;
            Serial.println("Alarma 3 debe sonar este dia");
            break;
            }
          }
        }
      }
    }else{
      if(misAlarmas[2].getHora() == bufferH){
        Serial.println("Alarma 3 fue activada");
        lcd.clear(); lcd.home(); lcd.print("Alarma 3 activada");
        miNota.encendido();
      }
    }
  }
}

void gestorAlarmas(){
  int8_t contador = 0;
  lcd.clear();
  lcd.home(); lcd.print("Gestor");

  lcd.setCursor(0, 1); lcd.print(">"); lcd.setCursor(1, 1); lcd.print("Alarma 1");
  lcd.setCursor(11, 1); lcd.print(misAlarmas[0].getEstado()? "$" : " ");

  lcd.setCursor(1, 2); lcd.print("Alarma 2");
  lcd.setCursor(11, 2); lcd.print(misAlarmas[1].getEstado()? "$" : " ");

  lcd.setCursor(1, 3); lcd.print("Alarma 3");
  lcd.setCursor(11, 3); lcd.print(misAlarmas[2].getEstado()? "$" : " ");

  do{
    if(digitalRead(boton1) == LOW){
      delay(esperaBoton);
      contador++;
      if(contador == 1){
        lcd.setCursor(0, 1); lcd.print(" "); lcd.setCursor(0, 2); lcd.print(">");
      }else if(contador == 2){
        lcd.setCursor(0, 2); lcd.print(" "); lcd.setCursor(0, 3); lcd.print(">");
      }else{
        contador = 0;
        lcd.setCursor(0, 3); lcd.print(" "); lcd.setCursor(0, 1); lcd.print(">");
      }
    }
    if(digitalRead(boton2) == LOW){
      delay(esperaBoton);
      if(contador > 0){
        contador--;
        if(contador == 1){
          lcd.setCursor(0, 3); lcd.print(" "); lcd.setCursor(0, 2); lcd.print(">");
        }else{
          lcd.setCursor(0, 2); lcd.print(" "); lcd.setCursor(0, 1); lcd.print(">");
        }
      }
    }
    comprobarAlarmas();
  }while(digitalRead(boton4) == HIGH);
  lcd.clear(); delay(500);
  infoAlarma(contador);
}

void setup() {
  Serial.begin(9600);
  Wire.begin(); 
  lcd.init(); 
  lcd.backlight();
  pinMode(boton2,INPUT_PULLUP);
  pinMode(boton3,INPUT_PULLUP);
  pinMode(boton4,INPUT_PULLUP);
  pinMode(boton1,INPUT_PULLUP);

  Rtc.Begin(); 
  if (!Rtc.IsDateTimeValid()) {
    lcd.home(); lcd.print("RtcDs1302 fallo");
    while(digitalRead(boton4) == HIGH){;}
  }

  RtcDateTime now = Rtc.GetDateTime();
  dia = now.DayOfWeek();
  dia++;
  Serial.println(dia);

  int8_t auxHora[3], contadorAux = 0, valorAux;

  //primeras 3 -> hora, ultimas 7 -> dias
  for(int8_t i = 0; i<10; i++){
    if(i < 3){
      valorAux = EEPROM.read(i);
      auxHora[i] = valorAux == -1? 0 : valorAux;

    }else{ 
      valorAux = EEPROM.read(i);
      diasAlarma[contadorAux] = valorAux == -1? 0 : valorAux;
      contadorAux++;
    }
  }
  contadorAux = 0;
  misAlarmas[0].setHora(auxHora);
  misAlarmas[0].setDias(diasAlarma);

  for(int8_t j = 10; j<20; j++){
    if(j < 13){
      valorAux = EEPROM.read(j);
      auxHora[contadorAux] = valorAux == -1? 0 : valorAux;
      contadorAux++;
    }
    else{
      if(j == 13)
        contadorAux = 0;
      valorAux = EEPROM.read(j);
      diasAlarma[contadorAux] = valorAux == -1? 0 : valorAux;
      contadorAux++;
    }
  }
  contadorAux = 0;
  misAlarmas[1].setHora(auxHora);
  misAlarmas[1].setDias(diasAlarma);

  for(int8_t o = 20; o<30; o++){
    if(o < 23){
      valorAux = EEPROM.read(o);
      auxHora[contadorAux] = valorAux == -1? 0 : valorAux;
      contadorAux++;
    }
    else{
      if(o == 23)
        contadorAux = 0;
      valorAux = EEPROM.read(o);
      diasAlarma[contadorAux] = valorAux == -1? 0 : valorAux;
      contadorAux++;
    }
  }
 misAlarmas[2].setHora(auxHora);
 misAlarmas[2].setDias(diasAlarma);

 for(uint8_t i = 0; i<7; i++)
  diasAlarma[i] = 0;

  Serial.println("Inicializacion terminada");

  lcd.home(); lcd.print("Alarmas Valji");  delay(850);  lcd.clear();
}

void loop() {
  lcd.clear();
  lcd.home(); lcd.print("Bot1 = Adelante");
  lcd.setCursor(0, 1); lcd.print("Bot2 = Atras");
  lcd.setCursor(0, 2); lcd.print("Bot3 = Salir");
  lcd.setCursor(0, 3); lcd.print("Bot4 = OK");
  delay(esperaInformacion);
  gestorAlarmas();
}
