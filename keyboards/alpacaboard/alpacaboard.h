#ifndef ALPACABOARD_H
#define ALPACABOARD_H

/* Because we're using the Feather BLE as the microcontroller board, we need a
 * GPIO expander. Using the MCP23008, we need to include the I2C and AVR libs.
 */
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
#include "i2c.h"
#include "quantum.h"

/* I2C addresses for MCP23008 */
/*  MCP23008 a0, a1, a2 are all connected to ground, all set to 0*/
#define I2C_ADDR          0b0100000
#define I2C_ADDR_WRITE    ( (I2C_ADDR << 1) | I2C_WRITE )
#define I2C_ADDR_READ     ( (I2C_ADDR << 1) | I2C_READ  )
/* Table 1-3 */
#define IODIR             0x00
#define GPPU              0x06
#define GPIO              0x09
#define OLAT              0x0A

void init_alpacaboard(void);
uint8_t init_mcp23008(void);


/* Layout is a modified ANSI */
#define LAYOUT( \
  	K000, K001, K002, K003, K004, K005, K006, K007, K008, K100, K101, K102, K103, K104, K105, \
  	K200, K201, K108, K202, K203, K204, K205, K206, K207, K208, K300, K301, K302, K303, K106, \
  	K400, K401, K402, K403, K404, K405, K406, K407, K408, K304, K305, K306, K307,       K107, \
  	K500, K501, K502, K503, K504, K505, K506, K507, K508, K600, K601, K308,       K603,       \
  	K700, K701, K702,                   K703,             K704, K604, K705, K706, K707, K708, \
  ) \
  { \
    // ESC, 1   , 2   , 3   , 4   , 5   , 6   , 7   , 8
  	{ K000, K001, K002, K003, K004, K005, K006, K007, K008 }, \
    // 9  , 0    , -   , =  , BSPC, Del , Home, End , E
  	{ K100, K101, K102, K103, K104, K105, K106, K107, K108 }, \
    // Tab, Q   , W   , R   , T   , Y   , U   , I   , O
  	{ K200, K201, K202, K203, K204, K205, K206, K207, K208 }, \
    // P  , [   , ]   , \   , L   , ;   , '   , ENT , Rshift
  	{ K300, K301, K302, K303, K304, K305, K306, K307, K308 }, \
    //Caps, A   , S   , D   , F   , G   , H   , J   , K
  	{ K400, K401, K402, K403, K404, K405, K406, K407, K408 }, \
    //LSFT, Z   , X   , C   , V   , B   , N   , M   , Com
  	{ K500, K501, K502, K503, K504, K505, K506, K507, K508 }, \
    //PER , /   , NONE , UP  , FN  , NONE , NONE , NONE , NONE
  	{ K600, K601, KC_NO, K603, K604, KC_NO, KC_NO, KC_NO, KC_NO }, \
    //LCTL, GUI , LALT, SPC , RALT, RCTL, LFT , DWN , RHT
  	{ K700, K701, K702, K703, K704, K705, K706, K707, K708 } \
  }

#endif
