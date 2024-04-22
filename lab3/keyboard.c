#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "i8042.h"

int hookid=1;//para ser diferente do hookid do timer
uint8_t scancode;

int (KBD_subscribe_int)(uint8_t *bit_no) {
  if(bit_no==NULL){
    return 1;
  }
  *bit_no=BIT(hookid);
  if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hookid)!=0){
    return 1;
  }
  return 0;
}

int (KBD_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hookid)!=0){
    return 1;
  }
  return 0;
}


int (read_scancode)(uint8_t port, uint8_t* code){
  uint8_t status;
  int i=0;
  while(i<10){ //limita o número de tentativas de leitura do scancode
    if(util_sys_inb(STATUS_REG, &status)!=0){ //status=STATUS_REG
      printf("Status error\n");
      return 1;
    }
    if((status&OBF)!=0){ //If output buffer has a scancode
        if(util_sys_inb(OUT_BUF,code)!=0) { //scancode=OUT_BUF
            printf("Read error\n");
            return 1;
        }
        if((status&TIMEOUT)!=0){
            printf("Timeout error\n");
            return 1;
        }
        if((status&PARITY)!=0){
            printf("Parity error\n");
            return 1;
        }
        if((status&AUX)!=0){
            printf("Mouse error\n");
            return 1;
        }
      return 0;//read scancode
    }
    i++;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

//read the status register and check if there was some communications error
//to read the scancode byte from the output buffer
void (kbc_ih)(){
  if(read_scancode(STATUS_REG, &scancode)!=0){
    printf("Read scancode error\n");
  }
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte) {
    uint8_t status;
    uint8_t n=0;
    while(n<10){
       /* if (read_scancode(STATUS_REG,&status) != 0){
            printf("Error: Status not available!\n");
            return 1;
        }*/
        if(util_sys_inb(STATUS_REG, &status)!=0){
          printf("Error: Status not available!\n");
          return 1;
        }
       // printf("%d\n",status&BIT(0));
        if((status&BIT(0))==0){
            if(sys_outb(port, commandByte) != 0){
                printf("Error: Could not write commandByte!\n");
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        n++;
    }
    return 1;
}

int keyboard_restore(){
  uint8_t commandByte;

    if (write_KBC_command(KBC_CMD_REG,0x20)!=0) return 1; //avisar o i8042 da leitura     
    if (read_scancode(OUT_BUF,&commandByte)!=0) return 1; //ler a configuração

    //if bit(0)==1 enable interrupt on OBF, from keyboard
    commandByte |= BIT(0);  

    if (write_KBC_command(KBC_CMD_REG,0x60) != 0) return 1; //avisar o i8042 da escrita
    if (write_KBC_command(OUT_BUF, commandByte) != 0) return 1; //escrever a configuração

    return 0;
}
