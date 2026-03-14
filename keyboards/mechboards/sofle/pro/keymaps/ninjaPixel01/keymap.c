// Copyright 2025 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// clang-format off

// Tap Dance declarations
enum {
    TD_NOODLE
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [TD_NOODLE] = ACTION_TAP_DANCE_DOUBLE(KC_A, KC_B),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    // When on Windows (layer 1) with Windows symbols (layer 3) active,
    // also enable Mac symbols (layer 2) so transparent keys fall through to it.
    if (IS_LAYER_ON_STATE(state, 1) && IS_LAYER_ON_STATE(state, 3)) {
        state |= (layer_state_t)1 << 2;
    }
    return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Mac Base (Layer 0)
    [0] = LAYOUT(
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                       KC_6,           KC_7,    KC_8,     KC_9,   KC_0,       LALT(KC_TAB),
    TD(TD_NOODLE),  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                KC_J,           KC_L,    KC_U,     KC_Y,   KC_BSLS,    LALT(KC_BSPC),
    KC_TAB,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                       KC_M,           KC_N,    KC_E,     KC_I,   KC_O,       KC_BSPC,
    EE_CLR, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                 KC_MUTE,     KC_MPLY, KC_K,           KC_H,    KC_COMMA, KC_DOT, KC_SLSH,    KC_LSFT,
                      KC_LCTL, KC_LALT, KC_LGUI, MT(MOD_RSFT, KC_ENT), KC_HYPR,     KC_MEH,  LT(2, KC_SPC),  KC_RGUI, KC_RALT,  KC_RCTL
  ),
    // Windows Base (Layer 0)
    [1] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                                                                _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                                                                _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                                                                _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                                              _______, _______, _______, _______, _______, _______, _______,
                      KC_LGUI, _______, KC_LCTL, _______, MT(MOD_LCTL|MOD_LSFT|MOD_LALT,KC_NO),        C(KC_LGUI), LT(3, KC_SPC), KC_RCTL, _______, KC_RGUI
    ),
    // Mac Layer 1
  [2] = LAYOUT(
    TG(4), KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                      KC_F6,   KC_F7,   KC_F8,    KC_F9,   KC_F10,  KC_F11,
    KC_SYSTEM_SLEEP,  _______,    KC_LBRC,    KC_LPRN,    KC_LCBR,    KC_GRV,                            _______,    KC_RCBR,    KC_RPRN,     KC_RBRC,    KC_SEMICOLON,    KC_F12,
    CW_TOGG, LALT(KC_DEL), KC_DEL,   KC_QUOTE, KC_SEMICOLON,  _______,                       KC_MINUS, KC_LEFT, KC_DOWN,  KC_UP,   KC_RIGHT, _______,
    KC_WWW_BACK, KC_WWW_FORWARD,  _______, KC_PLUS, KC_EQL, KC_RCBR,  _______,                 _______, _______, LGUI(KC_LEFT), KC_PGDN, KC_PGUP,  LGUI(KC_RIGHT), _______,
                      _______, _______, _______, _______,  _______,                 _______, _______,   _______, _______,  _______
  ),
    // Windows Layer 1
    [3] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
    LGUI(KC_L), _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
    _______, LCTL(KC_DEL), _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______, _______, _______,
                      _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______
  ),
    // Keyboard Layer
  [4] = LAYOUT(
    TO(0),   RM_NEXT, RM_PREV,  RM_TOGG, RM_VALD, RM_VALU,                          RM_SPDD, RM_SPDU, XXXXXXX, XXXXXXX, XXXXXXX ,TO(1),
    _______, XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,                          XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_CAPS,                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,
    _______, C(KC_Z), C(KC_X), C(KC_C), XXXXXXX, C(KC_V), _______,        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      _______, _______, _______, _______,   _______,        _______, _______, _______, _______, _______
  ),
    // transparent template layer
    [5] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______, _______, _______,
                      _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______
  )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
[0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(LSG(KC_LBRC), LSG(KC_RBRC))},
[1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(C(KC_TAB), C(S(KC_TAB)))},

[2] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
[3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},

// RGB Hue                                  RGB Saturation
[4] = { ENCODER_CCW_CW(RM_HUED, RM_HUEU), ENCODER_CCW_CW(RM_SATD, RM_SATU)},

[5] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)}
};
#endif
// clang-format on
