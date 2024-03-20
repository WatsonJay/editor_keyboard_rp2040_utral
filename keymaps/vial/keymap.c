/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "joystick.h"
#include "analog.h"

#define TEXT_FRAME_DURATION 450

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
#define _VRY GP27
#define _VRX GP28
/* PARAM */
static uint32_t type_count = 0 ;
static uint32_t oled_timer = 0;
static bool finished_timer = false;
static int actuation = 256; // actuation point for arrows (0-511)
uint8_t layer = 0;
int currwpm = 0;
bool arrows[4];
/* RGB MOVE TEXT */
uint32_t rgbText_timer = 0;
int current_rgbText_index = 0;
int rgbModTextLength = 10;
char *lastRgbText = "";
static bool textDirect = true;

/* luna */
/* settings */
#define MIN_WALK_SPEED      10
#define MIN_RUN_SPEED       50
/* timers */
uint32_t anim_timer = 0;
/* current frame */
uint8_t current_frame = 0;
/* advanced settings */
#define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#define ANIM_SIZE           32   // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024
bool isBarking  = false;
int barkCount = 6;
bool isJumping  = false;
bool showedJump = true;
bool isSneaking = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_LY0] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,
        KC_F13,  KC_F14,  KC_F15,  KC_F16,MO(_RGB),
        KC_F17,  KC_F18,  KC_F19,  KC_F20,KC_NO,
        KC_F21,  KC_F22,  KC_F23,  KC_F24,
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

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    //                  Encoder 1                         Encoder 2                         Encoder 3                        Encoder 4                         Encoder 5                        Encoder 6
    [_LY0] =   { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_WBAK, KC_WFWD), ENCODER_CCW_CW(TO(_LY1), TO(_LY8)), ENCODER_CCW_CW(KC_WH_U, KC_WH_D)},
    [_LY1] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(TO(_LY2), TO(_LY0)), ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY2] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(TO(_LY3), TO(_LY1)), ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY3] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(TO(_LY4), TO(_LY2)), ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY4] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(TO(_LY5), TO(_LY3)), ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY5] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(TO(_LY6), TO(_LY4)), ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY6] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(TO(_LY7), TO(_LY5)), ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY7] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(TO(_LY8), TO(_LY6)), ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_LY8] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(TO(_LY0), TO(_LY0)), ENCODER_CCW_CW(KC_NO, KC_NO)  },
    [_RGB] =   { ENCODER_CCW_CW(RGB_HUD, RGB_HUI), ENCODER_CCW_CW(RGB_SAD, RGB_SAI), ENCODER_CCW_CW(RGB_VAD, RGB_VAI), ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),ENCODER_CCW_CW(KC_NO, KC_NO),       ENCODER_CCW_CW(KC_NO, KC_NO)  },
};
#endif

void matrix_init_user(void) {
    type_count = 0;
}

void matrix_scan_user(void) {
    // Up
  	if (!arrows[0] && analogReadPin(_VRY) - 512 < -actuation) {
            dprintf("JOYSTICK up on\n");
  			arrows[0] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,0);
  			register_code16(keycode);
            isJumping = true;
  	} else if (arrows[0] &&  analogReadPin(_VRY) - 512 > -actuation) {
            dprintf("JOYSTICK up off\n");
  			arrows[0] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,0);
  			unregister_code16(keycode);
            isJumping = false;
  	}
    	// Down
  	if (!arrows[1] && analogReadPin(_VRY) - 512 > actuation) {
            dprintf("JOYSTICK down on\n");
  			arrows[1] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,1);
  			register_code16(keycode);
            isSneaking = true;
  	}else if (arrows[1] && analogReadPin(_VRY) - 512 < actuation) {
            dprintf("JOYSTICK down off\n");
  			arrows[1] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,1);
  			unregister_code16(keycode);
            isSneaking = false;
  	}
    // Left
  	if (!arrows[2] && analogReadPin(_VRX) - 512 < -actuation) {
            dprintf("JOYSTICK Left on\n");
  			arrows[2] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 4,2);
  			register_code16(keycode);
  	} else if (arrows[2] &&  analogReadPin(_VRX) - 512 > -actuation) {
            dprintf("JOYSTICK Left off\n");
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
    oled_timer = timer_read32();
    if (record->event.pressed) {
        type_count ++;
    }
    if(type_count > 99999) {
        type_count = 0;
    }
	return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    oled_timer = timer_read32();
    return false;
};

