#include "myFunctions.h"

void writeTxt(unsigned char page, unsigned char y, char *s) {
    int i = 0;
    while (*s != '\n' && *s != '\0') {
        putchGLCD(page, y + i, *(s++));
        i++;
    }
}

unsigned char debounce_buttons() {
    unsigned char b1 = read_buttons();
    __delay_ms(20);
    unsigned char b2 = read_buttons();

    unsigned char curr = b1 & b2;
    unsigned char new_press = curr & ~prev_buttons;

    prev_buttons = curr;

    return new_press;
}

int get_min(Screen s, Param p) {
    if (p == PARAM_TIME) return 0;
    if (p == PARAM_SPEED) return 100;

    if (s == ESBANDIT && p == PARAM_0) return 1;

    return 0;
}

int get_max(Screen s, Param p) {
    if (p == PARAM_TIME) return 60;
    if (p == PARAM_SPEED) return 1000;

    if (s == ESBANDIT && p == PARAM_0) return 5;

    return 100;
}

int get_step(Screen s, Param p) {
    if (p == PARAM_SPEED) return 100;
    if (p == PARAM_TIME) return 5;

    if (s == ESBANDIT && p == PARAM_0) return 1;

    return 10;
}

void increase_value(Screen s, Param p) {
    settings[s].value[p] += get_step(s, p);

    if (settings[s].value[p] > get_max(s, p)) {
        settings[s].value[p] = get_max(s, p);
    }
}

void decrease_value(Screen s, Param p) {
    settings[s].value[p] -= get_step(s, p);

    if (settings[s].value[p] < get_min(s, p)) {
        settings[s].value[p] = get_min(s, p);
    }
}


char *screen_name(Screen s) {
    switch (s) {
        case RENTAT: return "RENTAT";
        case ESBANDIT: return "ESBANDIT";
        case CENTRIFUGAT: return "CENTRIFUGAT";
        default: return "";
    }
}

char *param_unit(Screen s, Param p) {
    if (p == PARAM_TIME) return "s";
    if (p == PARAM_SPEED) return "rpm";

    if (s == ESBANDIT && p == PARAM_0) return "vegades";

    return "ml";
}


void draw_start_screen() {
    clearGLCD(0, 7, 0, 127);

    writeTxt(3, 5, "Samuel Mezquita");

    __delay_ms(1000);
    clearGLCD(3, 3, 0, 120);
}

byte get_page(Param p) {
   if (p == PARAM_0) return 2;
   if (p == PARAM_TIME) return 4;
   return 6;
}

void draw_param_line(unsigned char page, Param p) {

    if (p == selected_param) writeTxt(page, 60, ">");
    
    writeNum(page, 13, settings[selected_screen].value[p]);

    writeTxt(page, 18, param_unit(selected_screen, p));
}

void draw_screen(Screen s) {

    writeTxt(0, 2, screen_name(selected_screen));

    draw_param_line(2, PARAM_0);
    draw_param_line(4, PARAM_TIME);
    draw_param_line(6, PARAM_SPEED);
}

void my_clear(Screen s) {
     if (selected_screen == RENTAT) {
	 //clear header
	 clearGLCD(0, 0, 12, 48);
	 
	 //clear params
	 clearGLCD(2, 2, 12, 36);
	 clearGLCD(4, 4, 12, 42);
	 clearGLCD(6, 6, 12, 66);
	 
	 //clear :
	 clearGLCD(2, 2, 66, 72);
	 clearGLCD(4, 4, 66, 72);
	 clearGLCD(6, 6, 66, 72);
	 
	 //clear units
	 clearGLCD(2, 2, 108, 120);
	 clearGLCD(4, 4, 108, 114);
	 clearGLCD(6, 6, 108, 126);
     }
     if (selected_screen == ESBANDIT) {
	 clearGLCD(0, 0, 12, 60);
	 
	 clearGLCD(2, 2, 12, 54);
	 clearGLCD(4, 4, 12, 42);
	 clearGLCD(6, 6, 12, 66);
	 
	  //clear :
	 clearGLCD(2, 2, 66, 72);
	 clearGLCD(4, 4, 66, 72);
	 clearGLCD(6, 6, 66, 72);
	 
	 //clear units
	 clearGLCD(2, 2, 100, 126);
	 clearGLCD(4, 4, 108, 114);
	 clearGLCD(6, 6, 108, 126);
     }
     if (selected_screen == CENTRIFUGAT) {
	 clearGLCD(0, 0, 12, 78);
	 
	 clearGLCD(2, 2, 12, 72);
	 clearGLCD(4, 4, 12, 42);
	 clearGLCD(6, 6, 12, 66);
	 
	  //clear :
	 clearGLCD(2, 2, 66, 72);
	 clearGLCD(4, 4, 66, 72);
	 clearGLCD(6, 6, 66, 72);
	 
	 //clear units
	 clearGLCD(2, 2, 108, 120);
	 clearGLCD(4, 4, 108, 114);
	 clearGLCD(6, 6, 108, 126);
     }
}

void draw_edit(Param p) {
   byte page = get_page(p);
   clearGLCD(page, page, 128, 152);
   writeNum(page, 13, settings[selected_screen].value[p]);
}

void draw_arrow(Param p, byte old_page) {
   byte page = get_page(p);
   clearGLCD(old_page, old_page, 44, 54);
   clearGLCD(page, page, 44, 54);
   
   if (edit_mode) writeTxt(page, 60, ">>");
   else writeTxt(page, 60, ">");
}


void handle_buttons(unsigned char btn) {
   byte old_arrow_page = get_page(selected_param);
    
   if (btn & BTN_RIGHT) {
	   my_clear(selected_screen);
      selected_screen = (selected_screen + 1) % NUM_SCREENS;
      selected_param = PARAM_0;
	   draw_screen(selected_screen);
   }

   if (btn & BTN_LEFT) {
	   my_clear(selected_screen);
      selected_screen = (selected_screen - 1 + NUM_SCREENS) % NUM_SCREENS;
      selected_param = PARAM_0;
	   draw_screen(selected_screen);
   }

   if (btn & BTN_OK) {
      if (edit_mode == 0) edit_mode = 1;
        else edit_mode = 0;
	    draw_arrow(selected_param, old_arrow_page);
   }

   if (btn & BTN_UP) {
      if (edit_mode) {
         increase_value(selected_screen, selected_param);
	      draw_edit(selected_param);
      }
        else if (selected_param != PARAM_0) {
            selected_param = (selected_param - 1 + NUM_PARAMS) % NUM_PARAMS;
	        draw_arrow(selected_param, old_arrow_page);
        }
   }

   if (btn & BTN_DOWN) {
      if (edit_mode) {
         decrease_value(selected_screen, selected_param);
	      draw_edit(selected_param);
      }
      else if (selected_param != PARAM_SPEED) {
         selected_param = (selected_param + 1) % NUM_PARAMS;
	      draw_arrow(selected_param, old_arrow_page);
      }
   }
}

void init_pic() {
    ANSELB = 0x00;
    ANSELA = 0x00;
    ANSELD = 0x00;
    
    TRISA = 0x1F;
    
    PORTB = 0x00;
    PORTD = 0x00;
}
