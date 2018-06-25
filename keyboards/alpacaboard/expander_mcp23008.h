#ifndef EXPANDER_H
#define EXPANDER_H

#include <stdint.h>
#include "matrix.h"

#define MCP23008
/* These addresses are defined via the hardware. For the alpacaboard, all are set to 0 */
#define MCP23008_A0 0
#define MCP23008_A1 0
#define MCP23008_A2 0

#ifdef MCP23008
/* Expander Addr: 0b0-1-0-0-A2-A1-A0-r/w */
#define EXPANDER_ADDR ((0x4|(MCP23008_A0<<0)|(MCP23008_A1<<1)|(MCP23008_A2<<2)) << 1)
enum EXPANDER_REG_BANK {
  EXPANDER_REG_IODIR = 0,
  EXPANDER_REG_IPOL,
  EXPANDER_REG_GPINTEN,
  EXPANDER_REG_DEFVAL,
  EXPANDER_REG_INTCON,
  EXPANDER_REG_IOCON,
  EXPANDER_REG_GPPU,
  EXPANDER_REG_INTF,
  EXPANDER_REG_INTCAP,
  EXPANDER_REG_GPIO,
  EXPANDER_REG_OLAT,
};
#endif

void expander_init(void);
void expander_scan(void);
/* AlpacaBoard uses the MCP23008 to handle the rows. TODO: can this be made generic? */
matrix_row_t expander_read_row(void);
void expander_unselect_rows(void);
void expander_select_row(uint8_t row);

#endif
