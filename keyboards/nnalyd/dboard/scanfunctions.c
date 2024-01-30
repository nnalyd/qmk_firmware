#include "scanfunctions.h"
#include "analog.h"
#include "multiplexer.h"

void register_key(matrix_row_t *current_row, uint8_t current_col) {
    *current_row |= (1 << current_col);
}

void deregister_key(matrix_row_t *current_row, uint8_t current_col) {
    *current_row &= ~(1 << current_col);
}

void matrix_read_cols_static_actuation(matrix_row_t *current_row, uint8_t current_col, analog_key_t *key) {
    if (*current_row & (1 << current_col)) {
        if (key->current > (key->max - 5)) {
            deregister_key(current_row, current_col);
        }
    } else {
        if (key->current < (key->max - 5)) {
            register_key(current_row, current_col);
        }
    }
}

void matrix_read_cols_continuous_dynamic_actuation(matrix_row_t *current_row, uint8_t current_col, analog_key_t *key) {
    if (key->continuous_dynamic_actuation) {
        if (*current_row & (1 << current_col)) {
            /* Key is pressed
            Is key still moving down? */
            if (key->current < key->actuation) {
                key->actuation = key->current;
            } else if (key->current > key->actuation + g_config.release_sensitivity) { 
                /* Has key moved up enough to be released? */
                deregister_key(current_row, current_col);
                key->actuation = key->current;
            }
        } else {
            /* Key is not pressed
            Is the key still moving up? */
            if (key->current > key->actuation) {
                key->actuation = key->current;
            } else if (key->current < key->actuation - g_config.press_sensitivity) { 
                /* Has key moved down enough to be pressed? */
                register_key(current_row, current_col);
                key->actuation = key->current;
            }
        }
        if (key->current > (key->max - g_config.release_point)) {
            deregister_key(current_row, current_col);
            key->actuation = key->current;
            key->continuous_dynamic_actuation = false;
        }
    } else if (key->current < (key->max - g_config.actuation_point)) {
        register_key(current_row, current_col);
        key->actuation = key->current;
        key->continuous_dynamic_actuation = true;
    }
}
