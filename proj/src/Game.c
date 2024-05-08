
#include "Game.h"


/*
uint8_t k_index = 0;
uint8_t k_bytes[2];
extern uint8_t k_scancode;

extern int m_index;
extern uint8_t m_bytes[3];
extern struct packet m_packet;


int (gameLogic) () {
    make_xpm((xpm_map_t) maze2,1,1);

    Sprite *sp;
    sp = create_sprite((xpm_map_t)right1, 20, 20, 0, 0);
    drawing_xpm(sp);
    destroy_sprite(sp); 

    return 0;
}


*/



int (gameLogic) () {
    make_xpm((xpm_map_t) maze2,1,1);

    Sprite *sp;
    sp = create_sprite((xpm_map_t)right1, 20, 20, 0, 0);
    drawing_xpm(sp);
    destroy_sprite(sp); 

    return 0;
}

void handle_ingame_scancode(uint8_t scancode, Sprite *player) {
  if (!check_collision(player, '.', 810, 594)) {
    switch (scancode) {
        case D_KEY_MK:
            player->x += 1;
            break;

        case A_KEY_MK:
            player->x -= 1;
            break;

        case W_KEY_MK:
            player->y -= 1;
            break;
        
        case S_KEY_MK:
            player->y += 1;
            break;
        case A_KEY_BRK:
        case D_KEY_BRK:
           
            break;

        default:
            return;
    }
  }
}



/*
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
     
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }
      
      if (is_ipc_notify(ipc_status)) {      
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:    /
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

            if (msg.m_notify.interrupts & irq_set_mouse) { 
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
*/
