#include QMK_KEYBOARD_H

#define ____ KC_TRNS

enum keymap_layout {
  BASE = 0,
  FUNC
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap BASE: (Base Layer) Default Layer
   * ,------------------------------------------------------------.---------.
   * |Esc | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |  0| - | = |Backspa| Del|----|
   * |------------------------------------------------------------|----|----|
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|     \|Home|----|
   * |------------------------------------------------------------|----|----|
   * |CAPS    |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return |End |----|
   * |------------------------------------------------------------|----|----|
   * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift      | Up |----|
   * |-----------------------------------------------------------------|----|
   * |Ctrl|Win |Alt |        Space             |Alt |Func|Ctrl|Lft| Dn |Rig |
   * `----------------------------------------------------------------------'
   */
  [BASE] = LAYOUT(
    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL,              \
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,  KC_HOME,          \
    KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENTER,  KC_END,               \
    KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP,                      \
    KC_LCTL, KC_LGUI, KC_LALT,            KC_SPACE            , KC_RALT, OSL(FUNC), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT \
  ),

  /* Keymap FUNC: (Second Layer) Function Layer
   * ,------------------------------------------------------------.---------.
   * | ~ | F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Backspa |     |----|
   * |------------------------------------------------------------|-----|----|
   * |     |   |   |   |   |   |   |   |  7|  8|  9|   |   |      |PGUP |----|
   * |------------------------------------------------------------|-----|----|
   * |        |  |  |VOLD|MUTE|VOLU|  |  4|  5|  6|   |   |       |PGDWN|----|
   * |------------------------------------------------------------|-----|----|
   * |        |   |   |   |   |   |  0|  1|  2|  3|   |           | Mute|----|
   * |------------------------------------------------------------------|----|
   * |    |    |    |                          |   |    |    |MPRV| MPLY|MNXT|
   * `----------------------------------------------------------------------'
   */
  [FUNC] = LAYOUT(
    KC_GRV , KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 , KC_F7 , KC_F8 , KC_F9 ,KC_F10 ,KC_F11 ,KC_F12 , KC_BSPC,____,         \
    ____,____,____,____,____,____,____,____,KC_7,KC_8,KC_9,____,____,____, KC_PGUP,         \
    ____,____,____,KC_VOLD,KC_MUTE,KC_VOLU,____,KC_4,KC_5,KC_6,____,____,____,KC_PGDN,   \
    ____,____,____,____,____,____,____,KC_0, KC_1, KC_2,KC_3,____,   KC_MUTE,            \
    ____,____,____,          ____               ,____,____,____,KC_MPRV, KC_MPLY, KC_MNXT\
  )
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    // MACRODOWN only works in this function
      switch(id) {
      }
    return MACRO_NONE;
};

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

void led_set_user(uint8_t usb_led) {

}
