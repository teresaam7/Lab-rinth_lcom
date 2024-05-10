#include "graphics.h"


int (graphic_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.intno = 0x10;
  r.ax = 0x4F02;
  r.bx = BIT(14) | mode;
  
  if (sys_int86(&r) != 0) {
    printf("sys_int86() failed \n");
    return 1;
  }

  return 0;
}
vbe_mode_info_t mode_info;

int (initialize_frame_buffer) (uint16_t mode) {
  //vbe_mode_info_t mode_info;
  if (vbe_get_mode_info(mode, &mode_info) != 0) 
    return 1;

  frame.res_x = mode_info.XResolution;
  frame.res_y = mode_info.YResolution;

  frame.bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8; 
  frame.size = mode_info.XResolution * mode_info.YResolution * frame.bytesPerPixel;
  

  int r;
  struct minix_mem_range mr;
  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + frame.size;

  if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    printf("sys_privctl failed: %d\n", r);
    return 1;
  }
  
  frame.buffer = vm_map_phys(SELF, (void *)mr.mr_base, frame.size);
  if(frame.buffer == MAP_FAILED) {
    printf("Mapping video memory failed \n");
    return 1;
  }

  return 0;
}


int (draw_pixel_to_buffer) (uint16_t x, uint16_t y, uint32_t color) {
  if (x < 0 || y < 0 || x >= frame.res_x || y >= frame.res_y) return 0;

  unsigned int index = (y * frame.res_x + x) * frame.bytesPerPixel;
  if(memcpy(&draw_buffer[index], &color, frame.bytesPerPixel) == NULL) 
    return 1;

  return 0;
}


int (make_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi) {
    xpm_image_t image;
    uint32_t *colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &image);
    uint32_t transparent_color = xpm_transparency_color(XPM_8_8_8_8); 

    for (int y = 0 ; y < image.height ; y++) {
        for (int x = 0 ; x < image.width ; x++) {
            uint32_t current_color = colors[y * image.width + x];
            
            if (current_color != transparent_color) {
                if (draw_pixel_to_buffer(xi + x, yi + y, current_color)) {
                    printf("Drawing failed \n");
                    return 1;
                }
            }
        }
    }
    return 0; 
}


void (initialize_buffers)() {
  draw_buffer = malloc(frame.size);
}

void (free_buffers)() {
  free(draw_buffer);
}

void (update_frame)() {
  memcpy(frame.buffer, draw_buffer, frame.size);
}

void (clear_frame)() {
  memset(frame.buffer, 0, frame.size);
}

void (clear_drawing)() {
  memset(draw_buffer, 0, frame.size);
}


/*
uint8_t *display_buffer;

void (initialize_buffers)() {
  display_buffer = malloc(frame.size);
  draw_buffer = malloc(frame.size);
}

void (free_buffers)() {
  free(display_buffer);
  free(draw_buffer);;
}

void (update_display)() { 
  display_buffer = draw_buffer;
}

void (update_screen)() {
  memcpy(frame.buffer, display_buffer, frame.size);
}

void flip_pages() { // Flip?
    uint8_t *temp = display_buffer;
    display_buffer = draw_buffer;
    draw_buffer = temp;
}
*/
