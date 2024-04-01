#include "keyboard.h"

uint8_t scancode;
uint32_t count_sysinb;
int hook_id_keyboard = 1;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id_keyboard);
  
  int irq_line = 1;
  if (sys_irqsetpolicy(irq_line, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard) != 0)
    return 1;

  return 0;
}

int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_keyboard) != 0)
    return 1;
  
  return 0;
}


void (kbc_ih)() {
    if (read_kbc(STATUS_REG, &scancode) != 0)
        printf("Read scancode error\n");   
}

int (read_kbc)(uint8_t port, uint8_t* code) {
    int count_attemps = 0;
    uint8_t status;
    while (count_attemps < 10) {
        if (util_sys_inb(port, &status) != 0) {
            printf("Status error\n");
            return 1;
        }
        if (status & OBF)  {    // If the OutBuffer has a scancode (is full)
            if (util_sys_inb(OUT_BUF, code) != 0) {
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
        tickdelay(micros_to_ticks(10000));
        count_attemps++;
    }
    return 1;   // Unsucessfull
}


int (write_comm)(uint8_t port, uint8_t comm) {
  uint8_t status;
  int count_attemps = 0;

  while (count_attemps < 10) {
    if (util_sys_inb(STATUS_REG, &status) != 0)
      return 1;

    if (!(status & IBF)) {
      if (sys_outb(port, comm) != 0)
        return 1;
      
      return 0;
    }

    tickdelay(micros_to_ticks(10000));
    count_attemps++;
  }

  return 1;
}


int (restore_int)() {
  if (write_comm(KBC_CMD, READ_COMM_BYTE) != 0)
    return 1;

  uint8_t command_byte;
  if (read_kbc(OUT_BUF, &command_byte) != 0)
    return 1;

  command_byte = command_byte & ENABLE_INT_KEYBOARD;

  if (write_comm(KBC_CMD, WRITE_COMM_BYTE) != 0)
    return 1;

  if (write_comm(IN_BUF, command_byte) != 0)
    return 1;

  return 0;
}
