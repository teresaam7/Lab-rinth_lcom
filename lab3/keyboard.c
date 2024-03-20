#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8042.h"

int hook_id=1;
uint8_t scancode = 0;
int (read_check_status)(){
    return 1;
}


int (read_scancode)(uint8_t port, uint8_t *out, uint8_t mouse) {
    uint8_t status;
    int  i = 10;
    while (i > 0) {
        if (util_sys_inb(STATUS_REG, &status) != 0) {
        printf("Status error\n");
        return 1;
        }

        if ((OBF & status) != 0 ){
            if (util_sys_inb(port, out) != 0) {
                printf("Read output error\n");
                return 1;
            }

            if ((status & TIMEOUT) != 0){
                printf("Timeout error\n");
                return 1;
            }

            if ((status & PARITY) != 0){
                printf("Parity error\n");
                return 1;
            }

            if ( ((status & AUX) & !mouse) || (!(status & AUX) & mouse)){
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

void (kbc_ih)() {
    if (read_scancode(OUT_BUF, &scancode, 0) != 0){ 
        printf("Read scancode error\n");
    }
}