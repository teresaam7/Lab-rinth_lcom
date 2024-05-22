#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "Game.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

Sprite *sp,*start, *quit, *title_, *cursor, *life, *level1_, *level2_, *level3_;

extern uint8_t k_scancode;

int r;
message msg;
int ipc_status;
bool gameState_change = true;
GameState gameState;


uint8_t irq_set_keyboard;
uint8_t irq_set_mouse;
uint8_t irq_set_timer;
uint8_t irq_set_rtc;

int (proj_main_loop)(int argc, char *argv[]) {
  if (initialize_frame_buffer(0x115) != 0) {
    return 1;
  }

  if (graphic_mode(0x115) != 0) {
    return 1;
  }

  bool running = true;

  //initialize_buffers();

  if (write_mouse(ENABLE_DATA_MODE) != 0)
    return 1;

  if (keyboard_subscribe_int(&irq_set_keyboard) != 0)
    return 1;

  if (mouse_subscribe_int(&irq_set_mouse) != 0)
    return 1;

  if (timer_subscribe_int(&irq_set_timer) != 0)
    return 1;

  if (rtc_subscribe_int(&irq_set_rtc) != 0)
    return 1;


  gameState = MENU;
  
  draw_menu();

      while(k_scancode != SCAN_BREAK_ESC && running){
        switch(gameState){
          case MENU:
          if(gameState_change){
            draw_menu();
            gameState_change = false;}
          if(menuLogic( &running)!=0) return 1;
          break;
          case GAME: 
            if(gameState_change){
              draw_game_menu();
              gameState_change = false;}
            if (chooseLevelLogic()!= 0) return 1;
          break;
          case LEVEL1:
            if(gameState_change){
              draw_game();
              gameState_change = false;}
            if (gameLogic( &running)!= 0) return 1;
          case LEVEL2:
            if(gameState_change){
              draw_game();
              gameState_change = false;}
            if (gameLogic( &running)!= 0) return 1;
          case LEVEL3:
            if(gameState_change){
              draw_game();
              gameState_change = false;}
            if (gameLogic(&running)!= 0) return 1;
          break;
          case WIN:
            //if (gameLogic(&gameState, &running)!= 0) return 1;
          case LOSE:
            //if (gameLogic(&gameState, &running)!= 0) return 1;
          break;
          case EXIT:
            running = false;
          break; 
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

    //free_buffers();

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;

}
