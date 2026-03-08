# ninjaPixel01 Keymap

## Build Steps

Build firmware using ninjaPixel01 keymap:
```shell
qmk compile -kb mechboards/sofle/pro -km ninjaPixel01
```

## Flashing Steps

1. Make sure the target keyboard is unplugged
    1. **then** ensure TRRS cable is not attached (never remove TRRS cable when there is power to the board)
2. Run the flash command below.
3. Wait for the prompt: `Waiting for UF2 drive...`
4. Connect the left half via USB-C
5. Double-press the reset button (below the TRRS socket) to enter bootloader mode.  
   - The board will appear as a USB drive in Finder — this is expected.
6. QMK detects the drive and copies the firmware automatically.  It then disconnects the board
7. Repeat steps 1–5 for the right half.

The flash command:
```shell
qmk flash -kb mechboards/sofle/pro -km ninjaPixel01
```
