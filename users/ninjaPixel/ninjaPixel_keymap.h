// Copyright 2025 ninjaPixel
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Shared keymap data for all ninjaPixel keyboards.
// This header defines the keymaps array, encoder maps, and tap dance
// definitions. It MUST be included from each board's keymap.c (not from
// a separate .c file) because QMK's introspection system needs to see
// the array sizes at compile time within the keymap compilation unit.
//
// Board-specific differences are handled with preprocessor conditionals:
//   - RGB_MATRIX_ENABLE: Sofle Pro has RGB lighting, Rev1 does not.
//     Layer 4 (keyboard settings) and layer 4 encoder map use this.

#pragma once

#include "ninjaPixel.h"

// ──────────────────────────────────────────────────────────────
// Board-specific key aliases for layer 4 (keyboard settings).
// On the Sofle Pro (RGB_MATRIX_ENABLE), the top row has RGB controls.
// On boards without RGB (e.g. Rev1), those positions are dead keys.
// ──────────────────────────────────────────────────────────────
#ifdef RGB_MATRIX_ENABLE
// Layer 4 top row: RGB matrix controls (Sofle Pro)
#define NP_L4_1  RM_NEXT
#define NP_L4_2  RM_PREV
#define NP_L4_3  RM_TOGG
#define NP_L4_4  RM_VALD
#define NP_L4_5  RM_VALU
#define NP_L4_6  RM_SPDD
#define NP_L4_7  RM_SPDU
// Layer 4 encoders: RGB hue and saturation (Sofle Pro)
#define NP_ENC4_L_CCW  RM_HUED
#define NP_ENC4_L_CW   RM_HUEU
#define NP_ENC4_R_CCW  RM_SATD
#define NP_ENC4_R_CW   RM_SATU
#else
// Layer 4 top row: dead keys (no RGB on this board)
#define NP_L4_1  XXXXXXX
#define NP_L4_2  XXXXXXX
#define NP_L4_3  XXXXXXX
#define NP_L4_4  XXXXXXX
#define NP_L4_5  XXXXXXX
#define NP_L4_6  XXXXXXX
#define NP_L4_7  XXXXXXX
// Layer 4 encoders: transparent (no RGB on this board)
#define NP_ENC4_L_CCW  KC_TRNS
#define NP_ENC4_L_CW   KC_TRNS
#define NP_ENC4_R_CCW  KC_TRNS
#define NP_ENC4_R_CW   KC_TRNS
#endif

// clang-format off

// ──────────────────────────────────────────────────────────────
// Tap Dance definitions
// ──────────────────────────────────────────────────────────────
tap_dance_action_t tap_dance_actions[] = {
    [TD_NOODLE] = ACTION_TAP_DANCE_DOUBLE(KC_A, KC_B),
};

// ──────────────────────────────────────────────────────────────
// Keymaps — shared across all boards
// ──────────────────────────────────────────────────────────────
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
    LGUI(KC_LBRC),   LGUI(KC_RBRC),  _______, KC_PLUS,  KC_EQL,       KC_RCBR, _______,           _______,     _______,  LGUI(KC_LEFT), KC_PGDN, KC_PGUP, LGUI(KC_RIGHT),  _______,
                                     _______, _______,  LSFT(KC_ENT), _______, LSFT(KC_ENT),      _______,     _______,  _______,       _______, _______
    ),

    // Windows Symbols (Layer 3) — activated by holding LT(3, KC_SPC) on Windows
    // Sparse layer: only overrides keys that differ from the Mac symbol layer.
    // Transparent keys fall through to layer 2 (Mac symbols) thanks to the
    // layer_state_set_user() logic above.
    [3] = LAYOUT(
    _______,     _______,        _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
    LGUI(KC_L),  _______,        _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
    _______,     LCTL(KC_DEL),   _______, _______, _______, KC_LCTL,                           _______, _______, _______, _______, _______, _______,
    KC_WWW_BACK, KC_WWW_FORWARD, _______, _______, _______, _______, _______,         _______, _______, KC_HOME, _______, _______, KC_END, _______,
                                 _______, _______, _______, _______, LCTL(KC_J),      _______, _______, _______, _______, _______
    ),

    // Keyboard Settings (Layer 4) — toggled via TG(4) from layer 2
    // Provides OS mode switching (TO(0) = Mac, TO(1) = Windows),
    // Caps Lock, and undo/cut/copy/paste shortcuts.
    // On the Sofle Pro, the top row includes RGB controls.
    // On boards without RGB (e.g. Rev1), those keys are XXXXXXX.
    [4] = LAYOUT(
    TO(0),   NP_L4_1, NP_L4_2, NP_L4_3, NP_L4_4, NP_L4_5,                        NP_L4_6, NP_L4_7, XXXXXXX, XXXXXXX, XXXXXXX, TO(1),
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

// ──────────────────────────────────────────────────────────────
// Encoder maps — shared across all boards
// On the Sofle Pro, layer 4 controls RGB hue/saturation.
// On boards without RGB (e.g. Rev1), layer 4 encoders are transparent.
// ──────────────────────────────────────────────────────────────
#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    // Layer 0 (Mac base): left = volume, right = switch tabs (Cmd+Shift+[/])
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(LSG(KC_LBRC), LSG(KC_RBRC))},
    // Layer 1 (Windows base): left = transparent, right = Ctrl+Tab (CW) / Ctrl+Shift+Tab (CCW)
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(C(KC_TAB), C(S(KC_TAB)))},
    // Layer 2 (Mac symbols): left = prev/next track, right = transparent
    [2] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    // Layer 3 (Windows symbols): all transparent (falls through to layer 2/1/0)
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    // Layer 4 (Keyboard settings): RGB hue/saturation on Pro, transparent on others
    [4] = { ENCODER_CCW_CW(NP_ENC4_L_CCW, NP_ENC4_L_CW), ENCODER_CCW_CW(NP_ENC4_R_CCW, NP_ENC4_R_CW)},
    // Layer 5 (template): all transparent
    [5] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)}
};
#endif
// clang-format on
