#include <xc.h>
#include <string.h>
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
    const int max[NUM_PARAMS];
} ScreenSettings;

Screen selected_screen = RENTAT;
Param selected_param = PARAM_0;

ScreenSettings settings[NUM_SCREENS] = {
    // RENTAT: sabo, temps, velocitat
    {{0, 0, 100}, {100, 60, 1000}},

    // ESBANDIT: vegades, temps, velocitat
    {{1, 0, 100}, {5, 60, 1000}},

    // CENTRIFUGAT: suavitzant, temps, velocitat
    {{0, 0, 100}, {100, 60, 1000}}
};

unsigned char prev_buttons = 0;
unsigned char edit_mode = 0;

void writeTxt(unsigned char page, unsigned char y, const char *s);


//button debouncing
unsigned char debounce_buttons();

int get_min(Screen s, Param p);

int get_max(Screen s, Param p);

int get_step(Screen s, Param p);

void increase_value(Screen s, Param p);

void decrease_value(Screen s, Param p);

char *screen_name(Screen s);

char *param_unit(Screen s, Param p);

void draw_start_screen();

byte get_page(Param p);

void draw_param_line(unsigned char page, Param p);

void draw_screen(Screen s);

void my_clear(Screen s);

void draw_edit(Param p);

void draw_arrow(Param p, byte old_page);

void draw_progress_bar(byte p, byte y, byte percent);

void update_progress_bar(Screen s, Param p);

void init_pic();
