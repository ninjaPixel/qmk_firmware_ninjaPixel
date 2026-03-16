// Copyright 2025 ninjaPixel
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Shared header for all ninjaPixel keymaps.
// Contains the tap dance enum used across boards.
// The actual keymap data (keymaps array, encoder maps, tap dance definitions)
// lives in ninjaPixel_keymap.h and must be included from each board's keymap.c
// so that QMK's introspection system can see the array sizes at compile time.

#pragma once

#include QMK_KEYBOARD_H

// Tap Dance declarations
// TD_NOODLE: single tap = KC_A, double tap = KC_B
// Used on the far-left key of row 1 (where Caps Lock traditionally sits).
//
// TD_SCREENSHOT: OS-aware screenshot tap dance.
// Detects whether the Windows layer (layer 1) is active to choose the
// correct shortcut. See ninjaPixel_keymap.h for the full implementation.
//   Mac:     1 tap = selection (Cmd+Shift+4)
//            2 taps = full screen (Cmd+Shift+3)
//            3 taps = screenshot toolbar (Cmd+Shift+5)
//   Windows: 1 tap = Snipping Tool (Win+Shift+S)
//            2 taps = full screen (Print Screen)
enum {
    TD_NOODLE,
    TD_SCREENSHOT
};
