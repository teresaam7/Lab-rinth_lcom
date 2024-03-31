#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_
#include <lcom/lcf.h>
#include "i8042.h"

int (KBD_subscribe_int)(uint8_t *bit_no);

int (KBD_unsubscribe_int)();

void (kbc_ih)();

#endif
