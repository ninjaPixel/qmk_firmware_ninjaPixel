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
// OLED display — unified for both Sofle Pro and Rev1
//
// Master (left) half shows a three-section dashboard:
//   Row 0 : "Layer" header
//   Row 2 : active layer name  (updates on layer change)
//   Row 4 : "OS" header
//   Row 6 : detected OS name   (written once on detection)
//   Row 8 : "Rate" header
//   Row 10: loop rate           (updates every second)
//   Row 12-13: branding
//
// Secondary (right) half defers to the board-level code which
// renders the mechboards logo / WPM / RGB info (Pro) or the
// QMK logo (Rev1).
//
// On the Pro, the board-level display_oled.c master-half writes
// are disabled via CUSTOM_OLED_DISPLAY defined in the keymap's
// config.h.  On the Rev1, returning false from oled_task_user()
// blocks the board-level oled_task_kb() master rendering.
// ──────────────────────────────────────────────────────────────
#ifdef OLED_ENABLE

// ── State tracked for OLED rendering ──
static os_variant_t detected_os_cache = OS_UNSURE;
static bool         os_detected       = false;
static uint16_t     user_loop_rate    = 0;

// ── Helper: return a human-readable name for a layer number ──
static const char *user_layer_name(uint8_t layer) {
    switch (layer) {
        case 0:  return "Mac 0";
        case 1:  return "Win 0";
        case 2:  return "Mac 1";
        case 3:  return "Win 1";
        case 4:  return "KBD";
        case 5:  return "Tmpl";
        default: return "?????";
    }
}

// ── Helper: return a human-readable name for a detected OS ──
static const char *user_os_name(os_variant_t os) {
    switch (os) {
        case OS_MACOS:   return "MacOS";
        case OS_IOS:     return "iOS";
        case OS_WINDOWS: return "Win";
        case OS_LINUX:   return "Linux";
        default:         return "?????";
    }
}

// ── OS detection callback ──
// Captures the detected OS so oled_task_user() can display it.
// Returns true so board-level code can also act on the detection
// (e.g. the Pro's layer-switching logic).
bool process_detected_host_os_user(os_variant_t detected_os) {
    detected_os_cache = detected_os;
    os_detected       = true;
    return true;
}

// ── Loop rate computation ──
// Runs every scan cycle on the master half.  Counts iterations per
// second and stores the result for the Rate row on the OLED.
void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        static uint32_t     loop_count = 0;
        static fast_timer_t loop_time  = 0;
        loop_count++;
        if (timer_elapsed_fast(loop_time) > 1000) {
            loop_time      = timer_read_fast();
            user_loop_rate = loop_count > UINT16_MAX ? UINT16_MAX : (uint16_t)loop_count;
            loop_count     = 0;
        }
    }
}

// ── Main OLED task — renders the master-half dashboard ──
bool oled_task_user(void) {
    // Secondary half: let board-level code render its default content
    // (mechboards logo/WPM/RGB on Pro, QMK logo on Rev1).
    if (!is_keyboard_master()) {
        return true;
    }

    // ── Draw section headers once after boot ──
    static bool headers_drawn = false;
    if (!headers_drawn) {
        headers_drawn = true;

        oled_set_cursor(0, 0);
        oled_write("Layer", false);

        oled_set_cursor(0, 4);
        oled_write("OS", false);

        oled_set_cursor(0, 8);
        oled_write("Rate", false);

        // Branding in the remaining space
        oled_set_cursor(0, 12);
        oled_write("ninja", false);
        oled_set_cursor(0, 13);
        oled_write("Pixel", false);
    }

    // ── Layer name (row 2) — only re-render when layer changes ──
    static uint8_t last_layer = 255;
    uint8_t        cur_layer  = get_highest_layer(layer_state);
    if (cur_layer != last_layer) {
        last_layer = cur_layer;
        oled_set_cursor(0, 2);
        oled_write_ln(user_layer_name(cur_layer), false);
    }

    // ── OS name (row 6) — render once when OS is first detected ──
    static bool os_rendered = false;
    if (os_detected && !os_rendered) {
        os_rendered = true;
        oled_set_cursor(0, 6);
        oled_write_ln(user_os_name(detected_os_cache), false);
    }

    // ── Loop rate (row 10) — update when the value changes ──
    static uint16_t last_rate = 0;
    if (user_loop_rate != last_rate) {
        last_rate = user_loop_rate;
        oled_set_cursor(0, 10);
        oled_write_ln(get_u16_str(user_loop_rate, ' '), false);
    }

    // Return false to block any remaining board-level master-half
    // OLED writes (Rev1's oled_task_kb, Pro's guarded-out code).
    return false;
}

#endif // OLED_ENABLE
