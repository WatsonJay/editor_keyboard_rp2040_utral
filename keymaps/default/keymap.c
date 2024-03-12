#include QMK_KEYBOARD_H
#define _LY0 0
#define _LY1 1
#define _LY2 2
#define _LY3 3
#define _SET 4

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_LY0] = LAYOUT(
        KC_MSTP, KC_MPLY, KC_0, TO(1), KC_RGHT,
		KC_ESC,  KC_MUTE, KC_1, KC_2, KC_3,
		KC_WH_U, KC_WH_D, KC_4, KC_5, KC_6,
		KC_LEFT, KC_ENT,  KC_7, KC_8, KC_9,
		KC_VOLD, KC_VOLU, KC_PGDN, KC_PGUP
    ),
    [_LY1] = LAYOUT(
        _______, _______, _______, TO(2), _______,
		_______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______,
		_______, _______, _______, _______
    ),
    [_LY2] = LAYOUT(
        _______, _______, _______, TO(3), _______,
		_______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______,
		_______, _______, _______, _______
    ),
    [_LY3] = LAYOUT(
        _______, _______, _______, TO(4), _______,
		_______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______,
		_______, _______, _______, _______
    ),
    [_SET] = LAYOUT(
        KC_NO, KC_NO, KC_NO, TO(0), RGB_VAI,
		RGB_TOG,  KC_NO, RGB_M_P, RGB_M_B, RGB_M_R,
		RGB_SAI, RGB_SAD, RGB_M_SW, RGB_M_SN, RGB_M_K,
		RGB_VAD, KC_ENT,  RGB_M_X, RGB_M_G, RGB_M_T,
		RGB_RMOD, RGB_MOD, RGB_HUD, RGB_HUI
    )
};

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

// void keyboard_post_init_user(void) {
//   debug_enable = true;
//   debug_matrix = true;
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (!clockwise) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }
    } else if (index == 1) {
        if (!clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    } else if (index == 2){
        if (!clockwise) {
            tap_code(KC_LEFT);
        } else {
            tap_code(KC_RGHT);
        }
    }
    return true;
}

#ifdef OLED_ENABLE
    oled_rotation_t oled_init_user(oled_rotation_t rotation) {
        return rotation;
    }

    bool oled_task_user(void) {
        return false;
    }
#endif
