#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer > 2 || timer < 0 || freq > TIMER_FREQ || freq < 19) return 1;
  uint8_t st;uint8_t rbcom ;

  if(timer_get_conf(timer, &st) != 0) return 1;

  rbcom = ((st & 0x0F)|TIMER_LSB_MSB);
  uint32_t value = TIMER_FREQ / freq;

  if(sys_outb(TIMER_CTRL, rbcom)!= 0) return 1;
  
  if(timer == 1){rbcom |= TIMER_SEL1;}
  else if(timer == 2){rbcom |= TIMER_SEL2;}
  else rbcom |= TIMER_SEL0;

  if(sys_outb(TIMER_CTRL, rbcom) != 0) return 1;

  uint8_t lsb, msb;
  if(util_get_LSB(value, &lsb)!= 0 || util_get_MSB(value, &msb)!= 0) return 1;

  if(sys_outb(TIMER_0 + timer,lsb ) != 0) return 1;
    if(sys_outb(TIMER_0 + timer,msb ) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if(!bit_no) return 1;
  *bit_no = hook_id;
  if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,& hook_id)!= 0) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(& hook_id)!= 0) return 1;
  return 1;
}

void (timer_int_handler)() {
  counter ++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(!st || timer > 2 || timer < 0) return 1;
  uint8_t rbcom;
  rbcom = (BIT(7) | BIT(6) | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  if(sys_outb(TIMER_CTRL,rbcom) != 0) return 1;

  uint8_t timer_port = TIMER_0 + timer;
  
  if(util_sys_inb(timer_port,st)!= 0) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val conf;

  switch(field){

    case tsf_all:
    conf.byte = st;
    break;
    case tsf_initial:
     st = (st >> 4) & 0x03;
    if(st == 1) conf.in_mode = LSB_only;
    else if(st == 2) conf.in_mode = MSB_only;
    else if(st == 3) conf.in_mode = MSB_after_LSB;
    else conf.in_mode = INVAL_val;
    break;
    case tsf_mode:
    st = (st>>1) & 0x07;
    conf.count_mode = st;
    break;
    case tsf_base:
    conf.bcd = (BIT(0) & st);
    break;
  }
  if(timer_print_config(timer,field,conf)!= 0)return 1;

  return 0;
}
