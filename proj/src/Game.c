
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


extern Sprite *menu_bg, *title, *game_over, *start, *hover_start, *quit, *hover_quit, *cursor,
 *level1, *hover_level1, *level2, *hover_level2, *level3, *hover_level3, *maze, *player, *life;

extern Sprite *num0, *num1,*num2, *num3, *num4, *num5, *num6, *num7, *num8, *num9;

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
  drawing_sprite(title);
  drawing_sprite(level1);
  drawing_sprite(level2);
  drawing_sprite(level3);
  update_flip_frames();
}

void (update_menu_levels)(){
  drawing_bg(menu_bg);
  drawing_sprite(title);

  if (collision(cursor,level1)) drawing_sprite(hover_level1);
  else drawing_sprite(level1);

  if (collision(cursor,level2)) drawing_sprite(hover_level2);
  else drawing_sprite(level2);

  if (collision(cursor,level3)) drawing_sprite(hover_level3);
  else drawing_sprite(level3);

  drawing_sprite(cursor);

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
    switch (scancode) {
        case D_KEY_MK:
            player->x = player->x + 5;
            loading_xpm(get_next_sprite(D_KEY_MK), player);
            update_game(player);
            break;

        case A_KEY_MK:
            player->x = player->x - 5;
            loading_xpm(get_next_sprite(A_KEY_MK), player);
            update_game(player);
            break;

        case W_KEY_MK:
            player->y = player->y - 5;
            loading_xpm(get_next_sprite(W_KEY_MK), player);
            update_game(player);
            break;
        
        case S_KEY_MK:
            player->y = player->y + 5;
            loading_xpm(get_next_sprite(S_KEY_MK), player);
            update_game(player);
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
        default:
            return;
    }
    update_game(player);
}


void (draw_number)(Sprite *num_sprite, int x, int y) {
    num_sprite->x = x;
    num_sprite->y = y;
    drawing_sprite(num_sprite);
}

void draw_time(int total_seconds, int x, int y) {
    uint8_t minutes = total_seconds / 60;
    uint8_t seconds = total_seconds % 60;

    int x_offset = x;

    // Desenha os minutos
    switch (minutes / 10) {
        case 0: draw_number(num0, x_offset, y); break;
        case 1: draw_number(num1, x_offset, y); break;
        case 2: draw_number(num2, x_offset, y); break;
        case 3: draw_number(num3, x_offset, y); break;
        case 4: draw_number(num4, x_offset, y); break;
        case 5: draw_number(num5, x_offset, y); break;
        case 6: draw_number(num6, x_offset, y); break;
        case 7: draw_number(num7, x_offset, y); break;
        case 8: draw_number(num8, x_offset, y); break;
        case 9: draw_number(num9, x_offset, y); break;
    }
    x_offset += num0->width;

    switch (minutes % 10) {
        case 0: draw_number(num0, x_offset, y); break;
        case 1: draw_number(num1, x_offset, y); break;
        case 2: draw_number(num2, x_offset, y); break;
        case 3: draw_number(num3, x_offset, y); break;
        case 4: draw_number(num4, x_offset, y); break;
        case 5: draw_number(num5, x_offset, y); break;
        case 6: draw_number(num6, x_offset, y); break;
        case 7: draw_number(num7, x_offset, y); break;
        case 8: draw_number(num8, x_offset, y); break;
        case 9: draw_number(num9, x_offset, y); break;
    }
    x_offset += num0->width + 10;  // Adiciona o espaçamento entre minutos e segundos

    switch (seconds / 10) {
        case 0: draw_number(num0, x_offset, y); break;
        case 1: draw_number(num1, x_offset, y); break;
        case 2: draw_number(num2, x_offset, y); break;
        case 3: draw_number(num3, x_offset, y); break;
        case 4: draw_number(num4, x_offset, y); break;
        case 5: draw_number(num5, x_offset, y); break;
        case 6: draw_number(num6, x_offset, y); break;
        case 7: draw_number(num7, x_offset, y); break;
        case 8: draw_number(num8, x_offset, y); break;
        case 9: draw_number(num9, x_offset, y); break;
    }
    x_offset += num0->width;

    switch (seconds % 10) {
        case 0: draw_number(num0, x_offset, y); break;
        case 1: draw_number(num1, x_offset, y); break;
        case 2: draw_number(num2, x_offset, y); break;
        case 3: draw_number(num3, x_offset, y); break;
        case 4: draw_number(num4, x_offset, y); break;
        case 5: draw_number(num5, x_offset, y); break;
        case 6: draw_number(num6, x_offset, y); break;
        case 7: draw_number(num7, x_offset, y); break;
        case 8: draw_number(num8, x_offset, y); break;
        case 9: draw_number(num9, x_offset, y); break;
    }
}


void (draw_win)(int total_seconds) {
  drawing_bg(menu_bg);
  drawing_sprite(game_over);
  draw_time(total_seconds, 310, 305);
  update_flip_frames();
}

void (draw_lost)() {
  uint8_t hours, minutes, seconds;
  get_game_time(&hours, &minutes, &seconds);
  display_game_time();
  drawing_bg(menu_bg);
  drawing_sprite(game_over);
  update_flip_frames();
}

