#include "graphics.h"


int (graphic_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.intno = 0x10;
  r.ax = 0x4F02;
  r.bx = BIT(14) | mode;
  
  if (sys_int86(&r) != 0) {
    printf("sys_int86 failed for graphic mode\n");
    return 1;
  }

  return 0;
}


int (initialize_frame_buffer)(uint16_t mode) {
  if (vbe_get_mode_info(mode, &mode_info) != 0) 
    return 1;

  frame.res_x = mode_info.XResolution;
  frame.res_y = mode_info.YResolution;

  frame.bytes_pixel = (mode_info.BitsPerPixel + 7) / 8; 
  frame.size = mode_info.XResolution * mode_info.YResolution * frame.bytes_pixel;
  unsigned int size_total = 2 * frame.size;
  

  int r;
  struct minix_mem_range mr;
  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + size_total;

  if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    printf("sys_privctl failed: %d\n", r);
    return 1;
  }


  frame.buffer1 = vm_map_phys(SELF, (void *)mr.mr_base, size_total);
  if (frame.buffer1 == MAP_FAILED) {
    printf("Mapping video memory failed\n");
    return 1;
  }
  frame.buffer2 = frame.buffer1 + frame.size;

  display_buffer = 1; draw_buffer = frame.buffer2;

  return 0;
}


void (set_display_start)(int buffer) {
    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.intno = 0x10;
    r.ax = 0x4F07;
    r.bx = 0x00;

    r.cx = 0x00;

    if (buffer == 1) {
      r.dx = 0;
    } else {    
      r.dx = frame.res_y;
    }
    
    if (sys_int86(&r) != 0) {
        printf("sys_int86() failed for set display start\n");
    }
}


void (update_flip_frames)() { 
  if (display_buffer == 1) {
    set_display_start(2);
    draw_buffer = frame.buffer1;
  } else {
    set_display_start(1);
    draw_buffer = frame.buffer2;
  }

  clear_drawing();
}


void (clear_drawing)() {
  memset(draw_buffer, 0, frame.size);
}


int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *map) {
  if (x < 0 || y < 0 || x >= frame.res_x || y >= frame.res_y) return 0;

  unsigned int index = (y * frame.res_x + x) * frame.bytes_pixel;

  if (memcpy(&map[index], &color, frame.bytes_pixel) == NULL) 
    return 1;

  return 0;
}





void (drawing_to_buffer)(uint8_t *sp_map) {
  memcpy(draw_buffer, sp_map, frame.size);
}


void (initialize_buffers)() {
  draw_buffer = malloc(frame.size);
}

void (free_buffers)() {
  free(draw_buffer);
}


int (background_drawing)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t w, uint16_t h) {

  xpm_image_t image;
  uint32_t *colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &image);
  uint32_t transparent_color = xpm_transparency_color(XPM_8_8_8_8); 

  for (int y = (- 40) ; y < (h + 40) ; y++) {
      for (int x = (-40) ; x < (w + 40) ; x++) {
          uint32_t current_color = colors[ (yi+y) * image.width + (xi+x)];
          
          if (current_color != transparent_color) {
              if (draw_pixel(xi + x, yi + y, current_color, draw_buffer)) {
                  printf("Drawing failed \n");
                  return 1;
              }
          }
      }
  } 
  return 0;
}



/*
void (update_frame)() {
  memcpy(frame.buffer1, draw_buffer, frame.size);
}

void (clear_frame)() {
  memset(frame.buffer1, 0, frame.size);
}


int (background_drawing)(xpm_map_t xpm, uint16_t xi, uint16_t yi) {
  clear_drawing();
  if (drawing_xpm(xpm, xi, yi) != 1) {
    printf("Background drawing failed \n");
    return 1;
  }
  memcpy(background_buffer, draw_buffer, frame.size);
  clear_drawing();
  return 0;
}

void (update_frame_with_background)() {
  memcpy(frame.buffer1, background_buffer, frame.size);
  memcpy(frame.buffer1, draw_buffer, frame.size);
}
*/

/*
void get_display_start(int* buffer) {
    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.intno = 0x10;
    r.ax = 0x4F07;
    r.bh = 0x00;
    r.bl = 0x01;

    if (sys_int86(&r) != 0) {
        printf("sys_int86() failed for get display start\n");
    }

    uint32_t first_pixel = r.cx;
    if (first_pixel == 0) 
      *buffer = 1;
    else 
      *buffer = 2;
}
*/


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
  memcpy(frame.buffer1, display_buffer, frame.size);
}

void flip_pages() { // Flip?
    uint8_t *temp = display_buffer;
    display_buffer = draw_buffer;
    draw_buffer = temp;
}
*/
