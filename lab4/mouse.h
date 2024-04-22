#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_
#include <lcom/lcf.h>
#include "i8042.h"

int (write_KBC_command)(uint8_t port, uint8_t commandByte);
int (mouse_write)(uint8_t command);
void (mouse_ih)();
int (read_scancode)(uint8_t port, uint8_t* code, int aux);
void (mouse_sync_bytes)();
void (mouse_build_packet)();

#endif
