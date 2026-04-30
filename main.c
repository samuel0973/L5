/* Main.c
 * Processor: PIC18F45K22
 * Compiler:  MPLAB XC8
 */

#include <xc.h>
#include <string.h>
#include "config.h"
#include "GLCD.h"
#define _XTAL_FREQ 8000000  


int adc_antic = 10000;
const char * s_nom = "Samuel Mezquita\n";


void writeTxt(byte page, byte y, char * s) {
   int i=0;
   while (*s!='\n' && *s!='\0')
   {
      putchGLCD(page, y+i, *(s++));
      i++;
   }
}

show_rentat() {
   writeTxt(1, 0, "Sabo:\n");
   writeTxt(3, 0, "Temps:\n");
   writeTxt(5, 0, "Velocitat:\n");
}

show_esbandit() {
   writeTxt(1, 0, "Numero vegades:\n");
   writeTxt(3, 0, "Temps:\n");
   writeTxt(5, 0, "Velocitat:\n");
}

show_centrifugat() {
   writeTxt(1, 0, "Numero vegades:\n");
   writeTxt(3, 0, "Temps:\n");
   writeTxt(5, 0, "Velocitat:\n");
   writeTxt(7, 0, "OK = START");
}


void main(void)
{
   // Configuració de pins 
   ANSELA=0x00; 
   ANSELB=0x00;                  
   ANSELC=0x00;                  
   ANSELD=0x00;                  
   
   TRISA=0x00;  
   TRISD=0x00;    
   TRISB=0x00;

   PORTD=0x00;
   PORTB=0x00;  

   GLCDinit();
   clearGLCD(0,7,0,127);
   setStartLine(0);

   //PRESENTACIÓ INICIAL
   writeTxt(4, 4, s_nom);  // El teu nom a sota

   __delay_ms(1000);       // Esperem 1 segon
   clearGLCD(0,7,0,127);   // Tornem a esborrar per al bucle principal
   

   while (1)
   {  
      __delay_ms(10); // Pausa per evitar pampallugues excessives
   }
}
