# ninjaPixel QMK Userspace

## What is the QMK `users/` directory?

QMK's `users/` directory provides a **userspace** mechanism for sharing code across
multiple keyboard keymaps. Without it, each keymap lives in its own isolated directory
(e.g. `keyboards/<board>/keymaps/<name>/keymap.c`), and any shared logic — layer
definitions, tap dances, callback functions — has to be duplicated in every keymap.

The userspace solves this by giving you a single directory (`users/<name>/`) whose
contents are automatically compiled and linked into any keymap that opts in. This means:

- **One source of truth** for key layouts, layer logic, and shared behaviour.
- **Board-specific differences** handled with `#ifdef` conditionals rather than
  copy-pasted files that drift apart over time.
- **Consistent behaviour** across all your boards — change a key binding once and
  every board picks it up.

### How the build system connects it

When QMK builds a keymap, it looks for a `USER_NAME` variable in the keymap's
`rules.mk`. If set, it includes everything from `users/<USER_NAME>/`:

- `rules.mk` — additional source files, feature flags
- `config.h` — compile-time configuration overrides (if present)
- Any `.c` files listed in `SRC +=` are compiled and linked alongside the keymap

If `USER_NAME` is not explicitly set, it defaults to the keymap's directory name.

## How `users/ninjaPixel/` is structured

```
users/ninjaPixel/
├── readme.md               # This file
├── rules.mk                # Tells QMK to compile ninjaPixel.c
├── ninjaPixel.h            # Shared header: tap dance enum, common includes
├── ninjaPixel.c            # Callback functions (layer logic, RGB, OLED)
└── ninjaPixel_keymap.h     # Data arrays (keymaps, encoder maps, tap dance defs)
```

### Why two header files?

QMK's introspection system needs to know the size of the `keymaps[]`, `encoder_map[]`,
and `tap_dance_actions[]` arrays at compile time. These arrays must exist in the same
**compilation unit** as the keymap — they can't live in a separate `.c` file that gets
linked later.

This creates a split:

| File | Contains | Why it's there |
|---|---|---|
| `ninjaPixel_keymap.h` | `keymaps[]`, `encoder_map[]`, `tap_dance_actions[]` | Must be `#include`d from `keymap.c` so the arrays are visible to QMK's introspection |
| `ninjaPixel.c` | `layer_state_set_user()`, `rgb_matrix_indicators_advanced_user()`, `oled_task_user()` | Callback functions resolved at link time — safe in a separate compilation unit |
| `ninjaPixel.h` | Tap dance enum, `#include QMK_KEYBOARD_H` | Shared declarations used by both of the above |

### How board-specific differences work

Rather than `#ifdef` blocks inside `LAYOUT()` calls (which break tools like
`qmk c2json`), board-specific keys are defined as macros at the top of
`ninjaPixel_keymap.h`:

```c
#ifdef RGB_MATRIX_ENABLE
#define NP_L4_1  RM_NEXT    // Sofle Pro: RGB controls
#else
#define NP_L4_1  XXXXXXX    // Rev1: dead key
#endif
```

The `LAYOUT()` call then uses `NP_L4_1` etc., keeping the keymap data clean and
parseable by external tools.

### Currently supported boards

| Board | Keymap directory | Notes |
|---|---|---|
| Sofle Pro | `keyboards/mechboards/sofle/pro/keymaps/ninjaPixel01/` | RGB matrix, rotary encoders, RP2040 |
| Sofle Rev1 | `keyboards/sofle/keymaps/ninjaPixel_sofle_rev1/` | OLED, rotary encoders, ATmega32u4 (Elite-C v4) |

Both keymap directories contain a thin `keymap.c` that is just:

```c
#include "ninjaPixel_keymap.h"
```

And a `rules.mk` that points to this userspace:

```makefile
USER_NAME := ninjaPixel
```

## Adding a new board (e.g. Preonic)

The Preonic rev3 has a different physical layout (ortholinear grid), a different number
of keys, no rotary encoders, and no RGB. This means it **cannot share
`ninjaPixel_keymap.h` directly** because that file uses the Sofle's `LAYOUT()` macro
and assumes 60 keys with encoder positions.

### Recommended approach

The callback functions in `ninjaPixel.c` (layer-switching logic, OLED display) are
board-agnostic and will work as-is. Only the keymap data needs a separate file.

1. **Create the keymap directory:**
   ```
   keyboards/preonic/rev3_drop/keymaps/ninjaPixel_preonic/
   ├── keymap.c
   ├── rules.mk
   └── config.h        (if needed)
   ```

2. **Create a Preonic-specific keymap header** in the userspace:
   ```
   users/ninjaPixel/ninjaPixel_keymap_preonic.h
   ```
   This file defines `keymaps[]` and `tap_dance_actions[]` using the Preonic's
   `LAYOUT_preonic_grid` macro. No `encoder_map[]` needed (no encoders). Same layer
   structure (0-5) and same key bindings where physically possible, adapted to the
   Preonic's smaller grid.

