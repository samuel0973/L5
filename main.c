#include "myFunctions.h"

void main(void) {
    unsigned char btn;

    init_pic();
    GLCDinit();

    draw_start_screen();
    draw_screen(selected_screen);

    while (1) {
        btn = debounce_buttons();
        byte old_arrow_page = get_page(selected_param);

        if ((btn & BTN_RIGHT) && selected_screen != CENTRIFUGAT) {
            my_clear(selected_screen);
            selected_screen = (selected_screen + 1) % NUM_SCREENS;
            selected_param = PARAM_0;
            draw_screen(selected_screen);
        }

        if ((btn & BTN_LEFT) && selected_screen != RENTAT) {
            my_clear(selected_screen);
            selected_screen = (selected_screen - 1 + NUM_SCREENS) % NUM_SCREENS;
            selected_param = PARAM_0;
            draw_screen(selected_screen);
        }

        if (btn & BTN_OK) {
            if (edit_mode == 0)
                edit_mode = 1;
            else
                edit_mode = 0;

            draw_arrow(selected_param, old_arrow_page);
        }

        if (btn & BTN_UP) {
            if (edit_mode) {
                increase_value(selected_screen, selected_param);
                draw_edit(selected_param);
            } else if (selected_param != PARAM_0) {
                selected_param = (selected_param - 1 + NUM_PARAMS) % NUM_PARAMS;
                draw_arrow(selected_param, old_arrow_page);
            }
        }

        if (btn & BTN_DOWN) {
            if (edit_mode) {
                decrease_value(selected_screen, selected_param);
                draw_edit(selected_param);
            } else if (selected_param != PARAM_SPEED) {
                selected_param = (selected_param + 1) % NUM_PARAMS;
                draw_arrow(selected_param, old_arrow_page);
            }
        }
    }
}
