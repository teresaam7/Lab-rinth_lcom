
#include "Game.h"

uint8_t k_index = 0;
uint8_t k_bytes[2];
extern uint8_t k_scancode;
extern int counter;

extern int m_index;
extern uint8_t m_bytes[3];
extern struct packet m_packet;
extern vbe_mode_info_t mode_info;

Sprite *sp,*start, *quit, *cursor, *life, *level1_, *level2_, *level3_;

int (collision)(Sprite * sp1, Sprite * sp2){
  if(sp1->x < sp2->x || sp1 -> x > sp2->x + sp2->width) return 0;
  if(sp1->y < sp2->y || sp1 -> y > sp2->y + sp2->height) return 0;
  return 1;
}

void (draw_life_bar)(GameState gameState, Sprite **bar, int total_seconds) {
    switch(total_seconds){
        case 30:
            *bar = create_sprite((xpm_map_t)life5, 610, 5, 0, 0);
            drawing_sprite(*bar);
            break;
        case 60:
            *bar = create_sprite((xpm_map_t)life4, 610, 5, 0, 0);
            drawing_sprite(*bar);
            break;
        case 90:
            *bar = create_sprite((xpm_map_t)life3, 610, 5, 0, 0);
            drawing_sprite(*bar);
            break;
        case 140:
            *bar = create_sprite((xpm_map_t)life2, 610, 5, 0, 0);
            drawing_sprite(*bar);
            break;
        default:
            return;
    }
    clear_drawing();
    drawing_sprite(*bar);
    update_frame_with_background();
}

void (drawLevel) (GameState gameState, int x, int y, int width, int height) {
  uint8_t hours, minutes, seconds;
  get_game_time(&hours, &minutes, &seconds);

  switch(gameState) {
    case LEVEL1: 
      if (hours >= 6 && hours < 16) {
        background_drawing((xpm_map_t) mazeDay1, x, y, width, height); 
      } else if (hours >= 20 && hours < 6) {
        background_drawing((xpm_map_t) mazeDark1, x, y, width, height);
      }
      else {
        background_drawing((xpm_map_t) maze1, x, y, width, height);
      }
      break;
    case LEVEL2: 
      if (hours >= 6 && hours < 16) {
        background_drawing((xpm_map_t) mazeDay2, x, y, width, height); 
      } else if (hours >= 20 && hours < 6) {
        background_drawing((xpm_map_t) mazeDark2, x, y, width, height);
      }
      else {
        background_drawing((xpm_map_t) maze2, x, y, width, height);
      }
      break;
    case LEVEL3: 
      if (hours >= 6 && hours < 16) {
        background_drawing((xpm_map_t) mazeDay3, x, y, width, height); 
      } else if (hours >= 20 && hours < 6) {
        background_drawing((xpm_map_t) mazeDark3, x, y, width, height);
      }
      else {
        background_drawing((xpm_map_t) maze3, x, y, width, height);
      }
      break;
    default:
      return;
  }
}

void (draw_game_menu)() {
  drawing_xpm((xpm_map_t) menu,1,1);
  level1_ = create_sprite((xpm_map_t)level1, 315, 260, 0, 0);
  level2_ = create_sprite((xpm_map_t)level2, 315, 340, 0, 0);
  level3_ = create_sprite((xpm_map_t)level3, 315, 420, 0, 0);
  drawing_sprite(level1_);
  drawing_sprite(level2_);
  drawing_sprite(level3_);
}

void (draw_game)(){
  sp = create_sprite((xpm_map_t)right1, 20, 20, 0, 0);
  life = create_sprite((xpm_map_t)life1, 610, 5, 0, 0);
  drawing_sprite(sp);
  drawing_sprite(life);
}

void (draw_menu)(){
  drawing_xpm((xpm_map_t) menu,1,1);
  
  cursor = create_sprite((xpm_map_t)hand, 315, 200, 0, 0);
  start = create_sprite((xpm_map_t)start_button, 315, 300, 0, 0);
  quit = create_sprite((xpm_map_t)quit_button, 335, 380, 0, 0);
  drawing_sprite(start);
  drawing_sprite(quit);
  drawing_sprite(cursor);
}


void (draw_win)() {
  drawing_xpm((xpm_map_t) win,1,1);
  display_game_time();
}

void (draw_lost)() {
  drawing_xpm((xpm_map_t) win,1,1);
}

