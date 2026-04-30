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
const char * s1 = "L4A GLCD\n";
const char * s2 = "--------\n";

const char * s_nom = "Mar Cabrelles\n"; // Canvia-ho pel teu
const char * s2_nom = "Samuel Bru Mezquita\n"; // Canvia-ho pel teu

// --- FUNCIÓ PER LLEGIR L'ADC ---
int llegirADC(void) {
    ADCON0bits.GO = 1;         // Inicia la conversió
    while(ADCON0bits.GO);      // Espera que acabi (bit GO es posa a 0)
    return ((ADRESH << 8) + ADRESL); // Retorna el valor de 10 bits
}

void writeTxt(byte page, byte y, char * s) {
   int i=0;
   while (*s!='\n' && *s!='\0')
   {
      putchGLCD(page, y+i, *(s++));
      i++;
   }
}

mostra_dades() {
   writeTxt(1, 0, "Valor ADC:     \n");
   writeTxt(3, 0, "Tensio:  \n");
   writeTxt(5, 0, "Percent:      \n");
}

update_dades(unsigned int adc){
   int i;
   int volts_enters, volts_decimals;
   int percentatge;
      int total_decims = (int)((long)adc * 50 / 1023);
      volts_enters = total_decims / 10;    // Obtenim la part entera
      volts_decimals = total_decims % 10;
     
      percentatge = (int)(((long)adc * 100) / 1023);
      //Update dades
      //writeTxt(1,12, "     \n");
      clearGLCD(1,1,65,95);
      writeNum(1, 12, adc);
      //writeTxt(1, 17, "    \n"); // Espais en blanc per si el número passa de 1000 a 999

      clearGLCD(3,3,48,71);
      writeNum(3, 8, volts_enters);      // Escrivim la part entera
      putchGLCD(3, 9, '.');              // Pintem el punt decimal
      writeNum(3, 10, volts_decimals);   // Escrivim el decimal
      writeTxt(3, 12, "V\n");        // Unitat i espais per netejar
     
      //writeTxt(5, 9, "    \n");
      clearGLCD(5,5,48,71);
      writeNum(5, 9, percentatge);
      writeTxt(5, 12, "%\n");
     
      //Update barra (double casting per evitar overflow)
      int col_max = (int)((long)adc * 128 / 1023);

      for (i = 0; i < 128; i++)
      {
         if (i <= col_max) {
             writeByte(7, i, 0xFF); // Columna pintada (8 píxels d'alt)
         } else {
             writeByte(7, i, 0x00); // Columna buida
         }
      }
}

void main(void)
{
   // Configuració de pins (RA0 com entrada analògica)
   ANSELA=0x01; // El bit 0 és 1 (RA0 analògic), la resta 0
   ANSELB=0x00;                  
   ANSELC=0x00;                  
   ANSELD=0x00;                  
   
   TRISA=0x01;  // Port A0 com entrada
   TRISD=0x00;    
   TRISB=0x00;

   PORTD=0x00;
   PORTB=0x00;  

   // Configuració del registre ADC
   ADCON0 = 0x01; // Selecciona canal AN0 i activa el mòdul
   ADCON1 = 0x00; // Referències de tensió VDD i VSS
   ADCON2 = 0x91; // Justificació dreta i temps d'adquisició

   GLCDinit();
   clearGLCD(0,7,0,127);
   setStartLine(0);

   //PRESENTACIÓ INICIAL
   writeTxt(2, 8, s1);     // "L4A GLCD" al centre
   writeTxt(3, 8, s2);     // "L4A GLCD" al centre

   writeTxt(4, 4, s_nom);  // El teu nom a sota
   writeTxt(5, 4, s2_nom);  // El teu nom a sota

   __delay_ms(1000);       // Esperem 1 segon
   clearGLCD(0,7,0,127);   // Tornem a esborrar per al bucle principal
   int adc;
   mostra_dades();

   while (1)
   {  
      adc = llegirADC(); // Llegim el potenciòmetre (0 a 1023)

      if(adc != adc_antic) {
update_dades(adc);
adc_antic = adc;
      }
      __delay_ms(10); // Pausa per evitar pampallugues excessives
   }
}
