#include "ergodox_ez.h"
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "keymap_german.h"
#include "keymap_nordic.h"
#include "keymap_french.h"
#include "keymap_spanish.h"

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
};

const uint32_t PROGMEM unicode_map[] = {
    [E_LOL] 0x1F60A, // 😊
    [E_JOY] 0x1F602, // 😂
    [E_THI] 0x1F914, // 🤔
    [E_THU] 0x1F44D, // 👍
    [E_BIC] 0x1F4AA, // 💪
};

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE,
  EPRM,           
  RGB_SLD,        
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_DEF] = LAYOUT_ergodox(
    KC_ESC,           KC_F1,         KC_F2,         KC_F3,          KC_F4,          KC_F5,          KC_F6,          
    KC_TAB,           KC_Q,          KC_W,          KC_E,           KC_R,           KC_T,           _______,        
    LT(L_SYM,KC_CLCK),KC_A,          KC_S,          KC_D,           KC_F,           KC_G,           
    KC_LSPO,          CTL_T(KC_Z),   ALT_T(KC_X),   GUI_T(KC_C),    KC_V,           KC_B,           KC_MEH,         
    C_S_T(XXXXXXX),   LCA_T(XXXXXXX),KC_DLR,        KC_LEFT,        KC_RGHT,        
                                                                                    KC_ESC,         _______,        
                                                                                                    KC_HOME,        
                                                                    KC_SPC,         KC_BSPC,        KC_END,
        KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,         KC_F12,         KC_RBRC,        
        _______,        KC_Y,           KC_U,           KC_I,           KC_O,           LT(2,KC_P),     KC_LBRC,        
                        KC_H,           KC_J,           KC_K,           KC_L,           LT(2,KC_SCLN),  LT(L_EMO, KC_QUOT),        
        KC_HYPR,        KC_N,           KC_M,           KC_COMM,        KC_DOT,         RCTL_T(KC_SLSH),KC_RSPC,        
        KC_DOWN,        KC_UP,          KC_BSLS,        KC_GRV,         TG(3),          
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
        _______,        _______,        KC_ACL0,        KC_ACL1,        KC_ACL2,        _______,        _______,        
                        KC_LEFT,        KC_DOWN,        KC_UP,          KC_RGHT,        _______,        _______,        
        _______,        KC_WH_L,        KC_WH_D,        KC_WH_U,        KC_WH_R,        _______,        _______,        
        KC_VOLD,        KC_VOLU,        KC_MUTE,        _______,        _______,        
        _______,        _______,        
        _______,        
        _______,        _______,        _______
    ),

  [L_PLO] = LAYOUT_ergodox(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,        
    _______,        _______,        _______,        _______,        _______,        _______,        _______,        
    _______,        KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           
    _______,        KC_A,           KC_S,           KC_D,           KC_F,           KC_G,           _______,        
    _______,        _______,        _______,        _______,        _______,        
                                                                                    _______,        _______,        
                                                                                                    _______,        
                                                                    KC_C,           KC_V,           _______,        
        _______,        _______,        _______,        _______,        _______,        _______,        _______,        
        _______,        _______,        _______,        _______,        _______,        _______,        _______,        
                        KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           KC_LBRC,        
        _______,        KC_H,           KC_J,           KC_K,           KC_L,           KC_SCLN,        KC_QUOT,        
        _______,        _______,        _______,        _______,        _______,        
        _______,        _______,        
        _______,        
        _______,        KC_N,           KC_M
    ),

  [L_EMO] = LAYOUT_ergodox(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,        
    _______,        X(E_LOL),       X(E_JOY),       X(E_THI),       X(E_THU),       X(E_BIC),       _______,        
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
const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(1)
};

// leaving this in place for compatibilty with old keymaps cloned and re-compiled.
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
        case 0:
        if (record->event.pressed) {
          SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
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
