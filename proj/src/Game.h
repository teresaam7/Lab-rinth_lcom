#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

#include "keyboard.h"
#include "mouse.h"
#include "graphics.h"
#include "sprite.h"

#include "images/maze1.xpm"
#include "images/maze2.xpm"
#include "images/maze3.xpm"

#include "images/right1.xpm"

enum SpriteState {
    RIGHT1,
    RIGHT2,
    LEFT1,
    LEFT2,
    UP1,
    UP2,
    DOWN1,
    DOWN2
};

int (gameLogic) ();

enum SpriteState get_next_sprite(enum SpriteState current_state, uint8_t scancode) ;

void (handle_ingame_scancode)(uint8_t scancode, Sprite *player);

#endif

