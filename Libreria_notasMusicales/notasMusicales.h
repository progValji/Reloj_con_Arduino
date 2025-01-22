#ifndef notasMusicales_h
#define notasMusicales_h
#include "Arduino.h"

class notasMusicales{
        private:
            int pinBuzzer;
            int botonParo;
            float volumen;
        public:
            notasMusicales(int,int,float);
            void encendido(); 
            void begin();
};

#endif