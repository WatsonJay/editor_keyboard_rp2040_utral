/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "joystick.h"
#include "analog.h"

#define _LY0 0
#define _LY1 1
#define _LY2 2
#define _LY3 3
#define _LY4 4
#define _LY5 5
#define _LY6 6
#define _LY7 7
#define _SET 8
#define _VRY GP28
#define _VRX GP29

static uint32_t type_count = 0 ;
// static uint32_t oled_timer = 0;
static int actuation = 256; // actuation point for arrows (0-511)
uint8_t layer = 0;
int currwpm = 0;
bool arrows[4];

/* luna */
/* settings */
#define MIN_WALK_SPEED      10
#define MIN_RUN_SPEED       50
/* timers */
uint16_t anim_timer = 0;
/* current frame */
uint8_t current_frame = 0;
/* advanced settings */
static bool finished_timer = false;
#define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#define ANIM_SIZE           32   // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024

bool isBarking  = false;
int barkCount = 6;
bool isJumping  = false;
bool showedJump = true;
bool isSneaking = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_LY0] = LAYOUT(
        KC_MPLY, KC_MSEL, TO(_LY1), KC_P0,
        KC_P7,   KC_P8,   KC_P9,   KC_PSLS,
        KC_P4,   KC_P5,   KC_P6,   KC_PAST,
        KC_PPLS, KC_P3,   KC_P2,   KC_P1,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    ),
    [_LY1] = LAYOUT(
        _______, _______, TO(_LY2), _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    ),
    [_LY2] = LAYOUT(
        _______, _______, TO(_LY3), _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    ),
    [_LY3] = LAYOUT(
        _______, _______, TO(_LY4), _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    ),
    [_LY4] = LAYOUT(
        _______, _______, TO(_LY5), _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    ),
    [_LY5] = LAYOUT(
        _______, _______, TO(_LY6), _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    ),
    [_LY6] = LAYOUT(
        _______, _______, TO(_LY7), _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    ),
    [_LY7] = LAYOUT(
        _______, _______, TO(_SET), _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    ),
    [_SET] = LAYOUT(
        _______, _______, TO(_LY0), _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        _______, _______, _______,  _______,
        KC_NO,   KC_NO,   KC_NO,   KC_NO
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    //                  Encoder 1                         Encoder 2                         Encoder 3
    [_LY0] =   { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_BRID, KC_BRIU)  },
    [_LY1] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY2] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY3] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY4] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY5] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY6] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY7] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_SET] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },
};
#endif

void matrix_init_user(void) {
    type_count = 0;
}

void matrix_scan_user(void) {

      // Up
  	if (!arrows[0] && analogReadPin(_VRY) - 512 < -actuation) {
            uprintf("JOYSTICK up on\n");
  			arrows[0] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,0);
  			register_code16(keycode);
            isJumping = true;
  	} else if (arrows[0] &&  analogReadPin(_VRY) - 512 > -actuation) {
            uprintf("JOYSTICK up off\n");
  			arrows[0] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,0);
  			unregister_code16(keycode);
            isJumping = false;
  	}
		// Down
  	if (!arrows[1] && analogReadPin(_VRY) - 512 > actuation) {
            uprintf("JOYSTICK down on\n");
  			arrows[1] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,1);
  			register_code16(keycode);
            isSneaking = true;
  	}else if (arrows[1] && analogReadPin(_VRY) - 512 < actuation) {
            uprintf("JOYSTICK down off\n");
  			arrows[1] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,1);
  			unregister_code16(keycode);
            isSneaking = false;
  	}
    // Left
  	if (!arrows[2] && analogReadPin(_VRX) - 512 < -actuation) {
            uprintf("JOYSTICK Left on\n");
  			arrows[2] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,2);
  			register_code16(keycode);
  	} else if (arrows[2] &&  analogReadPin(_VRX) - 512 > -actuation) {
            uprintf("JOYSTICK Left off\n");
  			arrows[2] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,2);
  			unregister_code16(keycode);
  	}
    // Right
  	if (!arrows[3] && analogReadPin(_VRX) - 512 > actuation) {
            uprintf("JOYSTICK Right on\n");
  			arrows[3] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,3);
  			register_code16(keycode);
  	} else if (arrows[3] && analogReadPin(_VRX) - 512 < actuation) {
            uprintf("JOYSTICK Right off\n");
  			arrows[3] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,3);
  			unregister_code16(keycode);
  	}
}

