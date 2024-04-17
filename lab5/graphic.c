#include "graphic.h"

int (videocard_graphic_mode)(uint16_t submode) {
    reg86_t r86;
    memset(&r86, 0, sizeof(reg86)); 
    r86.intno = 0x10;        
    r86.ah = 0x4F;                  
    r86.al = 0x02;  
    reg86.ax = 0x4F02; 
    
    r86.bx = submode | LINEAR_BUFFER ; 

    if (sys_int86(&r86) != 0) {  
        printf("Set graphic mode failed\n");
        return 1;
    }
    return 0;
}