int (gameLogic) (GameState *gameState, bool * running) {
    initialize_buffers();

    if (write_mouse(ENABLE_DATA_MODE) != 0)
      return 1;

    uint8_t irq_set_keyboard;
    if (keyboard_subscribe_int(&irq_set_keyboard) != 0)
        return 1;

    uint8_t irq_set_mouse;
    if (mouse_subscribe_int(&irq_set_mouse) != 0)
      return 1;

    uint8_t irq_set_timer;
    if (timer_subscribe_int(&irq_set_timer) != 0)
      return 1;

    uint8_t irq_set_rtc;
    if (rtc_subscribe_int(&irq_set_rtc) != 0)
      return 1;
    
    int r;
    message msg;
    int ipc_status;

    if(*gameState == GAME){draw_game_menu();}
    if(*gameState == LEVEL1){draw_game();}
    if(*gameState == LEVEL2){draw_game();}
    if(*gameState == LEVEL3){draw_game();}
    if(*gameState == MENU){draw_menu();}

    update_frame_with_background();
    clear_drawing();

    bool gameState_change = false;
    int time = 60 * TIMER_MINUTES;
    while (k_scancode != SCAN_BREAK_ESC) { 

      if(gameState_change){
        if(*gameState == GAME) {draw_game_menu();}
        if(*gameState == MENU) {draw_menu();}
        if(*gameState == WIN) {draw_win();}
        if(*gameState == LEVEL1){draw_game();}
        if(*gameState == LEVEL2){draw_game();}
        if(*gameState == LEVEL3){draw_game();}
        if(*gameState == EXIT) {*running = false;
        break;}
        update_frame_with_background();
        clear_drawing();
        gameState_change = false;
      }  
     
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }
      
      if (is_ipc_notify(ipc_status)) {      
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:    
            if (msg.m_notify.interrupts & irq_set_keyboard) { 
                
                kbc_ih();
                if(*gameState == MENU) {
                  if (k_scancode == ENTER_MK ) {
                      *gameState = GAME;
                      gameState_change = true;  
                  }
                }
                if(*gameState == GAME){
                  if (k_scancode == BK_1) {
                      *gameState = LEVEL1;
                      gameState_change = true;  
                  }
                  if (k_scancode == BK_2) {
                      *gameState = LEVEL2;
                      gameState_change = true;  
                  }
                  if (k_scancode == BK_3) {
                      *gameState = LEVEL3;
                      gameState_change = true;  
                  }
                }

                if (*gameState == LEVEL1 ||*gameState == LEVEL2 || *gameState == LEVEL3  ) {
                  handle_ingame_scancode(*gameState,k_scancode, sp);
                }

                if (k_scancode == SCAN_FIRST_TWO) {
                    k_bytes[k_index] = k_scancode; k_index++;
                } else {
                    k_bytes[k_index] = k_scancode;
                    // kbd_print_scancode(!(scancode & MAKE_OR_BREAK), i == 0? 1: 2 , bytes);
                    k_index = 0;
                }
                // Need to implement win logic


            }

            if (msg.m_notify.interrupts & irq_set_mouse) { 
                mouse_ih();

                if (m_index == 3) {
                    store_bytes_packet();
                    mouse_print_packet(&m_packet);
                    m_index = 0;
                    if(*gameState == MENU){
                      mouse_print_packet(&m_packet);
                      handle_mouse_movement(cursor);
                      update_menu_frame(start, quit, cursor);
                    
                      if (m_packet.lb) {
                        if(collision(cursor, start)){
                          *gameState = GAME;
                          gameState_change = true;}    
                          if(collision(cursor, quit)){
                          *gameState = EXIT;
                          gameState_change = true;}                      
                      }
                    }
                }
            }

            if (*gameState == GAME && msg.m_notify.interrupts & irq_set_timer) {
              timer_int_handler(); 
              int clock = counter % 60;
              if (clock == 0) {
                timer_print_elapsed_time();
                time--;
              }
              draw_life_bar(*gameState, &life, time);
              if (time == 0) {
                *gameState = LOSE; 
                gameState_change = true; 
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

    if (timer_unsubscribe_int() != 0)
      return 1;

    if (rtc_unsubscribe_int() != 0)
      return 1;

    if (write_mouse(DISABLE_DATA_MODE) != 0)
      return 1;
    if(k_scancode == SCAN_BREAK_ESC)*running = false;

    free_buffers();

    return 0;
}

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

void handle_ingame_scancode(GameState gameState, uint8_t scancode, Sprite *player) {
    
    switch (scancode) {
        case D_KEY_MK:
            player->x = player->x + 5;
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, D_KEY_MK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;

        case A_KEY_MK:
            player->x = player->x - 5;
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, A_KEY_MK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;

        case W_KEY_MK:
            player->y = player->y- 5;
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, W_KEY_MK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;
        
        case S_KEY_MK:
            player->y = player->y + 5;
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, S_KEY_MK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;

        case A_KEY_BRK:
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, A_KEY_BRK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;

        case D_KEY_BRK:
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, D_KEY_BRK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;

        case S_KEY_BRK:
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, S_KEY_BRK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;

        case W_KEY_BRK:
            player=create_sprite((xpm_map_t)get_next_sprite((xpm_map_t)player->map, W_KEY_BRK), player->x, player->y, player->xspeed, player->yspeed);
            drawing_sprite(player);
            break;

        default:
            return;
    }
    clear_drawing();
    drawLevel (gameState, player->x, player->y, player->width, player->width);
    drawing_sprite(player);
    drawing_sprite(life);
    update_frame_with_background();
}


void (handle_mouse_movement)(Sprite * cursor){
  if(!(cursor->x + m_packet.delta_x <= 0)) cursor->x += m_packet.delta_x;
  if(!(cursor->y - m_packet.delta_y <= 0)) cursor->y -= m_packet.delta_y;
  if(cursor->x + cursor->width >= mode_info.XResolution)cursor->x = mode_info.XResolution - cursor->width;
  if(cursor->y + cursor->height >= mode_info.YResolution)cursor->y = mode_info.YResolution - cursor->height;
  if(cursor->x + cursor->width >= 785)cursor->x = 785 - cursor->width;
  if(cursor->y + cursor->height >= 575)cursor->y = 575 - cursor->height;
}


void(update_menu_frame)(Sprite * start,Sprite *quit, Sprite * cursor){
  clear_drawing();
  drawing_xpm((xpm_map_t) menu,1,1);

  if(collision(cursor,start)){
    Sprite* hover_start_sp = create_sprite((xpm_map_t)hover_start, 295, 293, 0, 0);
    drawing_sprite(hover_start_sp);
    
  }
  else drawing_sprite(start);
  if(collision(cursor,quit)){
    Sprite* hover_quit_sp = create_sprite((xpm_map_t)hover_quit, 315, 373, 0, 0);
    drawing_sprite(hover_quit_sp);
    
  }
  else drawing_sprite(quit);
  drawing_sprite(cursor);
  update_frame();
}

