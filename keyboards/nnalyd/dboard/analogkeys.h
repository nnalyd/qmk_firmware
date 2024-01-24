#include <stdint.h>
#include <stdbool.h>

enum analog_key_modes { dynamic_actuation = 0, continuous_dynamic_actuation, static_actuation, flashing };

/* global struct to save keypress logic params */
typedef struct {
    uint8_t mode;
    uint8_t actuation_point;
    uint8_t press_sensitivity;
    uint8_t release_sensitivity;
    uint8_t press_hysteresis;
    uint8_t release_hysteresis;
} analog_config; /* 6 bytes */
_Static_assert(sizeof(analog_config) == EECONFIG_KB_DATA_SIZE, "Size mismatch");
extern analog_config g_config;

typedef struct {
    /* For individual analog key data */
    uint16_t value;
    uint8_t extremum;
    int16_t offset;
    bool    static_actuation;
} analog_key_t;
extern analog_key_t keys[MATRIX_ROWS][MATRIX_COLS];
