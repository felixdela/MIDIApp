//####################TECLADOS##############################
//CONSULTAR TABLA DE IMPLEMENTACIÓN MIDI DE CADA DISPOSITIVO 
// Channel 1, KORG KARMA
// Channel 2, KORG N364
// Channel 3, YAMAHA SY-35
// Channel 4, NOVATION ULTRANOVA
//##########################################################
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include "botones.h"
//##########################################################
//################DEFINICIÓN DE ETIQUETAS###################
//##########################################################

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, midiOut);  // create a MIDI object called midiOut cambiado Serial1 a Serial para mega
int botones[8]={22,24,26,28,30,32,34,36};//cambiado numeros de pin para mega
int leds[8]={23,25,27,29,31,33,35,37};//cambiado numeros de pin para mega
void(*fbotones[8])()={boton1,boton2,boton3,boton4,boton5,boton6,boton7,boton8};
int activo = 0;
//##########################################################
//####CONFIGURACIÓN I/0 ####################################
//##########################################################

void setup() {
  for(int i=0;i<8;i++){
    pinMode(botones[i],INPUT);
    pinMode(leds[i], OUTPUT);
  }
  Serial.begin(31250);        // definir puerto midi cambiado Serial1 a Serial para mega
  fbotones[activo]();
  encender_led(activo);
}

void loop() {
//##################################################################
//###################################################### BOTÓN b ###
//##################################################################
  
  
  for(int b=0;b<8; b++){
    if (b == activo) continue;
    if (digitalRead(botones[b]) == HIGH) {       // ver estado botón
//##CÓDIGO QUE SE MANDARÁ CUANDO SE PULSE EL BOTÓN b ######
        fbotones[b]();//equivale a (*fbotones[b])()
        //delay(100); //probar a ver si esto funciona
//##FIN DE LOS MENSAJES MIDI DEL BOTÓN 1############################
        encender_led(b);   // pone el LED b en HIGH
        activo = b;
        // delay(2000);//para impedir que se lea mientras
        break;
       
      
    }
    
  }
//####FIN DEL CÓDIGO DEL BOTÓN b ####

}
