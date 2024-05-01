
#include "Game.h"

int (gameLogic) () {
    loadMazeXPM();
    return 0;
}

void (loadMazeXPM) () {
    xpm_image_t img;
    uint8_t *result = xpm_load(maze1, XPM_8_8_8_8, &img);
    if (result == NULL) {
        printf("Error loading maze XPM\n");
    }
}

