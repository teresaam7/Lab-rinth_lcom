#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "graphics.h"

vbe_mode_info_t mode_info;
uint8_t *frame_buffer;


int (graphics_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.intno = 0x10;
  r.ax = 0x4F02;
  r.bx = BIT(14) | mode;
  
  if ( sys_int86(&r) != 0 ) {
    printf("sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int (frame_buffer_func)(uint16_t mode){
  
  if( vbe_get_mode_info(mode, &mode_info) != 0 ) 
    return 1;

  unsigned int bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frameSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

  int r;
  struct minix_mem_range mr;

  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + frameSize;

  if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
  printf("sys_privctl (ADD_MEM) failed: %d\n", r);
  
  /* Map memory */
  frame_buffer = vm_map_phys(SELF, (void *)mr.mr_base, frameSize);
  if(frame_buffer == MAP_FAILED)
  printf("couldn’t map video memory");

  return 0;

}


int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  if(x>mode_info.XResolution || y >mode_info.YResolution) return 0;
  unsigned int bytesPerPixel = (mode_info.BitsPerPixel + 7)/8;
  unsigned int index = (mode_info.XResolution * y + x) * bytesPerPixel;
  if(memcpy(&frame_buffer[index], &color, bytesPerPixel) == NULL) return 1;
  return 0;
}


int (make_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_image_t image;

    uint32_t *colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &image);

    for (int h = 0 ; h < image.height ; h++) {
      for (int w = 0 ; w < image.width ; w++) {
        if (vg_draw_pixel(x + w, y + h, colors[w + h*image.width])) return 1;
      }
    }
    return 0; 
}
