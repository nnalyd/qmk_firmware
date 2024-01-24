#include "scanfunctions.h"
#include "analog.h"
#include "multiplexer.h"

void update_extremum(analog_key_t *key) {
    key->extremum = key->current;
}

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
            if (key->current > key->extremum) {
                update_extremum(key);

            } else if (key->current < key->extremum - 10) {
                /* Has key moved up enough to be released? */
                deregister_key(current_row, current_col);
                update_extremum(key);
            }
        } else {
            /* Key is not pressed
            Is the key still moving up? */
            if (key->current < key->extremum) {
                update_extremum(key);
            } else if (key->current > key->extremum + 10) {
                /* Has key moved down enough to be pressed? */
                register_key(current_row, current_col);
                update_extremum(key);
            }
        }
        if (key->current == key->max) {
            deregister_key(current_row, current_col);
            update_extremum(key);
            key->continuous_dynamic_actuation = false;
        }
    } else if (key->current > (key->max - 5)) {
        register_key(current_row, current_col);
        update_extremum(key);
        key->continuous_dynamic_actuation = true;
    }
}
