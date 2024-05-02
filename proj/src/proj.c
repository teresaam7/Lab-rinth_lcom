#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "Game.h"

extern uint8_t scancode;

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
  int ipc_status, r;
  message msg;
  uint8_t irq_set_kbc;
  
  if (keyboard_subscribe_int(&irq_set_kbc) != 0) {
    return 1;
  }

  while (scancode != BC_ESC) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_kbc) { // subscribed interrupt
            kbc_ih();
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
  }

  if (keyboard_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}


int (proj_main_loop)(int argc, char *argv[]) {
  if (frame_buffer_func(0x118) != 0) {
    return 1;
  }

  if (graphics_mode(0x118) != 0) {
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
  return 0;
}

