
#include "Game.h"

uint8_t k_index = 0;
uint8_t k_bytes[2];
extern uint8_t k_scancode;

extern int m_index;
extern uint8_t m_bytes[3];
extern struct packet m_packet;
extern vbe_mode_info_t mode_info;

extern bool gameState_change;
extern GameState gameState;


extern Sprite *menu_bg, *title, *start, *hover_start, *quit, *hover_quit, *cursor, *level1, *level2, *level3, *num, *maze, *player, *life;

/* Menu */
void (draw_menu)(){
  drawing_bg(menu_bg);
  drawing_sprite(start);
  drawing_sprite(quit);
  drawing_sprite(cursor);
  drawing_sprite(title);
  update_flip_frames(); 
}


void (update_menu)(){
  drawing_bg(menu_bg);
  drawing_sprite(title);

  if (collision(cursor,start)) drawing_sprite(hover_start);
  else drawing_sprite(start);

  if (collision(cursor,quit)) drawing_sprite(hover_quit);
  else drawing_sprite(quit);

  drawing_sprite(cursor);

  update_flip_frames();
}


int (collision)(Sprite * sp1, Sprite * sp2) {
  if(sp1->x < sp2->x || sp1 -> x > sp2->x + sp2->width) return 0;
  if(sp1->y < sp2->y || sp1 -> y > sp2->y + sp2->height) return 0;
  return 1;
}


/* Levels */
void (draw_menu_levels)() {
  drawing_sprite(menu_bg);
  drawing_sprite(level1);
  drawing_sprite(level2);
  drawing_sprite(level3);
  drawing_sprite(num);
  update_flip_frames();
}


void (load_level)(int level) {
  uint8_t hours, minutes, seconds;
  get_game_time(&hours, &minutes, &seconds);

  if (maze != NULL) 
    destroy_sprite(maze);

  switch (level) {
    case 1:
      if (hours >= 6 && hours < 14) {
        maze = create_sprite((xpm_map_t) mazeDay1, 0, 0, 0);
      } else if (hours >= 20 || hours < 6) {
        maze = create_sprite((xpm_map_t) mazeDark1, 0, 0, 0);
      } else {
        maze = create_sprite((xpm_map_t) maze1, 0, 0, 0);
      }
      break;

    case 2:
      if (hours >= 6 && hours < 14) {
        maze = create_sprite((xpm_map_t) mazeDay2, 0, 0, 0);
      } else if (hours >= 20 || hours < 6) {
        maze = create_sprite((xpm_map_t) mazeDark2, 0, 0, 0);
      } else {
        maze = create_sprite((xpm_map_t) maze2, 0, 0, 0);
      }
      break;

    case 3:
      if (hours >= 6 && hours < 14) {
        maze = create_sprite((xpm_map_t) mazeDay3, 0, 0, 0);
      } else if (hours >= 20 || hours < 6) {
        maze = create_sprite((xpm_map_t) mazeDark3, 0, 0, 0);
      } else {
        maze = create_sprite((xpm_map_t) maze3, 0, 0, 0);
      }
      break;

    default:
      return;
  }
}



/* Game */
void (update_game)(Sprite * player) {
  drawing_lantern(maze, player, 40);
  drawing_lantern(maze, cursor, 40);
  drawing_sprite(player);
  drawing_sprite(life);
  drawing_sprite(cursor);
  update_flip_frames();
}


xpm_map_t (get_next_sprite)(uint8_t scancode) {
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
            return (xpm_map_t)right1;
    }
}


void (handle_ingame_scancode)(uint8_t scancode, Sprite *player) {
    //printf("%d", player->x);
    switch (scancode) {
        case D_KEY_MK:
          if(check_collision(player, maze->width, maze->height)!=0){
             if(check_walls(player, maze->width, maze->height, 0)==0){
              player->x = player->x + 5;
              loading_xpm(get_next_sprite(D_KEY_MK), player);
              update_game(player);
             }
             else{
               loading_xpm(get_next_sprite(D_KEY_MK), player);
                update_game(player);
            }
            
          }
          else{
            player->x = player->x - 5;
          }

          break;

        case A_KEY_MK:
          if(check_collision(player, maze->width, maze->height)!=0){
             
            player->x = player->x - 5;
            loading_xpm(get_next_sprite(A_KEY_MK), player);
            update_game(player);
             
             
          }
          else{
            player->x = player->x + 5;
          }
            break;

        case W_KEY_MK:
          if(check_collision(player, maze->width, maze->height)!=0){
            
              player->y = player->y - 5;
              loading_xpm(get_next_sprite(W_KEY_MK), player);
              update_game(player);
            
            
          }
          else{
            player->y = player->y + 5;
          }
            break;
        
        case S_KEY_MK:
          if(check_collision(player, maze->width, maze->height)!=0){
            
              player->y = player->y + 5;
              loading_xpm(get_next_sprite(S_KEY_MK), player);
              update_game(player);
            
            
          }
          else{
            player->y = player->y - 5;
          }
            break;

        case A_KEY_BRK:
            loading_xpm(get_next_sprite(A_KEY_BRK), player);
            update_game(player);
            break;

        case D_KEY_BRK:
            loading_xpm(get_next_sprite(D_KEY_BRK), player);
            update_game(player);
            break;

        case S_KEY_BRK:
            loading_xpm(get_next_sprite(S_KEY_BRK), player);
            update_game(player);
            break;

        case W_KEY_BRK:
            loading_xpm(get_next_sprite(W_KEY_BRK), player);
            update_game(player);
            break;

        default:
            return;
    }
}


void (handle_mouse_movement)(Sprite * cursor){
  if(!(cursor->x + m_packet.delta_x <= 0)) cursor->x += m_packet.delta_x;
  if(!(cursor->y - m_packet.delta_y <= 0)) cursor->y -= m_packet.delta_y;
  if(cursor->x + cursor->width >= mode_info.XResolution)cursor->x = mode_info.XResolution - cursor->width;
  if(cursor->y + cursor->height >= mode_info.YResolution)cursor->y = mode_info.YResolution - cursor->height;
  if(cursor->x + cursor->width >= 785)cursor->x = 785 - cursor->width;
  if(cursor->y + cursor->height >= 575)cursor->y = 575 - cursor->height;
}


void (update_life_bar)(int total_seconds) {
    switch(total_seconds){
        case 30:
            loading_xpm((xpm_map_t)life5, life);
            break;
        case 60:
            loading_xpm((xpm_map_t)life4, life);
            break;
        case 90:
            loading_xpm((xpm_map_t)life3, life);
            break;
        case 140:
            loading_xpm((xpm_map_t)life2, life);
            break;
        //case 200:
            //loading_xpm((xpm_map_t)life1, life);
            //break;
        default:
            return;
    }
    
}





void (draw_win)() {
  //drawing_xpm((xpm_map_t) win,1,1);
  display_game_time();
}

void (draw_lost)() {
  //drawing_xpm((xpm_map_t) win,1,1);
}

