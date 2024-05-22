#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "sprite.h"

int (loading_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi, Sprite *sp) {
    xpm_image_t image;
    sp->map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &image);
    sp->height = image.height;
    sp->width = image.width; 
  
    return 0; 
}

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int speed) {
  Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
  if (sp == NULL )
    return NULL;

  if( ((char *)xpm) == NULL ) {
    printf("XPM is NULL \n");
    free(sp);
    return NULL;
  }

  loading_xpm(xpm, x, y, sp);
  sp->x = x;
  sp->y = y;
  sp->speed = speed;
  return sp;
}


void destroy_sprite(Sprite *sp) {
  if ( sp == NULL )
    return;

  free(sp->map);
  free(sp);
}


int drawing_sprite(Sprite *sp){
  uint32_t transparent_color = xpm_transparency_color(XPM_8_8_8_8); 

  for (int y = 0 ; y < sp->height ; y++) {
        for (int x = 0 ; x < sp->width ; x++) {
            uint32_t current_color = sp->map [y * sp->width + x];
            
            if (current_color != transparent_color) {
                if (draw_pixel(sp->x + x, sp->y + y, current_color)) {
                    printf("Drawing pixel failed \n");
                    return 1;
                }
            }
        }
    }
  //drawing_to_buffer(sp->map);
  return 0;
}


bool check_collision(Sprite *sprite1, int base_width, int base_height) {
    int sprite1_left = sprite1->x;
    int sprite1_right = sprite1->x + sprite1->width - 1;
    int sprite1_top = sprite1->y;
    int sprite1_bottom = sprite1->y + sprite1->height - 1;
    /*uint32_t color1= return_color((xpm_map_t)sprite1->map, (uint16_t)sprite1->x+5, (uint16_t)sprite1->y, base_width, base_height);
    uint32_t color2= return_color((xpm_map_t)sprite1->map, (uint16_t)sprite1->x-5, (uint16_t)sprite1->y, base_width, base_height);
    uint32_t color3= return_color((xpm_map_t)sprite1->map, (uint16_t)sprite1->x, (uint16_t)sprite1->y+5, base_width, base_height);
    uint32_t color4= return_color((xpm_map_t)sprite1->map, (uint16_t)sprite1->x, (uint16_t)sprite1->y-5,  base_width, base_height);
    printf("%x", color1);
    if(color1==0x660066||color2==0x660066||color3==0x660066||color4==0x660066){
      return true;
    }*/
    if (sprite1_left >= 0 && sprite1_right < base_width && sprite1_top >= 0 && sprite1_bottom < base_height) {
      return true; //pode andar
    }
  /*  if(sprite1->map[sprite1_bottom*base_width+sprite1_left]=='.'){
      printf("%s", "pontoooo");
      return true;
    }*/
   /* for (int y = sprite1_top; y <= sprite1_bottom; y++) {
        for (int x = sprite1_left; x <= sprite1_right; x++) {
            int base_x = x;
            int base_y = y;
            //printf("%s", "x");
            //printf("%s", "y"); 
            //"  c #660066", ->cor escura por onde nao pode ir
                 
                if ((int)base[base_y * base_width + base_x] != '.') {
                    return true; 
                }
        }
    }*/

    return false;
}
