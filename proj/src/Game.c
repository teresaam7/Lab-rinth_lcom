
#include "Game.h"

int (gameLogic) () {
    if (loadMazeXPM() != 0) {
        return 1;
    }
    return 0;
}

int (loadMazeXPM) () {
    int result = make_xpm(maze1, 40, 40);
    if (result != 0) {
        return 1;
        printf("Error loading maze XPM\n");
    }
    return 0;
}

