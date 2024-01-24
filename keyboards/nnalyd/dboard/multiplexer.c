#include "quantum.h"
#include "multiplexer.h"

const pin_t mux_pins[MUXES] = MUX_PINS;
const pin_t mux_selector_pins[MUX_SELECTOR_BITS] = MUX_SELECTOR_PINS;

void multiplexer_init(void) {
    for (uint8_t i = 0; i < MUX_SELECTOR_BITS; i++) {
        pin_t pin = mux_selector_pins[i];
        setPinOutput(pin);
    }
}

bool select_mux(uint8_t channel) {
    if (channel > MUX_CHANNELS) return 0;
    for (uint8_t i = 0; i < MUX_SELECTOR_BITS; i++) {
        writePin(mux_selector_pins[i], channel & (1 << i));
    }
    current_channel = channel;
    return 1;
}

const mux_t NC = {255,255};
const mux_t mux_index[MUXES][MUX_CHANNELS] = {
    {{1,1}, {1,2}, {0,2}, NC, {0,1}, {1,0}, {0,0}, {2,0}, {3,0}, {4,1}, {4,0}, {4,2}, {2,2}, {3,1}, {2,1}, {3,2}},
    {{1,4}, {1,5}, {0,6}, {2,5}, {0,5}, NC, {0,4}, {0,3}, {2,3}, {3,3}, {1,3}, {3,4}, NC, {4,3}, {2,4}, {3,5}},
    {{1,7}, {1,8}, {0,9}, NC, {0,8}, NC, {0,7}, {1,6}, NC, {2,6}, NC, {3,6}, NC, {3,7}, {2,8}, {2,7}},
    {{1,10}, {1,11}, {0,12}, NC, {0,11}, NC, {0,10}, {1,9}, {2,9}, {3,8}, NC, {4,4}, {3,10}, {3,9}, {2,10}, {4,5}},
    {{1,13}, NC, {0,13}, NC, {1,12}, NC, {2,11}, NC, NC, {4,6}, NC, {4,7}, NC, {3,11}, NC, {2,12}},
};
