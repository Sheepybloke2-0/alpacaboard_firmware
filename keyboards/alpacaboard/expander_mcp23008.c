/* Modified "expander.c" from the ErgoDone keyboard to work with the MCP23008
 * GPIO expander. Used by "matrix.c" to communicate with the keyboard's rows.
 */

#include <stdbool.h>
#include "action.h"
#include "i2cmaster.h"
#include "expander_mcp23008.h"
#include "debug.h"

static uint8_t expander_status = 0;

void expander_config(void);
uint8_t expander_write(uint8_t reg, uint8_t data);
uint8_t expander_read(uint8_t reg, uint8_t *data);

void expander_init(void)
{
  i2c_init();
  expander_scan();
}

void expander_scan(void)
{
  dprintf("expander status: %d ... ", expander_status);
  uint8_t ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);
  if (ret == 0) {
    i2c_stop();
    if (expander_status == 0) {
      dprintf("attached\n");
      expander_status = 1;
      expander_config();
      clear_keyboard();
    }
  }
  else {
    if (expander_status == 1) {
      dprintf("detached\n");
      expander_status = 0;
      clear_keyboard();
    }
  }
  dprintf("%d\n", expander_status);
}

void expander_unselect_rows(void)
{
  expander_write(EXPANDER_REG_IODIR, 0xFF);
}

void expander_select_row(uint8_t row)
{
  expander_write(EXPANDER_REG_IODIR, ~(1<<(row+1)));
}

void expander_config(void)
{
  expander_write(EXPANDER_REG_IPOL, 0xFF);
  expander_write(EXPANDER_REG_GPPU, 0xFF);
  expander_write(EXPANDER_REG_IODIR, 0xFF);
}

uint8_t expander_write(uint8_t reg, uint8_t data)
{
  if (expander_status == 0) {
    return 0;
  }
  uint8_t ret;
  ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);
  if (ret) goto stop;
  ret = i2c_write(reg);
  if (ret) goto stop;
  ret = i2c_write(data);
 stop:
  i2c_stop();
  return ret;
}

uint8_t expander_read(uint8_t reg, uint8_t *data)
{
  if (expander_status == 0) {
    return 0;
  }
  uint8_t ret;
  ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);
  if (ret) goto stop;
  ret = i2c_write(reg);
  if (ret) goto stop;
  ret = i2c_rep_start(EXPANDER_ADDR | I2C_READ);
  if (ret) goto stop;
  *data = i2c_readNak();
 stop:
  i2c_stop();
  return ret;
}
