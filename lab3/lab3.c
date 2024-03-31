#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include "keyboard.h"
#include <stdbool.h>
#include <stdint.h>

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

extern int scancode;
extern int count_sysinb;
uint8_t i=0;
uint8_t bytes[2];

//read the scancodes from the KBC using an IH
int(kbd_test_scan)() {
 int ipc_status;
 message msg;
 int r;
 uint8_t irq_set;

 //So that IH knows that a DD is interested in an interrupt
 if(KBD_subscribe_int(&irq_set)!=0){
  return 1;
 }

 //DD receives the notification of the IH about an interrupt
 while(scancode!=BREAK_ESC) { /* You may want to use a different condition */
    /* Get a request message. */
    if((r=driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                  //Upon an interrupt, read the scancode from the OUT_BUF
                  kbc_ih();
                  if(scancode==SCAN_ONE_TWO){
                    bytes[i]=scancode; 
                    i++;
                    continue;
                  }
                  bytes[i]=scancode;
                  kbd_print_scancode(!(scancode&BIT(7)), i==0 ? 1 : 2 , bytes);
                  i=0;
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }
  if(KBD_unsubscribe_int()!=0){
    return 1;
  }
  if(kbd_print_no_sysinb(count_sysinb)!=0){ 
    return 1;
  }
  return 0;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
