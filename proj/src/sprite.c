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
  xpm_load(xpm,XPM_8_8_8,&img);
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

bool check_collision(Sprite *sprite1, int base_width, int base_height) {
    int sprite1_left = sprite1->x;
    int sprite1_right = sprite1->x + sprite1->width - 1;
    int sprite1_top = sprite1->y;
    int sprite1_bottom = sprite1->y + sprite1->height - 1;
    printf("%s", "antes do for");
    printf("%s", "antes do for");
    if (sprite1_left >= 0 && sprite1_right < base_width && sprite1_top >= 0 && sprite1_bottom < base_height) {
      return true; 
    }
   /* for (int y = sprite1_top; y <= sprite1_bottom; y++) {
        for (int x = sprite1_left; x <= sprite1_right; x++) {
            int base_x = x;
            int base_y = y;
            //printf("%s", "x");
            //printf("%s", "y"); 
            //"  c #660066", ->cor escura por onde nao pode ir
            if(sprite1_left!='.')     
                if ((int)base[base_y * base_width + base_x] != '.') {
                    return true; 
                }
        }
    }*/

    return false;
}

int drawing_sprite(Sprite *sp){
  if (make_xpm((xpm_map_t)sp->map, sp->x, sp->y) != 0) {
    return 1;
  }
  return 0;
}
