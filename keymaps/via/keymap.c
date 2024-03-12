#include QMK_KEYBOARD_H
#include "print.h"
#define _LY0 0
#define _LY1 1
#define _LY2 2
// #define _LY3 3
#define _SET 3

uint16_t type_count = 0 ;

static uint32_t oled_timer = 0;

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
    // [_LY3] = LAYOUT(
    //     _______, _______, _______, TO(4), _______,
	// 	_______, _______, _______, _______, _______,
	// 	_______, _______, _______, _______, _______,
	// 	_______, _______, _______, _______, _______,
	// 	_______, _______, _______, _______
    // ),
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
    if (record->event.pressed) {
        type_count ++;
    }
    oled_timer = timer_read32();
	return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t layer;
    layer = biton32(layer_state);
    type_count ++;
    oled_timer = timer_read32();
    if (index == 0) { /* First encoder */
        if (!clockwise) {
           register_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 4, .col = 0
            }));
           unregister_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 4, .col = 0
            }));
        } else {
           register_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 4, .col = 1
            }));
           unregister_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 4, .col = 1
            }));
        }
    } else if (index == 1) {
        if (!clockwise) {
           register_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 4, .col = 2
            }));
           unregister_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 4, .col = 2
            }));
        } else {
            register_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 4, .col = 3
            }));
           unregister_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 4, .col = 3
            }));
        }
    } else if (index == 2){
        if (!clockwise) {
            register_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 3, .col = 0
            }));
           unregister_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 3, .col = 0
            }));
        } else {
            register_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 0, .col = 4
            }));
           unregister_code16(keymap_key_to_keycode(layer, (keypos_t) {.row = 0, .col = 4
            }));
        }
    }
    return true;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return rotation;
}

static void render_info(void) {
    oled_write_P(PSTR("Layer Num: "), false);
    switch (get_highest_layer(layer_state)) {
        case _LY0:
            oled_write_ln_P(PSTR("0"), false);
            break;
        case _LY1:
            oled_write_ln_P(PSTR("1"), false);
            break;
        case _LY2:
            oled_write_ln_P(PSTR("2"), false);
            break;
        case _SET:
            oled_write_ln_P(PSTR("3(set)"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }
}

static void render_timer(void) {
    uint32_t uptime_millsec = timer_read32 ();
    uint32_t uptime_sec = uptime_millsec / 1000 ;
   char string[9];
    string[8] = '\0';
    string[7] = '0' + uptime_sec % 10;
    string[6] = (uptime_sec /= 10) % 6 ? '0' + (uptime_sec) % 6 : '0';
    string[5] = ':';
    string[4] = (uptime_sec /= 6) % 10 ? '0' + (uptime_sec) % 10 : '0';
    string[3] = (uptime_sec /= 10) % 6 ? '0' + (uptime_sec) % 6 : '0';
    string[2] = ':';
    string[1] = (uptime_sec /= 6) % 10 ? '0' + (uptime_sec) % 10 : '0';
    string[0] = (uptime_sec /= 10) % 6 ? '0' + (uptime_sec) % 6 : '0';
    oled_write_P(PSTR("onTime: "), false);
    oled_write(string, false);
    oled_write_ln_P(PSTR(" "), false);
}

static void render_count(void) {
    oled_write_P(PSTR("tapcount: "), false);
    oled_write(get_u16_str(type_count, ' '), false);
    oled_write_ln_P(PSTR(" "), false);
}

static void render_wpm(void) {
    uint8_t n = get_current_wpm();
    char    wpm_counter[4];
    wpm_counter[3] = '\0';
    wpm_counter[2] = '0' + n % 10;
    wpm_counter[1] = (n /= 10) % 10 ? '0' + (n) % 10 : (n / 10) % 10 ? '0' : ' ';
    wpm_counter[0] = n / 10 ? '0' + n / 10 : ' ';
    oled_write_P(PSTR("WPM:"), false);
    oled_write(wpm_counter, false);
    oled_write_ln_P(PSTR(" "), false);
}

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };

    oled_write_P(qmk_logo, false);
}

bool oled_task_user(void) {
    if (timer_elapsed32(oled_timer) > OLED_TIMEOUT) {
        oled_off();
        return false;
    } else {
        oled_on();
    }
    static bool finished_timer = false;
    uint32_t uptime_millsec = timer_read32 ();
    if (!finished_timer && uptime_millsec < 2000) {
        render_logo();
    } else {
        if (!finished_timer) {
            oled_clear();
            finished_timer = true;
        }
        render_info();
        render_timer();
        render_count();
        render_wpm();
    }
    return false;
}
#endif

