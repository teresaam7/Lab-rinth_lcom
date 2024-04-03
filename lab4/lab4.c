// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "timer.c"

extern int counter;
extern uint8_t bIndex; 
extern struct packet mPacket;

typedef enum {
  START,
  UP,
  VERTEX,
  DOWN,
  END
} states;

states state = START;
uint16_t len = 0;


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
  int ipc_status;
  message msg;
  int r;
  uint8_t mouse_mask; 

   if (mouse_write(ENABLE_STRMODE) != 0) {
    return 1;
  }

  if (mouse_subscribe_int(&mouse_mask) != 0) {
    return 1;
  }

  while (cnt) { 
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 
          if (msg.m_notify.interrupts & mouse_mask){  
            mouse_ih();                                           
            if (bIndex == 3) { 
              mouse_init();                
              packet_contruction();       // Create packet        
              mouse_print_packet(&mPacket);
              cnt--;  
            }
          }
          break;
      }
    }
  }

  if (mouse_unsubscribe_int() != 0) {
    return 1;
  } 

  if (mouse_write(DISABLE) != 0) {
    return 1;
  }
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  int ipc_status;
  message msg;
  int r;
  uint8_t sec = 0;
  uint8_t mouse_mask = 0, timer_mask = 0; 
  uint16_t timerFreq = sys_hz();

  if (mouse_write(ENABLE_STRMODE) != 0) {
    return 1;
  } 

  if (mouse_subscribe_int(&mouse_mask) != 0) {
    return 1;
  }
  if (timer_subscribe_int(&timer_mask) != 0) {
    return 1;
  }

  while (sec < idle_time) { 

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 

          if (msg.m_notify.interrupts & timer_mask) { 
            timer_int_handler();
            if (counter % timerFreq == 0) {
              sec++;
            }
          }

          if (msg.m_notify.interrupts & mouse_mask){  
            mouse_ih();                                                   
            if (bIndex == 3) { 
              mouse_init();             
              packet_contruction();                // Create packet
              mouse_print_packet(&mPacket);      
            }
            sec = 0;
            counter = 0;
          }
      }
    }
  }
  if (timer_unsubscribe_int() != 0) {
    return 1;
  }
  if (mouse_unsubscribe_int() != 0) {
    return 1;
  }
  if (mouse_write(DISABLE) != 0) {
    return 1;
  }

  return 0;
}

void switch_state(uint8_t tolerance) {
    switch (state) {
        case START:
            if (mPacket.lb && !mPacket.rb && !mPacket.mb) {
                state = UP;
            }
            break;

        case UP:
            if ((mPacket.lb && mPacket.rb && !mPacket.mb) || (mPacket.lb && !mPacket.rb && mPacket.mb)) {
                state = VERTEX;
            } else if (!mPacket.lb && !mPacket.rb && !mPacket.mb) {
                state = START;
            }
            break;

        case VERTEX:
            if ((mPacket.lb && mPacket.rb && mPacket.mb) || (!mPacket.lb && !mPacket.rb && mPacket.mb)) {
                state = DOWN;
            } else if (!mPacket.lb && !mPacket.rb && !mPacket.mb) {
                state = END;
            }
            break;

        case DOWN:
            if ((mPacket.lb && !mPacket.rb && !mPacket.mb) || (!mPacket.lb && !mPacket.rb && !mPacket.mb)) {
                state = VERTEX;
            } else if (!mPacket.lb && !mPacket.rb && mPacket.mb) {
                state = END;
            }
            break;

        case END:
            break;
    }
}


int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status;
  int r;
  message msg;
  uint8_t mouse_mask;

  if (mouse_write(ENABLE_STRMODE) != 0) {
    return 1;
  }

  if (mouse_subscribe_int(&mouse_mask) != 0) {
    return 1;
  }

  while (state != END) { 
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 
          if (msg.m_notify.interrupts & mouse_mask){  
            mouse_ih();                                                                        
            if (bIndex == 3) {  
              mouse_init();                         
              packet_contruction();                                      
              switch_state(tolerance);      
              bIndex = 0;
            }
          }
          break;
      }
    }
  }

  if  (mouse_unsubscribe_int() != 0) {  
    return 1;
  }

  if (mouse_write(DISABLE) != 0) {
    return 1;
  }

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    while (cnt) { 
        if (mouse_write(M_READ) != 0) {
          return 1;
        } 
        mouse_ih();                                                                       
        if (bIndex == 3) {      
            mouse_init();                  
            packet_contruction();                     
            mouse_print_packet(&mPacket);
            cnt--;
            tickdelay(micros_to_ticks(period * 1000));    
        }
    }

    if (mouse_write(ENABLE_STRMODE) != 0) {
      return 1;
    }
    if (mouse_write(DISABLE) != 0) {
      return 1;
    }
  
    uint8_t command = minix_get_dflt_kbc_cmd_byte();       

    if (write_scancode(IN_CMD, OUT_BUF) != 0) {
      return 1; 
    } 
    if (write_scancode(OUT_BUF, command) != 0) {
      return 1;
    }
    return 0;
}


