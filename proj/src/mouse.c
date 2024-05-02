#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#include "KBC.h"
#include "i8042.h"


int hook_id_mouse = 3;

int index = 0;
uint8_t byte_mouse;
uint8_t bytes_mouse[3];
struct packet packet_mouse;


int (write_mouse) (uint8_t command) {
    int attemps_count = 10;
    uint8_t received_byte = NACK;

    while (attemps_count && (received_byte != ACK)) {
        if (write_kbc(KBC_CMD, WRITE_BYTE_MOUSE) != 0)
            return 1;
        if (write_kbc(IN_BUF, command) != 0)
            return 1;

        tickdelay(micros_to_ticks(20000));

        if (util_sys_inb(OUT_BUF, &received_byte) != 0)
            return 1;
            
        attemps_count--;
    }

    if (received_byte == ACK)
        return 0;

    return 1;
}


int (mouse_subscribe_int)(uint8_t* irq_set) {
    if (irq_set == NULL)
        return 1;

    *irq_set = BIT(hook_id_mouse);

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != 0)
        return 1;

    return 0;
}


int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id_mouse) != 0)
        return 1;
    return 0;
}


void (store_byte_mouse)() {
    if ((index == 0 && (byte_mouse & M_BYTE_1))     
            || (index > 0 && index < 3)) {        // Bit 3 of byte 1 is always 1
        bytes_mouse[index] = byte_mouse;
        index++;
    }
}


void (mouse_ih)() {
    if (read_kbc(OUT_BUF, &byte_mouse, 1) != 0)
        printf("Error reading a byte from mouse\n");
    else
        store_byte_mouse();
}


void (store_bytes_packet)() {
    for (int i = 0; i < 3; i++)
        packet_mouse.bytes[i] = bytes_mouse[i];
    
    packet_mouse.lb = bytes_mouse[0] & M_LB;
    packet_mouse.rb = bytes_mouse[0] & M_RB;
    packet_mouse.mb = bytes_mouse[0] & M_MB;

    packet_mouse.x_ov = bytes_mouse[0] & M_X_OVFL;
    packet_mouse.y_ov = bytes_mouse[0] & M_Y_OVFL;

    if (bytes_mouse[0] & M_MSB_X_DELTA) packet_mouse.delta_x = bytes_mouse[1] | 0xFF00;
    else packet_mouse.delta_x = bytes_mouse[1];

    if (bytes_mouse[0] & M_MSB_Y_DELTA) packet_mouse.delta_y = bytes_mouse[2] | 0xFF00;
    else packet_mouse.delta_y = bytes_mouse[2];
}
