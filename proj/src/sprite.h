#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_
#include <lcom/lcf.h>
#include "graphics.h"


typedef struct {
	int x, y, width, height, speed;
	uint32_t *map;     
} Sprite;


int (loading_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi, Sprite *sp);

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int speed);

void destroy_sprite(Sprite *sp);

int drawing_sprite(Sprite *sp);

bool check_collision(Sprite *sprite1, int base_width, int base_height);

#endif

