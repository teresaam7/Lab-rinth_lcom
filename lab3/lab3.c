#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "keyboard.h"
#include <stdbool.h>
#include <stdint.h>

uint8_t extern scancode;
int extern count_sysinb;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t irq_set;
  if(keyboard_subscribe_int(&irq_set)!= 0) return 1;

  int ipc_status; int r;
  message msg;
  while( scancode != BC_ESC ) { /* You may want to use a different condition */
    /* Get a request message. */
    if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
    printf("driver_receive failed with: %d", r);
    continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
    switch (_ENDPOINT_P(msg.m_source)) {
    case HARDWARE: /* hardware interrupt notification */
    if (msg.m_notify.interrupts & irq_set) { /* subscri*/ 
      kbc_ih();
      unsigned char size = 1;
      bool make = true;
      if(scancode == SIZE2_SCAN) size = 2;
      if((scancode & BREAK_CODE) != 0) make = false;
            kbd_print_scancode(make, size, &scancode);
    }
    break;
    default:
    break; /* no other notifications expected: do nothing*/
    }
    } else { /* received a standard message, not a notification*/
    /* no standard messages expected: do nothing */
    }

  }
  if(keyboard_unsubscribe_int()!= 0) return 1;
  if(kbd_print_no_sysinb(count_sysinb)!= 0)return 1;
  return 0;
}

int(kbd_test_poll)() { // ESTÁ A DAR MAL NAO SEI PQ ;-;
  while(scancode != BC_ESC){
    if (keyboard_read_scancode(OUT_BUF, &scancode,0) == 0) {
      kbd_print_scancode(!(scancode & BREAK_CODE), scancode == SIZE2_SCAN  ? 2 : 1, &scancode);
    }
  }
  if(kbd_print_no_sysinb(count_sysinb)!= 0)return 1;
  return keyboard_enable_interrupts();
  
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}




