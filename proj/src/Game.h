#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

#include "keyboard.h"
#include "mouse.h"
#include "rtc.h"
#include "graphics.h"
#include "sprite.h"


#include "images/maze1.xpm"
#include "images/mazes/mazeNoon1.xpm"
#include "images/mazes/mazeDark1.xpm"

#include "images/mazes/maze2.xpm"
#include "images/mazes/mazeNoon2.xpm"
#include "images/mazeDark2.xpm"

#include "images/mazes/maze3.xpm"
#include "images/mazes/mazeNoon3.xpm"
#include "images/mazes/mazeDark3.xpm"

#include "images/menu.xpm"
#include "images/win.xpm"

#include "images/right1.xpm"
#include "images/right2.xpm"
#include "images/left1.xpm"
#include "images/left2.xpm"
#include "images/up1.xpm"
#include "images/up2.xpm"
#include "images/down1.xpm"
#include "images/down2.xpm"

#include "images/lifeBar/life1.xpm"
#include "images/lifeBar/life2.xpm"
#include "images/lifeBar/life3.xpm"
#include "images/lifeBar/life4.xpm"
#include "images/lifeBar/life5.xpm"

#include "images/hand.xpm"
#include "images/start_button.xpm"
#include "images/hover_start.xpm"
#include "images/quit_button.xpm"
#include "images/hover_quit.xpm"

/*enum SpriteState {
    RIGHT1,
    RIGHT2,
    LEFT1,
    LEFT2,
    UP1,
    UP2,
    DOWN1,
    DOWN2
};*/

typedef enum {
    MENU,
    GAME,
    LEVELS,
    WIN,
    LOSE,
    EXIT
} GameState;

#define TIMER_MINUTES 3

int (menuLogic) (GameState *gameState, bool *running);
int (gameLogic) (GameState *gameState, bool *running);
int (winLogic) (GameState *gameState, bool *running);

void (change_maze_colors_based_on_time)();
void (draw_life_bar)(Sprite **bar, int total_seconds);

void (draw_game)();
void (draw_menu)();
void (draw_win)();
void (draw_lost)();


xpm_map_t (get_next_sprite)(xpm_map_t current_state, uint8_t scancode); 

void (handle_ingame_scancode)(uint8_t scancode, Sprite *player);
void (handle_mouse_movement)(Sprite * cursor);
void (update_menu_frame)(Sprite * start,Sprite * quit, Sprite * cursor);

#endif

