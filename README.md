# Reloj con Arduino  

Este proyecto implementa un reloj con funcionalidades adicionales como cronómetro, temporizador y alarmas utilizando un Arduino. El proyecto incluye varias funcionalidades y utiliza diferentes componentes de hardware y librerías.

## Tabla de contenido
- [Descripción de Archivos](#descripción-de-archivos)
- [Funcionalidades](#funcionalidades)
- [Componentes de Hardware](#componentes-de-hardware)
- [Librerias utilizadas](#librerías-utilizadas)
- [Uso](#uso)
- [Licencia](#licencia)

## Descripción de Archivos

- `alarmas/alarmas.ino`: Código fuente para la gestión de alarmas.
- `Libreria_notasMusicales/notasMusicales.cpp` y `Libreria_notasMusicales/notasMusicales.h`: Implementación de la librería para reproducir la nota musical de la pantera Rosa.
- `Archivos_RtcByMakuma`: Libreria Rtc by makuma, se ha implementado 2 metodos mas (Descarga la libreria y solo remplaza los archivos)
- `principal/principal.ino`: Código fuente principal que incluye funcionalidades de reloj, cronómetro y temporizador.

## Funcionalidades

### Reloj
Muestra la hora y la fecha actual utilizando un módulo RTC (Real Time Clock).

### Cronómetro
Permite medir intervalos de tiempo con precisión.

### Temporizador
Permite configurar un temporizador que cuenta hacia atrás desde un tiempo especificado. Incluye un modo de alarma, el cual, si se activa hace sonar el buzzer hasta que se presione el boton de paro.

### Alarmas
Permite configurar hasta tres alarmas con días específicos y horas determinadas. Las alarmas pueden ser activadas o desactivadas según sea necesario.

## Componentes de Hardware

- Arduino
- Módulo RTC DS1302
- Pantalla LCD I2C 20X4
- Buzzer
- Botones
- Jumpers o cables

>[!NOTE]
> Puedes usar cualquier Arduino, ya sea nano, uno, mega, etc.

>[!IMPORTANT]
>Es importante que se use el modulo RtcDS1302, ya que con otro modulo, los codigos no funcionaran

## Librerías Utilizadas

- `LiquidCrystal_I2C`: Para controlar la pantalla LCD.
- `RtcDS1302 y Rtc By Makuma`: Para interactuar con el módulo RTC DS1302.
- `notasMusicales`: Librería personalizada para reproducir nota musical de la pantera rosa.

## Uso

- Utiliza los botones para navegar por el menú y seleccionar las diferentes funcionalidades.
- Configura las alarmas, el temporizador y utiliza el cronómetro según sea necesario.

## Licencia

Este proyecto está bajo la Licencia Apache 2.0. Consulta el archivo `LICENSE` para más detalles.

