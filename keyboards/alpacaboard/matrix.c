#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "wait.h"
#include "action_layer.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "alpacaboard.h"
#include "expander_mcp23008.h"
#include "i2cmaster.h"
#ifdef DEBUG_MATRIX_SCAN_RATE
#include  "timer.h"
#endif

/*
 * This constant define not debouncing time in msecs, but amount of matrix
 * scan loops which should be made to get stable debounced results.
 *
 * On Ergodox matrix scan rate is relatively low, because of slow I2C.
 * Now it's only 317 scans/second, or about 3.15 msec/scan.
 * According to Cherry specs, debouncing time is 5 msec.
 *
 * And so, there is no sense to have DEBOUNCE higher than 2.
 */

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

// Debouncing: store for each key the number of scans until it's eligible to
// change.  When scanning the matrix, ignore any changes in keys that have
// already changed in the last DEBOUNCE scans.
static uint8_t matrix_debouncing[MATRIX_ROWS];

static void init_rows(void);
static void read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_row);
static void unselect_col(void);
static void unselect_col(uint8_t col);
static void select_col(uint8_t col);

#ifdef DEBUG_MATRIX_SCAN_RATE
uint32_t matrix_timer;
uint32_t matrix_scan_count;
#endif


__attribute__ ((weak))
void matrix_init_user(void) {}

__attribute__ ((weak))
void matrix_scan_user(void) {}

__attribute__ ((weak))
void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
  matrix_scan_user();
}

inline
uint8_t matrix_rows(void)
{
  return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
  return MATRIX_COLS;
}

void matrix_init(void)
{
  // disable JTAG since this chip is ATmega32u4
  MCUCR = (1<<JTD);
  MCUCR = (1<<JTD);

  // Using ROW2COL
  unselect_col();
  init_rows();

  // initialize matrix state: all keys off
  for (uint8_t i=0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
    matrix_debouncing[i] = 0;
    }
  }

// #ifdef DEBUG_MATRIX_SCAN_RATE
//   matrix_timer = timer_read32();
//   matrix_scan_count = 0;
// #endif

  matrix_init_quantum();

}

// void matrix_power_up(void) {
//   unselect_rows();
//   init_cols();
//
//   // initialize matrix state: all keys off
//   for (uint8_t i=0; i < MATRIX_ROWS; i++) {
//     matrix[i] = 0;
//   }
//
// #ifdef DEBUG_MATRIX_SCAN_RATE
//   matrix_timer = timer_read32();
//   matrix_scan_count = 0;
// #endif
// }

// Returns a matrix_row_t whose bits are set if the corresponding key should be
// eligible to change in this scan.
matrix_row_t debounce_mask(uint8_t row) {
  matrix_row_t result = 0;
  for (uint8_t j=0; j < MATRIX_COLS; ++j) {
    if (debounce_matrix[row * MATRIX_COLS + j]) {
      --debounce_matrix[row * MATRIX_COLS + j];
    } else {
      result |= (1 << j);
    }
  }
  return result;
}

// Report changed keys in the given row.  Resets the debounce countdowns
// corresponding to each set bit in 'change' to DEBOUNCE.
void debounce_report(matrix_row_t change, uint8_t row) {
  for (uint8_t i = 0; i < MATRIX_COLS; ++i) {
    if (change & (1 << i)) {
      debounce_matrix[row * MATRIX_COLS + i] = DEBOUNCE;
    }
  }
}

uint8_t matrix_scan(void)
{
  // ROW2COL style, assuming debouncing
  for (uint8_t current_col = 0; current_col < MATRIX_COL; current_col++) {
    bool matrix_changed = read_rows_on_col(matrix_debouncing, current_col);
    if (matrix_changed) {
      debouncing = true;
      debouncing_time = timer_read();
    }
  }

  if (debouncing && (timer_elapsed(debouncing_time) > DEBOUNCING_DELAY)) {
    for (uint8_t i = 0; i< MATRIX_ROWS; i++) {
      matrix[i] = matrix_debouncing[i;
    }
    debouncing = false;
  }

  matrix_scan_quantum();

  return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
  return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
  return matrix[row];
}

void matrix_print(void)
{
  print("\nr/c 0123456789ABCDEF\n");
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    phex(row); print(": ");
    pbin_reverse16(matrix_get_row(row));
    print("\n");
  }
}

