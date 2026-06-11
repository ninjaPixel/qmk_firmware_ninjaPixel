# Ferris Sweep — ninjaPixel keymap

A **Colemak** keymap for the Ferris Sweep.

## Hardware / converter

The stock `ferris/sweep` definition targets an **AVR Pro Micro** (ATmega32u4),
which builds a `.hex` and flashes over the `caterina` serial bootloader. Our
board is a generic **RP2040 Pro Micro** (USB-C) instead.

The switch is handled entirely by `keymap.json`:

```json
{
    "converter": "rp2040_ce"
}
```

`rp2040_ce` is the "Community Edition" Pro Micro RP2040 pinout that generic
RP2040 Pro Micro clones follow (and the same converter used by the Sofle Pro).
Do **not** use `promicro_rp2040` — it is deprecated and targets the specific
SparkFun board. (For the Sweep the two are electrically identical anyway: the
only pins that differ are RGB/VBUS pins this board doesn't use.)

QMK's converter system remaps the Pro Micro pin names from the board's
`keyboard.json` (the direct matrix pins and the `D2` split-serial pin) onto the
equivalent RP2040 GPIOs, and changes the build output from `.hex` to `.uf2`.

## Build

```bash
qmk compile -kb ferris/sweep -km ninjaPixel
```
## Flash

```bash
qmk flash -kb ferris/sweep -km ninjaPixel
```

When the CLI says it is looking for the device then put hit the `QK_BOOT` key (it's on the `LAYER_PICKER` layer).


### notes

Unlike the AVR board, the RP2040 is flashed by copying a `.uf2` file onto a
mass-storage drive — `qmk flash` automates the copy.

If I'm unable to use the `QMK_BOOT` key for whatever reason, then I should be able to put each half into boot mode by following this:

> You'll need to bridge the boot to the pin to the right of it (the 3rd soldered one from the top) while plugging it in.
From there a file explorer window should open for you to drag and drop your firmware into.
> Source: Mechboards help email - 5 June 2026

Then run the flash command. 

If `qmk flash` doesn't auto-detect the drive, drag the generated
`ferris_sweep_ninjaPixel.uf2` onto the `RPI-RP2` volume manually.

