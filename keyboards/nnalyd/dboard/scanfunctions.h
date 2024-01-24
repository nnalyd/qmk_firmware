#pragma once
#include "analogkeys.h"
#include <stdint.h>
#include "quantum.h"

void register_key(matrix_row_t *current_row, uint8_t current_col);

void deregister_key(matrix_row_t *current_row, uint8_t current_col);

void matrix_read_cols_static_actuation(matrix_row_t *current_row, uint8_t current_col, analog_key_t *key);
