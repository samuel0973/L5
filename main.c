#include "myFunctions.h"


void main() {
    unsigned char btn;

    init_pic();
    GLCDinit();

    draw_start_screen();
    draw_screen(selected_screen);

    while (1) {
        btn = debounce_buttons();

        if (btn) {
            handle_buttons(btn);
        }
    }
}
