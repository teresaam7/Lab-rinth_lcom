#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"

uint8_t i = 0;
uint8_t bytes[2];
extern uint8_t scancode;
extern uint32_t count_sysinb;

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
  uint8_t irq_set = 0;
  if (keyboard_subscribe_int(&irq_set) != 0)
    return 1;

  int r;
  message msg;
  int ipc_status;
  while (scancode != SCAN_BREAK_ESC) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                  kbc_ih();
                  if (scancode == SCAN_TWO_B) {
                    bytes[i] = scancode; i++;
                    continue;
                  }
                  bytes[i] = scancode;
                  kbd_print_scancode(!(scancode & MAKE_OR_BREAK), i == 0? 1: 2 , bytes);
                  i = 0;
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    }
  }

  if (keyboard_unsubscribe_int() != 0)
    return 1;

  if (kbd_print_no_sysinb(count_sysinb) != 0)
    return 1;
    
  return 0;
}

int(kbd_test_poll)() {
  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  return 1;
}
