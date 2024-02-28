#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /*
  uint8_t ctrl_word;   // Control Word - configuration command
  if (util_sys_inb(timer, &ctrl_word) != 0) 
    return 1;

  uint32_t init_val = TIMER_FREQ / freq;


  if (sys_outb(TIMER_CTRL, ctrl_word) != 0) 
    return 1;
  
  return 0;
  */
  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id);
  
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) 
    return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0)
    return 1;
  
  return 0;
}

void (timer_int_handler)() {
  counter++;
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
  uint8_t mode;   // Não é possível criar variáveis dentro do switch

  switch (field) {
    case tsf_all:
      tsf_val.byte = st;
      break;

    case tsf_initial:
      mode = (st >> 4) & 0x03;  //11
      if (mode == 1) tsf_val.in_mode = LSB_only;
      else if (mode == 2) tsf_val.in_mode = MSB_only;
      else if (mode == 3) tsf_val.in_mode = MSB_after_LSB;
      else tsf_val.in_mode = INVAL_val;
      break;

    case tsf_mode:
      mode = (st >> 1) & 0x07;  //111
      if (mode == 6) tsf_val.count_mode = 2;
      else if (mode == 7) tsf_val.count_mode = 3;
      else tsf_val.count_mode = mode;
      break; 
    
    case tsf_base:
      tsf_val.bcd = st & 0x01;   //bit 0
      break;

    default:
      return 1;
  }
  
  if (timer_print_config(timer, field, tsf_val) != 0) 
    return 1;

  return 0;
}
