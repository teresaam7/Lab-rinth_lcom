#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

#include "keyboard.h"
#include "mouse.h"
#include "graphics.h"
#include "images/maze1.xpm"
#include "images/maze2.xpm"
#include "images/menu.xpm"
#include "images/start_button.xpm"
#include "sprite.h"

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

typedef enum {
    MENU,
    GAME,
    ABOUTUS,
    EXIT
} GameState;

int (menuLogic) (GameState *gameState, bool *running);
int (gameLogic) (GameState *gameState, bool *running);


enum SpriteState get_next_sprite(enum SpriteState current_state, uint8_t scancode) ;

void (handle_ingame_scancode)(uint8_t scancode, Sprite *player);

#endif

