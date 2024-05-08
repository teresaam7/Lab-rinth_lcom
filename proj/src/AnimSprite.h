#ifndef _LCOM_ANIMSPRITE_H_
#define _LCOM_ANIMSPRITE_H_
#include <lcom/lcf.h>

#include <stdarg.h> // va_* macros are defined here
#include "sprite.h"
typedef struct {
Sprite *sp; // standard sprite
    int aspeed; // no. frames per pixmap
    int cur_aspeed; // no. frames left to next change
    int num_fig; // number of pixmaps
    int cur_fig; // current pixmap
    char **map; // array of pointers to pixmaps
} AnimSprite;
AnimSprite *create_animSprite(uint8_t no_pic, char *pic1[]);
//int animate_animSprite(AnimSprite *sp);
//void destroy_animSprite(AnimSprite *sp);

#endif

