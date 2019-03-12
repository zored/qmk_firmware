#include "ergodox_ez.h"
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "keymap_steno.h"

#define LCGS(code) LCTL(LGUI(LSFT(code)))
#define LCS(code) LCTL(LSFT(code))

#define SLT(kc) (QK_LSFT | QK_LALT | (kc))
#define ALT_TAB LALT(KC_TAB)
#define SLT_TAB SLT(KC_TAB)

#define L_DEF 0
#define L_SYM 1
#define L_NAV 2
#define L_PLO 3
#define L_EMO 4

enum unicode_names {
  E_LOL,
  E_JOY,
  E_THI,
  E_THU,
  E_BIC,
  E_SMI,
  E_FIR,
  E_HEA,
};

const uint32_t PROGMEM
unicode_map[] = {
  [E_LOL] 0x1F60A, // 😊
  [E_JOY] 0x1F602, // 😂
  [E_THI] 0x1F914, // 🤔
  [E_THU] 0x1F44D, // 👍
  [E_BIC] 0x1F4AA, // 💪
  [E_SMI] 0x1F60F, // 😏
  [E_FIR] 0x1F525, // 🔥
  [E_HEA] 0xFE0F, // ❤️
};

// Combos:
const uint16_t PROGMEM
combo_esc[] = {KC_D, KC_F, KC_J, KC_K, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
  COMBO(combo_esc, KC_ESC)
};

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE,
  EPRM,
  RGB_SLD,
};

enum dance_state_values {
  DANCE_Z = 1,
  DANCE_X,
  DANCE_CTRL,
  DANCE_CTRL_SHIFT,
  DANCE_ALT,
  DANCE_ALT_SHIFT,
};

enum dance_keys {
  DNC_Z = 0,
  DNC_X,
};

static int dance_state = 0;

bool is_holding(qk_tap_dance_state_t *state) {
  if (state->pressed) {
    return true;
  }

  // Interrupted in short period of time:
  if (state->interrupted && timer_elapsed(state->timer) < INTERRUPTION_TIMEOUT) {
    return true;
  }

  return false;
}

void dance_modifier_key(
  qk_tap_dance_state_t *state,
  int danceStateTap,
  int danceStateHold1,
  int danceStateHold2,
  uint8_t tapKeycode,
  uint8_t holdKeycode1,
  uint8_t holdKeycode2
) {
  // Not enough taps:
  if (state->count < 1) {
    return;
  }

  // Register modifiers:
  if (is_holding(state)) {
    // First modifier:
    register_code(holdKeycode1);
    dance_state = danceStateHold1;

    if (state->count == 1) {
      return;
    }

    // Second modifier:
    register_code(holdKeycode2);
    dance_state = danceStateHold2;
    return;
  }

  // Make previous taps (if needed):
  for (int i = 1; i < state->count; i++) {
    register_code(tapKeycode);
    unregister_code(tapKeycode);
  }

  // Last tap:
  register_code(tapKeycode);
  dance_state = danceStateTap;
}

void on_dance(qk_tap_dance_state_t *state, void *user_data) {
  uint16_t danceKey = state->keycode - QK_TAP_DANCE;

  switch (danceKey) {
    case DNC_Z:
      dance_modifier_key(
        state,
        DANCE_Z,
        DANCE_CTRL,
        DANCE_CTRL_SHIFT,
        KC_Z,
        KC_LCTRL,
        KC_LSHIFT
      );
      break;

    case DNC_X:
      dance_modifier_key(
        state,
        DANCE_X,
        DANCE_ALT,
        DANCE_ALT_SHIFT,
        KC_X,
        KC_LALT,
        KC_LSHIFT
      );
      break;
  }
}

void on_dance_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (dance_state) {
    case DANCE_Z:
      unregister_code(KC_Z);
      break;

    case DANCE_X:
      unregister_code(KC_X);
      break;

    case DANCE_CTRL:
      unregister_code(KC_LCTRL);
      break;

    case DANCE_CTRL_SHIFT:
      unregister_code(KC_LSHIFT);
      unregister_code(KC_LCTRL);
      break;

    case DANCE_ALT:
      unregister_code(KC_LALT);
      break;

    case DANCE_ALT_SHIFT:
      unregister_code(KC_LSHIFT);
      unregister_code(KC_LALT);
      break;
  }
  dance_state = 0;
}

