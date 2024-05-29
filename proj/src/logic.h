#ifndef LOGIC_H
#define LOGIC_H


#include <lcom/lcf.h>

#include "Game.h"
#include "serialPort.h"

int (loadSprites)();
int (gameStateInit)(bool * running);
int (keyboardLogic)();
int (mouseLogic)();
int (timerLogic)();

#endif


