// Copyright 2025 ninjaPixel
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Override encoder resolution from the board default of 2 to 4.
// With resolution 2 the encoders fire twice per detent (click),
// causing actions like tab-switching to skip two tabs at a time.
#undef ENCODER_RESOLUTION
#define ENCODER_RESOLUTION 4
