#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "graphics.h"

vbe_mode_info_t modeinfo;
uint8_t *frame_buffer;

int (graphics_mode)(uint16_t mode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); //zero em todo o reg86
       
    reg86.ah=0x4F;                    //parte + significativa de ax
    reg86.al=0x02;                    //parte - significativa de ax 
    reg86.bx=mode|BIT(14); 
    reg86.intno=0x10;                 
    if(sys_int86(&reg86)!=0){       //se erro retorna EFAULT 
        printf("Changing graphic mode error\n");
        return 1;
    }
    return 0;
}

int (frame_buffer_func)(uint16_t mode){
    if(vbe_get_mode_info(mode, &modeinfo)!=0){return 1;}
    unsigned int bytes_in_pixel = (modeinfo.BitsPerPixel+7)/8;
    unsigned int frame_size=modeinfo.XResolution * modeinfo.YResolution * bytes_in_pixel;

    struct minix_mem_range physical_address;
    physical_address.mr_base=modeinfo.PhysBasePtr; 
    physical_address.mr_limit=physical_address.mr_base + frame_size; 
    int r;
    if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physical_address))){
            panic("sys_privctl (ADD_MEM) failed: %d\n", r);
            return 1;
    }

    frame_buffer=vm_map_phys(SELF, (void*) physical_address.mr_base, frame_size);
    if(frame_buffer==NULL){
        printf("Virtual memory failed\n");
        return 1;
    }
    return 0;
}


int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  if(x>modeinfo.XResolution||y>modeinfo.YResolution){ return 1;}
  unsigned int bytesInPixel=(modeinfo.BitsPerPixel+7)/8;
  unsigned int index=(modeinfo.XResolution*y+x)*bytesInPixel;
  if(memcpy(frame_buffer+index, &color, bytesInPixel)==NULL){ return 1;}
  return 0;
}

int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for(int i=0; i<len; i++){
        if(vg_draw_pixel(x+i,y,color)!=0){
            vg_exit();
            return 1;
        }
    }
    return 0;
}

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(int i=0; i<height; i++){
        if(vg_draw_hline(x, y+i, width, color)!=0){
            vg_exit();
            return 1;
        }
    }
    return 0;
}

//tem de ser uint32_t
uint32_t R(uint32_t color){
    printf("%p",modeinfo.RedFieldPosition); //10
    printf("%p",modeinfo.RedMaskSize); //8
    return ((1<<modeinfo.RedMaskSize)-1)&(color>>modeinfo.RedFieldPosition);
}
uint32_t G(uint32_t color){
    return ((1<<modeinfo.GreenMaskSize)-1)&(color>>modeinfo.GreenFieldPosition);
}
uint32_t B(uint32_t color){
    return ((1<<modeinfo.BlueMaskSize)-1)&(color>>modeinfo.BlueFieldPosition);
}

int (make_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi) {
    xpm_image_t image;
    uint8_t *map;
    map=xpm_load(xpm, XPM_INDEXED, &image);
    for(uint8_t i=0; i<image.height; i++){
        for(uint8_t j=0; j<image.width; j++){
            if(vg_draw_pixel(xi+j, yi+i, *map)!=0){return 1;};
            map++;//proxima cor
        }
    }
    return 0;
}
