// ---------------------------------------------------------------------------
// Tap-hold tuning for home-row mods + the LT(_SECONDARY, KC_SPC) thumb.
//
// The base layer uses a mod-tap on every home-row key (HYPR/MEH/GUI/CTRL/ALT)
// and a layer-tap on the right thumb (space / _SECONDARY). The settings below
// decide, for every one of those keys, whether a given press is a *tap* (the
// letter / a space) or a *hold* (the modifier / the layer). Getting this wrong
// is what produced the "give in", "no" and "io" misfires: a same-hand roll
// where the first key lingered long enough to be mistaken for a hold.
// ---------------------------------------------------------------------------

// How long a tap-hold key must be held *on its own* before it resolves as a
// hold. 150 ms was aggressive: an ordinary same-hand roll can still have the
// first key down at the 150 ms mark, at which point the timeout path in
// action_tapping.c registers it as a HOLD with no handedness check (CHORDAL_HOLD
// is bypassed on timeout). 180 ms widens that window without making deliberate
// holds feel sluggish.
//
// The shared userspace config (users/ninjaPixel/config.h) already defines
// TAPPING_TERM 150 for the Sofle boards, so it must be #undef'd before being
// raised here. This override is Sweep-only: the Sofle keeps 150 (tuned for its
// MT(MOD_RSFT, KC_ENT) thumb), while the Sweep's home-row mods want a touch more.
#undef TAPPING_TERM
#define TAPPING_TERM 180

// Resolve as a hold *immediately* when another key is pressed AND released
// while the tap-hold key is still down. Good for fast opposite-hand mod usage;
// CHORDAL_HOLD below stops it from firing on same-hand rolls.
#define PERMISSIVE_HOLD

// Chordal Hold (the "opposite-hands" rule): a tap-hold key only resolves as a
// hold if the interrupting key is on the *other* hand. Same-hand chords settle
// as taps. The handedness map is auto-generated from the board's key
// coordinates in keyboard.json (left half 'L', right half 'R', thumbs included),
// so no manual chordal_hold_layout[] is needed.
//
// NOTE: CHORDAL_HOLD only applies on the press/release interrupt paths, i.e.
// while still inside TAPPING_TERM. It does NOT cover the timeout path, which is
// why FLOW_TAP_TERM is needed as well (below).
#define CHORDAL_HOLD

// Flow Tap: while typing in a continuous stream, force a tap-hold key to TAP
// when it is pressed within FLOW_TAP_TERM ms of the previous typing key —
// regardless of how long it is then held. This is the piece that actually fixes
// the reported bugs: it plugs the TAPPING_TERM timeout gap that CHORDAL_HOLD
// leaves open, so fast same-hand rolls ("no", "io") and "give in" (space rolled
// into the next word) come out as plain text instead of ⌘O / Meh+O / arrow keys.
//
// The default is_flow_tap_key() set covers KC_A..KC_Z, KC_SPC and common
// punctuation, and Flow Tap auto-disables whenever a ⌘/⌃/⌥ modifier is already
// held, so deliberate hotkeys (e.g. ⌘+letter) are unaffected.
//
// Tradeoff (chosen deliberately): because KC_SPC is in the default set, rolling
// a letter straight into a *held* space will be forced to a tap, so a brief
// pause is needed before holding space to reach _SECONDARY mid-flow. To exempt
// the thumb instead, override get_flow_tap_term() to return 0 for
// LT(_SECONDARY, KC_SPC).
#define FLOW_TAP_TERM 150

// Disable the "quick tap" auto-repeat: after a tap, holding the *same* key again
// within this term would repeat the tap keycode instead of acting as a hold.
// 0 turns it off entirely, which is the conventional choice for home-row mods.
#define QUICK_TAP_TERM 0
