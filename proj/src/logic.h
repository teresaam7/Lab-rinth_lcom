#ifndef LOGIC_H
#define LOGIC_H


#include <lcom/lcf.h>

#include "Game.h"

int (gameStateInit)(bool * running);
int (keyboardLogic)();
int (mouseLogic)();
int (timerLogic)();

#endif


