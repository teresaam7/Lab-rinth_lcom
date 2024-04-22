#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
//#include <lcf.h>
//#include <minix/syslib.h>
//#include <syslib.h>

int hookid=0;
int counter=0;

//timer_test_time_base
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(freq>TIMER_FREQ||freq<19){
    return 1;
  }

  uint8_t st;//selecionar o timer st=TIMER_0+timer
  if(timer_get_conf(timer,&st)!=0){
    return 1;
  }
  st=st&0x0F; //Don't change 4 least-significant bits(mode and bcd)
  st=st|TIMER_LSB_MSB;//Preferably, LSB followed by MSB (bit 4 e 5)

  uint8_t initialTimer;//porta do timer escolhido (0x40, 0x41 ou 0x42)
  switch(timer){  
    case 0: 
      st=st|TIMER_SEL0;
      initialTimer=TIMER_0;
      break;
    case 1:
      st=st|TIMER_SEL1;
      initialTimer=TIMER_1;
      break;
    case 2:
      st=st|TIMER_SEL2;
      initialTimer=TIMER_2;
      break;
    default:
      return 1;
  }
  if(sys_outb(TIMER_CTRL,st)!=0){ //Set the control word to the control register(set the value no fim da funçao)
    return 1;}

  //initial value do contador e partes mais e menos significativas
  uint16_t initialVal=TIMER_FREQ/freq;
  uint8_t msb, lsb;
  util_get_MSB(initialVal, &msb);
  util_get_LSB(initialVal, &lsb);

  //valor inicial do contador(lsb seguido de msb)
  if(sys_outb(initialTimer,lsb)!=0){
    return 1;}
  if(sys_outb(initialTimer,msb)!=0){
    return 1;}

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if(bit_no==NULL){
    return 1;
  }
  *bit_no=BIT(hookid);
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookid)!=0){
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hookid)!=0){
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

//timer_test_read_config
//recebe o número do temporizador(timer) como parâmetro 
//e retorna o valor de status do temporizador em st
//poe RBC no TIMER_CTRL e numero do timer() em st
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0){
     return 1;
  }
  uint8_t readbackcommand= (TIMER_RB_CMD|TIMER_RB_COUNT_|TIMER_RB_SEL(timer));//escolher o counter,status,timer

  if (sys_outb(TIMER_CTRL, (uint32_t)readbackcommand) != 0) { 
    return 1;
  }

  int timer_port= TIMER_0 + timer;
  if (util_sys_inb(timer_port, st) != 0) {
      return 1; 
  }

  return 0; 
}

//timer_test_read_config
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val status; 
  uint8_t mode; //nao pode ser criado dentro do switch
  switch(field){
    case tsf_all:
      status.byte=st;
    break;
    case tsf_initial:
      mode=(st>>4) & 0x03;
      if(mode==1){ 
        status.in_mode=LSB_only;}
      else if(mode==2){ 
        status.in_mode=MSB_only;}
      else if(mode==3){ 
        status.in_mode=MSB_after_LSB;}
      else{ 
        status.in_mode=INVAL_val;}
    break;
    case tsf_mode:
      mode=(st>>1) & 0x07;
      if(mode==6){ status.count_mode=2;} //mode 2=X10    010 e 110
      else if(mode==7){ status.count_mode=3;} //mode 3=X11   111 e 011
      else status.count_mode=mode;
    break;
    case tsf_base:
       status.bcd=st&TIMER_BCD; //true se é BCD 
    break;
    default:
       return 1;
  }
  if (timer_print_config(timer, field, status) != 0){
     return 1;
  }
  return 0;
}
