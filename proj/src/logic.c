
#include "logic.h"

extern uint8_t k_scancode;
extern struct packet m_packet;

extern bool gameState_change;
extern GameState gameState;
extern uint8_t k_index;
extern uint8_t k_bytes[2];
extern int gameTime;
extern int counter;

Sprite *sp,*start, *quit, *title_, *cursor, *life, *level1_, *level2_, *level3_, *num, *menu_bg;



int (gameStateInit)(bool * running) {
	if(gameState == MENU) {draw_menu();}
	if(gameState == GAME) {draw_game_menu();update_game_menu();}
	if(gameState == LEVEL1|gameState == LEVEL2|gameState == LEVEL3 ){draw_game();update_game_frame(sp);}
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
		handle_ingame_scancode(k_scancode, sp);

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
    update_menu_frame(start, quit, cursor);
                    
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
  	update_game_frame(sp);
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
