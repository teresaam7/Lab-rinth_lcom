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


int (drawing_to_buffer)(Sprite *sp, uint8_t *buffer) {
  uint32_t transparent_color = xpm_transparency_color(XPM_8_8_8_8); 

  for (int y = 0 ; y < sp->height ; y++) {
        for (int x = 0 ; x < sp->width ; x++) {
            uint32_t current_color = sp->map [y * sp->width + x];
            
            if (current_color != transparent_color) {
                if (draw_pixel(sp->x + x, sp->y + y, current_color, buffer)) {
                    printf("Drawing pixel failed \n");
                    return 1;
                }
            }
        }
    }
    return 0;
}


void (loading_bg_buffer)(Sprite *sp) {
  drawing_to_buffer(sp, bg_buffer);
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
  drawing_to_buffer(sp, draw_buffer);
  return 0;
}

