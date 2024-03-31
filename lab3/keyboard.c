#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "i8042.h"

int hookid=0;
uint8_t scancode;

int (KBD_subscribe_int)(uint8_t *bit_no) {
  if(bit_no==NULL){
    return 1;
  }
  *bit_no=BIT(hookid);
  if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hookid)!=0){
    return 1;
  }
  return 0;
}

int (KBD_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hookid)!=0){
    return 1;
  }
  return 0;
}


//read the status register and check if there was some communications error
//to read the scancode byte from the output buffer
void (kbc_ih)(){
  uint8_t status;
  int i=0;
  while(i<10){ //limita o número de tentativas de leitura do scancode
    if(util_sys_inb(STATUS_REG, &status)!=0){ //status=STATUS_REG
      printf("Status error\n");
      return;
    }
    if((status&OBF)!=0){ //If output buffer has a scancode
        if(util_sys_inb(OUT_BUF,&scancode)!=0) { //scancode=OUT_BUF
            printf("Read error\n");
            return;
        }
        if((status&TIMEOUT)!=0){
            printf("Timeout error\n");
            return;
        }
        if((status&PARITY)!=0){
            printf("Parity error\n");
            return;
        }
        if((status&AUX)!=0){
            printf("Mouse error\n");
            return;
        }
      return;//read scancode
    }
    i++;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  printf("Read scancode error\n");
  return;
}
