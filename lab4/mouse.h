#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"


void (mouse_ih)(); 	// Interrupt handler for the mouse controller

int (mouse_subscribe_int)(uint8_t *bit_no) ;

int (mouse_unsubscribe_int)();

int (mouse_write)(uint8_t command);

void (packet_contruction)() ; //Constructs a packet from raw data

void (mouse_init)();

#endif

