#include "keyboard.h"

int hookId_kb = 0;
uint8_t scancode = 0;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if(bit_no == NULL){return 1;}
    *bit_no = BIT(hookId_kb);

    if (sys_irqsetpolicy(KEYBOARD,IRQ_REENABLE|IRQ_EXCLUSIVE, & hookId_kb) != 0) { 
        return 1; 
    }
    return 0; 

}

int(keyboard_unsubscribe_int)(){
    if(sys_irqrmpolicy(&hookId_kb)!= 0) return 1;
    return 0;
}

int (keyboard_read_scancode)(uint8_t port, uint8_t *command,uint8_t mouse){
    if(!command) return 1;
    int n = 10;
    while(n>0){
        uint8_t st;
        if(util_sys_inb(STAT_REG, & st)!= 0)return 1; 
        if((OBF & st)!= 0){
            if(util_sys_inb(port, command)!= 0) return 1;
            if((st & TIMEOUT)!= 0) return 1;
            if((st & PARITY)!= 0) return 1;
                        if ( ((st & AUX) & !mouse) || (!(st & AUX) & mouse)) return 1;
        }
        return 0;
        tickdelay(micros_to_ticks(20000)); n--;
    }
    return 1;
}


void (kbc_ih)(){
    if (keyboard_read_scancode(OUT_BUF, &scancode,0)!=0) 
    printf("communication error");
}

int (keyboard_write_scancode)(uint8_t port, uint8_t command){
    int n = 10;
    uint8_t st;
    while(n){
        if(util_sys_inb(STAT_REG, & st)!= 0) return 1; 
        if((st & IBF) == 0){
            if(sys_outb(port,command)!= 0)return 1;
            return 0;
        }
    tickdelay(micros_to_ticks(20000)); n--;
    }
    return 1;
}

int (keyboard_enable_interrupts)(){
      uint8_t command;
  if(keyboard_write_scancode(IN_CMD, R_CMD)!= 0) return 1;
  if(keyboard_read_scancode(OUT_BUF, &command,0)!= 0) return 1;
  command |= BIT(0);
  if(keyboard_write_scancode(IN_CMD, W_CMD)!= 0) return 1;
  if(keyboard_write_scancode(W_CMD,command)!= 0) return 1;
return 0;
}



