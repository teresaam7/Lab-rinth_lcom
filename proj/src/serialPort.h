#ifndef __SERIALPORT_H_
#define __SERIALPORT_H_
#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include "SerialPorts.h"
#include "queue.h"
#include "Game.h"
#include "i8042.h"

int (sp_subscribe_int)(uint8_t* bitno);

int (sp_unsubscribe_int)();

void (initialize_sp)();

int (send_byte)(uint8_t byte);

int (receive_byte)();

Queue* (get_queue)();

void (sp_out)();

int (cleanInt_sp)();

void (sp_ih)();

#endif
