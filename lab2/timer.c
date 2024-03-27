#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
//#include <lcf.h>
//#include <minix/syslib.h>
//#include <syslib.h>

int hookid=0;
int counter=0;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if(bit_no==NULL){
    return 1;
  }
  *bit_no=BIT(hookid);
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookid)!=0){
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hookid)!=0){
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0){
     return 1;
  }
  uint8_t readbackcommand= (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  if (sys_outb(TIMER_CTRL, readbackcommand) != 0) { 
    return 1;
  }

  int timer_port= TIMER_0 + timer;
  if (util_sys_inb(timer_port, st) != 0) {
      return 1; 
  }

  return 0; 
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val status; 
  uint8_t mode; //nao pode ser criado dentro do switch
  switch(field){
    case tsf_all:
      status.byte=st;
    break;
    case tsf_initial:
      mode=(st>>4) & 0x03;
      if(mode==1){ 
        status.in_mode=LSB_only;}
      else if(mode==2){ 
        status.in_mode=MSB_only;}
      else if(mode==3){ 
        status.in_mode=MSB_after_LSB;}
      else{ 
        status.in_mode=INVAL_val;}
    break;
    case tsf_mode:
      mode=(st>>1) & 0x07;
      if(mode==6){ status.count_mode=2;} //mode 2=X10    010 e 110
      else if(mode==7){ status.count_mode=3;} //mode 3=X11   111 e 011
      else status.count_mode=mode;
    break;
    case tsf_base:
       status.bcd=st&TIMER_BCD; //true se é BCD 
    break;
    default:
       return 1;
  }
  if (timer_print_config(timer, field, status) != 0){
     return 1;
  }
  return 0;
}