qk_tap_dance_action_t
tap_dance_actions[] = {
    [DNC_Z] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, on_dance, on_dance_reset, TAPPING_TERM_TAP_DANCE),
    [DNC_X] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, on_dance, on_dance_reset, TAPPING_TERM_TAP_DANCE),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_DEF] = LAYOUT_ergodox(
    KC_ESC,           KC_F1,         KC_F2,         KC_F3,          KC_F4,          KC_F5,          KC_F6,
    KC_TAB,           KC_Q,          KC_W,          KC_E,           KC_R,           KC_T,           _______,
    LT(L_SYM,KC_CLCK),KC_A,          KC_S,          KC_D,           KC_F,           KC_G,
    KC_LSPO,          TD(DNC_Z),     TD(DNC_X),     GUI_T(KC_C),    KC_V,           KC_B,           KC_MEH,
    KC_LCTRL,         KC_LALT,       KC_DLR,        KC_LEFT,        KC_RGHT,
                                                                                    KC_ESC,         _______,
                                                                                                    KC_HOME,
                                                                    KC_SPC,         KC_BSPC,        KC_END,
        KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,         KC_F12,           KC_RBRC,
        _______,        KC_Y,           KC_U,           KC_I,           KC_O,           LT(L_NAV,KC_P),   KC_LBRC,
                        KC_H,           KC_J,           KC_K,           KC_L,           LT(L_NAV,KC_SCLN),LT(L_EMO, KC_QUOT),
        KC_HYPR,        KC_N,           KC_M,           KC_COMM,        KC_DOT,         RCTL_T(KC_SLSH),  KC_RSPC,
        KC_DOWN,        KC_UP,          KC_BSLS,        KC_GRV,         TG(L_PLO),
        KC_ESC,         _______,
        KC_PGUP,
        KC_PGDN,        KC_TAB,         KC_ENT
    ),

  [L_SYM] = LAYOUT_ergodox(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        KC_EXLM,        KC_AT,          KC_LCBR,        KC_RCBR,        KC_BSLS,        _______,
    _______,        KC_HASH,        KC_DLR,         KC_LPRN,        KC_RPRN,        KC_GRV,
    _______,        KC_PERC,        KC_CIRC,        KC_LBRC,        KC_RBRC,        KC_TILD,        _______,
    _______,        _______,        _______,        _______,        _______,
                                                                                    _______,        _______,
                                                                                                    _______,
                                                                    _______,        KC_DEL,         _______,
        _______,        _______,        _______,        _______,        _______,        _______,        _______,
        _______,        KC_PPLS,        KC_7,           KC_8,           KC_9,           _______,        KC_PIPE,
                        KC_EQL,         KC_4,           KC_5,           KC_6,           KC_PAST,        _______,
        _______,        KC_MINS,        KC_1,           KC_2,           KC_3,           KC_AMPR,        _______,
        KC_EQL,         KC_0,           KC_DOT,         _______,        _______,
        _______,        _______,
        _______,
        _______,        _______,        _______
    ),

  [L_NAV] = LAYOUT_ergodox(
    _______,        _______,        _______,       _______,        _______,        _______,        _______,
    _______,        _______,        KC_BTN2,        KC_MS_U,        KC_BTN1,        _______,        _______,
    _______,        _______,        KC_MS_L,        KC_MS_D,        KC_MS_R,        _______,
    _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        _______,        _______,        KC_BTN1,        KC_BTN2,
                                                                                    _______,        _______,
                                                                                                    _______,
                                                                    _______,        _______,        _______,
        _______,        KC_PWR,         KC_SLEP,        UC_M_WC,        UC_M_OS,        _______,        _______,
        _______,        _______,        _______,        _______,        _______,        _______,        _______,
                        KC_LEFT,        KC_DOWN,        KC_UP,          KC_RGHT,        _______,        _______,
        _______,        KC_WH_L,        KC_WH_D,        KC_WH_U,        KC_WH_R,        _______,        _______,
        KC_VOLD,        KC_VOLU,        KC_MUTE,        _______,        _______,
        _______,        _______,
        _______,
        _______,        _______,        _______
    ),

  [L_PLO] = LAYOUT_ergodox(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        STN_N1,         STN_N2,         STN_N3,         STN_N4,         STN_N5,         _______,
    _______,        STN_S1,         STN_TL,         STN_PL,         STN_HL,         STN_ST1,
    _______,        STN_S2,         STN_KL,         STN_WL,         STN_RL,         STN_ST2,           _______,
    _______,        _______,        _______,        _______,        _______,
                                                                                    _______,        _______,
                                                                                                    _______,
                                                                    STN_A,           STN_O,           _______,
        _______,        _______,        _______,        _______,        _______,        _______,         _______,
        _______,        STN_N6,         STN_N7,         STN_N8,         STN_N9,         STN_NA,          STN_NB, 
                        STN_ST3,        STN_FR,         STN_PR,         STN_LR,         LT(L_NAV,STN_TR),STN_DR,
        _______,        STN_ST4,        STN_RR,         STN_BR,         STN_GR,         STN_SR,          STN_ZR,
        _______,        _______,        _______,        _______,        _______,
        _______,        _______,
        _______,
        _______,        STN_E,           STN_U
    ),

  [L_EMO] = LAYOUT_ergodox(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        X(E_LOL),       X(E_JOY),       X(E_THI),       X(E_THU),       X(E_BIC),       _______,
    _______,        X(E_SMI),       X(E_FIR),       X(E_HEA),       _______,        _______,
    _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        _______,        _______,        _______,        _______,
                                                                                    _______,        _______,
                                                                                                    _______,
                                                                    _______,        _______,        _______,
        _______,        _______,        _______,        _______,        _______,        _______,        _______,
        _______,        _______,        _______,        _______,        _______,        _______,        _______,
                        _______,        _______,        _______,        _______,        _______,        _______,
        _______,        _______,        _______,        _______,        _______,        _______,        _______,
        _______,        _______,        _______,        _______,        _______,
        _______,        _______,
        _______,
        _______,        _______,        _______
    ),

  /*
  [4] = LAYOUT_ergodox(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        _______,        _______,        _______,        _______,        _______,
    _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        _______,        _______,        _______,        _______,
                                                                                    _______,        _______,
                                                                                                    _______,
                                                                    _______,        _______,        _______,
        _______,        _______,        _______,        _______,        _______,        _______,        _______,
        _______,        _______,        _______,        _______,        _______,        _______,        _______,
                        _______,        _______,        _______,        _______,        _______,        _______,
        _______,        _______,        _______,        _______,        _______,        _______,        _______,
        _______,        _______,        _______,        _______,        _______,
        _______,        _______,
        _______,
        _______,        _______,        _______
    ),
   */

};