uint8_t matrix_key_count(void)
{
  uint8_t count = 0;
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    count += bitpop16(matrix[i]);
  }
  return count;
}

/* Column pin configuration
 *
 * Adafruit BLE: 8    7    6    5    4    3    2    1    0
 *               PF7  PF6  PF5  PF4  PF1  PF0  PC7  PD6  PB7
 */
// static void  init_cols(void)
// {
//   // Adafruit BLE
//   DDRF  &= ~(1<<PF0 | 1<<PF1 | 1<<PF4 | 1<<PF5 | 1<<PF6 | 1<<PF7);
//   PORTF |=  (1<<PF0 | 1<<PF1 | 1<<PF4 | 1<<PF5 | 1<<PF6 | 1<<PF7);
//   DDRC  &= ~(1<<PC7);
//   PORTC |=  (1<<PC7);
//   DDRD  &= ~(1<<PD6);
//   PORTD |=  (1<<PD6);
//   DDRB  &= ~(1<<PB7);
//   PORTB |=  (1<<PB7);
// }
//
// static matrix_row_t read_cols(uint8_t row)
// {
//   return (PINF&(1<<PF7) ? 0 : (1<<8)) |
//     (PINF&(1<<PF6) ? 0 : (1<<7)) |
//     (PINF&(1<<PF5) ? 0 : (1<<6)) |
//     (PINF&(1<<PF4) ? 0 : (1<<5)) |
//     (PINF&(1<<PF1) ? 0 : (1<<4)) |
//     (PINF&(1<<PF0) ? 0 : (1<<3)) |
//     (PINC&(1<<PC7) ? 0 : (1<<2)) |
//     (PIND&(1<<PD6) ? 0 : (1<<1)) |
//     (PINB&(1<<PB7) ? 0 : (1<<0)) ;
// }

/* Row pin configuration
 *
 * Expander:  GP0   GP1   GP2   GP3   GP4   GP5  GP6  GP7
 */
static void init_rows(void)
{
  // Only need to do once to init the expander.
  i2c_init();

  // Set the GPIO to the same logic as input pin
  uint8_t ret = i2c_write_data(EXPANDER_REG_IPOL, 0x00);
  // Set the rows IO direction to In
  ret = i2c_write_data(EXPANDER_REG_IODIR, 0xFF);
  // Set the pull up resistors
  ret = i2c_write_data(EXPANDER_REG_GPPU, 0xFF);


}

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col)
{
    bool matrix_changed = false;

    // Select col and wait for col selecton to stabilize
    select_col(current_col);
    wait_us(30);

    // For each row...
    for(uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++)
    {

        // Store last value of row prior to reading
        matrix_row_t last_row_value = current_matrix[row_index];

        // Check row pin state
        if ((_SFR_IO8(row_pins[row_index] >> 4) & _BV(row_pins[row_index] & 0xF)) == 0)
        {
            // Pin LO, set col bit
            current_matrix[row_index] |= (ROW_SHIFTER << current_col);
        }
        else
        {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~(ROW_SHIFTER << current_col);
        }

        // Determine if the matrix changed state
        if ((last_row_value != current_matrix[row_index]) && !(matrix_changed))
        {
            matrix_changed = true;
        }
    }

    // Unselect col
    unselect_col(current_col);

    return matrix_changed;
}

static void unselect_rows(void)
{
  expander_unselect_rows();
}

static void select_row(uint8_t row)
{
  expander_select_row(row);
}

uint8_t i2c_write_data( uint8_t reg, uint8_t data)
{
    uint8_t ret = i2c_start(EXPANDER_ADDR | I2I2C_WRITE)
    if ret goto halt_i2c;
    ret = i2c_write(reg)
    if ret goto halt_i2c;
    ret = i2c_write(data)
    if ret goto halt_i2c;
    return 0;
  halt_i2c:
    i2c_stop();
    return ret;
}
