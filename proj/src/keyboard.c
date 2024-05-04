#include "keyboard.h"

uint8_t k_scancode;
int hook_id_keyboard = 1;


int (keyboard_subscribe_int)(uint8_t *irq_set) {
  if (irq_set == NULL)
    return 1;

  *irq_set = BIT(hook_id_keyboard);
  
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard) != 0)
    return 1;

  return 0;
}


int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_keyboard) != 0)
    return 1;
  
  return 0;
}


void (kbc_ih)() {
    if (read_kbc(OUT_BUF, &scancode, 0) != 0) 
        printf("Read scancode error\n");
}


int (restore_interrupts)() {
    if (write_kbc(KBC_CMD, READ_COMM_BYTE) != 0)
        return 1;

    uint8_t command_byte;
    if (read_kbc(OUT_BUF, &command_byte) != 0)
        return 1;

    command_byte = command_byte | ENABLE_INT_KEYBOARD;

    if (write_kbc(KBC_CMD, WRITE_COMM_BYTE) != 0)
        return 1;

    if (write_kbc(IN_BUF, command_byte) != 0)
        return 1;

    return 0;
}
