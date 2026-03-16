This fork of QMK is for the local development of the following keyboards:
- `mechboards/sofle/pro`
- `sofle/rev1`
- `preonic/rev3_drop` 

The `mechboards/sofle/pro` layout is mainly complete and the next stages of this project are to port the keymaps to the other 2 boards.
The base layout is for MacOS, using the Colemak layout. The layout is designed to also work with Windows, and there are additional layers to deal with the 
Windows operating system; these layers are mainly transparent keys (which fall through to the lower Mac layer) but some keys declare Windows-centric shortcuts / keys etc.

You should write verbose comments with any code you write .

# Hardware
## Sofle V1
- 2 x elite-c (v4) microcontroller
- ATmega32u4 controller

## Sofle Pro
- Rotary encoders
- OLED Screens
- RP2040 USB-C Controllers
