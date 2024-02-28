#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

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
  *bit_no=hookid;
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
  // Write read-back command to read input timer configuration
  uint8_t readbackcommand= (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  // Write the read-back command to the control register
  if (sys_outb(TIMER_CTRL, readbackcommand) != 0) {
    return 1; 
  }

  // Read the timer port to obtain the configuration
  int timer_port= TIMER_0 + timer;
  if (util_sys_inb(timer_port, st) != 0) {
      return 1; 
  }

  return 0; 
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
