#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_
#include <lcom/lcf.h>
#include "graphics.h"


typedef struct {
	int x, y, width, height, speed;
	uint32_t *map;     
} Sprite;

int (loading_xpm)(xpm_map_t xpm, Sprite *sp);
Sprite *create_sprite(xpm_map_t xpm, int x, int y, int speed);

void destroy_sprite(Sprite *sp);

int (drawing_to_buffer)(Sprite *sp, uint8_t *buffer);
int (drawing_sprite)(Sprite *sp);
int (drawing_to_buffer_lantern)(Sprite *bg, Sprite *sp, uint8_t *buffer, int lant_radius);
int (drawing_lantern)(Sprite *bg, Sprite *sp, int lant_radius);
bool (check_collision)(Sprite *player, Sprite *maze, Sprite *door1, Sprite *door2, int x_diff, int y_diff);
int (drawing_bg)();
int (loading_bg_sprite)(Sprite *sp);

#endif

