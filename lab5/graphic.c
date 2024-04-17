#include "graphic.h"

vbe_mode_info_t info;
uint8_t* video_mem;

int (videocard_graphic_mode)(uint16_t submode) {
    reg86_t r86;
    memset(&r86, 0, sizeof(r86)); 
    r86.intno = 0x10;        
    r86.ah = 0x4F;                  
    r86.al = 0x02;  
    r86.ax = 0x4F02; 

    r86.bx = submode | LINEAR_BUFFER ; 

    if (sys_int86(&r86) != 0) {  
        printf("Set graphic mode failed\n");
        return 1;
    }
    return 0;
}

int (frame)(uint16_t mode) {
    int r;
    struct minix_mem_range mr;

    memset(&info, 0, sizeof(info));
    if(vbe_get_mode_info(mode, &info)) {
        return 1;
    }
    unsigned numBytes = (info.BitsPerPixel + 7) / 8;
    unsigned int vram_size = info.XResolution * info.YResolution * numBytes;

    mr.mr_base = info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    }

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED) {
        panic("couldn’t map video memory");
        return 1;
    }

    return 0;
}


int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x > info.XResolution || y > info.YResolution) {
    return 1;
  } 

  unsigned numBytes = (info.BitsPerPixel + 7) / 8;
  unsigned int i = (info.XResolution * y + x) * numBytes;

  if (memcpy(&video_mem[i], &color,numBytes) == NULL) {
    return 1;
  }

  return 0;
}

int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0 ; i < len ; i++)  {
    if (draw_pixel(x+i, y, color) != 0) {
        return 1;
    }
  }  
  return 0;
}

int (color_normalization)(uint32_t color, uint32_t *new_color) {
  if (info.BitsPerPixel == 32) {
    *new_color = color;
  } else {
    *new_color = color & (BIT(info.BitsPerPixel) - 1);
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for( unsigned i = 0; i < height ; i++) {
    if (draw_hline(x, y+i, width, color) != 0) {
      vg_exit();
      return 1;
    }
  }
  return 0;
}

