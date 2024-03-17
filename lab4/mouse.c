#include "mouse.h"

int hookIdMouse = 3;
uint8_t current; 

void (mouse_ih)(){
  if (read_scancode(W_CMD, &current, 1) != 0) 
        printf("Read scancode error\n");
}

int (mouse_subscribe_int)(uint8_t *bit_no){
    if (!bit_no) {
    return 1;
  } 
  *bit_no = BIT(hookIdMouse);
  if (sys_irqsetpolicy(MOUSE,IRQ_REENABLE | IRQ_EXCLUSIVE, &hookIdMouse) != 0) {
    return 1;
  }

  return 0;
}

int (mouse_unsubscribe_int)(){
  return sys_irqrmpolicy(&hookIdMouse);
}

int (mouse_write)(uint8_t command) {
    int i = 10;
    uint8_t status;
    while (i) {
        if (write_scancode(W_CMD, &command, 0) != 0) {
            return 1;
        } 
        if (write_scancode(IN_CMD, M_WRITE_BYTE) != 0) {
            return 1;
        } 
        tickdelay(micros_to_ticks(20000)); 
        if (util_sys_inb(W_CMD, status) != 0) {
            return 1;
        }
        if (status == ACK) {
            return 0;
        }  
        i--;
    }

    return 1;
}