
#include "Game.h"

uint8_t k_index = 0;
uint8_t k_bytes[2];
extern uint8_t k_scancode;

extern int m_index;
extern uint8_t m_bytes[3];
extern struct packet m_packet;


int (gameLogic) () {
    initialize_buffers();

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

    make_xpm((xpm_map_t) maze2,1,1);

    Sprite *sp;
    sp = create_sprite((xpm_map_t)right1, 20, 20, 0, 0);
    drawing_sprite(sp);

    update_frame();
    clear_drawing();
    
    while (k_scancode != SCAN_BREAK_ESC) {    
     
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      int count = 0;
      
      if (is_ipc_notify(ipc_status)) {      
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:    
            if (msg.m_notify.interrupts & irq_set_keyboard) { 
                count++;
                kbc_ih();
                handle_ingame_scancode(k_scancode, sp);
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

    free_buffers();

    return 0;
}

/*enum SpriteState get_next_sprite(enum SpriteState current_state, uint8_t scancode) {
    switch (scancode) {
        case A_KEY_MK:
            return LEFT1;
        case D_KEY_MK:
            return RIGHT1;
        case W_KEY_MK:
            return UP1;
        case S_KEY_MK:
            return DOWN1;
        case A_KEY_BRK:
            return LEFT2;
        case D_KEY_BRK:
            return RIGHT2;
        case W_KEY_BRK:
            return UP2;
        case S_KEY_BRK:
            return DOWN2;
        default:
            return current_state;
    }
}*/

xpm_map_t get_next_sprite(xpm_map_t current_state, uint8_t scancode) {
    switch (scancode) {
        case A_KEY_MK:
            return (xpm_map_t)left1;
        case D_KEY_MK:
            return (xpm_map_t)right1;
        case W_KEY_MK:
            return (xpm_map_t)up1;
        case S_KEY_MK:
            return (xpm_map_t)down1;
        case A_KEY_BRK:
            return (xpm_map_t)left2;
        case D_KEY_BRK:
            return (xpm_map_t)right2;
        case W_KEY_BRK:
            return (xpm_map_t)up2;
        case S_KEY_BRK:
            return (xpm_map_t)down2;
        default:
            return (xpm_map_t)current_state;
    }
}

/*A personagem pinta o fundo enquanto anda -- dar fix*/

void handle_ingame_scancode(uint8_t scancode, Sprite *player) {
    //static enum SpriteState current_state = RIGHT1;
    //enum SpriteState next_state = get_next_sprite(current_state, scancode);
    /*Falta trocar de sprite quando anda para o lado*/
    //Sprite *sp;
    
    switch (scancode) {
        case D_KEY_MK:
            player->x = player->x + 1;
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, D_KEY_MK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;

        case A_KEY_MK:
            player->x = player->x - 1;
           // sp = create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, A_KEY_MK), 20, 20, 0, 0);
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, A_KEY_MK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;

        case W_KEY_MK:
            player->y = player->y- 1;
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, W_KEY_MK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;
        
        case S_KEY_MK:
            player->y = player->y + 1;
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, S_KEY_MK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;
        case A_KEY_BRK:
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, A_KEY_BRK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
        case D_KEY_BRK:
           // player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, D_KEY_BRK), player->x+1, player->y, player->xspeed, player->yspeed);
           // drawing_sprite(player);
            break;

        default:
            return;
    }
    clear_drawing();
    make_xpm((xpm_map_t) maze2,1,1);
    drawing_sprite(player);
    update_frame();
}

