#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_
#include <lcom/lcf.h>
int (graphics_mode)(uint16_t mode);
int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
<<<<<<< HEAD
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
=======
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
>>>>>>> 3f78205ee7b170d22000afa57f12f3a527914595
int (frame_buffer_func)(uint16_t mode);
uint32_t R( uint32_t color);
uint32_t G( uint32_t color);
uint32_t B( uint32_t color);
int (make_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi);

#endif
<<<<<<< HEAD


=======
>>>>>>> 3f78205ee7b170d22000afa57f12f3a527914595
