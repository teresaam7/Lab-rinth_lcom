#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "graphics.h"

vbe_mode_info_t modeinfo;
uint8_t *frame_buffer;

// Mudança do Minix do modo texto para modo gráfico
int (graphics_mode)(uint16_t mode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); //inicialização da estrutura com o valor 0 em todos os parâmetros
       
    reg86.ah=0x4F;                    //parte + significativa de ax
    reg86.al=0x02;                    //parte - significativa de ax 
    //reg86.ax = 0x4F02;              //é igual a fazer duas últimas instruções->Set VBE mode
    reg86.bx=mode|BIT(14);            //The mode must be passed in the BX register. Bit 14 of the BX register should be set
    reg86.intno=0x10;                 //intno sempre 0x10 -> BIOS video services
    if (sys_int86(&reg86)!=0) {       //se houver algum erro retorna EFAULT 
        printf("Changing graphic mode error\n");
        return 1;
    }
    return 0;
}

//Mapping the Linear Frame Buffer-> contruir o frame buffer virtual e físico
//What is displayed on a computer screen depends on the contents of the frame buffer in VRAM
//The VRAM is a physical memory region so before lab5 can access the frame buffer, it must map the VRAM to its address space using:
//void *vm_map_phys(endpoint_t who, void *phaddr, size_t len)-> receives physical address and returns virtual address
int (frame_buffer_func)(uint16_t mode){
    if(vbe_get_mode_info(mode, &modeinfo)!=0){return 1;}

    //calcular tamanho do frame buffer em bytes
    unsigned int bytes_in_pixel = (modeinfo.BitsPerPixel+7)/8;
    unsigned int frame_size=modeinfo.XResolution * modeinfo.YResolution * bytes_in_pixel;

    //preenchimento dos endereços físicos
    struct minix_mem_range physical_addresses;
    physical_addresses.mr_base=modeinfo.PhysBasePtr; //início físico do buffer
    physical_addresses.mr_limit=physical_addresses.mr_base + frame_size; //fim físico do buffer

    //alocação física da memória necessária para o frame buffer
    int r;
    if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physical_addresses))){//codigo retirado do guiao lab5
             panic("sys_privctl (ADD_MEM) failed: %d\n", r);
             return 1;
    }

    //alocação virtual da memória necessária para o frame buffer
    frame_buffer = vm_map_phys(SELF, (void*) physical_addresses.mr_base, frame_size);
    if (frame_buffer == NULL) {
        printf("Virtual memory allocation error\n");
        return 1;
    }
    return 0;
}


int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    
  //se cooredenadas invalidas
  if(x>modeinfo.XResolution || y>modeinfo.YResolution){ return 1;}
  
  //Bytes per pixel da cor escolhida e arredondado por excesso
  unsigned int bytesInPixel=(modeinfo.BitsPerPixel+7)/8;

  //índice (em bytes) da zona do píxel a colorir
  unsigned int index=(modeinfo.XResolution*y+x)*bytesInPixel;

  //copy bytesInPixel bytes of color starting from frame_buffer[index]
  if (memcpy(&frame_buffer[index], &color, bytesInPixel) == NULL) return 1;
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for(int i=0; i<len; i++){
        if(vg_draw_pixel(x+i,y,color)!=0){
            vg_exit();
            return 1;
        }
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(int i=0; i<height; i++){
        if(vg_draw_hline(x, y+i, width, color)!=0){
            vg_exit();
            return 1;
        }
    }
    return 0;
}
