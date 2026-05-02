#include <xc.h>
#include "config.h"
#include "GLCD.h"

#define _XTAL_FREQ 8000000

#define BTN_UP      0x02   // RA0
#define BTN_DOWN    0x08   // RA1
#define BTN_LEFT    0x10   // RA2
#define BTN_RIGHT   0x01   // RA3
#define BTN_OK      0x04   // RA4
#define BTN_MASK    0x1F

typedef enum {
    RENTAT,
    ESBANDIT,
    CENTRIFUGAT,
    NUM_SCREENS
} Screen;

typedef enum {
    PARAM_0,
    PARAM_TIME,
    PARAM_SPEED,
    NUM_PARAMS
} Param;

typedef struct {
    int value[NUM_PARAMS];
} ScreenSettings;

Screen selected_screen = RENTAT;
Param selected_param = PARAM_0;

ScreenSettings settings[NUM_SCREENS] = {
    {{0, 0, 100}},   // RENTAT: sabo, temps, velocitat
    {{1, 0, 100}},   // ESBANDIT: vegades, temps, velocitat
    {{0, 0, 100}}    // CENTRIFUGAT: suavitzant, temps, velocitat
};

unsigned char redraw_needed = 0;
unsigned char prev_buttons = 0;
unsigned char edit_mode = 0;


void writeTxt(unsigned char page, unsigned char y, char *s) {
    int i = 0;
    while (*s != '\n' && *s != '\0') {
        putchGLCD(page, y + i, *(s++));
        i++;
    }
}


unsigned char read_buttons(void) {
    // Pull-down: not pressed = 0, pressed = 1
    return PORTA & BTN_MASK;
}

unsigned char get_new_presses(void) {
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

char *param_name(Screen s, Param p) {
    if (s == RENTAT) {
        if (p == PARAM_0) return "Sabo";
        if (p == PARAM_TIME) return "Temps";
        if (p == PARAM_SPEED) return "Vel";
    }

    if (s == ESBANDIT) {
        if (p == PARAM_0) return "Vegades";
        if (p == PARAM_TIME) return "Temps";
        if (p == PARAM_SPEED) return "Vel";
    }

    if (s == CENTRIFUGAT) {
        if (p == PARAM_0) return "Suav.";
        if (p == PARAM_TIME) return "Temps";
        if (p == PARAM_SPEED) return "Vel";
    }

    return "";
}

char *param_unit(Screen s, Param p) {
    if (p == PARAM_TIME) return "s";
    if (p == PARAM_SPEED) return "rpm";

    if (s == ESBANDIT && p == PARAM_0) return "veg";

    return "ml";
}


void draw_start_screen() {
    clearGLCD(0, 7, 0, 127);

    writeTxt(4, 6, "Samuel Mezquita");

    __delay_ms(1000);
}

void draw_header(void) {
    writeTxt(0, 2, screen_name(selected_screen));
}

void draw_param_line(unsigned char page, Param p) {
    unsigned char col = 0;

    if (p == selected_param)
        writeTxt(page, col, ">");
    else
        writeTxt(page, col, " ");

    writeTxt(page, 2, param_name(selected_screen, p));
    writeTxt(page, 11, ":");

    writeNum(page, 13, settings[selected_screen].value[p]);

    writeTxt(page, 18, param_unit(selected_screen, p));
}

void draw_screen() {
    clearGLCD(0, 7, 0, 127);

    draw_header();

    draw_param_line(2, PARAM_0);
    draw_param_line(3, PARAM_TIME);
    draw_param_line(4, PARAM_SPEED);

    writeTxt(6, 0, "OK: canvia param");
}

void draw_edit(unsigned char page, unsigned char col) {

}


void handle_buttons(unsigned char btn) {
    if (btn & BTN_RIGHT) {
        selected_screen = (selected_screen + 1) % NUM_SCREENS;
        selected_param = PARAM_0;
        redraw_needed = 1;
    }

    if (btn & BTN_LEFT) {
        selected_screen = (selected_screen - 1 + NUM_SCREENS) % NUM_SCREENS;
        selected_param = PARAM_0;
        redraw_needed = 1;
    }

    if (btn & BTN_OK) {
        if (edit_mode == 0) edit_mode = 1;
        else edit_mode = 0;
        redraw_needed = 1;
    }

    if (btn & BTN_UP) {
        if (edit_mode) {
         increase_value(selected_screen, selected_param);
        }
        else {
            selected_param = (selected_param - 1 + NUM_PARAMS) % NUM_PARAMS;
        }
        redraw_needed = 1;
    }

    if (btn & BTN_DOWN) {
        if (edit_mode) {
         decrease_value(selected_screen, selected_param);
        }
        else {
            selected_param = (selected_param + 1) % NUM_PARAMS;
        }
        redraw_needed = 1;
    }
}

void init_pic(void) {
    ANSELB = 0x00;
    ANSELA = 0x00;
    ANSELD = 0x00;
    
    TRISA = 0x1F;
    
    PORTB = 0x00;
    PORTD = 0x00;
}

void main(void) {
    unsigned char btn;

    init_pic();
    GLCDinit();

    draw_start_screen();
    draw_screen();

    while (1) {
        btn = get_new_presses();

        if (btn) {
            handle_buttons(btn);
        }

        if (redraw_needed) {
            draw_menu();
            redraw_needed = 0;
        }
    }
}
