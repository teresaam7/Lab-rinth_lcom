
#include "Game.h"

uint8_t k_index = 0;
uint8_t k_bytes[2];
extern uint8_t k_scancode;

extern int m_index;
extern uint8_t m_bytes[3];
extern struct packet m_packet;


int (gameLogic) () {
    loadMazeXPM();
    return 0;
}

void (loadMazeXPM) () {
    xpm_image_t img;
    uint8_t *result = xpm_load(maze1, XPM_8_8_8_8, &img);
    if (result == NULL) {
        printf("Error loading maze XPM\n");
    }
}

int(kbc_interrupts)() {
    if (write_mouse(ENABLE_DATA_MODE) != 0)
      return 1;

    uint8_t irq_set_keyboard;
    if (keyboard_subscribe_int(&irq_set_keyboard) != 0)
        return 1;

    uint8_t irq_set_mouse;
    if (mouse_subscribe_int(&irq_set_mouse) != 0)
      return 1;
    
    int r;
    message msg;
    int ipc_status;
    while (1) {     // CHANGEEEEEEEEEEEEEEEEEEEE
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
                    k_bytes[k_index] = scancode;
                    // kbd_print_scancode(!(scancode & MAKE_OR_BREAK), i == 0? 1: 2 , bytes);
                    k_index = 0;
                }
            }

            if (msg.m_notify.interrupts & irq_set_mouse) { /* process it */
                mouse_ih();

                if (m_index == 3) {
                    store_bytes_packet();
                    mouse_print_packet(&m_packet);
                    m_index = 0;
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

    if (mouse_unsubscribe_int() != 0)
      return 1;

    if (write_mouse(DISABLE_DATA_MODE) != 0)
      return 1;

    return 0;

}
