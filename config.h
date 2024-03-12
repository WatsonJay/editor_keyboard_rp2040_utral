// Copyright 2023 WatsonJay (@WatsonJay)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Double tap the side button to enter bootloader */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP25
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

/* define oled display*/
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3
#ifdef OLED_ENABLE
    #define SSD1306OLED
    #define OLED_TIMEOUT 10000
    #define OLED_BRIGHTNESS 180
#endif

#ifdef POINTING_DEVICE_ENABLE
    #define ANALOG_JOYSTICK_X_AXIS_PIN GP29
    #define ANALOG_JOYSTICK_Y_AXIS_PIN GP28
#endif

/* 磨损均衡 RP2040 驱动程序配置 */
#define WEAR_LEVELING_LOGICAL_SIZE 40960
#define WEAR_LEVELING_BACKING_SIZE 81920

#undef RGBLIGHT_ANIMATIONS
#ifdef RGB_MATRIX_ENABLE
    #define WS2812_DI_PIN GP10
    #define RGB_MATRIX_LED_COUNT 12
    #define RGBLED_NUM 12
    #define RGB_MATRIX_CENTER { 111, 32 }

     /* Enable Framebuffer and keypress effects */
    #define RGB_MATRIX_FRAMEBUFFER_EFFECTS
    #define RGB_MATRIX_KEYPRESSES

    #define ENABLE_RGB_MATRIX_ALPHAS_MODS
    #define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    #define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_BREATHING
    #define ENABLE_RGB_MATRIX_BAND_SAT
    #define ENABLE_RGB_MATRIX_BAND_VAL
    #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
    #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
    #define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
    #define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
    #define ENABLE_RGB_MATRIX_CYCLE_ALL
    #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    #define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    #define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    #define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    #define ENABLE_RGB_MATRIX_DUAL_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    #define ENABLE_RGB_MATRIX_RAINDROPS
    #define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    #define ENABLE_RGB_MATRIX_HUE_BREATHING
    #define ENABLE_RGB_MATRIX_HUE_PENDULUM
    #define ENABLE_RGB_MATRIX_HUE_WAVE
    #define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
    #define ENABLE_RGB_MATRIX_PIXEL_FLOW
    #define ENABLE_RGB_MATRIX_PIXEL_RAIN

    #ifdef RGB_MATRIX_FRAMEBUFFER_EFFECTS
        #define ENABLE_RGB_MATRIX_TYPING_HEATMAP
        #define ENABLE_RGB_MATRIX_DIGITAL_RAIN
    #endif
    #if defined(RGB_MATRIX_KEYPRESSES) || defined(RGB_MATRIX_KEYRELEASES)
        #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
        #define ENABLE_RGB_MATRIX_SOLID_REACTIVE
        #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
        #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
        #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
        #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
        #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
        #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
        #define ENABLE_RGB_MATRIX_SPLASH
        #define ENABLE_RGB_MATRIX_MULTISPLASH
        #define ENABLE_RGB_MATRIX_SOLID_SPLASH
        #define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
    #endif
#endif