// Joystick config
joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    [0] = JOYSTICK_AXIS_VIRTUAL,
    [1] = JOYSTICK_AXIS_VIRTUAL
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        type_count ++;
    }
    if(type_count > 99999) {
        type_count = 0;
    }
	return true;
}

bool wpm_keycode_user(uint16_t keycode) {
	return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    // vial_encoder_update need to change return encoder_update_user(index, clockwise)
    type_count ++;
    if(type_count > 99999) {
        type_count = 0;
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
   uint8_t now_layer = get_highest_layer(state);
   if (now_layer != layer) {
        layer = now_layer;
        isBarking = true;
   }
   return state;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return rotation;
}

static void render_layer_info(void) {
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("LAYER NUM: "), false);
    oled_set_cursor(11, 0);
    switch (get_highest_layer(layer_state)) {
        case _LY0:
            oled_write_P(PSTR("0   "), false);
            break;
        case _LY1:
            oled_write_P(PSTR("1   "), false);
            break;
        case _LY2:
            oled_write_P(PSTR("2   "), false);
            break;
        case _LY3:
            oled_write_P(PSTR("3   "), false);
            break;
        case _LY4:
            oled_write_P(PSTR("4   "), false);
            break;
        case _LY5:
            oled_write_P(PSTR("5   "), false);
            break;
        case _LY6:
            oled_write_P(PSTR("6   "), false);
            break;
        case _LY7:
            oled_write_P(PSTR("7   "), false);
            break;
        case _SET:
            oled_write_P(PSTR("SET "), false);
            break;
        default:
            oled_write_P(PSTR("NONE"), false);
    }
}

static void render_count(void) {
    oled_set_cursor(5, 2);
    oled_write_P(PSTR("TAPCOUNT"), false);
    oled_set_cursor(8, 3);
    uint16_t n = type_count;
    char    type_counter[6];
    type_counter[5] = '\0';
    type_counter[4] = '0' + n % 10;
    type_counter[3] = (n /= 10) % 10 ? '0' + (n) % 10 : (n / 10) % 10 ? '0' : ' ';
    type_counter[2] = (n /= 10) % 10 ? '0' + (n) % 10 : (n / 10) % 10 ? '0' : ' ';
    type_counter[1] = (n /= 10) % 10 ? '0' + (n) % 10 : (n / 10) % 10 ? '0' : ' ';
    type_counter[0] = n / 10 ? '0' + n / 10 : ' ';
    oled_write(type_counter, false);
}

static void render_wpm(void) {
    uint8_t n = get_current_wpm();
    char    wpm_counter[4];
    wpm_counter[3] = '\0';
    wpm_counter[2] = '0' + n % 10;
    wpm_counter[1] = (n /= 10) % 10 ? '0' + (n) % 10 : (n / 10) % 10 ? '0' : ' ';
    wpm_counter[0] = n / 10 ? '0' + n / 10 : ' ';
    oled_set_cursor(0, 2);
    oled_write_P(PSTR("WPM"), false);
    oled_set_cursor(0, 3);
    oled_write(wpm_counter, false);
}

