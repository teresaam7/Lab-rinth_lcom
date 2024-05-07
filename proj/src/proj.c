#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "Game.h"

uint8_t k_index = 0;
uint8_t k_bytes[2];
extern uint8_t k_scancode;

extern int m_index;
extern uint8_t m_bytes[3];
extern struct packet m_packet;

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

int (receive_ESC)() {
  uint8_t irq_set_keyboard;
    if (keyboard_subscribe_int(&irq_set_keyboard) != 0)
        return 1;
    
    int r;
    message msg;
    int ipc_status;
    while (k_scancode != SCAN_BREAK_ESC) {    
      /* Get a request message. */
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }
      
      if (is_ipc_notify(ipc_status)) {      /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:    /* hardware interrupt notification */
            if (msg.m_notify.interrupts & irq_set_keyboard) { 
                kbc_ih();
                if (k_scancode == SCAN_FIRST_TWO) {
                    k_bytes[k_index] = k_scancode; k_index++;
                } else {
                    k_bytes[k_index] = k_scancode;
                    // kbd_print_scancode(!(scancode & MAKE_OR_BREAK), i == 0? 1: 2 , bytes);
                    k_index = 0;
                }
            }

            break;

          default:
            break; 
        }
      }
    }

    if (keyboard_unsubscribe_int() != 0)
        return 1;

  return 0;
}


int (proj_main_loop)(int argc, char *argv[]) {
  if (initialize_frame_buffer(0x115) != 0) {
    return 1;
  }

  if (graphic_mode(0x115) != 0) {
    return 1;
  }

  if (gameLogic()!= 0) {
    return 1;
  }

  if (receive_ESC() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    return 1;
  }
  //free_buffers();
  return 0;
}