bool wpm_keycode_user(uint16_t keycode) {
	return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
   uint8_t now_layer = get_highest_layer(state);
   static const char PROGMEM up_logo[] = {
        0x1E,0x00
    };
    static const char PROGMEM down_logo[] = {
        0x1F,0x00
    };
   if (now_layer != layer) {
        isBarking = true;
        oled_set_cursor(12, 1);
        if (now_layer > layer) {
            oled_write_P(up_logo, false);
        } else {
            oled_write_P(down_logo, false);
        }
        layer = now_layer;
   }
   return state;
}

#ifdef OLED_ENABLE
static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x80, 0x00, 0xc0, 0xe0, 0x80, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xf0, 0xf0, 0x3f, 0x3f, 0xf3, 0xf3, 0x3f, 0x3f, 0xf3, 0xf3, 0x3f, 0x3f, 0xf8,
        0xf0, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
        0xf0, 0xf8, 0x18, 0x18, 0x98, 0x18, 0x10, 0x00, 0xf0, 0xf8, 0x38, 0x38, 0x30, 0x30, 0xe0, 0x80,
        0x00, 0x00, 0xf8, 0x30, 0x18, 0x18, 0x18, 0xf8, 0xf8, 0x18, 0x18, 0x08, 0x00, 0xc0, 0xe0, 0x70,
        0x38, 0x18, 0x18, 0xf0, 0x80, 0x00, 0x10, 0xf0, 0xf8, 0x18, 0x98, 0xd0, 0x60, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xf0, 0x70, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0x18, 0x18, 0x18, 0xf8, 0xf8, 0x38,
        0x18, 0x08, 0x18, 0xb0, 0xf8, 0x18, 0x98, 0x90, 0x70, 0x00, 0x00, 0x00, 0xe0, 0xf8, 0x38, 0xf8,
        0xf0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x30,
        0x37, 0x33, 0x31, 0x31, 0x39, 0x19, 0x00, 0x1e, 0x3f, 0x38, 0x38, 0x38, 0x18, 0x1c, 0x0e, 0x01,
        0x00, 0x7f, 0x3f, 0x00, 0x00, 0x00, 0x74, 0x1f, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x1b, 0x30, 0x30,
        0x30, 0x38, 0x1c, 0x0f, 0x01, 0x00, 0x3e, 0x1f, 0x03, 0x0f, 0x1f, 0x31, 0xe0, 0xc0, 0x00, 0x00,
        0x00, 0x00, 0x0c, 0x3f, 0x30, 0x38, 0x1c, 0x1e, 0x7f, 0x3f, 0x00, 0x00, 0x40, 0x7f, 0x0f, 0x00,
        0x00, 0x00, 0x3c, 0x3f, 0x03, 0x0f, 0x1f, 0x39, 0x60, 0xc0, 0x3c, 0x3f, 0x07, 0x07, 0x02, 0x03,
        0x3f, 0xff, 0x00, 0x00, 0x7f, 0x3f, 0x30, 0x38, 0x18, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    oled_write_raw_P(qmk_logo, sizeof(qmk_logo));
}

static void render_layer_info(void) {
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("LAYER "), false);
    oled_set_cursor(9, 1);
    switch (get_highest_layer(layer_state)) {
        case _LY0:
            oled_write_P(PSTR("  0"), false);
            break;
        case _LY1:
            oled_write_P(PSTR("  1"), false);
            break;
        case _LY2:
            oled_write_P(PSTR("  2"), false);
            break;
        case _LY3:
            oled_write_P(PSTR("  3"), false);
            break;
        case _LY4:
            oled_write_P(PSTR("  4"), false);
            break;
        case _LY5:
            oled_write_P(PSTR("  5"), false);
            break;
        case _LY6:
            oled_write_P(PSTR("  6"), false);
            break;
        case _LY7:
            oled_write_P(PSTR("  7"), false);
            break;
        case _LY8:
            oled_write_P(PSTR("  8"), false);
            break;
        case _RGB:
            oled_write_P(PSTR("RGB"), false);
            break;
        default:
            oled_write_P(PSTR("ERR"), false);
    }
}

