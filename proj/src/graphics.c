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

  frame.bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8; 
  frame.size = mode_info.XResolution * mode_info.YResolution * frame.bytesPerPixel;
  

  int r;
  struct minix_mem_range mr;
  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_base2 = mr.mr_base + frame.size;

  if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    printf("sys_privctl failed: %d\n", r);
    return 1;
  }
  
  frame.buffer = vm_map_phys(SELF, (void *)mr.mr_base, frame.size*2);
  if(frame.buffer == MAP_FAILED) {
    printf("Mapping video memory failed 1\n");
    return 1;
  }

  frame.buffer2 = frame.buffer + frame.size;

  return 0;
}

void set_display_start(int buffer) {
    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.intno = 0x10;
    r.ax = 0x4F07;
    r.bx = 0x00;
    r.cx = 0x00;

    if (buffer )
    r.dx = frame.res_y;
    
    if (sys_int86(&r) != 0) {
        printf("sys_int86() failed for set display start\n");
    }
}

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

void flip_frames() { 
  // Get buffer atual 

  // Trocar pelo
}


int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *map) {
  if (x < 0 || y < 0 || x >= frame.res_x || y >= frame.res_y) return 0;

  unsigned int index = (y * frame.res_x + x) * frame.bytesPerPixel;

  if (memcpy(&map[index], &color, frame.bytesPerPixel) == NULL) 
    return 1;

  return 0;
}


int (drawing_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi, Sprite *sp) {
    xpm_image_t image;
    uint32_t *colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &image);
    uint32_t transparent_color = xpm_transparency_color(XPM_8_8_8_8); 

    sp->height = image.height;
    sp->width = image.width; 
  
    for (int y = 0 ; y < sp->height ; y++) {
        for (int x = 0 ; x < sp->width ; x++) {
            uint32_t current_color = colors [y * image.width + x];
            
            if (current_color != transparent_color) {
                if (draw_pixel(xi + x, yi + y, current_color, sp->map)) {
                    printf("Drawing pixel failed \n");
                    return 1;
                }
            }
        }
    }
    return 0; 
}


int (drawing_to_buffer)(uint8_t *sp_map) {
  memcpy(draw_buffer, sp_map, frame.size);
}


void (initialize_buffers)() {
  draw_buffer = malloc(frame.size);
  display_buffer = malloc(frame.size);
}

void (free_buffers)() {
  free(draw_buffer);
  free(display_buffer);
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
  memcpy(frame.buffer, background_buffer, frame.size);
  memcpy(frame.buffer, draw_buffer, frame.size);
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
