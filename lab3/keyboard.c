#include "keyboard.h"

uint8_t scancode;
uint32_t count_sysinb;
int hook_id = 1;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id);
  
  int irq_line = 1;
  if (sys_irqsetpolicy(irq_line, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0)
    return 1;

  return 0;
}

int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0)
    return 1;
  
  return 0;
}


void (kbc_ih)() {
    if (read_scancode(STATUS_REG, &scancode) != 0)
        printf("Read scancode error\n");   
}

int (read_scancode)(uint8_t port, uint8_t* code) {
    int  i = 0;
    uint8_t status;
    while (i < 10) {
        if (util_sys_inb(port, &status) != 0) {
            printf("Status error\n");
            return 1;
        }
        if (status & OBF)  {    // If the OutBuffer has a scancode (is full)
            if (util_sys_inb(port, code) != 0) {
                printf("Read error\n");
                return 1;
            }

            if ((status & TIMEOUT) != 0){
                printf("Timeout error\n");
                return 1;
            }
            if ((status & PARITY) != 0){
                printf("Parity error\n");
                return 1;
            }
            if ((status & AUX) != 0){
                printf("Mouse error\n");
                return 1;
            }
          return 0;   // Sucessfull read!
        }
        tickdelay(micros_to_ticks(1000));
        i++;
    }
    return 1;   // Unsucessfull
}
