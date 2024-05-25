
#include "logic.h"

extern uint8_t k_scancode;
extern struct packet m_packet;

extern bool gameState_change;
extern GameState gameState;
extern uint8_t k_index;
extern uint8_t k_bytes[2];
extern int gameTime;
extern int counter;

Sprite *menu_bg, *title, *game_over, *start, *hover_start, *quit, *hover_quit, *cursor, 
*level1, *hover_level1, *level2, *hover_level2, *level3, *hover_level3, *maze, *player, *life;

int (loadSprites)() {
  menu_bg =  create_sprite((xpm_map_t)menu, 1, 1, 0);

  title = create_sprite((xpm_map_t)title_, 200, 100, 0);
  game_over = create_sprite((xpm_map_t)game_over_, 245, 200, 0);

  start = create_sprite((xpm_map_t)start_button, 315, 300, 0);
  hover_start = create_sprite((xpm_map_t)hover_start_, 295, 293, 0);
  quit = create_sprite((xpm_map_t)quit_button, 335, 380, 0);
  hover_quit = create_sprite((xpm_map_t)hover_quit_, 315, 373, 0);
  cursor = create_sprite((xpm_map_t)hand, 315, 200, 0);
  
  level1 = create_sprite((xpm_map_t)level1_, 315, 260, 0);
	hover_level1 = create_sprite((xpm_map_t)hover_level1_, 295, 260, 0);
  level2 = create_sprite((xpm_map_t)level2_, 315, 340, 0);
	hover_level2 = create_sprite((xpm_map_t)hover_level2_, 295, 340, 0);
  level3 = create_sprite((xpm_map_t)level3_, 315, 420, 0);
	hover_level3 = create_sprite((xpm_map_t)hover_level3_, 295, 420, 0);


  player = create_sprite((xpm_map_t)right1, 20, 20, 0);
  life = create_sprite((xpm_map_t)life1, 610, 5, 0);

  if (menu_bg == NULL || title == NULL || start == NULL || hover_start == NULL || quit == NULL || hover_quit == NULL || 
      cursor == NULL || level1 == NULL || hover_level1 == NULL || level2 == NULL || hover_level2 == NULL ||
			level3 == NULL || hover_level3 == NULL || player == NULL || life == NULL)
      return 1;

  loading_bg_sprite(menu_bg);
  
  return 0;
}


int (gameStateInit)(bool * running) {
	if (gameState == MENU) {draw_menu();}
	if (gameState == LEVELS) {draw_menu_levels();}
	if (gameState == GAME) {update_game(player);}
  if (gameState == LOSE) {draw_lost();}
	if (gameState == EXIT) {*running = false;}
	gameState_change = false;
	return 0;
}

int (keyboardLogic)() {
	if (gameState == GAME) {
		handle_ingame_scancode(k_scancode, player);

		if (k_scancode == SCAN_FIRST_TWO) {
			k_bytes[k_index] = k_scancode; k_index++;
		} else {
			k_bytes[k_index] = k_scancode;
			k_index = 0;
		}
	}

  return 0;
}

int (mouseLogic) () {
	if (gameState == MENU){
		handle_mouse_movement(cursor);
    update_menu();
                    
    if (m_packet.lb) {
      if(collision(cursor, start)){
        gameState_change = true;
        gameState = LEVELS;
      }   
      if(collision(cursor, quit)){
        gameState_change = true;
        gameState = EXIT;
      }                      
    }
	}
	if (gameState == LEVELS) {
		handle_mouse_movement(cursor);
    update_menu_levels();
    if (m_packet.lb) {
      if(collision(cursor, level1)){
        load_level(1);
        gameState_change = true;
        gameState = GAME;
      }   
      if(collision(cursor, level2)){
        load_level(2);
        gameState_change = true;
        gameState = GAME;
      }  
      if(collision(cursor, level3)){
        load_level(3);
        gameState_change = true;
        gameState = GAME;
      }                      
    }
	}

	if (gameState == GAME){
		handle_mouse_movement(cursor); 
  	update_game(player);
	}

	return 0;
}


int (timerLogic) () {
	if (gameState == GAME){
    int clock = counter % 60;

    if (clock == 0) {
      timer_print_elapsed_time();
      printf("%d",gameTime);
      gameTime--;
    }

    update_life_bar(gameTime);
    //update_game(player);

    if (gameTime == 0) {
      gameState = LOSE; 
      gameState_change = true; 
    }
	}
  
	return 0;
}
