
#include "logic.h"

extern uint8_t k_scancode;
extern struct packet m_packet;

extern bool gameState_change;
extern bool multi;
extern bool isPlayer1;
extern GameState gameState;

extern int gameTime;
extern int counter;
static bool sentP1 = false;

Sprite *menu_bg, *title, *game_over,*win, *start, *hover_start, *quit, *hover_quit, *cursor, 
*level1, *hover_level1, *level2, *hover_level2, *level3, *hover_level3, *maze,*waiting, *button1, *button1_down, *button2, *button2_down, *door1, *door2,  *player, *player2, *life, *arrow;

Sprite *num0, *num1,*num2, *num3, *num4, *num5, *num6, *num7, *num8, *num9, *dot;
Sprite *smallNum0, *smallNum1,*smallNum2, *smallNum3, *smallNum4, *smallNum5, *smallNum6, *smallNum7, *smallNum8, *smallNum9, *divisor;

/**
 * @brief Loads all the sprites used in the game.
 * @return 0 if success and 1 if any sprite fails to load.
 */
int (loadSprites)() {
  menu_bg =  create_sprite((xpm_map_t)menu, 1, 1, 0);

  title = create_sprite((xpm_map_t)title_, 200, 100, 0);
  game_over = create_sprite((xpm_map_t)game_over_, 245, 200, 0);
  win = create_sprite((xpm_map_t)win_, 320, 200, 0);

  start = create_sprite((xpm_map_t)start_button, 315, 300, 0);
  hover_start = create_sprite((xpm_map_t)hover_start_, 295, 293, 0);
  quit = create_sprite((xpm_map_t)quit_button, 335, 380, 0);
  hover_quit = create_sprite((xpm_map_t)hover_quit_, 315, 373, 0);
  cursor = create_sprite((xpm_map_t)hand, 315, 200, 0);
  
  level1 = create_sprite((xpm_map_t)level1_, 315, 220, 0);
	hover_level1 = create_sprite((xpm_map_t)hover_level1_, 295, 220, 0);
  level2 = create_sprite((xpm_map_t)level2_, 315, 355, 0);
	hover_level2 = create_sprite((xpm_map_t)hover_level2_, 295, 355, 0);
  level3 = create_sprite((xpm_map_t)level3_, 325, 490, 0);
	hover_level3 = create_sprite((xpm_map_t)hover_level3_, 305, 490, 0);


  player = create_sprite((xpm_map_t)right1, 20, 20, 0);

  player2 = create_sprite((xpm_map_t)right1second, 60, 20, 0);
  life = create_sprite((xpm_map_t)life1, 610, 5, 0);
  waiting = create_sprite((xpm_map_t)waiting_, 240, 100, 0);
  arrow = create_sprite((xpm_map_t)arrow6, 745, 560, 0);
  
  num0 = create_sprite((xpm_map_t)num0_, 1, 1, 0);
  num1 = create_sprite((xpm_map_t)num1_, 1, 1, 0);
  num2 = create_sprite((xpm_map_t)num2_, 1, 1, 0);
  num3 = create_sprite((xpm_map_t)num3_, 1, 1, 0);
  num4 = create_sprite((xpm_map_t)num4_, 1, 1, 0);
  num5 = create_sprite((xpm_map_t)num5_, 1, 1, 0);
  num6 = create_sprite((xpm_map_t)num6_, 1, 1, 0);
  num7 = create_sprite((xpm_map_t)num7_, 1, 1, 0);
  num8 = create_sprite((xpm_map_t)num8_, 1, 1, 0);
  num9 = create_sprite((xpm_map_t)num9_, 1, 1, 0);
  dot = create_sprite((xpm_map_t)dot_, 1, 1, 0);

  smallNum0 = create_sprite((xpm_map_t)small0, 2, 2, 0);
  smallNum1 = create_sprite((xpm_map_t)small1, 2, 2, 0);
  smallNum2 = create_sprite((xpm_map_t)small2, 2, 2, 0);
  smallNum3 = create_sprite((xpm_map_t)small3, 2, 2, 0);
  smallNum4 = create_sprite((xpm_map_t)small4, 2, 2, 0);
  smallNum5 = create_sprite((xpm_map_t)small5, 2, 2, 0);
  smallNum6 = create_sprite((xpm_map_t)small6, 2, 2, 0);
  smallNum7 = create_sprite((xpm_map_t)small7, 2, 2, 0);
  smallNum8 = create_sprite((xpm_map_t)small8, 2, 2, 0);
  smallNum9 = create_sprite((xpm_map_t)small9, 2, 2, 0);
  divisor = create_sprite((xpm_map_t)divisor_, 2, 2, 0);
 
  if (menu_bg == NULL || title == NULL ||game_over == NULL || win == NULL || start == NULL || hover_start == NULL ||
      quit == NULL || hover_quit == NULL || cursor == NULL || level1 == NULL || hover_level1 == NULL || level2 == NULL || 
      hover_level2 == NULL || level3 == NULL || hover_level3 == NULL || player == NULL || player2 == NULL || life == NULL || 
      num0 == NULL || num1 == NULL ||num2 == NULL ||num3 == NULL ||num4 == NULL ||num5 == NULL ||num6 == NULL || 
      num7 == NULL ||num8 == NULL ||num9 == NULL || dot == NULL || smallNum0 == NULL ||  smallNum1 == NULL ||
      smallNum2 == NULL || smallNum3 == NULL || smallNum4 == NULL || smallNum5 == NULL || smallNum6 == NULL || 
      smallNum7 == NULL || smallNum8 == NULL || smallNum9 == NULL ||  divisor == NULL || arrow == NULL || waiting == NULL)
      return 1;

  loading_bg_sprite(menu_bg);
  
  return 0;
}

