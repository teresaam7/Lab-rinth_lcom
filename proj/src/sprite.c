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

    int center_x = sp->x + sp->width / 2;
    int center_y = sp->y + sp->height / 2;

    for (int y = -lant_radius; y <= lant_radius; y++) {
        for (int x = -lant_radius; x <= lant_radius; x++) {
            int pixel_x = center_x + x;
            int pixel_y = center_y + y;

            if (x * x + y * y <= lant_radius * lant_radius) {
                if (pixel_x >= 0 && pixel_x < bg->width && pixel_y >= 0 && pixel_y < bg->height) {
                    uint32_t current_color = bg->map[pixel_y * bg->width + pixel_x];
                    
                    if (current_color != transparent_color) {
                        if (draw_pixel(pixel_x, pixel_y, current_color, buffer)) {
                            printf("Drawing pixel failed \n");
                            return 1;
                        }
                    }
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

bool (check_collision)(Sprite *player, Sprite *maze, int x_diff, int y_diff) {
  int x_top = player->x;
  int y_top = player->y + player->height/2;
  uint32_t color = maze->map[30 * maze->width + 5];

  uint32_t next_color1 = color, next_color2 = color;

  if (x_diff > 0) {   // Right
    next_color1 = maze->map[(y_top) * maze->width + (x_top + player->width + x_diff)];
    next_color2 = maze->map[(y_top + player->height/2) * maze->width + (x_top + player->width + x_diff)];

  } else if (x_diff < 0) {    // Left
    next_color1 = maze->map[(y_top) * maze->width + (x_top + x_diff)];
    next_color2 = maze->map[(y_top + player->height/2) * maze->width + (x_top + x_diff)];
  
  } else if (y_diff > 0) {    // Bottom
    next_color1 = maze->map[(y_top + player->height/2 + y_diff) * maze->width + (x_top)];
    next_color2 = maze->map[(y_top + player->height/2 + y_diff) * maze->width + (x_top + player->width)];

  } else if (y_diff < 0) {    // Top
    next_color1 = maze->map[(y_top + y_diff) * maze->width + (x_top)];
    next_color2 = maze->map[(y_top + y_diff) * maze->width + (x_top + player->width)];

  }

  if ((color != next_color1) || (color != next_color2)) 
    return true;
  return false;
}

