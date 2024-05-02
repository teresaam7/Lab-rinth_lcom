#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"


int (keyboard_subscribe_int)(uint8_t *bit_no);
int (keyboard_unsubscribe_int)();
int (keyboard_read_scancode)(uint8_t port , uint8_t *scancode,uint8_t mouse);
int (keyboard_write_scancode)(uint8_t port , uint8_t command);
void (kbc_ih)();
int (keyboard_enable_interrupts)();
#endif
