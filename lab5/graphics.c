#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "graphics.h"

// Mudança do Minix do modo texto para modo gráfico
int (graphics_mode)(uint16_t mode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); //inicialização da estrutura com o valor 0 em todos os parâmetros
       
    reg86.ah=0x4F;                    //parte + significativa de ax
    reg86.al=0x02;                    //parte - significativa de ax
    //reg86.ax = 0x4F02;             //é igual a fazer duas últimas instruções->Set VBE mode
    reg86.bx=mode|BIT(14);           //determinação do submodo com memória linear
    reg86.intno=0x10;                 //intno sempre 0x10 -> BIOS video services
    if (sys_int86(&reg86)!=0) {      //se houver algum erro retorna EFAULT 
        printf("Changing graphic mode error\n");
        return 1;
    }
    return 0;
}
