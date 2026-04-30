/* Main.c
 * Processor: PIC18F45K22
 * Compiler:  MPLAB XC8
 */

#include <xc.h>
#include <string.h>
#include "config.h"
#include "GLCD.h"
#define _XTAL_FREQ 8000000  

#define BTN_UP 		0x01
#define BTN_DOWN 	0x02
#define BTN_RIGHT	0x04
#define BTN_LEFT	0x08
#define BTN_SELECT	0x10
#define BTN_MASK	0x1F


int adc_antic = 10000;
const char * s_nom = "Samuel Mezquita\n";


unsigned char read_buttons() {
   return PORTB & BTN_MASK;
}

unsigned char debounce_buttons() {
   unsigned char b1 = read_buttons();
   __delay_ms(20);
   unsigned char b2 = read_buttons();
   
   return b1 & b2;
}


void writeTxt(byte page, byte y, char * s) {
   int i=0;
   while (*s!='\n' && *s!='\0')
   {
      putchGLCD(page, y+i, *(s++));
      i++;
   }
}

show_rentat() {
   writeTxt(1, 2, "Sabo:\n");
   writeTxt(3, 2, "Temps:\n");
   writeTxt(5, 2, "Velocitat:\n");
}

show_esbandit() {
   writeTxt(1, 2, "Numero vegades:\n");
   writeTxt(3, 2, "Temps:\n");
   writeTxt(5, 2, "Velocitat:\n");
}

show_centrifugat() {
   writeTxt(1, 2, "Numero vegades:\n");
   writeTxt(3, 2, "Temps:\n");
   writeTxt(5, 2, "Velocitat:\n");
   writeTxt(7, 2, "OK = START");
}

update_arrow() {
   
}


void main(void)
{
   // Configuració de pins (all digital)
   ANSELA=0x00; 
   ANSELB=0x00;                  
   ANSELC=0x00;                  
   ANSELD=0x00;                  
   
   //enable buttons
   TRISA=0x0F;  
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
   writeTxt(1, 0, ">");
   unsigned char prev_buttons = 0;
   
   while (1)
   {  
      unsigned 
      show_rentat();
      __delay_ms(10); // Pausa per evitar pampallugues excessives
   }
}
