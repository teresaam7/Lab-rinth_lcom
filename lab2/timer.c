#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
  //uint32_t init_val = TIMER_FREQ / freq;
  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer < 0 || timer > 2) 
    return 1;

  // Write read-back command to read input timer configuration
  uint8_t rb_com = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)); // Read Back Command

  // Write the read-back command to the control register
  if (sys_outb(TIMER_CTRL, rb_com) != 0)
    return 1;
 
  // Read the timer port to obtain the configuration
  int timer_port = TIMER_0 + timer;
  if (util_sys_inb(timer_port,st) != 0)
    return 1;
    
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val tsf_val;

  switch (field) {
    case tsf_all:
      tsf_val.byte = st;
      break;

    /*
    case tsf_initial:
      uint8_t init = (st >> 4) & 0x03;  //11
      if (init == 1) tsf_val.in_mode = LSB_only;
      else if (init == 2) tsf_val.in_mode = MSB_only;
      else if (init == 3) tsf_val.in_mode = MSB_after_LSB;
      else tsf_val.in_mode = INVAL_val;
      break;

    case tsf_mode:
      uint8_t mode = (st >> 1) & 0x07;  //111
      if (mode == 6) tsf_val.count_mode = 2;
      else if (mode == 7) tsf_val.count_mode = 3;
      else tsf_val.count_mode = mode;
      break; 
    */

    case tsf_base:
      tsf_val.bcd = st & TIMER_BCD;   //bit 0 -> 0x01
      break;

    default:
      return 1;
  }
  
  if (timer_print_config(timer, field, tsf_val) != 0) 
    return 1;

  return 0;
}
