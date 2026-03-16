# Point to the shared ninjaPixel userspace (users/ninjaPixel/)
USER_NAME := ninjaPixel

ENCODER_MAP_ENABLE = yes
TAP_DANCE_ENABLE = yes
CAPS_WORD_ENABLE = yes

# Override bootloader for Elite-C v4 (uses Atmel DFU, not Caterina like Pro Micro)
BOOTLOADER = atmel-dfu
