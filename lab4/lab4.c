// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <mouse.h>
#include "timer.c"

extern struct packet packet_mouse;
extern uint8_t index_byte;
uint32_t packets_count = 0;

uint8_t time_count = 0;
extern int counter;

typedef enum {
  START,
  UP_LINE,
  VERTEX,
  DOWN_LINE,
  END
} State;
State state = START;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    if (write_mouse(ENABLE_DATA_MODE) != 0)
      return 1;

    uint8_t irq_set_mouse;
    if (mouse_subscribe_int(&irq_set_mouse) != 0)
      return 1;
    
    int r;
    message msg;
    int ipc_status;
    while (packets_count != cnt) { 
      /* Get a request message. */
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }
      
      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
            if (msg.m_notify.interrupts & irq_set_mouse) { /* process it */
              mouse_ih();

              if (index_byte == 3) {
                store_bytes_packet();
                mouse_print_packet(&packet_mouse);
                packets_count++;
                index_byte = 0;
              }
            }
            break;

          default:
            break; 
        }
      }
    }

    if (mouse_unsubscribe_int() != 0)
      return 1;

    if (write_mouse(DISABLE_DATA_MODE) != 0)
      return 1;

    return 0;
}


int (mouse_test_async)(uint8_t idle_time) {
    if (write_mouse(ENABLE_DATA_MODE) != 0)
      return 1;

    uint8_t irq_set_mouse;
    if (mouse_subscribe_int(&irq_set_mouse) != 0)
      return 1;

    uint8_t irq_set_timer;
    if (timer_subscribe_int(&irq_set_timer) != 0)
      return 1;
    
    int r;
    message msg;
    int ipc_status;
    int freq = sys_hz();
    while (time_count < idle_time) { 
      /* Get a request message. */
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }
      
      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
            if (msg.m_notify.interrupts & irq_set_mouse) { /* process it */
              mouse_ih();

              if (index_byte == 3) {
                store_bytes_packet();
                mouse_print_packet(&packet_mouse);
                packets_count++;
                index_byte = 0;
                counter = 0;
                time_count = 0;
              }
            }
            if (msg.m_notify.interrupts & irq_set_timer) {
              timer_int_handler();
              if (counter > freq) {
                counter = 0;
                time_count++;
              } 
            }
            break;

          default:
            break; 
        }
      }
    }

    if (mouse_unsubscribe_int() != 0)
      return 1;

    if (timer_unsubscribe_int() != 0)
      return 1;

    if (write_mouse(DISABLE_DATA_MODE) != 0)
      return 1;

    return 0;
}


void (state_machine)() {

  switch (state) {
    case START:
      o
      break;

    case UP_LINE:
      o
      break;

    case VERTEX:
      o
      break;

    case DOWN_LINE:
      o
      break;

    case END:
      o
      break;

    default: 
      break;
  }

}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    if (write_mouse(ENABLE_DATA_MODE) != 0)
      return 1;

    uint8_t irq_set_mouse;
    if (mouse_subscribe_int(&irq_set_mouse) != 0)
      return 1;
    
    int r;
    message msg;
    int ipc_status;
    while ( state != END ) { 
      /* Get a request message. */
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }
      
      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
            if (msg.m_notify.interrupts & irq_set_mouse) { /* process it */
              mouse_ih();

              if (index_byte == 3) {
                store_bytes_packet();
                mouse_print_packet(&packet_mouse);
                
                packets_count++;
                index_byte = 0;
              }
            }
            break;

          default:
            break; 
        }
      }
    }

    if (mouse_unsubscribe_int() != 0)
      return 1;

    if (write_mouse(DISABLE_DATA_MODE) != 0)
      return 1;

    return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
