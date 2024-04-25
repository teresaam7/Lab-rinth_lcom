#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "mouse.h"

uint8_t byte;
struct packet pp;
int index_counter=0;

void (mouse_ih)() {
    if(read_scancode(0x60, &byte, 1)!=0){
        printf("Couldn't read KBC");
    }
  //  else{
  //    mouse_sync_bytes(&index_counter);
  //  }
}

int (read_scancode)(uint8_t port, uint8_t* code, int aux){
  uint8_t status;
  int i=0;
  while(i<10){ //limita o número de tentativas de leitura do scancode
    if(util_sys_inb(STATUS_REG, &status)!=0){ //status=STATUS_REG
      printf("Status error\n");
      return 1;
    }

    if((status&OBF)!=0){ //If output buffer has a scancode, OBF cheio
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
        if(((status&AUX)==0)&&aux){ //se queremos mouse mas nao ha
            printf("No mouse\n");
            return 1;
        }
        if((status&AUX)&&(aux==0)){
            printf("No keyboard\n");
            return 1;
        }
      return 0;//read scancode
    }
    i++;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  printf("olaaa");
  return 1;
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte) {
    uint8_t status;
    uint8_t n=0;
    while(n<10){
        if(util_sys_inb(STATUS_REG, &status)!=0){
          printf("Error: Status not available!\n");
          return 1;
        }
        if((status&BIT(0))==0){ //se IBF não estiver cheio
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

void mouse_sync_bytes() {
  if ((index_counter==0) && (byte&BIT(3))) { //bit 3 do byte 1 do mouse packet está sempre ativo
    pp.bytes[index_counter]=byte;
    index_counter++;
  }
  else if ((index_counter>0)&&(index_counter<3)) {  
    pp.bytes[index_counter]=byte;
    index_counter++;
  }
}

void (mouse_build_packet)(){ //por os 3 bytes num packet
  pp.lb = pp.bytes[0]&LB;
  pp.mb = pp.bytes[0]&MB;
  pp.rb = pp.bytes[0]&RB;
  pp.x_ov = pp.bytes[0]&X_OVFL;
  pp.y_ov = pp.bytes[0]&Y_OVFL;
  if(pp.bytes[0]&MSB_X_DELTA){ //se msb negativo
    pp.delta_x=(0xFF00|pp.bytes[1]);
  }
  else{
    pp.delta_x=pp.bytes[1]; //se for positivo msb
  }

  if(pp.bytes[0]&MSB_Y_DELTA){
    pp.delta_y=0xFF00|pp.bytes[2];
  }
  else{
    pp.delta_y=pp.bytes[2];
  }
}


int (mouse_write)(uint8_t command) {
  uint8_t received_byte = 0xFE;
  int n=10;
  while (n&&(received_byte!=0xFA)){
        if (write_KBC_command(KBC_CMD_REG, 0xD4)!=0){
            return 1;
        }
        if (write_KBC_command(IN_BUF, command)!=0){
            return 1;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        if (util_sys_inb(OUT_BUF, &received_byte)!=0){
            return 1;
        }   
        n--;
  }
  if(received_byte!=0xFA){return 1;}
    return 0;
}
