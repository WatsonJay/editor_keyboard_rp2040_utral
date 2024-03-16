// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#define _LY0 0
#define _LY1 1
#define _LY2 2
#define _LY3 3
#define _LY4 4
#define _LY5 5
#define _LY6 6
#define _LY7 7
#define _LY8 8
#define _RGB 9

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_LY0] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   DB_TOGG, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        MO(_RGB),KC_NO,   KC_NO,   KC_NO,
        KC_MS_U, KC_MS_D, KC_MS_L, KC_MS_R
    ),
    [_LY1] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_LY2] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_LY3] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_LY4] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_LY5] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_LY6] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_LY7] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_LY8] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_RGB] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, RGB_TOG, KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    )
};
