// Copyright 2026 ninjaPixel
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Ferris Sweep — ninjaPixel keymap (proof-of-concept)
// ----------------------------------------------------
// This is a deliberately minimal, single-layer keymap whose only purpose is to
// prove that we can build and flash firmware for the RP2040-based Ferris Sweep.
// Once flashing is confirmed it will be expanded into a full multi-layer layout
// (and most likely folded into the shared users/ninjaPixel/ userspace, the same
// way the Sofle boards are).
//
// Hardware note — why the converter exists:
//   The stock ferris/sweep board targets an AVR Pro Micro (ATmega32u4) and so
//   builds a .hex flashed over the caterina serial bootloader. Our physical
//   board is a generic RP2040 Pro Micro (USB-C) instead, which is flashed by
//   copying a .uf2 onto the RPI-RP2 mass-storage drive. The AVR -> RP2040 switch
//   is handled entirely by the "converter" entry in this keymap's keymap.json:
//   "rp2040_ce" — the "Community Edition" pinout that the generic RP2040 Pro
//   Micro clones follow. (NOT "promicro_rp2040": that name is deprecated and
//   targets the specific SparkFun board; for the Sweep the two are electrically
//   identical anyway, as the only pins that differ are RGB/VBUS pins this board
//   doesn't use.) The converter transparently remaps the Pro Micro pin names in
//   the board's keyboard.json (the direct matrix pins and the D2 split-serial
//   pin) onto the equivalent RP2040 GPIOs. No pin definitions need to change.

#include QMK_KEYBOARD_H

// Layer indices. Only one layer for now, but named (rather than a bare 0) so
// that future layers can be added without renumbering everything.
enum layers {
    _COLEMAK,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base layer — standard Colemak (not Colemak-DH), MacOS-oriented.
     *
     * The Ferris Sweep is a 34-key split: a 3x5 grid of letter keys per hand
     * plus two thumb keys per hand. The LAYOUT macro takes the keys row by row,
     * left hand then right hand, and finishes with the four thumb keys.
     *
     *   ┌───────────────────────┐        ┌───────────────────────┐
     *   │  Q    W    F    P    G │        │  J    L    U    Y    ; │
     *   │  A    R    S    T    D │        │  H    N    E    I    O │
     *   │  Z    X    C    V    B │        │  K    M    ,    .    / │
     *   └─────────────┐         │        │         ┌─────────────┘
     *                 │  1  Boot│        │ Boot  2 │
     *                 └─────────┘        └─────────┘
     *
     * The four thumb keys are temporary placeholders. QK_BOOT sits on both
     * inner thumbs so the board can always be dropped back into the RP2040
     * bootloader from the keymap itself (handy while iterating — we never get
     * stuck unable to reflash); the outer thumbs are 1 and 2 just to have
     * something pressable. This whole thumb cluster will be replaced by the
     * real design (mod-taps, layer-taps, a dedicated nav/boot layer, etc.) in
     * the next iteration.
     */
    [_COLEMAK] = LAYOUT(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,         KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_D,         KC_H,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                                   KC_1,    QK_BOOT,      QK_BOOT, KC_2
    )
};
// clang-format on
