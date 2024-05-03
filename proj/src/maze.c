
#include "maze.h"

static char* maze = NULL;
extern vbe_mode_info_t modeinfo;
extern uint8_t *frame_buffer;

void (maze_allocation)() {
    maze = malloc(modeinfo.XResolution * modeinfo.YResolution*(modeinfo.BitsPerPixel/8) + 7);
} 

void (free_space) () {
    free(maze);
}

int (draw_maze)(bool draw) {
    if(draw) {
        make_xpm(maze1,0,0);
        memcpy(maze,frame_buffer,modeinfo.XResolution * modeinfo.YResolution*(modeinfo.BitsPerPixel/8));
    }
  
    else {
        memcpy(frame_buffer,maze,modeinfo.XResolution * modeinfo.YResolution*(modeinfo.BitsPerPixel/8));
    }
    return 0;
}

void (clear_screen)() {
    memset(frame_buffer,0,modeinfo.XResolution * modeinfo.YResolution*(modeinfo.BitsPerPixel/8));
    draw_maze(false);
}

