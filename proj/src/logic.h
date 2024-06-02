#ifndef LOGIC_H
#define LOGIC_H


#include <lcom/lcf.h>

#include "Game.h"
#include "serialPort.h"

#define WIN_POS_X 790
#define WIN_POS_Y 555

int (loadSprites)();
int (gameStateInit)(bool * running);
int (keyboardLogic)();
int (mouseLogic)();
int (timerLogic)();

#endif


