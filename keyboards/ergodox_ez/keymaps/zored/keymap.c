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

enum operating_systems {
  OS_MACOS = 1,
  OS_WINDOWS,
};

// Get operating system. Windows is default.
uint8_t get_os (void) {
  switch (get_unicode_input_mode()) {
    case UC_OSX:
      return OS_MACOS;

    case UC_WIN:
    case UC_WINC:
      return OS_WINDOWS;
  }

  return OS_WINDOWS;
}


enum unicode_names {
  E_LOL = 0,
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
// - Unique only!
// - Don't forget to update COMBO_COUNT.
const uint16_t PROGMEM combo_esc[] = {KC_Y, KC_U, COMBO_END};
const uint16_t PROGMEM combo_right_arrow[] = {KC_N, KC_M, COMBO_END};
const uint16_t PROGMEM combo_underscore[] = {KC_H, KC_J, COMBO_END};
const uint16_t PROGMEM combo_quit[] = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM combo_backslash[] = {KC_K, KC_L, COMBO_END};

enum combo_names {
  CMB_ESC = 0,
  CMB_RAR,
  CMB_UND,
  CMB_QUI,
  CMB_BSLS,
};

combo_t key_combos[COMBO_COUNT] = {
  [CMB_ESC] = COMBO_ACTION(combo_esc),
  [CMB_RAR] = COMBO_ACTION(combo_right_arrow),
  [CMB_UND] = COMBO_ACTION(combo_underscore),
  [CMB_QUI] = COMBO_ACTION(combo_quit),
  [CMB_BSLS] = COMBO_ACTION(combo_backslash),
};

void process_combo_event(uint8_t combo_index, bool pressed) {
  if (!pressed) {
    return;
  }

  switch(combo_index) {
    case CMB_ESC:
      tap_code(KC_ESC);
      break;

    case CMB_RAR:
      tap_code(KC_MINUS);

      // >
      register_code(KC_LSHIFT);
      tap_code(KC_DOT);
      unregister_code(KC_LSHIFT);
      break;

    case CMB_UND:
      // _
      register_code(KC_LSHIFT);
      tap_code(KC_MINUS);
      unregister_code(KC_LSHIFT);
      break;

    case CMB_QUI:
      switch (get_os()) {
        case OS_WINDOWS:
          // alt+f4
          register_code(KC_LALT);
          tap_code(KC_F4);
          unregister_code(KC_LALT);
          break;

        case OS_MACOS:
          // cmd+q
          register_code(KC_LCMD);
          tap_code(KC_Q);
          unregister_code(KC_LCMD);
          break;
      }
      break;

    case CMB_BSLS:
      tap_code(KC_BSLS);
      break;
  }
};

enum dance_state_values {
  DANCE_Z = 1,
  DANCE_X,
  DANCE_C,
  DANCE_SLSH,
  DANCE_DOT,
  DANCE_COMM,
  // Left:
  DANCE_LEFT_CTRL,
  DANCE_LEFT_CTRL_SHIFT,
  DANCE_LEFT_ALT,
  DANCE_LEFT_ALT_SHIFT,
  DANCE_LEFT_GUI,
  DANCE_LEFT_GUI_SHIFT,
  // Right:
  DANCE_RIGHT_CTRL,
  DANCE_RIGHT_CTRL_SHIFT,
  DANCE_RIGHT_ALT,
  DANCE_RIGHT_ALT_SHIFT,
  DANCE_RIGHT_GUI,
  DANCE_RIGHT_GUI_SHIFT,
};

enum dance_keys {
  DNC_Z = 0,
  DNC_X,
  DNC_C,
  DNC_SLSH,
  DNC_DOT,
  DNC_COMM,
  // Last:
  DNC_AMOUNT
};

static int dance_states[DNC_AMOUNT] = {0};

void dance_modifier_key(
  qk_tap_dance_state_t *state,
  int danceStateTap,
  int danceStateHold1,
  int danceStateHold2,
  uint8_t tapKeycode,
  uint8_t holdKeycode1,
  uint8_t holdKeycode2,
  uint16_t danceKey
) {
  // Not enough taps:
  if (state->count < 1) {
    return;
  }

  // Register modifiers:
  if (state->pressed) {
    // First modifier:
    register_code(holdKeycode1);
    dance_states[danceKey] = danceStateHold1;

    if (state->count == 1) {
      return;
    }

    // Second modifier:
    register_code(holdKeycode2);
    dance_states[danceKey] = danceStateHold2;
    return;
  }

  // Make previous taps (if needed):
  for (int i = 1; i < state->count; i++) {
    tap_code(tapKeycode);
  }

  // Last tap:
  register_code(tapKeycode);
  dance_states[danceKey] = danceStateTap;
}

void on_dance(qk_tap_dance_state_t *state, void *user_data) {
  uint16_t danceKey = state->keycode - QK_TAP_DANCE;

  switch (danceKey) {
    case DNC_Z:
      dance_modifier_key(
        state,
        DANCE_Z,
        DANCE_LEFT_CTRL,
        DANCE_LEFT_CTRL_SHIFT,
        KC_Z,
        KC_LCTRL,
        KC_LSHIFT,
        DNC_Z
      );
      break;

    case DNC_X:
      dance_modifier_key(
        state,
        DANCE_X,
        DANCE_LEFT_ALT,
        DANCE_LEFT_ALT_SHIFT,
        KC_X,
        KC_LALT,
        KC_LSHIFT,
        DNC_X
      );
      break;

    case DNC_C:
      dance_modifier_key(
        state,
        DANCE_C,
        DANCE_LEFT_GUI,
        DANCE_LEFT_GUI_SHIFT,
        KC_C,
        KC_LGUI,
        KC_LSHIFT,
        DNC_C
      );
      break;

    case DNC_SLSH:
      dance_modifier_key(
        state,
        DANCE_SLSH,
        DANCE_RIGHT_CTRL,
        DANCE_RIGHT_CTRL_SHIFT,
        KC_SLSH,
        KC_RCTRL,
        KC_RSHIFT,
        DNC_SLSH
      );
      break;

    case DNC_DOT:
      dance_modifier_key(
        state,
        DANCE_DOT,
        DANCE_RIGHT_ALT,
        DANCE_RIGHT_ALT_SHIFT,
        KC_DOT,
        KC_RALT,
        KC_RSHIFT,
        DNC_DOT
      );
      break;

    case DNC_COMM:
      dance_modifier_key(
        state,
        DANCE_COMM,
        DANCE_RIGHT_GUI,
        DANCE_RIGHT_GUI_SHIFT,
        KC_COMM,
        KC_RGUI,
        KC_RSHIFT,
        DNC_COMM
      );
      break;
  }
}

void on_dance_reset(qk_tap_dance_state_t *state, void *user_data) {
  uint16_t danceKey = state->keycode - QK_TAP_DANCE;
  switch (dance_states[danceKey]) {
    case DANCE_Z:
      unregister_code(KC_Z);
      break;

    case DANCE_X:
      unregister_code(KC_X);
      break;

    case DANCE_C:
      unregister_code(KC_C);
      break;

    case DANCE_SLSH:
      unregister_code(KC_SLSH);
      break;

    case DANCE_DOT:
      unregister_code(KC_DOT);
      break;

    case DANCE_COMM:
      unregister_code(KC_COMM);
      break;

    // Left:
    case DANCE_LEFT_CTRL:
      unregister_code(KC_LCTRL);
      break;

    case DANCE_LEFT_CTRL_SHIFT:
      unregister_code(KC_LSHIFT);
      unregister_code(KC_LCTRL);
      break;

    case DANCE_LEFT_ALT:
      unregister_code(KC_LALT);
      break;

    case DANCE_LEFT_ALT_SHIFT:
      unregister_code(KC_LSHIFT);
      unregister_code(KC_LALT);
      break;

    case DANCE_LEFT_GUI:
      unregister_code(KC_LGUI);
      break;

    case DANCE_LEFT_GUI_SHIFT:
      unregister_code(KC_LSHIFT);
      unregister_code(KC_LGUI);
      break;

    // Right:
    case DANCE_RIGHT_CTRL:
      unregister_code(KC_RCTRL);
      break;

    case DANCE_RIGHT_CTRL_SHIFT:
      unregister_code(KC_RSHIFT);
      unregister_code(KC_RCTRL);
      break;

    case DANCE_RIGHT_ALT:
      unregister_code(KC_RALT);
      break;

    case DANCE_RIGHT_ALT_SHIFT:
      unregister_code(KC_RSHIFT);
      unregister_code(KC_RALT);
      break;

    case DANCE_RIGHT_GUI:
      unregister_code(KC_RGUI);
      break;

    case DANCE_RIGHT_GUI_SHIFT:
      unregister_code(KC_RSHIFT);
      unregister_code(KC_RGUI);
      break;
  }
  dance_states[danceKey] = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    // Left (ctrl - alt - gui)
    [DNC_Z] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, on_dance, on_dance_reset, TAPPING_TERM_TAP_DANCE),
    [DNC_X] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, on_dance, on_dance_reset, TAPPING_TERM_TAP_DANCE),
    [DNC_C] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, on_dance, on_dance_reset, TAPPING_TERM_TAP_DANCE),

    // Right: (ctrl - alt - gui)
    [DNC_SLSH] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, on_dance, on_dance_reset, TAPPING_TERM_TAP_DANCE),
    [DNC_DOT] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, on_dance, on_dance_reset, TAPPING_TERM_TAP_DANCE),
    [DNC_COMM] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, on_dance, on_dance_reset, TAPPING_TERM_TAP_DANCE),
};

