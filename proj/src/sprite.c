#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "sprite.h"

/** Creates a new sprite from XPM "pic", with specified
* position (within the screen limits) and speed;
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/

Sprite *create_sprite(xpm_map_t xpm, int x, int y, double xspeed, double yspeed) {
  Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
  xpm_image_t img;
  if (sp == NULL )
    return NULL;

  sp->map = (const char *)xpm;
  if( sp->map == NULL ) {
    free(sp);
    return NULL;
  }
  sp->width = img.width; 
  sp->height=img.height;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  sp->x = x;
  sp->y = y;
  return sp;
}

void destroy_sprite(Sprite *sp) {
  if( sp == NULL )
    return;
  if(sp->map) free((void *)sp->map);
  free(sp);
  sp = NULL; 
}

int drawing_xpm(Sprite *sp){
  if (make_xpm((xpm_map_t)sp->map, sp->x, sp->y) != 0) {
    return 1;
  }
  return 0;
}

