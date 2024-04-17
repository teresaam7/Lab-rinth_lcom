#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <lcom/lcf.h>
#include <math.h>
#include "vbe.h"

int (videocard_graphic_mode)(uint16_t submode);

int (frame)(uint16_t mode);

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (color_normalization)(uint32_t color, uint32_t *new_color);

#endif

