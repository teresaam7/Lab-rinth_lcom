#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"

int (read_scancode)(uint8_t port, uint8_t *out, uint8_t mouse);

int (write_scancode)(uint8_t port, uint8_t command);

#endif
