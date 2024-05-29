#include "serialPort.h"

int hook_id_sp = 6;
extern GameState gameState;
static Queue * receiveQueue;
extern bool multi;

int (sp_subscribe_int)(uint8_t* bitno){
    *bitno = BIT(hook_id_sp);
    if(sys_irqsetpolicy(IRQ_COM1 , IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_sp)) return 1;
    return 0;
}

int (sp_unsubscribe_int)(){
    if(sys_irqrmpolicy(&hook_id_sp)) return 1;
    return 0;
}

void (initialize_sp)(){
    uint8_t ier;
    if(util_sys_inb(BASE_COM1+ IER, &ier)!= 0) 
        return;
    ier &= 0xF0;
    if(sys_outb(BASE_COM1+ IER,ier | IER_ERBFI)!= 0) 
        return;
    receiveQueue = newQueue(128);
}

int (send_byte)(uint8_t byte){
    uint8_t st, i = 10;
    while(i){
      --i;  
      // Read status
      if(util_sys_inb(BASE_COM1 + LSR, &st) != 0)
        return 1;
      if(st & LSR_THRE)
        if (sys_outb(BASE_COM1 + THR, byte) != 0)
            return 1;
    }
    return 1;
}

int (receive_byte)(){
    uint8_t st, rbr;
    // Read status/
    if( util_sys_inb(BASE_COM1 + LSR, &st)!= 0) 
        return 1;
    if(st & LSR_DR){
      if(util_sys_inb(BASE_COM1 + RBR, &rbr)!= 0)
        return 1;
      if(!(st & (LSR_OE| LSR_PE| LSR_FE))){
          enqueue(receiveQueue,rbr);
          return 0;
      }
    }
    return 1;
}

Queue* (get_queue)(){
    return receiveQueue;
}

void (sp_out)(){
    clearQueue(receiveQueue);
}

int (cleanInt_sp)(){
    if(sys_outb(BASE_COM1 + FCR,FCR_CLEAR)!= 0) 
        return 1;
    while(!queueIsEmpty(receiveQueue))
        dequeue(receiveQueue);
    return 0;
}

void (sp_ih)(){
    uint8_t iir;
    if(util_sys_inb(BASE_COM1+ IIR, &iir) != 0) 
        return;
    if(!(iir & IIR_NIP))
        if(iir & IIR_IP == IIR_CT)
            while(receive_byte());
}

/*

bool (handle_coop_start)(){
    if(frontQueue(receiveQueue) == 0x53){
        send_byte(0x54);
    }else if(frontQueue(receiveQueue) == 0x54){
        send_byte(0x55);
    }else if(frontQueue(receiveQueue) == 0x55){
        uint8_t srandByte = 0;
        while(srandByte == ACK || srandByte == NACK || srandByte == ERROR || srandByte == 0){
            srandByte = rand();
        }
        send_byte(0x56);
        send_byte(srandByte);
        srandom(srandByte);
    }else if(frontQueue(receiveQueue) == 0x56){
        dequeue(receiveQueue);
        uint8_t srandByte = dequeue(receiveQueue);
        while(srandByte == 0){
            read_byte();
            srandByte = dequeue(receiveQueue);
        }
        srandom(srandByte);
        swap_characters();
        swapped = true;
        gameState = GAME;
        set_power_up_alarm(1);
        set_enemy_throw(0xF);
        multi =true;
        send_byte(0x57);
    }else if(frontQueue(receiveQueue) == 0x57){
        gameState = GAME;
        set_power_up_alarm(1);
        set_enemy_throw(0xF);
        multi = true;
    }
    dequeue(receiveQueue);
    return true;
}


*/
