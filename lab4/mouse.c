#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#include "i8042.h"


int hook_id_mouse = 3;
uint8_t bytes_mouse[3];
uint8_t byte_mouse;

uint8_t index_byte;
struct packet packet_mouse;

/*
int (mouse_command)(uint8_t data_mode) {
    
    if (sys_outb(KBC_CMD, WRITE_BYTE_MOUSE) != 0)
        return 1;

    return 1;
}


int (kbc_command)(uint8_t data_mode) {
    
    if (sys_outb(KBC_CMD, WRITE_BYTE_MOUSE) != 0)
        return 1;

    return 1;
}
*/


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


int (read_kbc)(uint8_t* byte, uint8_t status_aux) {
    uint8_t status;
    int attemps_count = 10;

    while (attemps_count != 0) {
        if (util_sys_inb(STATUS_REG, &status) != 0) {
            printf("Error reading status from KBC\n");
            return 1;
        }

        if (status & OBF) {
            if (util_sys_inb(OUT_BUF, &byte_mouse) != 0){    // Always do this, even with errors
                printf("Error reading output buffer from KBC\n");
                return 1;
            }

            if (status & PARITY) {
                printf("Parity error\n");
                return 1;
            }
            if (status & TIMEOUT) {
                printf("Timeout error\n");
                return 1;
            }
            if ((status & AUX) & !status_aux) {
                printf("Not keyboard data: error\n");
                return 1;
            }
            if (!(status & AUX) & status_aux) {
                printf("Not mouse data: error\n");
                return 1;
            }

            return 0; 
        }

        tickdelay(micros_to_ticks(2000));
        attemps_count--;
    }

    return 1;
}


void (store_byte_mouse)() {
    if ((index_byte == 0 && (byte_mouse & BIT(3)))     
            || (index_byte > 0 && index_byte < 3)) { // Bit 3 of byte 1 is always 1
        bytes_mouse[index_byte] = byte_mouse;
        index_byte++;
    }
}


void (mouse_ih)() {
    if (read_kbc(&byte_mouse, 1) != 0)
        printf("Error reading a byte from mouse\n");
    else
        store_byte_mouse();
}


void (store_bytes_packet)() {
    for (int i = 0; i < 3; i++)
        packet_mouse.bytes[i] = bytes_mouse[i];
    
    packet_mouse.lb = bytes_mouse[0] & LB;
    packet_mouse.rb = bytes_mouse[0] & RB;
    packet_mouse.mb = bytes_mouse[0] & MB;

    packet_mouse.x_ov = bytes_mouse[0] & X_OVFL;
    packet_mouse.y_ov = bytes_mouse[0] & Y_OVFL;

    packet_mouse.delta_x = (bytes_mouse[0] & MSB_X_DELTA) ? (bytes_mouse[1] | 0xFF00) : bytes_mouse[1]; 
    packet_mouse.delta_y = (bytes_mouse[0] & MSB_Y_DELTA) ? (bytes_mouse[2] | 0xFF00) : bytes_mouse[2];
}
