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
} ScreenSettings;

Screen selected_screen = RENTAT;
Param selected_param = PARAM_0;

ScreenSettings settings[NUM_SCREENS] = {
    // RENTAT: sabo, temps, velocitat
    {{0, 0, 100}},

    // ESBANDIT: vegades, temps, velocitat
    {{1, 0, 100}},

    // CENTRIFUGAT: suavitzant, temps, velocitat
    {{0, 0, 100}}
};

unsigned char prev_buttons = 0;
unsigned char edit_mode = 0;

void writeTxt(unsigned char page, unsigned char y, const char *s);


//button debouncing
unsigned char debounce_buttons();

//returns minimum value of a Parameter p in Setting s
int get_min(Screen s, Param p);

//returns maximum value of a Parameter p in Setting s
int get_max(Screen s, Param p);

//obtain value to increase/decrease a Parameter p in Setting s
int get_step(Screen s, Param p);

//increases value and modifies progress bar
void increase_value(Screen s, Param p);

//decreases value and modifies progress bar
void decrease_value(Screen s, Param p);

//returns screen name 
char *screen_name(Screen s);

//returns unit of a Parameter p in Setting s
char *param_unit(Screen s, Param p);

//initialize GLCD screen with my name
void draw_start_screen();

//returns page level [0:7] of a Parameter p
byte get_page(Param p);

//draws a progress bar, arrow, value and units
void draw_param_line(unsigned char page, Param p);

//draws Setting header and lines
void draw_screen(Screen s);

//clears current screen of Setting s
void my_clear(Screen s);

//rewrites modified value of a Parameter p
void draw_edit(Param p);

//rewrites arrow at Parameter p from old position
void draw_arrow(Param p, byte old_page);

//draws progress bar at p [0:64], y [0:128] percent[0:100]
void draw_progress_bar(byte p, byte y, byte percent);

//updates progress bar at Settings s and Parameter p
void update_progress_bar(Screen s, Param p);


void init_pic();
