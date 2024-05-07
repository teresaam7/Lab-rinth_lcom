#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_
#include <lcom/lcf.h>


typedef struct {
	int x, y; // current position
	int width, height; // dimensions
	double xspeed, yspeed; // current speed
	char *map; // the pixmap
} Sprite;

Sprite *create_sprite(const char *pic[], int x, int y,
int xspeed, int yspeed);
void destroy_sprite(Sprite *sp);
//int animate_sprite(Sprite *sp);
/* Some useful non-visible functions */
//static int draw_sprite(Sprite *sp, char *base);
//static int check_collision(Sprite *sp, char *base);

#endif
