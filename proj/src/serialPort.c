#include "serialPort.h"

int hook_id_sp = 6;
extern GameState gameState;
extern Sprite *player, *player2;
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
    receiveQueue = newQueue(20);
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
    enqueue(receiveQueue, byte);
    return 1;
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

void (send_scancode)(uint8_t scancode){
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
}

void (manage_button)(uint8_t scancode, bool isPlayer1) {
  if(isPlayer1) {
    handle_ingame_scancode(scancode, player);
  } 
  else{
    handle_ingame_scancode_multi(scancode, player2);
    send_scancode(scancode);
  } 
}

bool (handle_start_multi)(){
    if(frontQueue(receiveQueue) == 0x53){
        send_byte(0x54);
        printf("AAAAAAAA");
    }else if(frontQueue(receiveQueue) == 0x54){
        send_byte(0x55);
        printf("BBBBBBBB");
    }else if(frontQueue(receiveQueue) == 0x55){
        uint8_t srandByte = 0;
        while(srandByte == ACK || srandByte == NACK || srandByte == END || srandByte == 0){
            srandByte = rand();
        }
        send_byte(0x56);
        send_byte(srandByte);
        srandom(srandByte);
        printf("CCCCCC");
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
        printf("DDDDDDDD");
    }else if(frontQueue(receiveQueue) == 0x57){
        gameState = GAME;
        multi = true;
        printf("FFFFFFFFF");
    }
    dequeue(receiveQueue);
    return true;
}

void (sp_handler)(){
  sp_ih();
  printf("ZZZZZZZZZZ");
  if (gameState == MULTI)
    handle_start_multi();
  else if (gameState == GAME && multi) 
    handle_receive_info();
  else
    cleanInt_sp();
  
}

void (handle_receive_info)(){
   if(queueIsEmpty(receiveQueue)) return;
    while(!queueIsEmpty(receiveQueue)){
        uint8_t curByte = dequeue(receiveQueue);

        if(curByte == END){
            gameState = LOSE;
            return;
        }
        else{ 
            uint8_t scancode;
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

            if(!invalid) 
                manage_button(scancode, false);
            }
    }
}


