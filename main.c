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
#define BTN_DOWN 	   0x02
#define BTN_RIGHT	   0x04
#define BTN_LEFT	   0x08
#define BTN_SELECT	0x10
#define BTN_MASK	   0x1F


int adc_antic = 10000;
const char * s_nom = "Samuel Mezquita\n";

unsigned int rentat_settings[3] = {0};
unsigned int esbandit_settings[3] = {0};
unsigned int centrifugat_settings[4] = {0};

typedef enum {
   RENTAT, 
   ESBANDIT,
   CENTRIFUGAT
} Modes;

Modes selected_mode = RENTAT;


unsigned char read_buttons() {
   return PORTB & BTN_MASK;
}

void writeTxt(byte page, byte y, char * s) {
   int i=0;
   while (*s!='\n' && *s!='\0')
   {
      putchGLCD(page, y+i, *(s++));
      i++;
   }
}

void show_rentat() {
   selected_mode = RENTAT;
   writeTxt(1, 2, "Sabo:");
   writeNum(1, 6, rentat_settings[0]);
   writeTxt(1,7, "ml");
   writeTxt(3, 2, "Temps:");
   writeNum(3, 6, rentat_settings[1]);
   writeTxt(3, 7, "s");
   writeTxt(5, 2, "Velocitat:");
   writeNum(5, 6, rentat_settings[2]);
   writeTxt(5, 7, "rpm");
}

void show_esbandit() {
   selected_mode = ESBANDIT;
   writeTxt(1, 2, "Numero vegades:\n");
   writeTxt(3, 2, "Temps:\n");
   writeTxt(5, 2, "Velocitat:\n");
}

void show_centrifugat() {
   selected_mode = CENTRIFUGAT;
   writeTxt(1, 2, "Numero vegades:\n");
   writeTxt(3, 2, "Temps:\n");
   writeTxt(5, 2, "Velocitat:\n");
   writeTxt(7, 2, "OK = START");
}

update_arrow() {
   
}

void increase_param(const unsigned int ptr, const Modes selected_mode) {
   if (selected_mode == RENTAT) {
      //Sabo
      if (ptr == 0) {
         if (rentat_settings[0] < 1000) ++rentat_settings[0];

      }
      //Temps
      else if (ptr == 1) {
         if (rentat_settings[1] < 60) ++rentat_settings[1];

      }
      //Velocitat
      else if (ptr == 2) {
         if (rentat_settings[2] < 1000) ++rentat_settings[2];
      }
   }
   else if (selected_mode == ESBANDIT) {
      //nVegades
      if (ptr == 0) {
         if (esbandit_settings[0] < 5) ++esbandit_settings[0];

      }
      //Temps
      else if (ptr == 1) {
         if (esbandit_settings[1] < 60) ++esbandit_settings[1];

      }
      //Velocitat
      else if (ptr == 2) {
         if (esbandit_settings[2] < 1000) ++esbandit_settings[2];

      }

   }
   else if (selected_mode == CENTRIFUGAT) {
      //Suavitzant
      if (ptr == 0) {
         if (centrifugat_settings[0] < 1000) ++centrifugat_settings[0];

      }
      //Temps
      else if (ptr == 1) {
         if (centrifugat_settings[1] < 60) ++centrifugat_settings[1];

      }
      //Velocitat
      else if (ptr == 2) {
         if (centrifugat_settings[2] < 1000) ++centrifugat_settings[2];

      }
   }
}

void decrease_param(const unsigned int ptr, const Modes selected_mode) {
   if (selected_mode == RENTAT) {
      //Sabo
      if (ptr == 0) {
         if (rentat_settings[0] > 0) --rentat_settings[0];

      }
      //Temps
      else if (ptr == 1) {
         if (rentat_settings[1] > 0) --rentat_settings[1];

      }
      //Velocitat
      else if (ptr == 2) {
         if (rentat_settings[2] > 100) --rentat_settings[2];

      }
   }
   else if (selected_mode == ESBANDIT) {
      //nVegades
      if (ptr == 0) {
         if (esbandit_settings[0] > 1) --esbandit_settings[0];

      }
      //Temps
      else if (ptr == 1) {
         if (esbandit_settings[1] > 0) --esbandit_settings[1];

      }
      //Velocitat
      else if (ptr == 2) {
         if (esbandit_settings[2] > 100) --esbandit_settings[2];

      }

   }
   else if (selected_mode == CENTRIFUGAT) {
      //Suavitzant
      if (ptr == 0) {
         if (centrifugat_settings[0] > 0) --centrifugat_settings[0];

      }
      //Temps
      else if (ptr == 1) {
         if (centrifugat_settings[1] > 0) --centrifugat_settings[1];

      }
      //Velocitat
      else if (ptr == 2) {
         if (centrifugat_settings[2] > 100) --centrifugat_settings[2];

      }
   }
}

void clear_screen(Modes selected_mode) {

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
   
   unsigned int selected = 0;
   unsigned int ptr = 0;

   
   while (1)
   {  
      unsigned char curr = read_buttons();

      unsigned char new_press = curr & ~prev;

      if (new_press) {
         __delay_ms(10);

         unsigned char confirm = read_buttons();

         new_press &= confirm;   // debounce
         curr = confirm;         // stable state
      }

      if (new_press & BTN_UP) {
         //change value
        if (selected) {
            //Increase param
            increase_param(ptr, selected_mode);
        }
        //move arrow up
        else {
            int max = 2;
            if (selected_mode == CENTRIFUGAT) max = 3;
            if (ptr > 0) --ptr;
            else ptr = max;
        }
      }
      if (new_press & BTN_DOWN) {
         //change value
        if (selected) {
            //Decrease param
            decrease_param(ptr, selected_mode);
        }
        //move arrow down
        else {
            int max = 2;
            if (selected_mode == CENTRIFUGAT) max = 3;
            if (ptr < max) ++ptr;
            else ptr = 0;
        }
        
      }
      if (new_press & BTN_RIGHT) {
         if (selected_mode == RENTAT) {
            clear_screen();
            show_esbandit();
         }
         else if (selected_mode == ESBANDIT) {
            clear_screen();
            show_centrifugat();
         }
        
      }
      if (new_press & BTN_LEFT) {
         if (selected_mode == CENTRIFUGAT) {
            clear_screen();
            show_esbandit();
         }
         else if(selected_mode == ESBANDIT) {
            clear_screen();
            show_rentat();
         }
        
      }
      if (new_press & BTN_SELECT) {
         if (selected == 0) selected = 1;
         else selected = 0;
      }

      prev = curr;
      __delay_ms(10); // Pausa per evitar pampallugues excessives
   }
}
