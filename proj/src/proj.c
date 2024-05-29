#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "logic.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint8_t k_scancode; 

int r;
message msg;
int ipc_status;
int gameTime = 60 * TIMER_MINUTES;
bool gameState_change = true;
GameState gameState;
bool multi = false;


uint8_t irq_set_keyboard;
uint8_t irq_set_mouse;
uint8_t irq_set_timer;
uint8_t irq_set_rtc;
uint8_t irq_set_sp;

extern int m_index;
extern uint8_t m_bytes[3];
extern struct packet m_packet;
extern vbe_mode_info_t mode_info;


int (proj_main_loop)(int argc, char *argv[]) {
  if (initialize_frame_buffer(0x115) != 0) {
    return 1;
  }

  if (graphic_mode(0x115) != 0) {
    return 1;
  }

  initialize_buffers();

  if (loadSprites()) {
    return 1;
  }
  initialize_sp();
  bool running = true;

  if (write_mouse(ENABLE_DATA_MODE) != 0)
    return 1;

  if (keyboard_subscribe_int(&irq_set_keyboard) != 0)
    return 1;

  if (mouse_subscribe_int(&irq_set_mouse) != 0)
    return 1;

  if (timer_subscribe_int(&irq_set_timer) != 0)
    return 1;

  if (rtc_subscribe_int(&irq_set_rtc) != 0)
    return 1;

  if (sp_subscribe_int(&irq_set_sp) != 0)
    return 1;

  sp_ih();
  gameState = MENU;
  draw_menu();

  while(k_scancode != SCAN_BREAK_ESC && running){
    if(gameState_change){
      gameStateInit(&running);
    }

    if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {      
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:   
          if (msg.m_notify.interrupts & irq_set_keyboard) { 
            kbc_ih();
            keyboardLogic();
          }

          if (msg.m_notify.interrupts & irq_set_mouse) {
            mouse_ih(); 
            if (m_index == 3) {
              store_bytes_packet();
              m_index = 0;
              mouse_print_packet(&m_packet);
              mouseLogic();
            }
          }

          if ( msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler(); 
            timerLogic();
          }
          if (msg.m_notify.interrupts & irq_set_sp) {
            sp_handler();
          }
          break;
        default: break;
      } 
    } 
  }
      
    if (keyboard_unsubscribe_int() != 0)
      return 1;

    if (mouse_unsubscribe_int() != 0)
      return 1;

    if (timer_unsubscribe_int() != 0)
      return 1;

    if (rtc_unsubscribe_int() != 0)
      return 1;

    if (sp_unsubscribe_int() != 0)
      return 1;

    if (write_mouse(DISABLE_DATA_MODE) != 0)
      return 1;

  if (vg_exit() != 0) {
    return 1;
  }

  free_buffers();

  return 0;

}
