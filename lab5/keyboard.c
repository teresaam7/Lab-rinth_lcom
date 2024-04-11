#include "keyboard.h"

uint8_t scancode = 0;
int kHookId = 1;
bool make = true;
uint8_t bytes[2];
uint8_t size = 1;
bool full_scancode = true;

int (read_scancode)(uint8_t port, uint8_t *out, uint8_t mouse) {
    uint8_t status;
    int  i = 10;
    while (i > 0) {
        if (util_sys_inb(STAT_REG, &status) != 0) {
        printf("Status error\n");
        return 1;
        }

        if ((OBF & status) != 0 ){
            if (util_sys_inb(port, out) != 0) {
                printf("Read output error\n");
                return 1;
            }

            if (status & TIMEOUT){
                printf("Timeout error\n");
                return 1;
            }

            if (status & PARITY){
                printf("Parity error\n");
                return 1;
            }

            if ( (status & AUX) && !mouse){
                printf("Mouse error\n");
                return 1;
            }

            if (!(status & AUX) && mouse) {
                printf("Mouse error\n");
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        i--;
    }
    return 1;
}

int (write_scancode)(uint8_t port, uint8_t command) {
    uint8_t status;
    uint8_t i = 10;

    while (i) {
        if (util_sys_inb(STAT_REG, &status) != 0){
            printf("Error: Status not available!\n");
            return 1;
        }

        if ((status & IBF) == 0){
            if(sys_outb(port, command) != 0){
                printf("Error: Could not write commandByte!\n");
                return 1;
            }

            return 0;
        }

        tickdelay(micros_to_ticks(20000));
        i--;
    }
    return 1;
}


void (kbc_ih)() {
    if (read_scancode(OUT_BUF, &scancode, 0) != 0) 
        printf("Read scancode error\n");
}

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (!bit_no) {
    return 1;
  } 
  *bit_no = BIT(kHookId);
   if (sys_irqsetpolicy( KEYBOARD,IRQ_REENABLE | IRQ_EXCLUSIVE, &kHookId) != 0)  {
    return 1;
  }

  return 0;
}

int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy( &kHookId) != 0) {
    return 1;
  }

  return 0;
}

void (kbc_verify_scancode)() {
    if (scancode == SCAN_1OF2) {
        bytes[0] = scancode;
        full_scancode = false;
    }
    else {
        if (scancode & MAKEORBREAK) {
            make = false;
        }
        else {
            make = true;
        }
        if (!(full_scancode)) {
            size = 2;
            bytes[1] = scancode;
            full_scancode = true;
        }
        else {
            size = 1;
            bytes[0] = scancode;
        }
    }
}

int (restore_interrupts)() {
    uint8_t command;

    if (write_scancode(IN_CMD, R_CMD) != 0) {  // warns i8042 that is about to read
        return 1;
    } 

    if (read_scancode(OUT_BUF, &command, 0) != 0) {  // reads configuration
        return 1;
    }         

    command |= BIT(0);   // activation of interruption bit

    if (write_scancode(IN_CMD, W_CMD) != 0) {  // warns i8042 that is about to write
        return 1;
    }   
    if (write_scancode(W_CMD, command) != 0) { // writes configuration
        return 1;
    }
    return 0;
}

