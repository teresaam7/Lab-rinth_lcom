
#include "logic.h"

extern uint8_t k_scancode;
extern struct packet m_packet;

extern bool gameState_change;
extern GameState gameState;
extern uint8_t k_index;
extern uint8_t k_bytes[2];
extern int gameTime;
extern int counter;

Sprite *menu_bg, *title_, *start, *quit, *cursor, *level1_, *level2_, *level3_, *num, *player, *life;

int (loadSprites)() {
  menu_bg =  create_sprite((xpm_map_t)menu, 1, 1, 0);
  title_ = create_sprite((xpm_map_t)title, 170, 100, 0);
  start = create_sprite((xpm_map_t)start_button, 315, 300, 0);
  quit = create_sprite((xpm_map_t)quit_button, 335, 380, 0);
  cursor = create_sprite((xpm_map_t)hand, 315, 200, 0);
  
  level1_ = create_sprite((xpm_map_t)level1, 315, 260, 0);
  level2_ = create_sprite((xpm_map_t)level2, 315, 340, 0);
  level3_ = create_sprite((xpm_map_t)level3, 315, 420, 0);
  num = create_sprite((xpm_map_t)ar, 30, 530, 0);

  player = create_sprite((xpm_map_t)right1, 20, 20, 0);
  life = create_sprite((xpm_map_t)life1, 610, 5, 0);

  if (menu_bg == NULL || title_ == NULL || start == NULL || quit == NULL || cursor == NULL || level1_ == NULL ||
      level2_ == NULL || level3_ == NULL || num == NULL || player == NULL || life == NULL)
      return 1;

  loading_bg_sprite(menu_bg);
  
  return 0;
}


int (gameStateInit)(bool * running) {
	if(gameState == MENU) {draw_menu();}
	if(gameState == GAME) {draw_game_menu();update_game_menu();}
	if(gameState == LEVEL1|gameState == LEVEL2|gameState == LEVEL3 ){draw_game();update_game_frame(player);}
	if(gameState == EXIT) {*running = false;}
	gameState_change = false;
	return 0;
}

int (keyboardLogic)() {
	if(gameState == GAME) {
		update_game_menu();
    if (k_scancode == BK_1) {
      gameState = LEVEL1;
      gameState_change = true;  
    }
    if (k_scancode == BK_2) {
    	gameState = LEVEL2;
      gameState_change = true;  
    }
    if (k_scancode == BK_3) {
      gameState = LEVEL3;
      gameState_change = true;  
    }
	}

	if(gameState == LEVEL1|gameState == LEVEL2|gameState == LEVEL3 ) {
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

	if(gameState == MENU){
		handle_mouse_movement(cursor);
    update_menu();
                    
    if (m_packet.lb) {
      if(collision(cursor, start)){
        gameState_change = true;
        gameState = GAME;
      }   
      if(collision(cursor, quit)){
        gameState_change = true;
        gameState = EXIT;
      }                      
    }
	}

	if(gameState == LEVEL1|gameState == LEVEL2|gameState == LEVEL3){
		handle_mouse_movement(cursor); 
  	update_game_frame(player);
	}
	return 0;
}


int (timerLogic) () {

	if(gameState == LEVEL1|gameState == LEVEL2|gameState == LEVEL3){
  int clock = counter % 60;
  if (clock == 0) {
    timer_print_elapsed_time();
    gameTime--;
  }
  draw_life_bar(&life, gameTime);
    if (gameTime == 0) {
      gameState = LOSE; 
      gameState_change = true; 
  	}
	}
	return 0;
}
