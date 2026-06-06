# Ferris Sweep — ninjaPixel keymap

A minimal, single-layer **Colemak** keymap for the Ferris Sweep. Its only job
right now is to prove that we can build and flash firmware for the **RP2040**
version of this board. A fuller multi-layer layout will follow.

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

## Flash (RP2040)

Unlike the AVR board, the RP2040 is flashed by copying a `.uf2` file onto a
mass-storage drive — `qmk flash` automates the copy:

1. Put a half into bootloader mode. Any of:
   - double-tap the reset button, or
   - hold the top-left key (top-right on the right half) while plugging it in
     (bootmagic), or
   - press the `BOOT`/`RESET` button on the controller, or
   - press a key mapped to `QK_BOOT` (temporarily on the inner thumb keys in
     this keymap).

   The board mounts as an `RPI-RP2` drive in Finder.
2. Run:
   ```bash
   qmk flash -kb ferris/sweep -km ninjaPixel
   ```

If `qmk flash` doesn't auto-detect the drive, drag the generated
`ferris_sweep_ninjaPixel.uf2` onto the `RPI-RP2` volume manually.

### Both halves flash in one go

In practice a single flash run programmed **both** halves at once — there's no
need to flash each side separately the way the Sofle does. The Sweep defines no
handedness method (no `EE_HANDS`, no `SPLIT_HAND_PIN`), so it uses QMK's default
runtime handedness: both halves run the **same** firmware image and detect which
side has the USB cable at boot. The one `.uf2` is therefore valid for either
half — there is no separate left/right build to manage.

(If a half ever behaves oddly after an update, just flash that half on its own
with the exact same command.)