3. **The Preonic's `keymap.c`** includes the Preonic-specific header:
   ```c
   #include "ninjaPixel_keymap_preonic.h"
   ```

4. **The Preonic's `rules.mk`** opts into the userspace:
   ```makefile
   USER_NAME := ninjaPixel
   TAP_DANCE_ENABLE = yes
   CAPS_WORD_ENABLE = yes
   ```
   No `ENCODER_MAP_ENABLE` (no encoders). No `BOOTLOADER` override (uses the default).

5. **Shared code still works automatically:**
   - `ninjaPixel.c` is compiled and linked (via `rules.mk`'s `SRC += ninjaPixel.c`)
   - `layer_state_set_user()` handles Mac/Windows layer switching
   - `rgb_matrix_indicators_advanced_user()` compiles out (no `RGB_MATRIX_ENABLE`)
   - `oled_task_user()` compiles out (no `OLED_ENABLE`)

### What stays shared vs. what diverges

| Component | Shared | Board-specific |
|---|---|---|
| Layer-switching logic (`layer_state_set_user`) | Shared in `ninjaPixel.c` | - |
| RGB underglow disable | Shared in `ninjaPixel.c` (compiles out if no RGB) | - |
| OLED display | Shared in `ninjaPixel.c` (compiles out if no OLED) | - |
| Tap dance enum | Shared in `ninjaPixel.h` | - |
| Tap dance definitions | Per-board keymap header | Different key counts may need different placement |
| `keymaps[]` array | - | Per-board (different `LAYOUT` macro, different key count) |
| `encoder_map[]` | - | Per-board (Preonic has no encoders) |

### Future consideration: shared key rows

If you find yourself maintaining the same key sequences across multiple keymap headers
(e.g. the same function key row, same modifier thumb cluster), you could extract those
as macros in a shared file:

```c
// users/ninjaPixel/ninjaPixel_common_keys.h
#define NP_FKEY_ROW  KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, ...
```

Each board's keymap header then uses these macros within its own `LAYOUT()` calls.
This adds complexity, so only do it if drift between boards becomes a real problem.

## Generating the SVG keymap visualisation

The SVG is generated using [keymap-drawer](https://github.com/caksoylar/keymap-drawer)
and QMK's `c2json` tool.

### Command

From the repository root, run the following to create a `.svg` keymap in the root dir:

```bash
# For the Sofle Pro:
qmk c2json -kb mechboards/sofle/pro -km ninjaPixel01 --no-cpp \
    users/ninjaPixel/ninjaPixel_keymap.h \
  | keymap parse -c 10 -q - > keymap_vis.yaml \
  && keymap draw keymap_vis.yaml > keymap_sofle.svg

# For the Sofle Rev1 (this results in the same thing!):
qmk c2json -kb sofle/rev1 -km ninjaPixel_sofle_rev1 --no-cpp \
    users/ninjaPixel/ninjaPixel_keymap.h \
  | keymap parse -c 10 -q - > keymap_vis.yaml \
  && keymap draw keymap_vis.yaml > keymap_sofle.svg
```

### Why `--no-cpp` is needed

Since `keymap.c` is just `#include "ninjaPixel_keymap.h"`, the default `c2json`
behaviour (which runs the C preprocessor) fails — it can't resolve the userspace
include path. Using `--no-cpp` tells it to parse the file as raw text, which works
because `ninjaPixel_keymap.h` contains the `LAYOUT()` calls directly.

### Caveat: unexpanded macros

With `--no-cpp`, board-specific macros like `NP_L4_1` appear as-is in the JSON output
rather than being expanded to their actual keycodes (e.g. `RM_NEXT` on Pro, `XXXXXXX`
on Rev1). This is cosmetic only — the compiled firmware is unaffected. If this matters
for the visualisation, you can customise the keymap-drawer YAML after generation to
replace the macro names with human-readable labels.

## Build and flash commands

These are unchanged from normal QMK usage:

```bash
# Sofle Pro
qmk compile -kb mechboards/sofle/pro -km ninjaPixel01
qmk flash   -kb mechboards/sofle/pro -km ninjaPixel01

# Sofle Rev1
qmk compile -kb sofle/rev1 -km ninjaPixel_sofle_rev1
qmk flash   -kb sofle/rev1 -km ninjaPixel_sofle_rev1
```

## Layer structure

All boards share the same 6-layer structure:

| Layer | Name | Purpose |
|---|---|---|
| 0 | Mac Base | Colemak layout, Mac modifier positions |
| 1 | Windows Base | Transparent overlay, overrides Mac-specific keys for Windows |
| 2 | Mac Symbols | F-keys, brackets, navigation (activated by `LT(2, KC_SPC)`) |
| 3 | Windows Symbols | Sparse overrides for Windows-specific symbol differences |
| 4 | Keyboard Settings | OS mode switching, Caps Lock, RGB controls (Pro only) |
| 5 | Template | All transparent — placeholder for future use |
