
#include "Game.h"
#include "maze.h"

int (gameLogic) () {
    maze_allocation();
    draw_maze(true);
    /*

    if (loadMazeXPM() != 0) {
        return 1;
    }

    */
    return 0;
}


