#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id_timer = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > TIMER_FREQ) 
    return 1;

  uint8_t ctrl_word;   // Control Word - configuration command
  if (timer_get_conf(timer, &ctrl_word) != 0) 
    return 1;

  ctrl_word = ctrl_word & 0x0F;   // Keep the 4 LS bits (mode and base)
  ctrl_word = ctrl_word | TIMER_LSB_MSB;    // Set initialization mode

  uint8_t addr_timer;
  switch (timer) {  // Set the timer on the Control Word and store the adress
    case 0:
      addr_timer = TIMER_0;
      break;

    case 1:
      ctrl_word = ctrl_word | TIMER_SEL1;
      addr_timer = TIMER_1;
      break;

    case 2:
      ctrl_word = ctrl_word | TIMER_SEL2;
      addr_timer = TIMER_2;
      break;

    default: 
      return 1;
  }

  // Set the control word to the control register -> set the value later
  if (sys_outb(TIMER_CTRL, ctrl_word) != 0) 
    return 1;

  uint16_t init_val = TIMER_FREQ / freq;  // Initial value for the timer
  uint8_t LSB_val; util_get_LSB(init_val, &LSB_val);
  uint8_t MSB_val; util_get_MSB(init_val, &MSB_val);
  
  // Load the initial value to the timer register - each byte at a time (LSB -> MSB)
  if (sys_outb(addr_timer, LSB_val) != 0)
    return 1;
  if (sys_outb(addr_timer, MSB_val) != 0)
    return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id_timer);

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) != 0) 
    return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_timer) != 0)
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
