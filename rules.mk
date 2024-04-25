# 设置EEPROM磨损均衡
EEPROM_DRIVER = wear_leveling
WEAR_LEVELING_DRIVER = rp2040_flash

RGB_MATRIX_ENABLE = yes

WPM_ENABLE = yes

DYNAMIC_MACRO_ENABLE = yes

I2C_DRIVER_REQUIRED = yes

OLED_ENABLE = no
OLED_DRIVER = ssd1306

CUSTOM_MATRIX = lite

SRC += analog.c \
	   matrix.c \
	   paj7620.c
