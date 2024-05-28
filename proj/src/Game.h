#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

#include "keyboard.h"
#include "mouse.h"
#include "rtc.h"
#include "graphics.h"
#include "sprite.h"
#include "serialPort.h"


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
#include "images/game_over.xpm"

#include "images/levels/level1.xpm"
#include "images/levels/level2.xpm"
#include "images/levels/level3.xpm"
#include "images/levels/hover_level1.xpm"
#include "images/levels/hover_level2.xpm"
#include "images/levels/hover_level3.xpm"

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

#include "images/numbers/num0.xpm"
#include "images/numbers/num1.xpm"
#include "images/numbers/num2.xpm"
#include "images/numbers/num3.xpm"
#include "images/numbers/num4.xpm"
#include "images/numbers/num5.xpm"
#include "images/numbers/num6.xpm"
#include "images/numbers/num7.xpm"
#include "images/numbers/num8.xpm"
#include "images/numbers/num9.xpm"
#include "images/numbers/dot.xpm"

#include "images/smallNumbers/small0.xpm"
#include "images/smallNumbers/small1.xpm" 
#include "images/smallNumbers/small2.xpm"
#include "images/smallNumbers/small3.xpm"
#include "images/smallNumbers/small4.xpm"
#include "images/smallNumbers/small5.xpm"
#include "images/smallNumbers/small6.xpm"
#include "images/smallNumbers/small7.xpm"
#include "images/smallNumbers/small8.xpm"
#include "images/smallNumbers/small9.xpm"
#include "images/smallNumbers/divisor.xpm" 

#include "images/hand.xpm"
#include "images/title.xpm"
#include "images/start_button.xpm"
#include "images/hover_start.xpm"
#include "images/quit_button.xpm"
#include "images/hover_quit.xpm"


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
void (update_menu_levels)();
int (collision)(Sprite * sp1, Sprite * sp2);

void (draw_menu_levels)();
void (load_level)();

void (update_game)(Sprite * player);
xpm_map_t (get_next_sprite)(uint8_t scancode);
void (handle_ingame_scancode)(uint8_t scancode, Sprite *player);
void (handle_mouse_movement)(Sprite * cursor);
void (update_life_bar)(int total_seconds);

void (draw_number)(Sprite *num_sprite, int x, int y);
void (draw_time)(int total_seconds, int x, int y);
void (draw_time_small)(uint8_t hours, uint8_t minutes, int x, int y);

void (draw_win)(int total_seconds);
void (draw_lost)();


#endif

