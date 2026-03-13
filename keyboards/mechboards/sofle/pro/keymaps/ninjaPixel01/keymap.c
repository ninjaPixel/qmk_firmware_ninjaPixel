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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                       KC_6,           KC_7,    KC_8,     KC_9,   KC_0,       LALT(KC_TAB),
    TD(TD_NOODLE),  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                KC_J,           KC_L,    KC_U,     KC_Y,   KC_BSLS,    LALT(KC_BSPC),
    KC_TAB,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                       KC_M,           KC_N,    KC_E,     KC_I,   KC_O,       KC_BSPC,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                 KC_MUTE,     KC_MPLY, KC_K,           KC_H,    KC_COMMA, KC_DOT, KC_SLSH,    KC_LSFT,
                      KC_LCTL, KC_LALT, KC_LGUI, MT(MOD_RSFT, KC_ENT), KC_HYPR,     KC_MEH,  LT(1, KC_SPC),  KC_RGUI, KC_RALT,  KC_RCTL
  ),
  [1] = LAYOUT(
    TG(2), KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                      KC_F6,   KC_F7,   KC_F8,    KC_F9,   KC_F10,  KC_F11,
    KC_GRV,  KC_X,    KC_LBRC,    KC_LPRN,    KC_LCBR,    KC_GRV,                            _______,    KC_RCBR,    KC_RPRN,     KC_RBRC,    KC_SEMICOLON,    KC_F12,
    CW_TOGG, LALT(KC_DEL), KC_DEL,   KC_QUOTE, KC_SEMICOLON,  _______,                       KC_MINUS, KC_LEFT, KC_DOWN,  KC_UP,   KC_RIGHT, KC_PIPE,
    _______, KC_EQL,  KC_MINS, KC_PLUS, KC_EQL, KC_RCBR,  _______,                 _______, _______, LGUI(KC_LEFT), KC_PGDN, KC_PGUP,  LGUI(KC_LEFT), _______,
                      _______, _______, _______, _______,  _______,                 _______, _______,   _______, _______,  _______
  ),
  [2] = LAYOUT(
    TO(0),   RGB_MOD, RGB_RMOD, RGB_TOG, RGB_VAI, RGB_VAD,                          RGB_SPI, RGB_SPD, _______, _______, _______ ,_______,
    _______, KC_INS,  KC_PSCR, KC_APP,  XXXXXXX, XXXXXXX,                          KC_PGUP, XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX, KC_BSPC,
    _______, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_CAPS,                          KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,  XXXXXXX,
    _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX, _______,        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                      _______, _______, _______, MO(3),   _______,        _______, _______, _______, _______, _______
  ),
    [3] = LAYOUT(
    XXXXXXX, XXXXXXX, XXXXXXX ,XXXXXXX, XXXXXXX, XXXXXXX,                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, XXXXXXX, XXXXXXX,                          XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
    RM_NEXT, RM_HUED, RM_SATD, RM_VALD, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                      _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______
  )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
[0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(LSG(KC_LBRC), LSG(KC_RBRC))},
[1] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
[2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
[3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)}
};
#endif
// clang-format on
