// Copyright 2025 ninjaPixel
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Shared callback functions for all ninjaPixel keyboards.
// Contains layer-switching logic, RGB indicator control (Pro only),
// and OLED display (Rev1 only).
//
// The keymap data arrays (keymaps, encoder_map, tap_dance_actions) live in
// ninjaPixel_keymap.h and are included from each board's keymap.c instead,
// because QMK's introspection system needs them in the keymap compilation unit.

#include "ninjaPixel.h"

// ──────────────────────────────────────────────────────────────
// Layer-switching logic for dual Mac/Windows OS support
// ──────────────────────────────────────────────────────────────
//
// Problem this solves:
//   On Windows, we use LT(3, KC_SPC) to hold-activate layer 3 for symbols.
//   Layer 3 only defines the few keys that differ between Mac and Windows;
//   everything else is KC_TRNS (transparent). Normally those transparent keys
//   would fall through to layer 1 (Windows base), not to layer 2 (Mac symbols)
//   where the actual symbol definitions live.
//
//   To fix this, whenever layers 1 AND 3 are both active we also force-enable
//   layer 2, so the lookup order becomes 3 → 2 → 1 → 0. Transparent keys on
//   layer 3 now resolve against the Mac symbol layer, which is what we want.
//
// Why the else-if is needed:
//   QMK calls this callback whenever the layer state changes. When the user
//   releases LT(3, KC_SPC), QMK clears layer 3 from the state — but layer 2,
//   which we manually set with `state |=`, is NOT automatically cleared.
//   Without the else-if branch, layer 2 would remain stuck on after release.
//   The else-if detects "still on Windows, but layer 3 was just released" and
//   explicitly clears layer 2.
//
// Mac side is unaffected:
//   Layer 1 is only active in Windows mode, so neither branch fires on Mac.
//   LT(2, KC_SPC) on Mac activates layer 2 directly via QMK as normal.
layer_state_t layer_state_set_user(layer_state_t state) {
    if (IS_LAYER_ON_STATE(state, 1) && IS_LAYER_ON_STATE(state, 3)) {
        // Windows + symbols held: force layer 2 on so transparent keys on
        // layer 3 fall through to the Mac symbol definitions on layer 2.
        state |= (layer_state_t)1 << 2;
    } else if (IS_LAYER_ON_STATE(state, 1)) {
        // Windows but symbols released: clear layer 2 so we return to the
        // normal Windows base (layer 1) without a stale symbol layer.
        state &= ~((layer_state_t)1 << 2);
    }
    return state;
}

// ──────────────────────────────────────────────────────────────
// RGB Matrix — Sofle Pro only
// Disable underglow LEDs (the 5 bottom-facing LEDs per half) while leaving
// per-key LEDs unaffected. Called every frame by QMK's RGB Matrix system.
// ──────────────────────────────────────────────────────────────
#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = led_min; i < led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
            rgb_matrix_set_color(i, 0, 0, 0); // force off
        }
    }
    return false; // allow other indicator callbacks to run
}
#endif

// ──────────────────────────────────────────────────────────────
// OLED display — Sofle Rev1 only
// Overrides the board-level oled_task_kb() layer names with names
// that match this keymap's actual layer structure.
// The master half shows the active layer name and branding text.
// The secondary half falls through to the board-level QMK logo render.
// ──────────────────────────────────────────────────────────────
#ifdef OLED_ENABLE
bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // Show the active layer name
        oled_write_P(PSTR("\n\n"), false);
        switch (get_highest_layer(layer_state)) {
            case 0:
                oled_write_ln_P(PSTR("Mac 0"), false);
                break;
            case 1:
                oled_write_ln_P(PSTR("Win 0"), false);
                break;
            case 2:
                oled_write_ln_P(PSTR("Mac 1"), false);
                break;
            case 3:
                oled_write_ln_P(PSTR("Win 1"), false);
                break;
            case 4:
                oled_write_ln_P(PSTR("KBD"), false);
                break;
            case 5:
                oled_write_ln_P(PSTR("Tmpl"), false);
                break;
            default:
                oled_write_ln_P(PSTR("?????"), false);
                break;
        }
        oled_write_P(PSTR("\n\n"), false);

        // Display branding text over two lines
        oled_write_ln_P(PSTR("ninja"), false);
        oled_write_ln_P(PSTR("Pixel"), false);
    }
    // On the master half we've already written our custom display above,
    // so return false to stop the board-level oled_task_kb() from
    // overwriting it with the default layer names.
    // On the secondary half we haven't written anything, so return true
    // to let the board-level code render the QMK logo as usual.
    return !is_keyboard_master();
}
#endif
