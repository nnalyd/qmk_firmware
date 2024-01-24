#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "print.h"
#include "quantum.h"
#include "analog.h"
#include "multiplexer.h"
#include "scanfunctions.h"

analog_key_t keys[MATRIX_ROWS][MATRIX_COLS] = {0};
uint16_t max_key = 0;

void matrix_init_custom(void) {
    //generate_lut();
    multiplexer_init();

    //get_sensor_offsets();
    // wait_ms(100); // Let ADC reach steady state
    //get_sensor_offsets();
}

matrix_row_t previous_matrix[MATRIX_ROWS];
bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    memcpy(previous_matrix, current_matrix, sizeof(previous_matrix));
    for (uint8_t channel = 0; channel < MUX_CHANNELS; channel++) {
        uint8_t channel_greycoded = (channel >> 1) ^ channel;
        select_mux(channel_greycoded);
        for (uint8_t mux = 0; mux < 5; mux++) {
            uint8_t current_row = mux_index[mux][channel_greycoded].row;
            uint8_t current_col = mux_index[mux][channel_greycoded].col;

            if (current_row == 255 || current_col == 255) continue;
            pin_t pin = mux_pins[mux];

            analog_key_t *key = &keys[current_row][current_col];
            key->current = analogReadPin(pin);

            // Check if the current key is the highest
            if (key->current > key->max) {
                key->max = key->current;
            }

            matrix_read_cols_static_actuation(&current_matrix[current_row], current_col, key);
            //matrix_read_cols_continuous_dynamic_actuation(&current_matrix[current_row], current_col, key);
        }
    }
    return memcmp(previous_matrix, current_matrix, sizeof(previous_matrix)) != 0;
}
