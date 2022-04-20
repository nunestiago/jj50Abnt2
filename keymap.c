/*
Base Copyright 2017 Luiz Ribeiro <luizribeiro@gmail.com>
Modified 2017 Andrew Novak <ndrw.nvk@gmail.com>
Modified 2018 Wayne Jones (WarmCatUK) <waynekjones@gmail.com>
Modified 2019 AbstractKB
Modified 2022 TNunes <ntiagon@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public LicensezZZ
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

enum layers {
  _DEFLT,
  _RAISE,
  _LOWER,
  _FN
};

enum custom_keycodes {
  MYRGB_TG = SAFE_RANGE
};

bool rgbinit = true;
bool rgbon = true;

const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM = {1,5,5}; //only using the first one

void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom();
  led_set_user(host_keyboard_leds());
}

layer_state_t layer_state_set_user(layer_state_t state) {
  switch (biton32(state)) {
    case _RAISE:
      rgblight_sethsv_noeeprom(170,255,255);
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
    case _LOWER:
      rgblight_sethsv_noeeprom(0,255,255);
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
    case _FN:
      rgblight_sethsv_noeeprom(0,170,170);
      rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
      break;
    default: //_DEFLT
      rgblight_sethsv_noeeprom(0,0,255);
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
  }

  return state;
}

void led_set_user(uint8_t usb_led) {
  if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
    rgblight_mode_noeeprom(RGBLIGHT_MODE_ALTERNATING);
  } else {
    layer_state_set_user(layer_state);
  }
}

void myrgb_toggle(void) {
  if (rgbon) {
    rgblight_disable_noeeprom();
    rgbon = false;
  } else {
    rgblight_enable_noeeprom();
    layer_state_set_user(layer_state);
    led_set_user(host_keyboard_leds());
    rgbon = true;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch(keycode) {
    case MYRGB_TG:
      if (record->event.pressed) {
        myrgb_toggle();
      }
      return false;
    default:
      return true;
  }
}

enum {
    TD_COMM = 0,
    TD_DOT = 1,
    TD_SLSH = 2,
    TD_INT1 = 3,
    TD_ALT = 4,
    TD_CAPLOCK = 5
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for ;, twice for :
    [TD_COMM] = ACTION_TAP_DANCE_DOUBLE(KC_COMM, LSFT(KC_COMM)),
    [TD_DOT] = ACTION_TAP_DANCE_DOUBLE(KC_DOT, LSFT(KC_DOT)),
    [TD_SLSH] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, LSFT(KC_SLSH)),
    [TD_INT1] = ACTION_TAP_DANCE_DOUBLE(KC_INT1, LSFT(KC_INT1)),
    [TD_CAPLOCK] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
    [TD_ALT] = ACTION_TAP_DANCE_DOUBLE(KC_RALT, KC_LALT),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_DEFLT] = LAYOUT (\
KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC, \
KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_DEL, \
KC_GESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_QUOT, KC_SFTENT, \
LSFT_T(KC_CAPS), KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, TD(TD_COMM), TD(TD_DOT), TD(TD_SLSH), TD(TD_INT1), \
LCTL_T(KC_RBRC),  KC_LGUI, LALT_T(KC_BSLS), MO(1), MO(3), KC_SPC, KC_SPC, MO(2), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT  \
    ),

    [_RAISE] = LAYOUT( \
KC_GRV, KC_TRNS, KC_F4, KC_F5, KC_NLCK, KC_PSLS, KC_PAST, KC_PMNS, KC_NO, KC_MINS,KC_EQL, KC_BSPC, \
KC_TAB, KC_PSCR, KC_SLCK, KC_PAUS, KC_P7, KC_P8, KC_P9, KC_PPLS, KC_NO, KC_LBRC, KC_RBRC, KC_DEL, \
KC_ESC, KC_INS, KC_HOME, KC_PGUP, KC_P4, KC_P5, KC_P6, KC_PCMM, KC_SCLN, KC_QUOT, KC_BSLS, KC_ENT, \
KC_LSFT, KC_NUBS, KC_END, KC_PGDN, KC_P1, KC_P2, KC_P3, KC_PEQL, KC_COMM, KC_DOT, KC_SLSH, KC_QUOT, \
LCTL_T(KC_RBRC),  KC_LGUI, LALT_T(KC_BSLS), KC_LALT, KC_TRNS, KC_P0, KC_P0, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT \
    ),

    [_LOWER] = LAYOUT( \
KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, \
KC_TAB, KC_WH_U, KC_MS_U, KC_WH_D, KC_P, KC_G, KC_J, KC_BTN3, KC_U, KC_Y, KC_SCLN, KC_DEL, \
KC_ESC, KC_MS_L, KC_MS_D, KC_MS_R, KC_T, KC_D, KC_H, KC_BTN1, KC_BTN2, KC_I, KC_O, KC_QUOT, \
KC_LSFT, KC_MPRV, KC_MSTP, KC_MPLY, KC_MNXT, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_ENT, \
LCTL_T(KC_RBRC),  KC_LGUI, LALT_T(KC_BSLS), KC_VOLD, KC_VOLU, KC_SPC, KC_SPC, MO(2), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT  \
    ),


    [_FN] = LAYOUT( \
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RESET, \
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MYRGB_TG    \
    )
};


