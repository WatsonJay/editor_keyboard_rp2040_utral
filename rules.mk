# This file intentionally left blank
# MCU name
MCU = atmega32u4

# Processor frequency
F_CPU = 16000000

# Bootloader selection
BOOTLOADER = caterina


# Build Options
#   comment out to disable the options.
#
BOOTMAGIC_ENABLE = yes	# Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE = yes	# Mouse keys(+4700)
EXTRAKEY_ENABLE = yes	# Audio control and System control(+450)
CONSOLE_ENABLE = no	# Console for debug(+400)
COMMAND_ENABLE = no    # Commands for debug and configuration
NKRO_ENABLE = yes		# USB Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
BACKLIGHT_ENABLE = no  # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no  # Enable keyboard RGB underglow
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812
AUDIO_ENABLE = no
WPM_ENABLE = yes
ENCODER_ENABLE = yes
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
