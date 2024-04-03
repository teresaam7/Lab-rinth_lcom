#include "keyboard.h"

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



