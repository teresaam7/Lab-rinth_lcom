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

bool (sp_enable_int)();

bool (sp_disable_int)();

void (sp_config)();

void (initialize_sp)();

int (send_byte)(uint8_t byte);

int (send_queue_bytes)();

int (receive_byte)();

Queue* (get_receive_queue)();

Queue* (get_send_queue)();

void (sp_out)();

int (cleanInt_sp)();

void (sp_ih)();

void (send_scan)(uint8_t scancode);

void (manage_button)(uint8_t scancode, bool isPlayer1);

bool (handle_start_multi)();

void (handle_receive_info)();

void (sp_handler)();


#endif
