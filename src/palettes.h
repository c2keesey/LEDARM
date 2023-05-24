#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

DEFINE_GRADIENT_PALETTE (ocean_gp) {
    0, 19, 22, 255,
    64, 0, 240, 240,
    191, 0, 6, 126,
    255, 19, 22, 255
};

DEFINE_GRADIENT_PALETTE (ocean_gp_v2) {
    0, 19, 22, 255,
    43, 1, 177, 217,
    127, 17, 0, 119,
    170, 1, 177, 217,
    255, 19, 22, 255
};

DEFINE_GRADIENT_PALETTE (soft_blue_pink_gp) {
    0, 142, 197, 252,
    127, 224, 195, 252,
    255, 142, 197, 252
};

DEFINE_GRADIENT_PALETTE (soft_watermellon) {
    0, 252, 142, 142,
    127, 98, 255, 96,
    254, 252, 142, 142
};

DEFINE_GRADIENT_PALETTE (strong_watermellon) {
    0, 255, 9, 9,
    127, 23, 255, 20,
    254, 255, 9, 9
};

DEFINE_GRADIENT_PALETTE (emerald_dragon) {
    0, 86, 250, 66,
    63, 8, 175, 34,
    127, 9, 231, 255,
    191, 8, 175, 34,
    254, 86, 250, 66
};

DEFINE_GRADIENT_PALETTE (valentine) {
    0, 208, 27, 80,
    84, 108, 78, 250,
    170, 200, 53, 242,
    254, 252, 55, 170
};