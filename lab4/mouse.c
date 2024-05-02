#include <lcom/lcf.h>
#include "i8042.h"
#include "i8254.h"
#include <stdint.h>
#include <stdio.h>

int mouse_hook_id = 3;

int mouse_subscribe_int(uint8_t *bit_no){
    if(!bit_no) return 1;
    *bit_no = BIT(mouse_hook_id);
    if(sys_irqsetpolicy(IRQ_MOUSE,IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)!=0) return 1;
    return 0;
}

int mouse_unsubscribe_int(uint8_t *bit_no){
    if(!bit_no) return 1;
    if(sys_irqrmpolicy(&mouse_hook_id)!= 0) return 1;
    return 0;
}