static void render_rgb_info(void) {
    // all rgb modes
    static const PROGMEM char *colorMods[] = {"NONE","SOLID COLOR", "ALPHAS MODS", "GRADIENT UP DOWN", "GRADIENT LEFT RIGHT", "BREATHING", "BAND SAT", "BAND VAL", "BAND PINWHEELSAT",
                                          "BAND PINWHEEL VAL", "BAND SPIRAL SAT", "BAND SPIRAL VAL", "CYCLE ALL", "CYCLE LEFT RIGHT", "CYCLE UP DOWN", "RAINBOW MOVING CHEVRON",
                                          "CYCLE OUT IN", "CYCLE OUT IN DUAL", "CYCLE PINWHEEL", "CYCLE SPIRAL", "DUAL BEACON", "RAINBOW BEACON", "RAINBOW PINWHEELS", "RAINBOW PINWHEELS",
                                          "RAINDROPS", "JELLYBEAN RAINDROPS", "HUE BREATHING", "HUE PENDULUM", "HUE WAVE", "PIXEL FRACTAL", "PIXEL FLOW", "PIXEL RAIN", "TYPING HEATMAP",
                                          "DIGITAL RAIN", "SOLID REACTIVE SIMPLE", "SOLID REACTIVE", "SOLID REACTIVE WIDE", "SOLID REACTIVE MULTIWIDE", "SOLID REACTIVE CROSS",
                                          "SOLID REACTIVE CROSS", "SOLID REACTIVE NEXUS", "SOLID REACTIVE MULTINEXUS", "SPLASH", "MULTISPLASH", "SOLID SPLASH", "SOLID MULTISPLASH",
                                          "STARLIGHT", "STARLIGHT DUAL HUE", "STARLIGHT DUAL SAT", "RIVERFLOW"};

    // move long text
    void move_text(char* text) {
        if (strcmp(lastRgbText, text) != 0) {
            current_rgbText_index = 0;
            lastRgbText = text;
            oled_write_P(PSTR("          "), false);
            oled_set_cursor(3, 0);
        }
        char dest[11] = {""};
        if (strlen(text) > rgbModTextLength) {
            if (textDirect) {
                strncpy(dest, text + current_rgbText_index, rgbModTextLength);
                if (current_rgbText_index >= strlen(text) - rgbModTextLength) {
                    textDirect = !textDirect;
                }else {
                    current_rgbText_index += 1;
                }
            }else {
                strncpy(dest, text + current_rgbText_index, rgbModTextLength);
                if (current_rgbText_index <= 0) {
                    textDirect = !textDirect;
                }else {
                    current_rgbText_index -= 1;
                }
            }
            dest[10] = '\0';
        } else {
            strncpy(dest, text, strlen(text));
        }
        oled_write_P(PSTR(dest), false);
    }
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("RGB "), false);
    uint8_t rgbModNum = rgb_matrix_get_mode();
    const char *modStr = colorMods[rgbModNum];
    if (timer_elapsed32(rgbText_timer) > TEXT_FRAME_DURATION) {
        rgbText_timer = timer_read32();
        oled_set_cursor(4, 0);
        move_text( (char *)modStr);
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
    oled_write(type_counter, true);
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
    oled_write(wpm_counter, true);
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

    /* animation timer */
    if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animate_luna();
    }
}

bool oled_task_user(void) {
    #if OLED_TIMEOUT > 0
    uprintf("%ld ms, %ld ms, %d\n", oled_timer, timer_elapsed32(oled_timer), get_current_wpm());
    if (timer_elapsed32(oled_timer) > OLED_TIMEOUT && get_current_wpm() == 000) {
        oled_clear();
        oled_off();
        return false;
    } else {
        oled_on();
    }
    #endif
    uint32_t uptime_millsec = timer_read32 ();
    if (!finished_timer && uptime_millsec < 2000) {
        render_logo();
    } else {
        if (!finished_timer) {
            oled_clear();
            finished_timer = true;
        }
        render_layer_info();
        render_rgb_info();
        render_count();
        currwpm = get_current_wpm();
        render_wpm();
        render_luna(16, 1);
    }
    return false;
}

void oled_render_boot(bool bootloader) {
    oled_set_cursor(0, 1);
    if (bootloader) {
        oled_write_P(PSTR("Awaiting New Firmware "), false);
    } else {
        oled_write_P(PSTR("Rebooting "), false);
    }
}

bool shutdown_user(bool jump_to_bootloader) {
    oled_render_boot(jump_to_bootloader);
    return false;
}

#endif
