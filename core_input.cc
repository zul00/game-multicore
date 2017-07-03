#include <helix.h>

#include "game_param.h"

#include "core_input.h"

char const *btn_string[] =
{
  "ENTER",
  "RIGHT",
  "DOWN",
  "LEFT",
  "UP",
  "NO_EVENT"
};

/**
 * @brief Process to handle input
 *
 */
void *core_input(void *arg)
{
  int16_t bt_state = 0;
  uint8_t idx = 0;

  printf("Hello Poll!!!\n");

  // Check FIFO
  wr_btn->validate();
  while(1)
  {
    // Get buttons state
    bt_state = buttons_state();

    // Scan button state
    for (idx=0; idx<5; idx++)
    {
      if (bt_state & (1 << idx))
      { // Push index of active button
        wr_btn->push((btn_event_e)idx);
      }
    }

    usleep(BTN_SCAN_PERIOD);
  }

  return NULL;
}