/**
 * @brief Initializes the game state.
 * This function handles the initialization of different game states and draws the screen or updates the game.
 * If the game state is EXIT the running flag is set to false which shows that the game will terminate.
 * @param running Pointer to the running state of the game.
 * @return 0 if success.
 */
int (gameStateInit)(bool * running) {
	if (gameState == MENU) {draw_menu();}
	if (gameState == LEVELS) {draw_menu_levels();}
	if (gameState == GAME) {update_game();}
  if (gameState == MULTI) {draw_waiting();}
  if (gameState == WIN) {draw_win(300-gameTime);}
  if (gameState == LOSE) {draw_lost();}
	if (gameState == EXIT) {*running = false;}
	gameState_change = false;
	return 0;
}

/**
 * @brief Handles keyboard input logic.
 * This function manages button presses based on the current keyboard scancode and sees 
 * if the player has arrived at the winning position. Then, it updates the game state.
 * @return 0 if success.
 */
int (keyboardLogic)() {
  if (gameState == MULTI) {
    if (k_scancode == 0x2D) {isPlayer1 = true; printf("PLAYER1");} // X
    if (k_scancode == 0x2C) {isPlayer1 = false; printf("PLAYER2");} // Z

		send_byte(0x53);
	}
	if (gameState == GAME) {
    if (!multi) {
      handle_ingame_scancode(k_scancode, player);
    } else  {
      if (isPlayer1 && !sentP1) {printf("OLAAA"); manage_button(k_scancode, true); }
      else {printf("OIIIIII"); manage_button(k_scancode, false);}
      //send_scan(k_scancode); //sp_ih();
    }
	}
  if((player->x == 790 ) && (player->y == 555)){
    gameState_change = true;
    gameState = WIN;
  }

  return 0;
}

/**
 * @brief Handles mouse input logic.
 * This function handles mouse movement and updates the game state based on mouse interactions.
 * It sees if there are collisions between the cursor and buttons/levels to change the game state.
 * @return 0 if success.
 */
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
        gameState = MULTI;
      }   

      /*if(gameState == MULTI){
          send_byte(0x53);
        }  */                 
    }
	}

	if (gameState == GAME){
		handle_mouse_movement(cursor); 
  	update_game();
	}

	return 0;
}


/**
 * @brief Handles timer logic.
 * This function processes timer events during the game state. It updates the game clock, 
 * manages the game timer, and handles game over conditions. Also, it updates the arrow 
 * sprite and life bar based on the time that the user still has to complete the level.
 * @return 0 if success.
 */
int (timerLogic) () {
	if (gameState == GAME){
    int clock = counter % 60;

    if (clock == 0) {
      timer_print_elapsed_time();
      printf("%d",gameTime);
      gameTime--;
    }
    update_arrow_sprite(gameTime);
    update_life_bar(gameTime);

    if (gameTime == 0) {
      gameState = LOSE;
      gameState_change = true; 
    }
	}
  
	return 0;
}

