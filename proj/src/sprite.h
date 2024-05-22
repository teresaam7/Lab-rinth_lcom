#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_
#include <lcom/lcf.h>
#include "graphics.h"


typedef struct {
	int x, y, width, height, speed;
	uint8_t *map;     
} Sprite;


Sprite *create_sprite(xpm_map_t xpm, int x, int y, int speed);

void destroy_sprite(Sprite *sp);

int drawing_sprite(Sprite *sp);

bool check_collision(Sprite *sprite1, int base_width, int base_height);

#endif

