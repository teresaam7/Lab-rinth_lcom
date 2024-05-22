#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "sprite.h"

typedef struct Frame {
    uint8_t *buffer, *buffer2;
    unsigned int res_x, res_y, size, bytesPerPixel;
} Frame; 


int nBuffer;
Frame frame;
uint8_t *draw_buffer, *display_buffer;  
vbe_mode_info_t mode_info;

int (graphic_mode)(uint16_t mode);
int (initialize_frame_buffer) (uint16_t mode);

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *map);
int (drawing_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi, Sprite *sp) ;

void (initialize_buffers)();
void (free_buffers)();
void (update_frame)();
void (clear_frame)();
void (clear_drawing)();

int (background_drawing)(xpm_map_t xpm, uint16_t xi, uint16_t yi);
void (update_frame_with_background)();

#endif