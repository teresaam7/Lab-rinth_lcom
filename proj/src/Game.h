#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

#include "keyboard.h"
#include "mouse.h"
#include "rtc.h"
#include "graphics.h"
#include "sprite.h"


#include "images/mazes/maze1.xpm"
#include "images/mazes/mazeDay1.xpm"
#include "images/mazes/mazeDark1.xpm"

#include "images/mazes/maze2.xpm"
#include "images/mazes/mazeDay2.xpm"
#include "images/mazes/mazeDark2.xpm"

#include "images/mazes/maze3.xpm"
#include "images/mazes/mazeDay3.xpm"
#include "images/mazes/mazeDark3.xpm"

#include "images/menu.xpm"
#include "images/win.xpm"

#include "images/levels/level1.xpm"
#include "images/levels/level2.xpm"
#include "images/levels/level3.xpm"

#include "images/right1.xpm"
#include "images/right2.xpm"
#include "images/left1.xpm"
#include "images/left2.xpm"
#include "images/up1.xpm"
#include "images/up2.xpm"
#include "images/down1.xpm"
#include "images/down2.xpm"

#include "images/ar.xpm"

#include "images/lifeBar/life1.xpm"
#include "images/lifeBar/life2.xpm"
#include "images/lifeBar/life3.xpm"
#include "images/lifeBar/life4.xpm"
#include "images/lifeBar/life5.xpm"

#include "images/hand.xpm"
#include "images/title.xpm"
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
    LEVELS,
    GAME,
    WIN,
    LOSE,
    EXIT
} GameState;

#define TIMER_MINUTES 3

void (draw_menu)();
void (update_menu)();
int (collision)(Sprite * sp1, Sprite * sp2);

void (draw_menu_levels)();
void (load_level)();

void (update_game)(Sprite * player);
xpm_map_t (get_next_sprite)(uint8_t scancode);
void (handle_ingame_scancode)(uint8_t scancode, Sprite *player);
void (handle_mouse_movement)(Sprite * cursor);
void (update_life_bar)(int total_seconds);


void (draw_win)();
void (draw_lost)();


#endif

