#include "myFunctions.h"

void writeTxt(unsigned char page, unsigned char y, const char *s) {
    int i = 0;
    while (*s != '\n' && *s != '\0') {
        putchGLCD(page, y + i, *(s++));
        i++;
    }
}


unsigned char debounce_buttons() {
    unsigned char b1 = PORTA & BTN_MASK;
    __delay_ms(20);
    unsigned char b2 = PORTA & BTN_MASK;

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
    update_progress_bar(s, p);
}

void decrease_value(Screen s, Param p) {
    settings[s].value[p] -= get_step(s, p);

    if (settings[s].value[p] < get_min(s, p)) {
        settings[s].value[p] = get_min(s, p);
    }
    update_progress_bar(s, p);
}


char *screen_name(Screen s) {
   if (s == RENTAT) return "RENTAT";
   if (s == ESBANDIT) return "ESBANDIT";
   if (s == CENTRIFUGAT) return "CENTRIFUGAT";
   return "";
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
    update_progress_bar(selected_screen, p);

    if (p == selected_param) {
        if (edit_mode) writeTxt(page, 11, ">>");
        else writeTxt(page, 11, ">");
    }

    writeNum(page, 13, settings[selected_screen].value[p]);
    writeTxt(page, 18, param_unit(selected_screen, p));
}

void draw_screen(Screen s) {
    edit_mode = 0;
    writeTxt(0, 1, screen_name(selected_screen));

    draw_param_line(2, PARAM_0);
    draw_param_line(4, PARAM_TIME);
    draw_param_line(6, PARAM_SPEED);
}

void my_clear(Screen s) {
     if (selected_screen == RENTAT) {
	 //clear header
	 clearGLCD(0, 0, 12, 48);
	 
	 //clear arrow 
	 clearGLCD(2, 2, 40, 64);
	 clearGLCD(4, 4, 40, 64);
	 clearGLCD(6, 6, 40, 64);
	 
	 //clear values
	 clearGLCD(2, 2, 60, 84);
	 clearGLCD(4, 4, 60, 84);
	 clearGLCD(6, 6, 60, 84);
	  
	 //clear units
	 clearGLCD(2, 2, 108, 120);
	 clearGLCD(4, 4, 108, 114);
	 clearGLCD(6, 6, 108, 126);
     }
     if (selected_screen == ESBANDIT) {
	 clearGLCD(0, 0, 12, 60);
	 
	  //clear arrow 
	 clearGLCD(2, 2, 40, 64);
	 clearGLCD(4, 4, 40, 64);
	 clearGLCD(6, 6, 40, 64);
	 
	 //clear values
	 clearGLCD(2, 2, 60, 84);
	 clearGLCD(4, 4, 60, 84);
	 clearGLCD(6, 6, 60, 84);
	  
	 //clear units
	 clearGLCD(2, 2, 100, 126);
	 clearGLCD(4, 4, 108, 114);
	 clearGLCD(6, 6, 108, 126);
     }
     if (selected_screen == CENTRIFUGAT) {
	 clearGLCD(0, 0, 12, 64);
	 
	  //clear arrow 
	 clearGLCD(2, 2, 40, 64);
	 clearGLCD(4, 4, 40, 64);
	 clearGLCD(6, 6, 40, 64);
	 
	 //clear values
	 clearGLCD(2, 2, 60, 84);
	 clearGLCD(4, 4, 60, 84);
	 clearGLCD(6, 6, 60, 84);
	  
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
   clearGLCD(old_page, old_page, 50, 64);
   clearGLCD(page, page, 50, 64);
   
   if (edit_mode) writeTxt(page, 11, ">>");
   else writeTxt(page, 11, ">");
}

void init_pic() {
    ANSELB = 0x00;
    ANSELA = 0x00;
    ANSELD = 0x00;
    
    TRISA = 0x1F;
    
    PORTB = 0x00;
    PORTD = 0x00;
}

void draw_progress_bar(byte p, byte y, byte percent) {
    const int length = 8;
    int totalPixels = length * 5;
    int filledPixels = (totalPixels * percent) / 100;

    for (int i = 0; i < totalPixels; ++i) {
        byte pattern;

        if (i == 0 || i == totalPixels - 1) pattern = 0x7F; 
        else if (i < filledPixels) pattern = 0x7F; 
        else pattern = 0x41; // empty area

        writeByte(p, y + i, pattern);
    }
}

void update_progress_bar(Screen s, Param p) {
    long val = (long)settings[s].value[p];
    long min = (long)get_min(s, p);
    long max = (long)get_max(s, p);
    long percentage = ((val - min) * 100L) / (max - min);
    
    byte page = get_page(p);
    if (s == ESBANDIT && p == PARAM_0 && settings[s].value[p] == 1) percentage = 0;
    if (p == PARAM_SPEED && settings[s].value[p] == 100) percentage = 0; 
    draw_progress_bar(page, 5, (byte)percentage);
}
