#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

int util_sys_inb(int port, uint8_t* n){
    if(n==NULL){return 1;}
uint32_t temp;
if(sys_inb(port, &temp)!=0){return 1;}
*n=temp&0xFF;
return 0;
}
