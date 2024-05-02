// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "keyboard.h"
#include "timer.c"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

#include <math.h>
uint8_t extern scancode;

int set_graphic_mode(uint16_t mode) {
reg86_t r;
memset(&r, 0,sizeof(r));
r.ax = 0x4F02;
r.bx = BIT(14)|mode;
r.intno = 0x10;
if( sys_int86(&r) != 0 ) {
printf("set_vbe_mode:(): sys_int86() failed \n");
return 1;
}
return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if(set_graphic_mode(mode)!= 0)return 1;
  sleep(delay);
  if(vg_exit()!= 0) return 1;
  return 0;
}

vbe_mode_info_t mode_info;
 uint8_t *frame_buffer;

int (set_frame_buffer)(uint16_t mode){
   //memset(&mode_info, 0, sizeof(mode_info));
  if(vbe_get_mode_info( mode, & mode_info)!= 0) return 1;

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

int (normalize_color) ( uint32_t color,  uint32_t * normColor) {
  if(mode_info.BitsPerPixel == 32){
    *normColor = color;
    return 0;
  }
  *normColor = color & (BIT(mode_info.BitsPerPixel) - 1 );

  return 0;
}

int waiting_ESC_key(){
  int ipc_status;
  message msg;
  int r;
  uint8_t irq_set;

  if(keyboard_subscribe_interrupts(&irq_set) != 0) return 1;

  while( scancode!= BREAK_ESC ) { 
    if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
      if (msg.m_notify.interrupts & irq_set) { 
        kbc_ih();
      }
      break;
      default:
      break; 
      }
    }
  }
  if(keyboard_unsubscribe_interrupts()!= 0) return 1;
  return 0;
}

int(vg_draw_pixel)( uint16_t x, uint16_t y,uint32_t color){
  if(x>mode_info.XResolution || y >mode_info.YResolution) return 1;
  unsigned int bytesPerPixel = (mode_info.BitsPerPixel + 7)/8;
  unsigned int index = (mode_info.XResolution * y + x) * bytesPerPixel;
  if(memcpy(&frame_buffer[index], &color, bytesPerPixel) == NULL) return 1;
  return 0;
}

int(vg_draw_line)( uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint32_t color){
  for(unsigned i = 0; i<width; i++){
    if(vg_draw_pixel(x+i, y, color)!= 0) return 1;
  }
  return 0;
}

int(draw_rectangle)( uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint32_t color){
  /*
  for(unsigned j = 0; j < height; j++){
    if(vg_draw_line(x, y+j, width,height,color)!= 0) return 1;
  }*/
  for(unsigned i = 0; i<width; i++){
    if(vg_draw_pixel(x+i, y-i, color)!= 0) return 1;
  }
  return 0;
}




int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  if(set_frame_buffer(mode)!= 0)return 1; 
  if(set_graphic_mode(mode)!= 0)return 1;
  uint32_t normColor;
  if(normalize_color(color,&normColor) != 0) return 1;
  if(draw_rectangle(x, y, width, height,color)!= 0) return 1;
  if(waiting_ESC_key()!= 0) return 1;
  if(vg_exit()!= 0) return 1;

  return 1;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if(set_frame_buffer(mode)!= 0)return 1; 
  if(set_graphic_mode(mode)!= 0)return 1;
  int height = mode_info.YResolution/no_rectangles;
  int width = mode_info.XResolution/no_rectangles;

  for(int row = 0; row <no_rectangles ; row++){
    for(int col = 0; col < no_rectangles; col++){

      uint32_t color;

      if(mode_info.MemoryModel == 0x06){// nao percebo de onde vem o 0x06!!!
        uint32_t R = ((((1<<mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition)) + col * step) % (1 << mode_info.RedMaskSize);

        uint32_t G = ((((1<<mode_info.GreenMaskSize)- 1) & (first >> mode_info.GreenFieldPosition)) + row * step) % (1 << mode_info.GreenMaskSize); 

        uint32_t B = ((((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition)) + (col + row) * step) % (1 << mode_info.BlueMaskSize); 
        color = (R << mode_info.RedFieldPosition)|(G << mode_info.GreenFieldPosition)|(B << mode_info.BlueFieldPosition);

      }
      else{ color = (first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel);}
      if(draw_rectangle(col*width, row*height, width, height, color)!= 0)return 1;
    }
  }

  if(waiting_ESC_key()!= 0)return 1;

  if(vg_exit()!= 0)return 1;

  return 0;
}


int (print_xpm) (xpm_map_t xpm, uint16_t x, uint16_t y){

    xpm_image_t image;

  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &image);

  for(int j = 0; j < image.height; j++){
    for(int i = 0; i < image.width; i++){
      if(vg_draw_pixel(x+i,y+j,*colors)!=0) return 1;
      colors ++;
    }
  }
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  if(set_frame_buffer(0x105)!= 0) return 1;
  if(set_graphic_mode(0x105)!= 0) return 1;

  if(print_xpm(xpm,x,y)!= 0) return 1;

  if(waiting_ESC_key()!= 0) return 1;
  if(vg_exit()!= 0) return 1;
  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  int ipc_status;
  message msg;
  uint8_t irq_kb, irq_timer;

  uint8_t verticalDirection = 0;
  if(xi == xf){verticalDirection = 1;}
  if(xi != xf && yi != yf) return 1;

  if(timer_subscribe_int(&irq_timer) != 0) return 1;
  if(keyboard_subscribe_interrupts(&irq_kb)!= 0) return 1;

  if(timer_set_frequency(0, fr_rate)!= 0)return 1;
  if(set_frame_buffer(0x105)!= 0) return 1;
  if(set_graphic_mode(0x105)!= 0) return 1;

  if(print_xpm(xpm, xi, yi)!= 0)return 1;

  while(scancode!= BREAK_ESC && (xi < xf || yi < yf)){
      if( (driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
      if (msg.m_notify.interrupts & irq_kb) { 
        kbc_ih();
      }
      if (msg.m_notify.interrupts & irq_timer) { 
        if(draw_rectangle(xi,yi, xf-xi, yf-yi, 0xFFFFFF)!= 0) return 1;

        if(verticalDirection){
          yi += speed;
          if(yi > yf) yi = yf;
        }
        else{
          xi +=speed;
          if(xi>xf) xi = xf;
        }
        if(print_xpm(xpm,xi,yi)!= 0) return 1;
      }
      }
    }



  }

  if(vg_exit()!=0) return 1;
  if(timer_unsubscribe_int()!=0) return 1;
  if(keyboard_unsubscribe_interrupts()!= 0) return 1;

  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