static void render_rgb_info(void) {
    // oled_set_cursor(4, 2);
    // oled_write_P(PSTR("Animation"), false);
    oled_set_cursor(0, 1);
    // oled_write(get_u8_str(rgb_matrix_get_mode(), ' '), false);
    switch (rgb_matrix_get_mode()) {
        case 1:
            oled_write_P(PSTR("Solid         "), false);
            break;
        case 2:
            oled_write_P(PSTR("Alphas        "), false);
            break;
        case 3:
            oled_write_P(PSTR("GradientUD    "), false);
            break;
        case 4:
            oled_write_P(PSTR("GradientLR    "), false);
            break;
        case 5:
            oled_write_P(PSTR("Breathing     "), false);
            break;
        case 6:
            oled_write_P(PSTR("BandSat       "), false);
            break;
        case 7:
            oled_write_P(PSTR("BandVal       "), false);
            break;
        case 8:
            oled_write_P(PSTR("PinwheelS     "), false);
            break;
        case 9:
            oled_write_P(PSTR("PinwheelV     "), false);
            break;
        case 10:
            oled_write_P(PSTR("SpiralS       "), false);
            break;
        case 11:
            oled_write_P(PSTR("SpiralV       "), false);
            break;
        case 12:
            oled_write_P(PSTR("CycleAll      "), false);
            break;
        case 13:
            oled_write_P(PSTR("CycleLR       "), false);
            break;
        case 14:
            oled_write_P(PSTR("CycleUD       "), false);
            break;
        case 15:
            oled_write_P(PSTR("RainbowMC     "), false);
            break;
        case 16:
            oled_write_P(PSTR("CycleOI       "), false);
            break;
        case 17:
            oled_write_P(PSTR("CycleOID      "), false);
            break;
        case 18:
            oled_write_P(PSTR("CycleP        "), false);
            break;
        case 19:
            oled_write_P(PSTR("CycleS        "), false);
            break;
        case 20:
            oled_write_P(PSTR("DualBeacon    "), false);
            break;
        case 21:
            oled_write_P(PSTR("RainbowB      "), false);
            break;
        case 22:
            oled_write_P(PSTR("RainbowP      "), false);
            break;
        case 23:
            oled_write_P(PSTR("Raindrops     "), false);
            break;
        case 24:
            oled_write_P(PSTR("JellybeanRain "), false);
            break;
        case 25:
            oled_write_P(PSTR("HueBreathing  "), false);
            break;
        case 26:
            oled_write_P(PSTR("HuePendulum   "), false);
            break;
        case 27:
            oled_write_P(PSTR("HueWave       "), false);
            break;
        case 28:
            oled_write_P(PSTR("PixelRain     "), false);
            break;
        case 29:
            oled_write_P(PSTR("PixelFlow     "), false);
            break;
        case 30:
            oled_write_P(PSTR("PixelFractal  "), false);
            break;
        case 31:
            oled_write_P(PSTR("TypingHeatmap "), false);
            break;
        case 32:
            oled_write_P(PSTR("DigitalRain   "), false);
            break;
        case 33:
            oled_write_P(PSTR("SolidReactiveS"), false);
            break;
        case 34:
            oled_write_P(PSTR("SolidReactive "), false);
            break;
        case 35:
            oled_write_P(PSTR("SolidReactiveW"), false);
            break;
        case 36:
            oled_write_P(PSTR("SolidReactiveM"), false);
            break;
        case 37:
            oled_write_P(PSTR("SolidReactiveC"), false);
            break;
        case 38:
            oled_write_P(PSTR("SolidReactiveM"), false);
            break;
        case 39:
            oled_write_P(PSTR("SolidReactiveN"), false);
            break;
        case 40:
            oled_write_P(PSTR("SolidReactiveM"), false);
            break;
        case 41:
            oled_write_P(PSTR("Splash        "), false);
            break;
        case 42:
            oled_write_P(PSTR("Multisplash   "), false);
            break;
        case 43:
            oled_write_P(PSTR("SolidSplash   "), false);
            break;
        case 44:
            oled_write_P(PSTR("SolidM        "), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined  "), false);
    }
}

/* logic */
static void render_luna(int LUNA_X, int LUNA_Y) {
    /* Sit */
    static const char PROGMEM sit[2][3][ANIM_SIZE] = {
        /* 'sit1', 32x22px */
        {
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        },

        /* 'sit2', 32x22px */
        {
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        }
    };

    /* Walk */
    static const char PROGMEM walk[2][3][ANIM_SIZE] = {
        /* 'walk1', 32x22px */
        {
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80, 0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03, 0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        },

        /* 'walk2', 32x22px */
        {
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        }
        };

    /* Run */
    static const char PROGMEM run[2][3][ANIM_SIZE] = {
        /* 'run1', 32x22px */
        {
            { 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 }
        },

        /* 'run2', 32x22px */
        {
            { 0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        }};

    /* Bark */
    static const char PROGMEM bark[2][3][ANIM_SIZE] = {
        /* 'bark1', 32x22px */
        {
            { 0x00, 0xc0, 0x20, 0x10, 0xd0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        },

        /* 'bark2', 32x22px */
        {
            { 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x2c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x20, 0x4a, 0x09, 0x10 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        }};

    /* Sneak */
    static const char PROGMEM sneak[2][3][ANIM_SIZE] = {
        /* 'sneak1', 32x22px */
        {
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x1e, 0x21, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x04, 0x04, 0x04, 0x03, 0x01, 0x00, 0x00, 0x09, 0x01, 0x80, 0x80, 0xab, 0x04, 0xf8, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x02, 0x06, 0x18, 0x20, 0x20, 0x38, 0x08, 0x10, 0x18, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00 }
        },

        /* 'sneak2', 32x22px */
        {
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa0, 0x20, 0x40, 0x80, 0xc0, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x40, 0x55, 0x82, 0x7c, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x04, 0x18, 0x10, 0x08, 0x10, 0x20, 0x28, 0x34, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 }
        }};

    /* animation */
    void animate_luna(void) {
        int START_Y = 0;
        /* jump */
        if (isJumping || !showedJump) {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y + 2);
            oled_write("     ", false);
            START_Y = LUNA_Y - 1;
            showedJump = true;
        } else {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y - 1);
            oled_write("     ", false);
            START_Y = LUNA_Y;
        }

        /* switch frame */
        current_frame = (current_frame + 1) % 2;
        /* current status */
        if (isBarking) {
            oled_set_cursor(LUNA_X, START_Y);
            oled_write_raw_P(bark[current_frame][0], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 1);
            oled_write_raw_P(bark[current_frame][1], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 2);
            oled_write_raw_P(bark[current_frame][2], ANIM_SIZE);
            if (barkCount >= 0){
                barkCount--;
            } else {
                isBarking = false;
                barkCount = 6;
            }
        } else if (isSneaking) {
            oled_set_cursor(LUNA_X, START_Y);
            oled_write_raw_P(sneak[current_frame][0], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 1);
            oled_write_raw_P(sneak[current_frame][1], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 2);
            oled_write_raw_P(sneak[current_frame][2], ANIM_SIZE);
        } else if (currwpm <= MIN_WALK_SPEED) {
            oled_set_cursor(LUNA_X, START_Y);
            oled_write_raw_P(sit[current_frame][0], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 1);
            oled_write_raw_P(sit[current_frame][1], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 2);
            oled_write_raw_P(sit[current_frame][2], ANIM_SIZE);

        } else if (currwpm <= MIN_RUN_SPEED) {
            oled_set_cursor(LUNA_X, START_Y);
            oled_write_raw_P(walk[current_frame][0], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 1);
            oled_write_raw_P(walk[current_frame][1], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 2);
            oled_write_raw_P(walk[current_frame][2], ANIM_SIZE);
        } else {
            oled_set_cursor(LUNA_X, START_Y);
            oled_write_raw_P(run[current_frame][0], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 1);
            oled_write_raw_P(run[current_frame][1], ANIM_SIZE);
            oled_set_cursor(LUNA_X, START_Y + 2);
            oled_write_raw_P(run[current_frame][2], ANIM_SIZE);
        }
    }

    #if OLED_TIMEOUT > 0
    /* the animation prevents the normal timeout from occuring */
    if (last_input_activity_elapsed() > OLED_TIMEOUT && last_led_activity_elapsed() > OLED_TIMEOUT) {
        oled_off();
        return;
    } else {
        oled_on();
    }
    #endif

    /* animation timer */
    if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animate_luna();
    }
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
    uint32_t uptime_millsec = timer_read32 ();
    if (!finished_timer && uptime_millsec < 2000) {
        render_logo();
    } else {
        if (!finished_timer) {
            oled_clear();
            finished_timer = true;
        }
        currwpm = get_current_wpm();
        render_layer_info();
        render_count();
        render_wpm();
        render_rgb_info();
        render_luna(16, 1);
    }
    return false;
}
#endif