bool suspended = false;
const uint16_t PROGMEM
fn_actions[] = {
[1] = ACTION_LAYER_TAP_TOGGLE(1)
};

// leaving this in place for compatibilty with old keymaps cloned and re-compiled.
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
  switch (id) {
    case 0:
      if (record->event.pressed) {
        SEND_STRING(QMK_KEYBOARD
        "/"
        QMK_KEYMAP
        " @ "
        QMK_VERSION);
      }
      break;
  }
  return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // dynamically generate these.
    case EPRM:
      if (record->event.pressed) {
        eeconfig_init();
      }
      return false;
      break;
  }
  return true;
}


void matrix_init_user(void) {
  steno_set_mode(STENO_MODE_GEMINI);
}

uint32_t layer_state_set_user(uint32_t state) {
  ergodox_board_led_off();
  uint8_t layer = biton32(state);

  ergodox_board_led_off();
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();
  switch (layer) {
    case 1:
      // ergodox_right_led_1_on();
      break;
    case 2:
      // ergodox_right_led_2_on();
      break;
    case 3:
      ergodox_right_led_3_on();
      break;
    case 4:
      // ergodox_right_led_1_on();
      // ergodox_right_led_2_on();
      break;
    case 5:
      // ergodox_right_led_1_on();
      // ergodox_right_led_3_on();
      break;
    case 6:
      // ergodox_right_led_2_on();
      // ergodox_right_led_3_on();
      break;
    case 7:
      // ergodox_right_led_1_on();
      // ergodox_right_led_2_on();
      // ergodox_right_led_3_on();
      break;
    default:
      break;
  }
  return state;
};
