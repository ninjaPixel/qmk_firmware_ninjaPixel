
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
    _LAYER_PICKER,
    _NUMBERS,
    _FN_KEYS,
    _SECONDARY
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Base layer — standard Colemak (not Colemak-DH), MacOS-oriented.
    [_COLEMAK] = LAYOUT(
        KC_Q,    KC_W,    KC_F,    KC_P,                 KC_B,            KC_J,               KC_L,                   KC_U,    KC_Y,    KC_SCLN,
        KC_A,    KC_R,    KC_S,    KC_T,                 KC_G,            KC_M,               KC_N,                   KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_D,                 KC_V,            KC_K,               KC_H,                   KC_COMM, KC_DOT,  KC_SLSH,
                                   MT(MOD_RSFT, KC_ENT), KC_LGUI,         OSL(_LAYER_PICKER), LT(_SECONDARY, KC_SPC)
    ),



    // Layer picker layer and sticky modifiers
    [_LAYER_PICKER] = LAYOUT(
        KC_L,    _______, _______, _______, QK_BOOT,             _______,      _______,        _______,      _______, _______,
        _______, _______, _______, _______, _______,             _______,      _______,        _______,      _______, _______,
        _______, _______, _______, _______, _______,             _______,      TO(_NUMBERS),   TO(_FN_KEYS), _______, _______,
                                   _______, _______,             TO(_COLEMAK), _______
    ),


    [_NUMBERS] = LAYOUT(
        KC_N,    _______, _______, _______, _______,            _______,      KC_7,     KC_8, KC_9, _______,
        _______, _______, _______, _______, _______,            _______,      KC_4,     KC_5, KC_6, _______,
        _______, _______, _______, _______, _______,            KC_0,         KC_1,     KC_2, KC_3, _______,
                                   _______, _______,            TO(_COLEMAK), LT(_SECONDARY, KC_SPC)
    ),

    [_FN_KEYS] = LAYOUT(
        KC_F,    _______, _______, _______, _______,            _______,      KC_F7,     KC_F8, KC_F9, KC_F12,
        _______, _______, _______, _______, _______,            _______,      KC_F4,     KC_F5, KC_F6, KC_F11,
        _______, _______, _______, _______, _______,            _______,      KC_F1,     KC_F2, KC_F3, KC_F10,
                                   _______, _______,            TO(_COLEMAK), _______
    ),

    // Quick access layer
    [_SECONDARY] = LAYOUT(
        KC_ESC,  _______, _______, _______, _______,             _______,       _______, _______, _______, LALT(KC_BSPC),
        KC_TAB,  _______, _______, _______, _______,             KC_MINUS,      KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,
        _______, _______, _______, _______, _______,             _______,       _______, _______, _______, _______,
                                   _______, KC_BSPC,             TO(_COLEMAK),  _______
    )
    // Template
    // [_FOO] = LAYOUT(
    //     KC_T,    _______, _______, _______, _______,      _______,      _______, _______, _______, _______,
    //     _______, _______, _______, _______, _______,      _______,      _______, _______, _______, _______,
    //     _______, _______, _______, _______, _______,      _______,      _______, _______, _______, _______,
    //                                _______, _______,      TO(_COLEMAK), _______
    // )
};
// clang-format on
