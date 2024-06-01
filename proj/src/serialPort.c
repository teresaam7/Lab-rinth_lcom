#include "serialPort.h"

int hook_id_sp = 6;
extern GameState gameState;
extern Sprite *player, *player2;
static Queue * receiveQueue, *sendQueue;
extern bool multi;
bool isPlayer1;
static bool hold_reg_empty = true;

int (sp_subscribe_int)(uint8_t* bitno){
    *bitno = BIT(hook_id_sp);
    if(sys_irqsetpolicy(IRQ_COM1 , IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_sp)) return 1;
    return 0;
}

int (sp_unsubscribe_int)(){
    if(sys_irqrmpolicy(&hook_id_sp)) return 1;
    return 0;
}

int ser_config(){
    uint8_t reg;
    reg = (BIT(7));
    bool res = sys_outb(BASE_COM1+ LCR, reg);  
        
    if(res) return !OK;
    res = sys_outb(BASE_COM1+ 0x01, 0);  

    if(res) return !OK;
    res =  sys_outb(BASE_COM1+ 0x00, 1); 

    if(res) return !OK;
    reg =  sys_outb(BASE_COM1+ LCR, reg); 
    
    res = sys_outb(BASE_COM1+ LCR, reg);

    if(res) return !OK;
    return OK;
}


bool (sp_enable_int)(){
    uint8_t reg;
    reg = (BIT(0));
    return sys_outb(BASE_COM1+ IER, reg);  
}


bool (sp_disable_int)(){
    uint8_t reg;
    util_sys_inb(IER + BASE_COM1, &reg);
    reg &= ~(BIT(0) | BIT(1)| BIT(2) | BIT(3));
    return sys_outb(BASE_COM1+ IER, reg); 
}



void (initialize_sp)(){
    uint8_t ier;
    if(util_sys_inb(BASE_COM1+ IER, &ier)!= 0) 
        return;
    ier &= 0xF0;
    if(sys_outb(BASE_COM1+ IER,ier | IER_ERBFI)!= 0) 
        return;
    receiveQueue = newQueue(100);
    sendQueue = newQueue(100);
    ser_config();
}


int (send_byte)(uint8_t byte){
    int pushed = enqueue(sendQueue,byte);
    if(hold_reg_empty){
        return send_queue_bytes();
    }else return pushed;
}

int (send_queue_bytes)(){
    if(queueIsEmpty(sendQueue)){
        hold_reg_empty = true;
        return 1;
    }

    uint8_t empty_transmitter;

    while(!queueIsEmpty(sendQueue)){
        sys_outb(BASE_COM1 + THR,frontQueue(sendQueue));
        dequeue(sendQueue);
        util_sys_inb(BASE_COM1+LSR, &empty_transmitter);
        empty_transmitter &= LSR_THRE ;
        hold_reg_empty = empty_transmitter;
        if(!empty_transmitter) return 1;
    }
    return 0;
}

int (receive_byte)(){
    uint8_t st, rbr;
    // Read status
    if( util_sys_inb(BASE_COM1 + LSR, &st)!= 0) 
        return 1;
    if(st & LSR_DR){
      if(util_sys_inb(BASE_COM1 + RBR, &rbr)!= 0)
        return 1;
      if(!(st & (LSR_OE| LSR_PE| LSR_FE))){
          enqueue(receiveQueue,rbr);
          //printf("ENQUEUE: %u", rbr);
          return 0;
      }
    }
    return 1;
}

Queue* (get_receive_queue)(){
    return receiveQueue;
}

Queue* (get_send_queue)(){
    return sendQueue;
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
    printf("CHEGUEII");
    uint8_t reg;
    util_sys_inb(BASE_COM1+IIR, &reg);
    while(!(reg & IIR_NIP)) {
        if(reg & IIR_RDA){
            while(0 == receive_byte());
            util_sys_inb(BASE_COM1+IIR, &reg);
            printf("AQUI1");
        }
        if(reg & IIR_THRE){
            send_queue_bytes();
            util_sys_inb(BASE_COM1+IIR, &reg);
            printf("AQUI2");
        }
    }
}

