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

int drawing_xpm(Sprite *sp);

bool check_collision(Sprite *sprite1, const char *base, int base_width, int base_height);

#endif

