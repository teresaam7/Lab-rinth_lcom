// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "graphics.h"
#include "keyboard.c"
#include "utils.c"
#include "timer.c"

// Any header files included below this line should have been created by you

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

int hook_id_keyboard=1;
int hook_id_timer=0;
extern vbe_mode_info_t modeinfo;
int (ESC_break)() {
  int ipc_status;
  message msg;
  uint8_t irq_keyboard;
  irq_keyboard=BIT(hook_id_keyboard);
  if(sys_irqsetpolicy(1, IRQ_EXCLUSIVE|IRQ_REENABLE, &hook_id_keyboard)!= 0) return 1;

  while (scancode != 0x81){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & irq_keyboard) 
            kbc_ih();
            break;
        default:
          break; 
      }
    }
  }

  if (sys_irqrmpolicy(&hook_id_keyboard) != 0) return 1;
  return 0;
}

//When this function is invoked, your program should change to the video mode specified in its mode argument.
//After delay seconds, it should go back to Minix's default text mode.
int(video_test_init)(uint16_t mode, uint8_t delay) {
  if(graphics_mode(mode)!=0){return 1;}
  
  //tickdelay(micros_to_ticks(delay));//para unidades inferior ao segundo
  sleep(delay);//para segundos

  //Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
  if(vg_exit()!=0){
    return 1;
  }
  return 0;
}

// Draw a rectangle on the screen in the desired mode
int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

    if(frame_buffer_func(mode)!=0){return 1;}
    if(graphics_mode(mode)!=0){return 1;}
    //if(frame_buffer_func(mode)!=0){return 1;}
    
    if(vg_draw_rectangle(x, y, width, height, color)!=0){
        return 1;
    }

    if(ESC_break()!=0){return 1;}//acaba quando esc break pressionado

    if(vg_exit()!=0){
    return 1;
    }
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if(frame_buffer_func(mode)!=0){return 1;}//tem de ser antes do graphics por causa dos testes
  if(graphics_mode(mode)!=0){return 1;}
  printf("olaaaaaa");
  printf("%x",modeinfo.MemoryModel);
  
  uint16_t width=modeinfo.XResolution/no_rectangles;	//length in pixels of the rectangle along the horizontal direction
  uint16_t height=modeinfo.YResolution/no_rectangles;
  uint32_t color;
  for(uint8_t i=0; i<no_rectangles; i++){//linha
    for(uint8_t j=0; j<(int)no_rectangles; j++){ //coluna
        if(modeinfo.MemoryModel!= 0x06){//indexed mode. se for ==0x105 nao da
        color=(first + (i * no_rectangles + j) * step) % (1 << modeinfo.BitsPerPixel); 
        }
        else{ //directed mode. equivalente a else if modeinfo.MemoryModel==0x06
           //printf("%x",modeinfo.RedFieldPosition);
           //printf("%x",modeinfo.RedMaskSize);
           uint32_t red=(R(first) + j * step) % (1 << modeinfo.RedMaskSize); 
           uint32_t green=(G(first) + i * step) % (1 << modeinfo.GreenMaskSize); 
           uint32_t blue=(B(first) + (j + i) * step) % (1 << modeinfo.BlueMaskSize); 
           color=(red<<modeinfo.RedFieldPosition) | (green<<modeinfo.GreenFieldPosition) | (blue<<modeinfo.BlueFieldPosition);
        }
        if(vg_draw_rectangle((uint16_t) j*width, (uint16_t) i*height, width, height, color)!=0){return 1;}
    }
  }

  if(ESC_break()!=0){return 1;}//acaba quando esc break pressionado
  if(vg_exit()!=0){
    return 1;
  }
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if(frame_buffer_func(0x105)!=0){return 1;}
  if(graphics_mode(0x105)!=0){return 1;}

  //load_xpm() that is provided by the LCF to convert an XPM into a pixmap
  xpm_image_t img;
  uint8_t *map;
  //retorna um apontador para um array de cores preenchido de acordo com o XPM
  map=xpm_load(xpm, XPM_INDEXED, &img);
  for(uint8_t i=0; i<img.height; i++){
    for(uint8_t j=0; j<img.width; j++){
        if(vg_draw_pixel(x+j, y+i, *map)!=0){return 1;};
        map++;//proxima cor
    }
  }

  if(ESC_break()!=0){return 1;}//acaba quando esc break pressionado

  if(vg_exit()!=0){
    return 1;
  }
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

    if(frame_buffer_func(0x105)!=0){return 1;}
    if(graphics_mode(0x105)!=0){return 1;}

    int move_vertical;//1 se movimento vertical
    if((xi==xf)&&(yi<yf)){move_vertical=1;}
    else if((yi==yf)&&(xi<xf)){
        move_vertical=0;
    }
    make_xpm(xpm, xi, yi);

    int r;
    int ipc_status;
    message msg;
    uint8_t irq_timer;
    uint8_t irq_keyboard;
    irq_timer=BIT(hook_id_timer);
    irq_keyboard=BIT(hook_id_keyboard);
    if(sys_irqsetpolicy(0, IRQ_REENABLE, &hook_id_timer)!=0){return 1;}
    if(sys_irqsetpolicy(1, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id_keyboard)!=0){return 1;}
    // Atualiza o sistema para a frame rate dada
    if (timer_set_frequency(0, fr_rate) != 0) return 1; 

    while((scancode!=0x81)&&((xi<xf)||(yi<yf)) ) { // You may want to use a different condition 
       // Get a request message. 
       if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
       printf("driver_receive failed with: %d", r);
       continue;
       }
       if (is_ipc_notify(ipc_status)) { //received notification 
       switch (_ENDPOINT_P(msg.m_source)) {
       case HARDWARE: // hardware interrupt notification 
       if (msg.m_notify.interrupts & irq_keyboard) { // subscri
           kbc_ih();
       }
        if (msg.m_notify.interrupts & irq_timer) { // subscri
           // Apaga o desenho anterior para dar a sensação de movimetno
           if (vg_draw_rectangle(xi, yi, 100, 100, 0xFFFFFF) != 0) return 1;
           if(move_vertical==0){
            xi+=speed;
            make_xpm(xpm, xi, yi);
            }
           else{
            yi+=speed;
            make_xpm(xpm, xi, yi);
           }
        }
         break;
       default:
         break; // no other notifications expected: do nothi
       }
       } else { // received a standard message, not a notification
         // no standard messages expected: do nothing 
       }
    }

    if(sys_irqrmpolicy(&hook_id_timer)!=0){return 1;}
    if(sys_irqrmpolicy(&hook_id_keyboard)!=0){return 1;}
  //  if(ESC_break()!=0){return 1;}//acaba quando esc break pressionado
    if(vg_exit()!=0){
    return 1;
    }
  return 0;
}

//NAO FAZERRRRRRRRR
int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
