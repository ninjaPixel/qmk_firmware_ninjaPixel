// Copyright 2025 ninjaPixel
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Sofle Rev1 keymap — ported from Sofle Pro (mechboards/sofle/pro)
// Mac/Windows dual-OS Colemak layout.
// The Rev1 has no RGB lighting, so all RGB-related code has been removed.

#include QMK_KEYBOARD_H

// clang-format off

// Tap Dance declarations
// TD_NOODLE: single tap = KC_A, double tap = KC_B
// Used on the far-left key of row 1 (where Caps Lock traditionally sits).
enum {
    TD_NOODLE
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [TD_NOODLE] = ACTION_TAP_DANCE_DOUBLE(KC_A, KC_B),
};

// Layer structure:
//   Layer 0 — Mac base (Colemak)
//   Layer 1 — Windows base (toggled on/off to switch OS mode)
//   Layer 2 — Mac symbols, activated by LT(2, KC_SPC) on Mac
//   Layer 3 — Windows symbol overrides (sparse — most keys are KC_TRNS)
//   Layer 4 — Keyboard settings layer (toggle with TG(4) from layer 2)
//   Layer 5 — Transparent template layer (unused, available for future use)
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Mac Base (Layer 0) — Colemak layout
    // Left encoder: KC_MUTE (volume mute)
    // Right encoder placeholder: KC_MPLY (media play/pause)
    [0] = LAYOUT(
    KC_ESC,        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,          KC_7,    KC_8,     KC_9,   KC_0,    LALT(KC_TAB),
    TD(TD_NOODLE), KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                        KC_J,          KC_L,    KC_U,     KC_Y,   KC_BSLS, LALT(KC_BSPC),
    KC_TAB,        KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                        KC_M,          KC_N,    KC_E,     KC_I,   KC_O,    KC_BSPC,
    XXXXXXX,       KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                 KC_MUTE,      KC_MPLY, KC_K,          KC_H,    KC_COMMA, KC_DOT, KC_SLSH, KC_LSFT,
                            KC_LCTL, KC_LALT, KC_LGUI, MT(MOD_RSFT, KC_ENT), KC_HYPR,      KC_MEH,  LT(2, KC_SPC), KC_RGUI, KC_RALT,  KC_RCTL
    ),

    // Windows Base (Layer 1) — mostly transparent, overrides Mac-specific keys
    // Only defines keys that differ on Windows (e.g. different modifier combos,
    // Ctrl+Backspace instead of Alt+Backspace, etc.)
    [1] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                                                                       _______, _______, _______, _______, _______, LCTL(LGUI(KC_W)),
    _______, _______, _______, _______, _______, _______,                                                                       _______, _______, _______, _______, _______, LCTL(KC_BSPC),
    _______, _______, _______, _______, _______, _______,                                                                       _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                                   _______,    _______,       _______, _______, _______, _______, _______,
                      KC_LGUI, _______, KC_LCTL, _______, MT(MOD_LCTL|MOD_LSFT|MOD_LALT,KC_NO),      C(KC_LGUI), LT(3, KC_SPC), KC_RCTL, _______, KC_RGUI
    ),

    // Mac Symbols (Layer 2) — activated by holding LT(2, KC_SPC)
    // Contains F-keys, brackets, navigation, and other symbol keys
    [2] = LAYOUT(
    TG(4),           KC_F1,          KC_F2,   KC_F3,    KC_F4,        KC_F5,                                   KC_F6,    KC_F7,         KC_F8,   KC_F9,   KC_F10,          KC_F11,
    KC_SYSTEM_SLEEP, _______,        KC_LBRC, KC_LPRN,  KC_LCBR,      KC_GRV,                                  _______,  KC_RCBR,       KC_RPRN, KC_RBRC, KC_SEMICOLON,    KC_F12,
    CW_TOGG,         LALT(KC_DEL),   KC_DEL,  KC_QUOTE, KC_SEMICOLON, KC_LALT,                                 KC_MINUS, KC_LEFT,       KC_DOWN, KC_UP,   KC_RIGHT,        _______,
    KC_WWW_BACK,     KC_WWW_FORWARD, _______, KC_PLUS,  KC_EQL,       KC_RCBR, _______,           _______,     _______,  LGUI(KC_LEFT), KC_PGDN, KC_PGUP, LGUI(KC_RIGHT),  _______,
                                     _______, _______,  LSFT(KC_ENT), _______, LSFT(KC_ENT),      _______,     _______,  _______,       _______, _______
    ),

    // Windows Symbols (Layer 3) — activated by holding LT(3, KC_SPC) on Windows
    // Sparse layer: only overrides keys that differ from the Mac symbol layer.
    // Transparent keys fall through to layer 2 (Mac symbols) thanks to the
    // layer_state_set_user() logic above.
    [3] = LAYOUT(
    _______,    _______,      _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
    LGUI(KC_L), _______,      _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
    _______,    LCTL(KC_DEL), _______, _______, _______, KC_LCTL,                           _______, _______, _______, _______, _______, _______,
    _______,    _______,      _______, _______, _______, _______, _______,         _______, _______, KC_HOME, _______, _______, KC_END, _______,
                              _______, _______, _______, _______, LCTL(KC_J),      _______, _______, _______, _______, _______
    ),

    // Keyboard Settings (Layer 4) — toggled via TG(4) from layer 2
    // Provides OS mode switching (TO(0) = Mac, TO(1) = Windows),
    // Caps Lock, and undo/cut/copy/paste shortcuts.
    // NOTE: RGB keys from the Sofle Pro have been removed (Rev1 has no RGB).
    [4] = LAYOUT(
    TO(0),   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(1),
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_CAPS,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, C(KC_Z), C(KC_X), C(KC_C), XXXXXXX, C(KC_V), _______,      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______
    ),

    // Transparent template (Layer 5) — all keys pass through to lower layers.
    // Kept as a starting point for adding new layers in the future.
    [5] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
                      _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______
    )
};

// Encoder map — defines rotary encoder behavior per layer.
// The Rev1 has two encoders (one per half), same as the Sofle Pro.
// Layer 4 encoder entries use KC_TRNS since RGB controls were removed.
#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    // Layer 0 (Mac base): left = volume, right = switch tabs (Cmd+Shift+[/])
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(LSG(KC_LBRC), LSG(KC_RBRC))},
    // Layer 1 (Windows base): left = transparent, right = Ctrl+Shift+Tab / Ctrl+Tab
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(C(S(KC_TAB)), C(KC_TAB))},
    // Layer 2 (Mac symbols): left = prev/next track, right = transparent
    [2] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    // Layer 3 (Windows symbols): all transparent (falls through to layer 2/1/0)
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    // Layer 4 (Keyboard settings): all transparent (no RGB on Rev1)
    [4] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    // Layer 5 (template): all transparent
    [5] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)}
};
#endif
// clang-format on
