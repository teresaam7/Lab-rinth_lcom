// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "mouse.h"

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

int hook_id=2;
extern uint8_t byte;
extern int index_counter; 
extern struct packet pp;

int (mouse_test_packet)(uint32_t cnt) {
    int ipc_status;
    message msg;
    int r;
    uint8_t irq_set;

    irq_set=BIT(hook_id);
    if(sys_irqsetpolicy(12 ,IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id)!=0){return 1;}//So that IH knows that a DD is interested in an interrupt
    if(mouse_enable_data_reporting()!=0){return 1;}
    //if(mouse_write(0xF4)!=0){return 1;}
   
    //DD receives the notification of the IH about an interrupt
    while(cnt) {//reads only one byte per interrupt
    // Get a request message. 
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
      
      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
            if (msg.m_notify.interrupts & irq_set) { // subscri... process it 
                mouse_ih();
            //    if((byte&0xFF)!=0){ -> nao pode ser
                  // pp.bytes[counter]=byte; -> nao pq temos de verificar se bit 3 ativo
                 //  index_counter++; -> incrementa 2x
                   mouse_sync_bytes(&index_counter);        
                   if(index_counter==3){
                     mouse_build_packet();
                     index_counter=0;
                     mouse_print_packet(&pp);
                     cnt--;
                   }
                }
              //  }
            break;

          default:
            break; 
        }
      }
    }

   // if (mouse_write(0xF5) != 0) return 1;
    if(sys_irqrmpolicy(&hook_id)!=0){return 1;} 

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    //can be easily written by combining code from mouse_test_packet() and kbd_test_timed_scan()
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
