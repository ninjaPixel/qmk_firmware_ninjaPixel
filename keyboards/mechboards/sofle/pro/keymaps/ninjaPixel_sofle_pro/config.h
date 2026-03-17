// Copyright 2025 ninjaPixel
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Opt out of the board-level master-half OLED rendering in display_oled.c.
// This lets the shared user-level code in ninjaPixel.c draw its own
// Layer / OS / Rate dashboard on the left OLED, identical to the Rev1.
#define CUSTOM_OLED_DISPLAY
