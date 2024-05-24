#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "sprite.h"

int (loading_xpm)(xpm_map_t xpm, Sprite *sp) {
    xpm_image_t image;
    sp->map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &image);
    sp->height = image.height;
    sp->width = image.width; 
  
    return 0; 
}


Sprite* (create_sprite)(xpm_map_t xpm, int x, int y, int speed) {
  Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
  if (sp == NULL )
    return NULL;

  if( ((char *)xpm) == NULL ) {
    printf("XPM is NULL \n");
    free(sp);
    return NULL;
  }

  loading_xpm(xpm, sp);
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



int (drawing_to_buffer)(Sprite *sp, uint8_t *buffer) {
  uint32_t transparent_color = xpm_transparency_color(XPM_8_8_8_8); 

  for (int y = 0 ; y < sp->height ; y++) {
      for (int x = 0 ; x < sp->width  ; x++) {
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


int (drawing_sprite)(Sprite *sp) {
  if (drawing_to_buffer(sp, draw_buffer) != 0) {
    printf("Drawing sprite failed \n");
    return 1;
  }

  return 0;
}


int (drawing_to_buffer_lantern)(Sprite *bg, Sprite *sp, uint8_t *buffer, int lant_radius) {
  uint32_t transparent_color = xpm_transparency_color(XPM_8_8_8_8); 

  for (int y = (-lant_radius) ; y < (sp->height+lant_radius) ; y++) {
      for (int x = (-lant_radius) ; x < (sp->width + lant_radius) ; x++) {
          uint32_t current_color = bg->map [(sp->y + y) * bg->width + (sp->x + x)];
          
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


int (drawing_lantern)(Sprite *bg, Sprite *sp, int lant_radius) {
  if (drawing_to_buffer_lantern(bg, sp, draw_buffer, lant_radius) != 0) {
    printf("Drawing background failed \n");
    return 1;
  }

  return 0;
}


int (drawing_bg)() {
  if (draw_background() != 0) {
    printf("Drawing background failed \n");
    return 1;
  }

  return 0;
}


int (loading_bg_sprite)(Sprite *sp) {
  if (drawing_to_buffer(sp, bg_buffer) != 0) {
    printf("Loading background failed \n");
    return 1;
  }

  return 0;
}

bool check_collision(Sprite *sprite1, int base_width, int base_height) {
    int sprite1_left = sprite1->x;
    int sprite1_right = sprite1->x + sprite1->width - 1;
    int sprite1_top = sprite1->y;
    int sprite1_bottom = sprite1->y + sprite1->height - 1;
    printf("%s", "antes do for");
    printf("%s", "antes do for");
    if (sprite1_left >= 0 && sprite1_right <= base_width && sprite1_top >= 0 && sprite1_bottom <= base_height) {
      return true; 
    }
    return false;
}

