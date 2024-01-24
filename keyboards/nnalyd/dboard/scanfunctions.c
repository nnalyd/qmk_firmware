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
        if (key->value > 300) {
            deregister_key(current_row, current_col);
        }
    } else {
        if (key->value < 300) {
            register_key(current_row, current_col);
        }
    }
}
