#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_
#include <lcom/lcf.h>
#include "graphics.h"


typedef struct {
	int x, y;
	int width, height;
	double xspeed, yspeed;
	const char *map;     
} Sprite;

Sprite *create_sprite(xpm_map_t xpm, int x, int y, double xspeed, double yspeed);
void destroy_sprite(Sprite *sp);
//int animate_sprite(Sprite *sp);
/* Some useful non-visible functions */
//static int draw_sprite(Sprite *sp, char *base);
//static int check_collision(Sprite *sp, char *base);

int drawing_xpm(Sprite *sp);

#endif

