// https://docs.qmk.fm/#/config_options
#undef TAPPING_TERM
#define TAPPING_TERM 130

// Timeout comfortable for tapping key multiple times:
#undef TAPPING_TERM_TAP_DANCE
#define TAPPING_TERM_TAP_DANCE 270

#undef COMBO_TERM
#define COMBO_TERM 130

// Timeout for which interruption is taken into account
#undef INTERRUPTION_TIMEOUT
#define INTERRUPTION_TIMEOUT 60

#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED 10

#undef MOUSEKEY_WHEEL_TIME_TO_MAX
#define MOUSEKEY_WHEEL_TIME_TO_MAX 150

#undef MOUSEKEY_WHEEL_MAX_SPEED
#define MOUSEKEY_WHEEL_MAX_SPEED 4

// https://github.com/qmk/qmk_firmware/blob/master/docs/feature_leader_key.md
// #define LEADER_TIMEOUT 300

#define UNICODE_SELECTED_MODES UC_OSX, UC_WINC

#undef COMBO_COUNT
#define COMBO_COUNT 5

#define FORCE_NKRO