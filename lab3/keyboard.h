#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"

int (read_scancode)(uint8_t port, uint8_t *out, uint8_t mouse);

void (kbc_ih)() ; 	// Interrupt handler for the keyboard controller

int (keyboard_subscribe_int)(uint8_t *bit_no) ;

int (keyboard_unsubscribe_int)();

int (kbc_extract_scancode)();

void (kbc_verify_scancode)();

int (write_scancode)(uint8_t port, uint8_t command);

int (restore_interrupts)();


#endif