enum custom_keycodes {
  ZKC_BTL = SAFE_RANGE
};

enum layers {
  L_DEF = 0,
  L_PLO,
  L_SYM,
  L_NAV,
  L_EMO,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_DEF] = LAYOUT_ergodox(
    KC_ESC,           KC_F1,         KC_F2,         KC_F3,          KC_F4,          KC_F5,          KC_F6,
    KC_TAB,           KC_Q,          KC_W,          KC_E,           KC_R,           KC_T,           _______,
    LT(L_SYM,KC_CLCK),KC_A,          KC_S,          KC_D,           KC_F,           KC_G,
    KC_LSPO,          TD(DNC_Z),     TD(DNC_X),     TD(DNC_C),      KC_V,           KC_B,           KC_MEH,
    KC_LCTRL,         KC_LALT,       KC_DLR,        KC_LEFT,        KC_RGHT,
                                                                                    KC_ESC,         _______,
                                                                                                    KC_HOME,
                                                                    KC_SPC,         KC_BSPC,        KC_END,
        KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,         KC_F12,           KC_RBRC,
        _______,        KC_Y,           KC_U,           KC_I,           KC_O,           LT(L_NAV,KC_P),   KC_LBRC,
                        KC_H,           KC_J,           KC_K,           KC_L,           LT(L_NAV,KC_SCLN),LT(L_EMO, KC_QUOT),
        KC_HYPR,        KC_N,           KC_M,           TD(DNC_COMM),   TD(DNC_DOT),    TD(DNC_SLSH),     KC_RSPC,
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
    _______,        KC_MENU,        KC_BTN2,        KC_MS_U,        KC_BTN1,        _______,        _______,
    _______,        _______,        KC_MS_L,        KC_MS_D,        KC_MS_R,        _______,
    _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        _______,        _______,        KC_BTN1,        KC_BTN2,
                                                                                    _______,        _______,
                                                                                                    _______,
                                                                    _______,        _______,        _______,
        _______,        KC_PWR,         KC_SLEP,        UC_M_WC,        UC_M_OS,        _______,        _______,
        _______,        _______,        KC_BRID,        KC_BRIU,        _______,        _______,        _______,
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
    _______,        _______,        _______,        STN_A,          STN_O,
                                                                                    _______,        _______,
                                                                                                    _______,
                                                                    STN_A,           STN_O,           _______,
        _______,        _______,        _______,        _______,        _______,        _______,         _______,
        _______,        STN_N6,         STN_N7,         STN_N8,         STN_N9,         STN_NA,          STN_NB,
                        STN_ST3,        STN_FR,         STN_PR,         STN_LR,         STN_TR,          STN_DR,
        _______,        STN_ST4,        STN_RR,         STN_BR,         STN_GR,         STN_SR,          STN_ZR,
        STN_E,          STN_U,          _______,        _______,        _______,
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
        ZKC_BTL,        _______,        _______,        _______,        _______,        _______,        _______,
        _______,        _______,        KC_F7,          KC_F8,          KC_F9,          _______,        _______,
                        _______,        KC_F4,          KC_F5,          KC_F6,          _______,        _______,
        _______,        _______,        KC_F1,          KC_F2,          KC_F3,          _______,        _______,
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

void matrix_init_user(void) {
  steno_set_mode(STENO_MODE_GEMINI);
}

void dim_leds(void) {
  ergodox_board_led_off();
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bool complete = false;

  switch (keycode) {
    case ZKC_BTL:
      if (record->event.pressed) {
        clear_keyboard();
        bootloader_jump();
        complete = true;
      }
      break;
  }

  return !complete;
};

uint32_t layer_state_set_user(uint32_t state) {
  dim_leds();

  switch (biton32(state)) {
    case L_EMO:
      ergodox_right_led_2_on();
      break;
    case L_PLO:
      ergodox_right_led_3_on();
      break;
  }

  return state;
};
