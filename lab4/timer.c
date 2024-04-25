#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hookidd=0;
int count=0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  if(bit_no==NULL){
    return 1;
  }
  *bit_no=BIT(hookidd);
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookidd)!=0){
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hookidd)!=0){
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  count++;
}