void (send_scan)(uint8_t scancode){
    uint8_t send_code = 0;
    if(scancode == A_KEY_MK){
        send_code |= BIT(0);
    }else if(scancode == A_KEY_BRK){
        send_code |= BIT(1);
    }else if(scancode == D_KEY_MK){
        send_code |= BIT(2);
    }else if(scancode == D_KEY_BRK){
        send_code |= BIT(3);
    }else if(scancode == W_KEY_MK){
        send_code |= BIT(4);
    }else if(scancode == W_KEY_BRK){
        send_code |= BIT(5);
    }else if(scancode == S_KEY_MK){
        send_code |= BIT(6);
    }else if(scancode == S_KEY_BRK){
        send_code |= BIT(7);
    }
    send_byte(send_code);
    printf("SEND_SCAN:  %u\n", send_code);
}

void (manage_button)(uint8_t scancode, bool isPlay1) {
    printf("SOOOOO");
  if (isPlay1) {
    printf("COOOO");
    handle_ingame_scancode(scancode, player);
  } 
  else {
    printf("ROOOOOO");
    handle_ingame_scancode_multi(scancode, player2);
  } 
  send_scan(scancode);
}

bool (handle_start_multi)(){
    if(frontQueue(receiveQueue) == 0x53){
        send_byte(0x54);
    }else if(frontQueue(receiveQueue) == 0x54){
        send_byte(0x55);
    }else if(frontQueue(receiveQueue) == 0x55){
        uint8_t srandByte = 0;
        while(srandByte == ACK || srandByte == NACK || srandByte == END || srandByte == 0){
            srandByte = rand();
        }
        send_byte(0x56);
        send_byte(srandByte);
        srandom(srandByte);
    }else if(frontQueue(receiveQueue) == 0x56){
        dequeue(receiveQueue);
        uint8_t srandByte = dequeue(receiveQueue);
        while(srandByte == 0){
            receive_byte();
            srandByte = dequeue(receiveQueue);
        }
        srandom(srandByte);
        gameState = GAME;
        multi =true;
        send_byte(0x57);
    }else if(frontQueue(receiveQueue) == 0x57){
        gameState = GAME;
        multi = true;
    }
    while(!queueIsEmpty(receiveQueue))
        dequeue(receiveQueue);
    //send_queue_bytes();
    return true;
}


void (sp_handler)(){
    printf("TEEEEEE");
  sp_ih();
  if (gameState == MULTI)
    handle_start_multi();
  else if (gameState == GAME && multi) {
    handle_receive_info();
    printf("RIZA");
  }
  else
    cleanInt_sp();
}

void (handle_receive_info)(){
    printf("AAAAAAA");
    if(queueIsEmpty(receiveQueue)) return;
    printf("BBBBBBB");
    while(!queueIsEmpty(receiveQueue)){
        printf("CCCCCCC");
        uint8_t curByte = dequeue(receiveQueue);

        if(curByte == END){
            gameState = LOSE;
            return;
        }
      
        printf("XXXXXXXXXX\n");
        printf("SCAN: %u\n", curByte);
        uint8_t scancode = 0x00;
        bool invalid = false;
        if(curByte & BIT(0)) 
            scancode = A_KEY_MK;
        else if(curByte & BIT(1)) 
            scancode = A_KEY_BRK;
        else if(curByte & BIT(2)) 
            scancode = D_KEY_MK;
        else if(curByte & BIT(3)) 
            scancode = D_KEY_BRK;
        else if(curByte & BIT(4)) 
            scancode = W_KEY_MK;
        else if(curByte & BIT(5))
            scancode = W_KEY_BRK;
        else if(curByte & BIT(6)) 
            scancode = S_KEY_MK;
        else if(curByte & BIT(7)) 
            scancode = S_KEY_BRK;
        else
            invalid = true;
        if(!invalid) {
            printf("ZZZZZZZZZZZZZZZZ");
            if (isPlayer1) manage_button(scancode, false);
            else manage_button(scancode, true);
        }
    }
}


