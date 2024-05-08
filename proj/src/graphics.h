#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_
#include <lcom/lcf.h>

int (graphics_mode)(uint16_t mode);

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (frame_buffer_func)(uint16_t mode);

uint32_t R( uint32_t color);
uint32_t G( uint32_t color);
uint32_t B( uint32_t color);

uint32_t (direct_mode)(uint32_t red, uint32_t green, uint32_t blue);
uint32_t (indexed_mode)(uint16_t column, uint16_t row, uint8_t step, uint32_t color, uint8_t no_rectangles);

int (make_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi);

#endif


